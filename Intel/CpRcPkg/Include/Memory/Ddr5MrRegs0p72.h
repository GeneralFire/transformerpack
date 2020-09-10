/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _DDR5_MR_REGS_H_
#define _DDR5_MR_REGS_H_

//
// DDR5 DRAM Spec 0.7.2 Mode Register Definitions.
//

#define DDR5_MODE_REGISTER_0_REG 0x0
typedef union {
  struct {
    UINT8 burst_length : 2;
                            /* Burst Length - Bits[1:0]
                               Register Type - R/W
                               00B: BL16
                               01B: BC8 OTF
                               10B: BL32 (Optional)
                               11B: BL32 OTF (Optional)
                            */
    UINT8 cas_latency_rl : 5;
                            /* CAS Latency (RL) - Bits[6:2]
                               Register Type - R/W
                               00000B: 22
                               00001B: 24
                               00010B: 26
                               00011B: 28
                               ...
                               10011B: 60
                               10100B: 62
                               10101B: 64
                               10110B: 66
                               All other encodings reserved.
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[7]
                               Register Type - RFU
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_0_STRUCT;

#define DDR5_MODE_REGISTER_1_REG 0x1
typedef union {
  struct {
    UINT8 pda_enumerate_id : 4;
                            /* PDA Enumerate ID - Bits[3:0]
                               Register Type - R
                               This is a Read Only MR field, which is only programmed through an MPC command with the PDA Enumerate ID opcode.
                               xxxxB Encoding is set with MPC command with the PDA Enumerate ID opcode.
                               This can only be set when PDA Enumerate Programming Mode is enabled and the associated DRAMs DQ0 is asserted LOW.
                               The PDA Enumerate ID opcode includes 4 bits for this encoding.
                               Default setting is 1111B
                            */
    UINT8 pda_select_id : 4;
                            /* PDA Select ID - Bits[7:4]
                               Register Type - R
                               This is a Read Only MR field, which is only programmed through an MPC command with the PDA Select ID opcode.
                               xxxxB Encoding is set with MPC command with the PDA Select ID opcode.
                               The PDA Select ID opcode includes 4 bits for this encoding.
                               1111B = all DRAMs execute MRW, MPC, and VrefCA commands
                               For all other encodings, DRAMs execute MRW, MPC, and VrefCA commands only if PDA Select ID[3:0] = PDA Enumerate ID[3:0], with some exceptions for specific MPC commands that execute regardless of PDA Select ID.
                               Default setting is 1111B
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_1_STRUCT;

#define DDR5_MODE_REGISTER_2_REG 0x2
typedef union {
  struct {
    UINT8 read_preamble_enable : 1;
                            /* Read Preamble Enable - Bits[0]
                               Register Type - R/W
                               0B: Normal Mode (Default)
                               1B: Read Preamble Training
                            */
    UINT8 write_leveling : 1;
                            /* Write Leveling - Bits[1]
                               Register Type - R/W
                               0B: Normal Mode (Default)
                               1B: Write Leveling
                            */
    UINT8 wl_internal_cycle_alignment : 4;
                            /* WL Internal Cycle Alignment - Bits[5:2]
                               Register Type - R/W
                               xxxxB: WL Internal Cycle Alignment (cycles)
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[6]
                               Register Type - RFU
                               RFU
                            */
    UINT8 internal_write_timing : 1;
                            /* Internal Write Timing - Bits[7]
                               Register Type - R/W
                               0B: Disable
                               1B: Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_2_STRUCT;

#define DDR5_MODE_REGISTER_3_REG 0x3
typedef union {
  struct {
    UINT8 two_n_mode : 1;
                            /* 2N Mode - Bits[0]
                               Register Type - R
                               0B: 2N Mode (Default)
                               1B: 1N Mode
                            */
    UINT8 max_power_savings_mode : 1;
                            /* Max Power Savings Mode - Bits[1]
                               Register Type - R/W
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 cs_assertion_duration_mpc : 1;
                            /* CS Assertion Duration (MPC) - Bits[2]
                               Register Type - R/W
                               0B: Multiple cycles of CS assertion supported for MPC and VrefCA commands
                               1B: Only a single cycle of CS assertion supported for MPC and VrefCA commands
                            */
    UINT8 rfu : 3;
                            /* RFU - Bits[5:3]
                               Register Type - RFU
                               RFU
                            */
    UINT8 dll_enable : 1;
                            /* DLL Enable - Bits[6]
                               Register Type - R/W
                               0B: Disable
                               1B: Enable (Default)
                            */
    UINT8 dll_reset : 1;
                            /* DLL Reset - Bits[7]
                               Register Type - W
                               0B: Normal (Default)
                               1B: DLL Reset
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_3_STRUCT;

#define DDR5_MODE_REGISTER_4_REG 0x4
typedef union {
  struct {
    UINT8 refresh_rate : 3;
                            /* Refresh Rate - Bits[2:0]
                               Register Type - R
                               000B: SDRAM Low Temperature operating limit exceeded
                               001B: RFU
                               010B: tREFI x2 (0.5x Refresh Rate)
                               011B: tREFI x1 (1x Refresh Rate) default
                               100B: tREFI /2 (2x Refresh Rate)
                               101B: tREFI /4 (4x Refresh Rate)
                               110B: RFU
                               111B: SDRAM High Temperature operating limit exceeded
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[3]
                               Register Type - R/W
                               RFU
                            */
    UINT8 refresh_trfc_mode : 1;
                            /* Refresh tRFC Mode - Bits[4]
                               Register Type - R/W
                               0B: Normal Refresh Mode (tRFC1)
                               1B: Fine Granularity Refresh Mode (tRFC2)
                            */
    UINT8 rfu1 : 2;
                            /* RFU1 - Bits[6:5]
                               Register Type - RFU
                               RFU
                            */
    UINT8 tuf_temperature_update_flag : 1;
                            /* TUF (Temperature Update Flag) - Bits[7]
                               Register Type - R
                               0B: No change in OP[3:1] since last MR4 read (default)
                               1B: Change in OP[3:1] since last MR4 read
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_4_STRUCT;

#define DDR5_MODE_REGISTER_5_REG 0x5
typedef union {
  struct {
    UINT8 data_output_disable : 1;
                            /* Data Output Disable - Bits[0]
                               Register Type - W
                               0B: Normal Operation (Default)
                               1B: Outputs Disabled
                            */
    UINT8 pull_up_output_driver_impedance : 2;
                            /* Pull-up Output Driver Impedance - Bits[2:1]
                               Register Type - R/W
                               00B: RZQ/7 (34)
                               01B: RZQ/6 (40)
                               10B: RZQ/5 (48)
                               11B: RFU
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[3]
                               Register Type - RFU
                               RFU
                            */
    UINT8 tdqs_enable : 1;
                            /* TDQS Enable - Bits[4]
                               Register Type - R/W
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 dm_enable : 1;
                            /* DM Enable - Bits[5]
                               Register Type - R/W
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 pull_down_output_driver_impedance : 2;
                            /* Pull-Down Output Driver Impedance - Bits[7:6]
                               Register Type - R/W
                               00B: RZQ/7 (34)
                               01B: RZQ/6 (40)
                               10B: RZQ/5 (48)
                               11B: RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_5_STRUCT;

#define DDR5_MODE_REGISTER_6_REG 0x6
typedef union {
  struct {
    UINT8 write_recovery_time : 4;
                            /* Write Recovery Time - Bits[3:0]
                               Register Type - R/W
                               0000B: 48nCK
                               0001B: 54nCK
                               0010B: 60nCK
                               0011B: 66nCK
                               0100B: 72nCK
                               0101B: 78nCK
                               0110B: 84nCK
                               0111B: 90nCK
                               1000B: 96nCK
                               1001B: RFU
                               1010B: RFU
                               1011B: RFU
                               All other encodings reserved
                            */
    UINT8 trtp : 4;
                            /* tRTP - Bits[7:4]
                               Register Type - R/W
                               0000B: 12nCK
                               0001B: 14nCK
                               0010B: 15nCK
                               0011B: 17nCK
                               0100B: 18nCK
                               0101B: 20nCK
                               0110B: 21nCK
                               0111B: 23nCK
                               1000B: 24nCK
                               All other encodings reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_6_STRUCT;

#define DDR5_MODE_REGISTER_8_REG 0x8
typedef union {
  struct {
    UINT8 read_preamble_settings : 3;
                            /* Read Preamble Settings - Bits[2:0]
                               Register Type - R/W
                               000B: 1 tCK - 10 Pattern
                               001B: 2 tCK - 0010 Pattern
                               010B: 2 tCK - 1110 Pattern (DDR4 Style)
                               011B: 3 tCK - 000010 Pattern
                               100B: 4 tCK - 00001010 Pattern
                               101B: Reserved
                               110B: Reserved
                               111B: Reserved
                            */
    UINT8 write_preamble_settings : 2;
                            /* Write Preamble Settings - Bits[4:3]
                               Register Type - R/W
                               00B: Reserved
                               01B: 2 tCK - 0010 Pattern
                               10B: 3 tCK - 000010 Pattern
                               11B: 4 tCK - 00001010 Pattern
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[5]
                               Register Type - RFU
                               RFU
                            */
    UINT8 read_postamble_settings : 1;
                            /* Read Postamble Settings - Bits[6]
                               Register Type - R/W
                               0B: 0.5 tCK - 0 Pattern
                               1B: 1.5 tCK - 010 Pattern
                            */
    UINT8 write_postamble_settings : 1;
                            /* Write Postamble Settings - Bits[7]
                               Register Type - R/W
                               0B: 0.5 tCK - 0 Pattern
                               1B: 1.5 tCK - 000 Pattern
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_8_STRUCT;

#define DDR5_MODE_REGISTER_9_REG 0x9
typedef union {
  struct {
    UINT8 int_vref_dq_mon : 1;
                            /* Int VREF DQ Mon - Bits[0]
                               Register Type - R/W
                               0B: Normal (Default)
                               1B: Int VREF DQ Monitor
                            */
    UINT8 int_vref_ca_mon : 1;
                            /* Int VREF CA Mon - Bits[1]
                               Register Type - R/W
                               0B: Normal (Default)
                               1B: Int VREF CA Monitor
                            */
    UINT8 rfu : 5;
                            /* RFU - Bits[6:2]
                               Register Type - R/W
                               RFU
                            */
    UINT8 tm : 1;
                            /* TM - Bits[7]
                               Register Type - R/W
                               0B: Normal (Default)
                               1B: Test Mode
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_9_STRUCT;

#define DDR5_MODE_REGISTER_10_REG 0xa
typedef union {
  struct {
    UINT8 vrefdq_cal_value : 8;
                            /* VrefDQ Cal Value - Bits[7:0]
                               Register Type - R/W
                               0000:0000B: --Thru--
                               1111:1111B: See Table Below
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_10_STRUCT;

#define DDR5_MODE_REGISTER_11_REG 0xb
typedef union {
  struct {
    UINT8 vrefca_cal_value : 8;
                            /* VrefCA Cal Value - Bits[7:0]
                               Register Type - R
                               0000:0000B: --Thru--
                               1111:1111B: See Table Below
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_11_STRUCT;

#define DDR5_MODE_REGISTER_12_REG 0xc
typedef union {
  struct {
    UINT8 tccd_l_tdllk : 4;
                            /* tCCD_L / tDLLK - Bits[3:0]
                               Register Type - R/W
                               0000B: --Thru--
                               1111B: See Table Below
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - TBD
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_12_STRUCT;

#define DDR5_MODE_REGISTER_14_REG 0xe
typedef union {
  struct {
    UINT8 ecs_error_register_index : 4;
                            /* ECS Error Register Index - Bits[3:0]
                               Register Type - R/W
                               CID[3:0]
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[4]
                               Register Type - RFU
                               RFU
                            */
    UINT8 code_word_row_count : 1;
                            /* Code Word/Row Count - Bits[5]
                               Register Type - R/W
                               0B: ECS counts Rows with errors
                               1B: ECS counts Code words with errors
                            */
    UINT8 ecs_reset_counter : 1;
                            /* ECS Reset Counter - Bits[6]
                               Register Type - R/W
                               Reset ECC Counter
                            */
    UINT8 ecs_entry : 1;
                            /* ECS Entry - Bits[7]
                               Register Type - R/W
                               0B: Normal Operation (Default)
                               1B: Enable ECS Entry
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_14_STRUCT;

#define DDR5_MODE_REGISTER_15_REG 0xf
typedef union {
  struct {
    UINT8 ecs_error_threshold : 8;
                            /* ECS Error Threshold - Bits[7:0]
                               Register Type - R/W
                               TBD
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_15_STRUCT;

#define DDR5_MODE_REGISTER_16_REG 0x10
typedef union {
  struct {
    UINT8 r_7_0 : 8;
                            /* R[7:0] - Bits[7:0]
                               Register Type - R/W
                               Contains 8 bits of the row address with the highest error count
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_16_STRUCT;

#define DDR5_MODE_REGISTER_17_REG 0x11
typedef union {
  struct {
    UINT8 r_15_8 : 8;
                            /* R[15:8] - Bits[7:0]
                               Register Type - R/W
                               Contains 8 bits of the row address with the highest error count
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_17_STRUCT;

#define DDR5_MODE_REGISTER_18_REG 0x12
typedef union {
  struct {
    UINT8 bg_2_0_ba_1_0_r_17_16 : 8;
                            /* BG[2:0],BA[1,0], R[17,16] - Bits[7:0]
                               Register Type - R/W
                               Contains 8 bits of the row address with the highest error count
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_18_STRUCT;

#define DDR5_MODE_REGISTER_19_REG 0x13
typedef union {
  struct {
    UINT8 code_word_error_count : 6;
                            /* Code Word Error Count - Bits[5:0]
                               Register Type - R/W
                               Contains number of errors within the row with the most errors
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - TBD
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_19_STRUCT;

#define DDR5_MODE_REGISTER_20_REG 0x14
typedef union {
  struct {
    UINT8 ect_bits_0_to_7 : 8;
                            /* ECT bits 0 to 7 - Bits[7:0]
                               Register Type - R/W
                               Contains 8 bits of the Row or Code Words with errors count.
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_20_STRUCT;

#define DDR5_MODE_REGISTER_21_REG 0x15
typedef union {
  struct {
    UINT8 ect_bits_15_to_8 : 8;
                            /* ECT bits 15 to 8 - Bits[7:0]
                               Register Type - R/W
                               Contains 8 bits of the Row or Code Words with errors count.
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_21_STRUCT;

#define DDR5_MODE_REGISTER_22_REG 0x16
typedef union {
  struct {
    UINT8 ect_bits_17_to_16 : 2;
                            /* ECT bits 17 to 16 - Bits[1:0]
                               Register Type - R/W
                               Contains 2 bits of the Row or Code Words with errors count.
                            */
    UINT8 rfu : 6;
                            /* RFU - Bits[7:2]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_22_STRUCT;

#define DDR5_MODE_REGISTER_23_REG 0x17
typedef union {
  struct {
    UINT8 hppr : 1;
                            /* hPPR - Bits[0]
                               Register Type - R/W
                               0B: Disable
                               1B: Enable
                            */
    UINT8 sppr : 1;
                            /* sPPR - Bits[1]
                               Register Type - R/W
                               0B: Disable
                               1B: Enable
                            */
    UINT8 rfu : 6;
                            /* RFU - Bits[7:2]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_23_STRUCT;

#define DDR5_MODE_REGISTER_24_REG 0x18
typedef union {
  struct {
    UINT8 ppr_guard_key : 8;
                            /* PPR Guard Key - Bits[7:0]
                               Register Type - W
                               See PPR Section for Sequence
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_24_STRUCT;

#define DDR5_MODE_REGISTER_25_REG 0x19
typedef union {
  struct {
    UINT8 read_training_pattern_format : 1;
                            /* Read Training Pattern Format - Bits[0]
                               Register Type - R/W
                               0B: Serial
                               1B: LFSR
                            */
    UINT8 lfsr0_pattern_option : 1;
                            /* LFSR0 Pattern Option - Bits[1]
                               Register Type - R/W
                               0B: LFSR
                               1B: Clock
                            */
    UINT8 lfsr1_pattern_option : 1;
                            /* LFSR1 Pattern Option - Bits[2]
                               Register Type - R/W
                               0B: LFSR
                               1B: Clock
                            */
    UINT8 rfu : 5;
                            /* RFU - Bits[7:3]
                               Register Type - RFU
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_25_STRUCT;

#define DDR5_MODE_REGISTER_26_REG 0x1a
typedef union {
  struct {
    UINT8 read_pattern_lfsr_seed_ui_0 : 1;
                            /* Read Pattern / LFSR Seed UI 0 - Bits[0]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_1 : 1;
                            /* Read Pattern / LFSR Seed UI 1 - Bits[1]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_2 : 1;
                            /* Read Pattern / LFSR Seed UI 2 - Bits[2]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_3 : 1;
                            /* Read Pattern / LFSR Seed UI 3 - Bits[3]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_4 : 1;
                            /* Read Pattern / LFSR Seed UI 4 - Bits[4]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_5 : 1;
                            /* Read Pattern / LFSR Seed UI 5 - Bits[5]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_6 : 1;
                            /* Read Pattern / LFSR Seed UI 6 - Bits[6]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_7 : 1;
                            /* Read Pattern / LFSR Seed UI 7 - Bits[7]
                               Register Type - R/W
                               UI<7:0> data for serial mode, LFSR0 seed for LFSR mode
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_26_STRUCT;

#define DDR5_MODE_REGISTER_27_REG 0x1b
typedef union {
  struct {
    UINT8 read_pattern_lfsr_seed_ui_8 : 1;
                            /* Read Pattern / LFSR Seed UI 8 - Bits[0]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_9 : 1;
                            /* Read Pattern / LFSR Seed UI 9 - Bits[1]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_10 : 1;
                            /* Read Pattern / LFSR Seed UI 10 - Bits[2]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_11 : 1;
                            /* Read Pattern / LFSR Seed UI 11 - Bits[3]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_12 : 1;
                            /* Read Pattern / LFSR Seed UI 12 - Bits[4]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_13 : 1;
                            /* Read Pattern / LFSR Seed UI 13 - Bits[5]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_14 : 1;
                            /* Read Pattern / LFSR Seed UI 14 - Bits[6]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
    UINT8 read_pattern_lfsr_seed_ui_15 : 1;
                            /* Read Pattern / LFSR Seed UI 15 - Bits[7]
                               Register Type - R/W
                               UI<15:8> data for serial mode, LFSR1 seed for LFSR mode
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_27_STRUCT;

#define DDR5_MODE_REGISTER_28_REG 0x1c
typedef union {
  struct {
    UINT8 dq_invert_lower_dq_bits : 8;
                            /* DQ Invert (Lower DQ Bits) - Bits[7:0]
                               Register Type - R/W
                               OP[0]: DQL0 (DQ0)
                               OP[1]: DQL1 (DQ1)
                               OP[2]: DQL2 (DQ2)
                               OP[3]: DQL3 (DQ3)
                               OP[4]: DQL4 (DQ4)
                               OP[5]: DQL5 (DQ5)
                               OP[6]: DQL6 (DQ6)
                               OP[7]: DQL7 (DQ7)
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_28_STRUCT;

#define DDR5_MODE_REGISTER_29_REG 0x1d
typedef union {
  struct {
    UINT8 dq_invert_upper_dq_bits : 8;
                            /* DQ Invert (Upper DQ Bits) - Bits[7:0]
                               Register Type - R/W
                               OP[0]: DQU0 (DQ8)
                               OP[1]: DQU1 (DQ9)
                               OP[2]: DQU2 (DQ10)
                               OP[3]: DQU3 (DQ11)
                               OP[4]: DQU4 (DQ12)
                               OP[5]: DQU5 (DQ13)
                               OP[6]: DQU6 (DQ14)
                               OP[7]: DQU7 (DQ15)
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_29_STRUCT;

#define DDR5_MODE_REGISTER_30_REG 0x1e
typedef union {
  struct {
    UINT8 lfsr_assignment_dql0_dqu0 : 1;
                            /* LFSR Assignment DQL0/DQU0 - Bits[0]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql1_dqu1 : 1;
                            /* LFSR Assignment DQL1/DQU1 - Bits[1]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql2_dqu2 : 1;
                            /* LFSR Assignment DQL2/DQU2 - Bits[2]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql3_dqu3 : 1;
                            /* LFSR Assignment DQL3/DQU3 - Bits[3]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql4_dqu4 : 1;
                            /* LFSR Assignment DQL4/DQU4 - Bits[4]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql5_dqu5 : 1;
                            /* LFSR Assignment DQL5/DQU5 - Bits[5]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql6_dqu6 : 1;
                            /* LFSR Assignment DQL6/DQU6 - Bits[6]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
    UINT8 lfsr_assignment_dql7_dqu7 : 1;
                            /* LFSR Assignment DQL7/DQU7 - Bits[7]
                               Register Type - R/W
                               0B: LFSR0
                               1B: LFSR1
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_30_STRUCT;

#define DDR5_MODE_REGISTER_31_REG 0x1f
typedef union {
  struct {
    UINT8 read_training_pattern_address : 8;
                            /* Read Training Pattern Address - Bits[7:0]
                               Register Type - R
                               This MR address is reserved.
                               There are no specific register fields associated with this address.
                               In response to the MRR to this address the DRAM will send the BL16 read training pattern.
                               All 8 bits associated with this MR address are reserved.
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_31_STRUCT;

#define DDR5_MODE_REGISTER_32_REG 0x20
typedef union {
  struct {
    UINT8 ck_odt : 3;
                            /* CK ODT - Bits[2:0]
                               Register Type - R
                               000B: RTT_OFF (Disable) Group A default
                               001B: RZQ/0.5 (480)
                               010B: RZQ/1 (240)
                               011B: RZQ/2 (120)
                               100B: RZQ/3 (80)
                               101B: RZQ/4 (60)
                               110B: RFU
                               111B: RZQ/6 (40) Group B default
                            */
    UINT8 cs_odt : 3;
                            /* CS ODT - Bits[5:3]
                               Register Type - R
                               000B: RTT_OFF (Disable) Group A default
                               001B: RZQ/0.5 (480)
                               010B: RZQ/1 (240)
                               011B: RZQ/2 (120)
                               100B: RZQ/3 (80)
                               101B: RZQ/4 (60)
                               110B: RFU
                               111B: RZQ/6 (40) Group B default
                            */
    UINT8 ca_odt_strap_value : 1;
                            /* CA_ODT Strap Value - Bits[6]
                               Register Type - R
                               0B: Strap Configured to Group A
                               1B: Strap Configured to Group B
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[7]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_32_STRUCT;

#define DDR5_MODE_REGISTER_33_REG 0x21
typedef union {
  struct {
    UINT8 ca_odt : 3;
                            /* CA ODT - Bits[2:0]
                               Register Type - R
                               000B: RTT_OFF (Disable) Group A default
                               001B: RZQ/0.5 (480)
                               010B: RZQ/1 (240)
                               011B: RZQ/2 (120)
                               100B: RZQ/3 (80) Group B default
                               101B: RZQ/4 (60)
                               110B: RFU
                               111B: RZQ/6 (40)
                            */
    UINT8 dqs_rtt_park : 3;
                            /* DQS_RTT_PARK - Bits[5:3]
                               Register Type - R
                               000B: RTT_OFF
                               001B: RZQ (240)
                               010B: RZQ/2 (120)
                               011B: RZQ/3 (80) default
                               100B: RZQ/4 (60)
                               101B: RZQ/5 (48)
                               110B: RZQ/6 (40)
                               111B: RZQ/7 (34)
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_33_STRUCT;

#define DDR5_MODE_REGISTER_34_REG 0x22
typedef union {
  struct {
    UINT8 rtt_park : 3;
                            /* RTT_PARK - Bits[2:0]
                               Register Type - R
                               000B: RTT_OFF
                               001B: RZQ (240)
                               010B: RZQ/2 (120)
                               011B: RZQ/3 (80)
                               100B: RZQ/4 (60) default
                               101B: RZQ/5 (48)
                               110B: RZQ/6 (40)
                               111B: RZQ/7 (34)
                            */
    UINT8 rtt_wr : 3;
                            /* RTT_WR - Bits[5:3]
                               Register Type - R/W
                               000B: RTT_OFF
                               001B: RZQ (240)
                               010B: RZQ/2 (120)
                               011B: RZQ/3 (80) default
                               100B: RZQ/4 (60)
                               101B: RZQ/5 (48)
                               110B: RZQ/6 (40)
                               111B: RZQ/7 (34)
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - TBD
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_34_STRUCT;

#define DDR5_MODE_REGISTER_35_REG 0x23
typedef union {
  struct {
    UINT8 rtt_nom_wr : 3;
                            /* RTT_NOM_WR - Bits[2:0]
                               Register Type - R/W
                               000B: RTT_OFF
                               001B: RZQ (240)
                               010B: RZQ/2 (120)
                               011B: RZQ/3 (80) default
                               100B: RZQ/4 (60)
                               101B: RZQ/5 (48)
                               110B: RZQ/6 (40)
                               111B: RZQ/7 (34)
                            */
    UINT8 rtt_nom_rd : 3;
                            /* RTT_NOM_RD - Bits[5:3]
                               Register Type - R/W
                               000B: RTT_OFF
                               001B: RZQ (240)
                               010B: RZQ/2 (120)
                               011B: RZQ/3 (80) default
                               100B: RZQ/4 (60)
                               101B: RZQ/5 (48)
                               110B: RZQ/6 (40)
                               111B: RZQ/7 (34)
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - TBD
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_35_STRUCT;

#define DDR5_MODE_REGISTER_36_REG 0x24
typedef union {
  struct {
    UINT8 rtt_loopback : 3;
                            /* RTT_Loopback - Bits[2:0]
                               Register Type - R/W
                               000B: RTT_OFF
                               001B: RFU
                               010B: RFU
                               011B: RFU
                               100B: RFU
                               101B: RZQ/5 (48) default
                               110B: RFU
                               111B: RFU
                            */
    UINT8 rfu : 5;
                            /* RFU - Bits[7:3]
                               Register Type - TBD
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_36_STRUCT;

#define DDR5_MODE_REGISTER_37_REG 0x25
typedef union {
  struct {
    UINT8 odtlon_wr_offset : 3;
                            /* ODTLon_WR_Offset - Bits[2:0]
                               Register Type - R/W
                               000B: RFU
                               001B: RFU
                               010B: -3 Clocks
                               011B: -2 Clocks
                               100B: -1 Clock
                               101B: 0 Clock - Default
                               110B: +1 Clock
                               111B: RFU
                            */
    UINT8 odtloff_wr_offset : 3;
                            /* ODTLoff_WR_Offset - Bits[5:3]
                               Register Type - R/W
                               000B: RFU
                               001B: RFU
                               010B: +3 Clocks
                               011B: +2 Clocks
                               100B: -1 Clock
                               101B: 0 Clock - Default
                               110B: +1 Clock
                               111B: RFU
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - TBD

                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_37_STRUCT;

#define DDR5_MODE_REGISTER_38_REG 0x26
typedef union {
  struct {
    UINT8 odtlon_wr_nt_offset : 3;
                            /* ODTLon_WR_NT_Offset - Bits[2:0]
                               Register Type - R/W
                               000B: RFU
                               001B: RFU
                               010B: -3 Clocks
                               011B: -2 Clocks
                               100B: -1 Clock
                               101B: 0 Clock - Default
                               110B: +1 Clock
                               111B: RFU
                            */
    UINT8 odtloff_wr_nt_offset : 3;
                            /* ODTLoff_WR_NT_Offset - Bits[5:3]
                               Register Type - R/W
                               000B: RFU
                               001B: RFU
                               010B: +3 Clocks
                               011B: +2 Clocks
                               100B: -1 Clock
                               101B: 0 Clock - Default
                               110B: +1 Clock
                               111B: RFU
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - TBD

                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_38_STRUCT;

#define DDR5_MODE_REGISTER_39_REG 0x27
typedef union {
  struct {
    UINT8 odtlon_rd_nt_offset : 3;
                            /* ODTLon_RD_NT_Offset - Bits[2:0]
                               Register Type - R/W
                               000B: RFU
                               001B: RFU
                               010B: -3 Clocks
                               011B: -2 Clocks
                               100B: -1 Clock
                               101B: 0 Clock - Default
                               110B: +1 Clock
                               111B: RFU
                            */
    UINT8 odtloff_rd_nt_offset : 3;
                            /* ODTLoff__RD_NT_Offset - Bits[5:3]
                               Register Type - R/W
                               000B: RFU
                               001B: RFU
                               010B: +3 Clocks
                               011B: +2 Clocks
                               100B: -1 Clock
                               101B: 0 Clock - Default
                               110B: +1 Clock
                               111B: RFU
                            */
    UINT8 rfu : 2;
                            /* RFU - Bits[7:6]
                               Register Type - TBD

                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_39_STRUCT;

#define DDR5_MODE_REGISTER_40_REG 0x28
typedef union {
  struct {
    UINT8 read_dqs_offset_timing : 3;
                            /* Read DQS offset timing - Bits[2:0]
                               Register Type - R/W
                               000B: 0 Clock (DEFAULT)
                               001B: 1 Clock
                               010B: 2 Clocks
                               011B: 3 Clocks
                               100B: RFU
                               101B: RFU
                               110B: RFU
                               111B: RFU
                            */
    UINT8 rfu : 5;
                            /* RFU - Bits[7:3]
                               Register Type - TBD
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_40_STRUCT;

#define DDR5_MODE_REGISTER_45_REG 0x2d
typedef union {
  struct {
    UINT8 dqs_interval_timer_run_time : 8;
                            /* DQS interval timer run time - Bits[7:0]
                               Register Type - W
                               0000 0000B: DQS interval timer stop via MPC Command (Default)
                               0000 0001B: DQS timer stops automatically at 16th clocks after timer start
                               0000 0010B: DQS timer stops automatically at 32nd clocks after timer start
                               0000 0011B: DQS timer stops automatically at 48th clocks after timer start
                               0000 0100B: DQS timer stops automatically at 64th clocks after timer start -------------- Thru --------------
                               0011 1111B: DQS timer stops automatically at (63X16)th clocks after timer start
                               01XX XXXXB: DQS timer stops automatically at 2048th clocks after timer start
                               10XX XXXXB: DQS timer stops automatically at 4096th clocks after timer start
                               11XX XXXXB: DQS timer stops automatically at 8192nd clocks after timer start
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_45_STRUCT;

#define DDR5_MODE_REGISTER_46_REG 0x2e
typedef union {
  struct {
    UINT8 dqs_oscillator_count_lsb : 8;
                            /* DQS Oscillator Count - LSB - Bits[7:0]
                               Register Type - Read Only
                               0 - 255 LSB DRAM DQS Oscillator Count
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_46_STRUCT;

#define DDR5_MODE_REGISTER_47_REG 0x2f
typedef union {
  struct {
    UINT8 dqs_oscillator_count_msb : 8;
                            /* DQS Oscillator Count - MSB - Bits[7:0]
                               Register Type - Read Only
                               0 - 255 MSB DRAM DQS Oscillator Count
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_47_STRUCT;

#define DDR5_MODE_REGISTER_48_REG 0x30
typedef union {
  struct {
    UINT8 dql0_dqu0 : 1;
                            /* DQL0/DQU0 - Bits[0]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql1_dqu1 : 1;
                            /* DQL1/DQU1 - Bits[1]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql2_dqu2 : 1;
                            /* DQL2/DQU2 - Bits[2]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql3_dqu3 : 1;
                            /* DQL3/DQU3 - Bits[3]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql4_dqu4 : 1;
                            /* DQL4/DQU4 - Bits[4]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql5_dqu5 : 1;
                            /* DQL5/DQU5 - Bits[5]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql6_dqu6 : 1;
                            /* DQL6/DQU6 - Bits[6]
                               Register Type - R/W
                               Valid
                            */
    UINT8 dql7_dqu7 : 1;
                            /* DQL7/DQU7 - Bits[7]
                               Register Type - R/W
                               Valid
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_48_STRUCT;

#define DDR5_MODE_REGISTER_50_REG 0x32
typedef union {
  struct {
    UINT8 read_crc_enable : 1;
                            /* Read CRC enable - Bits[0]
                               Register Type - RW
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 write_crc_enable_lower_nibble : 1;
                            /* Write CRC enable lower nibble - Bits[1]
                               Register Type - RW
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 write_crc_enable_upper_nibble : 1;
                            /* Write CRC enable upper nibble - Bits[2]
                               Register Type - RW
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 write_crc_error_status : 1;
                            /* Write CRC error status - Bits[3]
                               Register Type - RW
                               0B: Clear
                               1B: Error
                            */
    UINT8 write_crc_auto_disable_enable : 1;
                            /* Write CRC auto-disable enable - Bits[4]
                               Register Type - RW
                               0B: Disable (Default)
                               1B: Enable
                            */
    UINT8 write_crc_auto_disable_status : 1;
                            /* Write CRC auto-disable status - Bits[5]
                               Register Type - RW
                               0B: Not triggered
                               1B: Triggered
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[6]
                               Register Type - RFU
                               RFU
                            */
    UINT8 rfu1 : 1;
                            /* RFU1 - Bits[7]
                               Register Type - RFU
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_50_STRUCT;

#define DDR5_MODE_REGISTER_51_REG 0x33
typedef union {
  struct {
    UINT8 write_crc_auto_disable_threshold : 7;
                            /* Write CRC auto-disable threshold - Bits[6:0]
                               Register Type - RW
                               0000000B: 0
                               ...
                               1111111B: 127
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[7]
                               Register Type - RFU
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_51_STRUCT;

#define DDR5_MODE_REGISTER_52_REG 0x34
typedef union {
  struct {
    UINT8 write_crc_auto_disable_window : 7;
                            /* Write CRC auto-disable window - Bits[6:0]
                               Register Type - RW
                               0000000B: 0
                               ...
                               1111111B: 127
                            */
    UINT8 rfu : 1;
                            /* RFU - Bits[7]
                               Register Type - RFU
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_52_STRUCT;

#define DDR5_MODE_REGISTER_53_REG 0x35
typedef union {
  struct {
    UINT8 loopback_output_select : 5;
                            /* Loopback Output Select - Bits[4:0]
                               Register Type - R/W
                               00000B: Loopback Disabled (Default)
                               00001B: Loopback DML
                               00010B: Loopback DMU (X16 only)
                               00011B: Vendor Specific
                               00100B: Vendor Specific
                               00101B: RFU
                               ...thru..
                               01111B: RFU
                               10000B: Loopback DQL0
                               10001B: Loopback DQL1
                               10010B: Loopback DQL2
                               10011B: Loopback DQL3
                               10100B: Loopback DQL4 (X8 and X16 only)
                               10101B: Loopback DQL5 (X8 and X16 only)
                               10110B: Loopback DQL6 (X8 and X16 only)
                               10111B: Loopback DQL7 (X8 and X16 only)
                               11000B: Loopback DQU0 (X16 only)
                               11001B: Loopback DQU1 (X16 only)
                               11010B: Loopback DQU2 (X16 only)
                               11011B: Loopback DQU3 (X16 only)
                               11100B: Loopback DQU4 (X16 only)
                               11101B: Loopback DQU5 (X16 only)
                               11110B: Loopback DQU6 (X16 only)
                               11111B: Loopback DQU7 (X16 only)
                            */
    UINT8 loopback_select_phase : 2;
                            /* Loopback Select Phase - Bits[6:5]
                               Register Type - R/W
                               00B: Loopback Select Phase A
                               01B: Loopback Select Phase B (4-way and 2-way interleave only)
                               10B: Loopback Select Phase C (4-way interleave only)
                               11B: Loopback Select Phase D (4-way interleave only)
                            */
    UINT8 loopback_output_mode : 1;
                            /* Loopback Output Mode - Bits[7]
                               Register Type - R/W
                               0B: DQS Qualified Output (Default)
                               1B: WE Qualified Output
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_53_STRUCT;

#define DDR5_MODE_REGISTER_63_REG 0x3f
typedef union {
  struct {
    UINT8 dram_scratch_pad : 8;
                            /* DRAM Scratch Pad - Bits[7:0]
                               Register Type - R/W
                               Any value is valid
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_63_STRUCT;

#define DDR5_MODE_REGISTER_112_REG 0x70
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_112_STRUCT;

#define DDR5_MODE_REGISTER_113_REG 0x71
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_113_STRUCT;

#define DDR5_MODE_REGISTER_114_REG 0x72
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_114_STRUCT;

#define DDR5_MODE_REGISTER_115_REG 0x73
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_115_STRUCT;

#define DDR5_MODE_REGISTER_116_REG 0x74
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_116_STRUCT;

#define DDR5_MODE_REGISTER_120_REG 0x78
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_120_STRUCT;

#define DDR5_MODE_REGISTER_121_REG 0x79
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_121_STRUCT;

#define DDR5_MODE_REGISTER_122_REG 0x7a
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_122_STRUCT;

#define DDR5_MODE_REGISTER_123_REG 0x7b
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_123_STRUCT;

#define DDR5_MODE_REGISTER_124_REG 0x7c
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_124_STRUCT;

#define DDR5_MODE_REGISTER_128_REG 0x80
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_128_STRUCT;

#define DDR5_MODE_REGISTER_129_REG 0x81
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_129_STRUCT;

#define DDR5_MODE_REGISTER_130_REG 0x82
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_130_STRUCT;

#define DDR5_MODE_REGISTER_131_REG 0x83
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_131_STRUCT;

#define DDR5_MODE_REGISTER_132_REG 0x84
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_132_STRUCT;

#define DDR5_MODE_REGISTER_136_REG 0x88
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_136_STRUCT;

#define DDR5_MODE_REGISTER_137_REG 0x89
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_137_STRUCT;

#define DDR5_MODE_REGISTER_138_REG 0x8a
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_138_STRUCT;

#define DDR5_MODE_REGISTER_139_REG 0x8b
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_139_STRUCT;

#define DDR5_MODE_REGISTER_140_REG 0x8c
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_140_STRUCT;

#define DDR5_MODE_REGISTER_144_REG 0x90
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_144_STRUCT;

#define DDR5_MODE_REGISTER_145_REG 0x91
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_145_STRUCT;

#define DDR5_MODE_REGISTER_146_REG 0x92
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_146_STRUCT;

#define DDR5_MODE_REGISTER_147_REG 0x93
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_147_STRUCT;

#define DDR5_MODE_REGISTER_148_REG 0x94
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_148_STRUCT;

#define DDR5_MODE_REGISTER_152_REG 0x98
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_152_STRUCT;

#define DDR5_MODE_REGISTER_153_REG 0x99
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_153_STRUCT;

#define DDR5_MODE_REGISTER_154_REG 0x9a
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_154_STRUCT;

#define DDR5_MODE_REGISTER_155_REG 0x9b
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_155_STRUCT;

#define DDR5_MODE_REGISTER_156_REG 0x9c
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_156_STRUCT;

#define DDR5_MODE_REGISTER_160_REG 0xa0
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_160_STRUCT;

#define DDR5_MODE_REGISTER_161_REG 0xa1
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_161_STRUCT;

#define DDR5_MODE_REGISTER_162_REG 0xa2
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_162_STRUCT;

#define DDR5_MODE_REGISTER_163_REG 0xa3
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_163_STRUCT;

#define DDR5_MODE_REGISTER_164_REG 0xa4
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_164_STRUCT;

#define DDR5_MODE_REGISTER_168_REG 0xa8
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_168_STRUCT;

#define DDR5_MODE_REGISTER_169_REG 0xa9
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_169_STRUCT;

#define DDR5_MODE_REGISTER_170_REG 0xaa
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_170_STRUCT;

#define DDR5_MODE_REGISTER_171_REG 0xab
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_171_STRUCT;

#define DDR5_MODE_REGISTER_172_REG 0xac
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_172_STRUCT;

#define DDR5_MODE_REGISTER_176_REG 0xb0
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_176_STRUCT;

#define DDR5_MODE_REGISTER_177_REG 0xb1
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_177_STRUCT;

#define DDR5_MODE_REGISTER_178_REG 0xb2
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_178_STRUCT;

#define DDR5_MODE_REGISTER_179_REG 0xb3
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_179_STRUCT;

#define DDR5_MODE_REGISTER_180_REG 0xb4
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_180_STRUCT;

#define DDR5_MODE_REGISTER_184_REG 0xb8
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_184_STRUCT;

#define DDR5_MODE_REGISTER_185_REG 0xb9
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_185_STRUCT;

#define DDR5_MODE_REGISTER_186_REG 0xba
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_186_STRUCT;

#define DDR5_MODE_REGISTER_187_REG 0xbb
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_187_STRUCT;

#define DDR5_MODE_REGISTER_188_REG 0xbc
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_188_STRUCT;

#define DDR5_MODE_REGISTER_192_REG 0xc0
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_192_STRUCT;

#define DDR5_MODE_REGISTER_193_REG 0xc1
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_193_STRUCT;

#define DDR5_MODE_REGISTER_194_REG 0xc2
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_194_STRUCT;

#define DDR5_MODE_REGISTER_195_REG 0xc3
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_195_STRUCT;

#define DDR5_MODE_REGISTER_196_REG 0xc4
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_196_STRUCT;

#define DDR5_MODE_REGISTER_200_REG 0xc8
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_200_STRUCT;

#define DDR5_MODE_REGISTER_201_REG 0xc9
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_201_STRUCT;

#define DDR5_MODE_REGISTER_202_REG 0xca
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_202_STRUCT;

#define DDR5_MODE_REGISTER_203_REG 0xcb
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_203_STRUCT;

#define DDR5_MODE_REGISTER_204_REG 0xcc
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_204_STRUCT;

#define DDR5_MODE_REGISTER_208_REG 0xd0
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_208_STRUCT;

#define DDR5_MODE_REGISTER_209_REG 0xd1
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_209_STRUCT;

#define DDR5_MODE_REGISTER_210_REG 0xd2
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_210_STRUCT;

#define DDR5_MODE_REGISTER_211_REG 0xd3
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_211_STRUCT;

#define DDR5_MODE_REGISTER_212_REG 0xd4
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_212_STRUCT;

#define DDR5_MODE_REGISTER_216_REG 0xd8
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_216_STRUCT;

#define DDR5_MODE_REGISTER_217_REG 0xd9
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_217_STRUCT;

#define DDR5_MODE_REGISTER_218_REG 0xda
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_218_STRUCT;

#define DDR5_MODE_REGISTER_219_REG 0xdb
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_219_STRUCT;

#define DDR5_MODE_REGISTER_220_REG 0xdc
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_220_STRUCT;

#define DDR5_MODE_REGISTER_224_REG 0xe0
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_224_STRUCT;

#define DDR5_MODE_REGISTER_225_REG 0xe1
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_225_STRUCT;

#define DDR5_MODE_REGISTER_226_REG 0xe2
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_226_STRUCT;

#define DDR5_MODE_REGISTER_227_REG 0xe3
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_227_STRUCT;

#define DDR5_MODE_REGISTER_228_REG 0xe4
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_228_STRUCT;

#define DDR5_MODE_REGISTER_232_REG 0xe8
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_232_STRUCT;

#define DDR5_MODE_REGISTER_233_REG 0xe9
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_233_STRUCT;

#define DDR5_MODE_REGISTER_234_REG 0xea
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_234_STRUCT;

#define DDR5_MODE_REGISTER_235_REG 0xeb
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_235_STRUCT;

#define DDR5_MODE_REGISTER_236_REG 0xec
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_236_STRUCT;

#define DDR5_MODE_REGISTER_240_REG 0xf0
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_240_STRUCT;

#define DDR5_MODE_REGISTER_241_REG 0xf1
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_241_STRUCT;

#define DDR5_MODE_REGISTER_242_REG 0xf2
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_242_STRUCT;

#define DDR5_MODE_REGISTER_243_REG 0xf3
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_243_STRUCT;

#define DDR5_MODE_REGISTER_244_REG 0xf4
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_244_STRUCT;

#define DDR5_MODE_REGISTER_248_REG 0xf8
typedef union {
  struct {
    UINT8 dfe_gain_bias : 3;
                            /* DFE Gain Bias - Bits[2:0]
                               Register Type - R/W
                               000B: DFE Gain Bias Step 0 (Default)
                               001B: DFE Gain Bias Step 1
                               010B: DFE Gain Bias Step 2
                               011B: DFE Gain Bias Step 3
                               100B: RFU
                               101B: RFU
                               111B: RFU
                            */
    UINT8 sign_bit_gain_bias : 1;
                            /* Sign Bit Gain Bias - Bits[3]
                               Register Type - R/W
                               0B: Positive DFE Gain Bias (Default)
                               1B: Negative DFE Gain Bias
                            */
    UINT8 rfu : 4;
                            /* RFU - Bits[7:4]
                               Register Type - R/W
                               RFU
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_248_STRUCT;

#define DDR5_MODE_REGISTER_249_REG 0xf9
typedef union {
  struct {
    UINT8 dfe_tap_1_bias : 6;
                            /* DFE Tap-1 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-1 Bias Step 0 (Default)
                               000001B: DFE Tap-1 Bias Step 1
                               000010B: DFE Tap-1 Bias Step 2
                               000011B: DFE Tap-1 Bias Step 3
                               100110B: DFE Tap-1 Bias Step +39
                               100101B: DFE Tap-1 Bias Step +38
                               :
                               000101B: DFE Tap-1 Bias Step +5
                               100111B: DFE Tap-1 Bias Step 39
                               101000B: DFE Tap-1 Bias Step 40
                               101001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_1_bias : 1;
                            /* Sign Bit DFE Tap-1 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-1 Bias (Default)
                               1B: Negative DFE Tap-1 Bias
                            */
    UINT8 enable_disable_dfe_tap_1 : 1;
                            /* Enable/Disable DFE Tap-1 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-1 Disable (Default)
                               1B: DFE Tap-1 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_249_STRUCT;

#define DDR5_MODE_REGISTER_250_REG 0xfa
typedef union {
  struct {
    UINT8 dfe_tap_2_bias : 6;
                            /* DFE Tap-2 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-2 Bias Step 0 (Default)
                               000001B: DFE Tap-2 Bias Step 1
                               000010B: DFE Tap-2 Bias Step 2
                               000011B: DFE Tap-2 Bias Step 3
                               001110B: DFE Tap-2 Bias Step +14
                               001011B: DFE Tap-2 Bias Step +13
                               :
                               000101B: DFE Tap-2 Bias Step +5
                               000100B: DFE Tap-2 Bias Step +4
                               001111B: DFE Tap-2 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_2_bias : 1;
                            /* Sign Bit DFE Tap-2 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-2 Bias (Default)
                               1B: Negative DFE Tap-2 Bias
                            */
    UINT8 enable_disable_dfe_tap_2 : 1;
                            /* Enable/Disable DFE Tap-2 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-2 Disable (Default)
                               1B: DFE Tap-2 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_250_STRUCT;

#define DDR5_MODE_REGISTER_251_REG 0xfb
typedef union {
  struct {
    UINT8 dfe_tap_3_bias : 6;
                            /* DFE Tap-3 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-3 Bias Step 0 (Default)
                               000001B: DFE Tap-3 Bias Step 1
                               000010B: DFE Tap-3 Bias Step 2
                               000011B: DFE Tap-3 Bias Step 3
                               001011B: DFE Tap-3 Bias Step +11
                               001010B: DFE Tap-3 Bias Step +10
                               :
                               000101B: DFE Tap-3 Bias Step +5
                               000100B: DFE Tap-3 Bias Step +4
                               001111B: DFE Tap-3 Bias Step 15
                               010000B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_3_bias : 1;
                            /* Sign Bit DFE Tap-3 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-3 Bias (Default)
                               1B: Negative DFE Tap-3 Bias
                            */
    UINT8 enable_disable_dfe_tap_3 : 1;
                            /* Enable/Disable DFE Tap-3 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-3 Disable (Default)
                               1B: DFE Tap-3 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_251_STRUCT;

#define DDR5_MODE_REGISTER_252_REG 0xfc
typedef union {
  struct {
    UINT8 dfe_tap_4_bias : 6;
                            /* DFE Tap-4 Bias - Bits[5:0]
                               Register Type - R/W
                               000000B: DFE Tap-4 Bias Step 0 (Default)
                               000001B: DFE Tap-4 Bias Step 1
                               000010B: DFE Tap-4 Bias Step 2
                               000011B: DFE Tap-4 Bias Step 3
                               001000B: DFE Tap-4 Bias Step +8
                               000111B: DFE Tap-4 Bias Step +7
                               000110B: DFE Tap-4 Bias Step +6
                               000101B: DFE Tap-4 Bias Step +5
                               001000B: DFE Tap-4 Bias Step 8
                               001001B: RFU
                               :
                               111111B: RFU
                            */
    UINT8 sign_bit_dfe_tap_4_bias : 1;
                            /* Sign Bit DFE Tap-4 Bias - Bits[6]
                               Register Type - R/W
                               0B: Positive DFE Tap-4 Bias (Default)
                               1B: Negative DFE Tap-4 Bias
                            */
    UINT8 enable_disable_dfe_tap_4 : 1;
                            /* Enable/Disable DFE Tap-4 - Bits[7]
                               Register Type - R/W
                               0B: DFE Tap-4 Disable (Default)
                               1B: DFE Tap-4 Enable
                            */
  } Bits;
  UINT8 Data;
} DDR5_MODE_REGISTER_252_STRUCT;

#endif  // #ifndef _DDR5_MR_REGS_H_
