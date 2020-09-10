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

#ifndef _DDR5_DB_REGS_H_
#define _DDR5_DB_REGS_H_

//
// DDR5 Data Buffer Spec Rev0.7 Register Definitions.
//

/**
  RW[80] - Features Control Word
**/
#define DDR5_DATA_BUFFER_RW80_REG 0x80
typedef union {
  struct {
    UINT8 bcom_command_rate_read_only : 1;
                            /* BCOM Command Rate (Read Only) - Bits[0]
                              0 = 1N,
                              1 = 2N (Default)
                            */
    UINT8 low_nibble_i_o_buffer_disable : 1;
                            /* Low Nibble I/O buffer Disable - Bits[1]
                              0 = Enable,
                              1 = Disable 2,3
                            */
    UINT8 upper_nibble_i_o_buffer_disable : 1;
                            /* Upper Nibble I/O buffer Disable - Bits[2]
                              0 = Enable,
                              1 = Disable 2,3
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 rank0_presents : 1;
                            /* Rank0 Presents - Bits[3]
                              0 = Package Rank 0 Enabled,
                              1 = Package Rank 0 Disabled
                            */
    UINT8 rank_1_present : 1;
                            /* Rank 1 Present - Bits[4]
                              0 = Package Rank 1 Enabled,
                              1 = Package Rank 1 Disabled
                            */
    UINT8 reserved_1 : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW80_STRUCT;

/**
  RW[81] - Buffer Configuration Control Word
**/
#define DDR5_DATA_BUFFER_RW81_REG 0x81
typedef union {
  struct {
    UINT8 pba_enumerate_programming_mode : 1;
                            /* PBA Enumerate Programming Mode - Bits[0]
                              0 = PBA Enumerate Mode Disabled,
                              1 = PBA Enumerate Mode Enabled
                            */
    UINT8 vrefdq_boadcast : 1;
                            /* VrefDQ boadcast - Bits[1]
                              0 = Disabled,
                              1 = Enabled
                            */
    UINT8 vrefmdq_broadcast : 1;
                            /* VrefMDQ broadcast - Bits[2]
                              0 = Disabled,
                              1 = Enabled
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW81_STRUCT;

/**
  RW[82] - Transparent and DQ Pass Through Support Control Word
**/
#define DDR5_DATA_BUFFER_RW82_REG 0x82
typedef union {
  struct {
    UINT8 transparent_mode_enable : 1;
                            /* Transparent Mode Enable - Bits[0]
                              0 = Transparent mode disabled,
                              1 = Transparent mode enabled
                            */
    UINT8 termination_disable_for_tpm_and_dq_ptm : 1;
                            /* Termination disable for TPM and DQ PTM - Bits[1]
                              0 = Write direction (default),
                              1 = Read direction
                            */
    UINT8 dq_pass_through_mode : 1;
                            /* DQ Pass Through Mode - Bits[2]
                              0 = Disabled,
                              1 = Enabled
                            */
    UINT8 dq_pass_through_mode_direction_select : 1;
                            /* DQ pass through mode direction select - Bits[3]
                              0 = Write direction (default),
                              1 = Read direction
                            */
    UINT8 reserved : 4;
                            /* Reserved - Bits[7:4]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW82_STRUCT;

/**
  RW[83] - M]DQS, [M]DQ Training Modes Control Word
**/
#define DDR5_DATA_BUFFER_RW83_REG 0x83
typedef union {
  struct {
    UINT8 training_mode : 4;
                            /* Normal operation - Bits[3:0]
                              0000 = Exit any of the training mode
                              0001 = DRAM Interface Receive Enable Training
                              0010 = MDQS Read Delay Training
                              0011 = DRAM Write Leveling
                              0100 = MDQ Write Delay Training
                              0101 = Host Interface Write Leveling
                              0110 = Host Interface Read Training
                              0111 = Reserved
                              1000 = Reserved
                            */
    UINT8 hpa : 1;
                            /* HPA - Bits[4]
                              0 = Host Preamble Training Mode disabled,
                              1 = Host Preamble Training Mode enabled
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW83_STRUCT;

/**
  RW[84] - LRDIMM Operating Speed
**/
#define DDR5_DATA_BUFFER_RW84_REG 0x84
typedef union {
  struct {
    UINT8 operating_speed : 4;
                            /* Operating Speed - Bits[3:0]
                              0000 = DDR5-3200 (2800 MT/s < f < 3200 MT/s),
                              0001 = DDR5-3600 (3200 MT/s < f < 3600 MT/s),
                              0010 = DDR5-4000 (3600 MT/s < f < 4000 MT/s),
                              0011 = DDR5-4400 (4000 MT/s < f < 4400 MT/s),
                              0100 = DDR5-4800 (4400 MT/s < f < 4800 MT/s),
                              0101 = DDR5-5200(4800 MT/s < f < 5200 MT/s),
                              0110 = DDR5 5600 (5200 MT/s < f < 5600 MT/s),
                              0111 = DDR5 6000 (5600 MT/s < f < 6000 MT/s),
                              1000 = DDR5 6400 (6000 MT/s < f < 6400 MT/s),
                              1001 = Reserved,
                              1010 = Reserved,
                              1011 = Reserved,
                              1100 = Reserved,
                              1101 = Reserved,
                              1110 = Reserved
                            */
    UINT8 pll_bypass_mode : 4;
                            /* PLL Bypass Mode - Bits[3:0]
                              1111 = PLL bypass mode enabled
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 buffer_v_dd_operating_voltage : 1;
                            /* Buffer V DD Operating Voltage - Bits[7]
                              0 = 1.1 V,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW84_STRUCT;

/**
  RW[85] - Fine Granularity DIMM Operating Speed Control Word
**/
#define DDR5_DATA_BUFFER_RW85_REG 0x85
typedef union {
  struct {
    UINT8 fine_granularity_operating_speed_in_terms_of : 5;
                            /* Fine Granularity Operating Speed in terms of f bin , where f bin is the top speed for the
                               speed range selected in RW05 OP[3:0] - Bits[4:0]
                              00000=(f bin - 20 MT/s) < f f bin,
                              00001=(f bin - 40 MT/s) < f (f bin - 20 MT/s),
                              00010=(f bin - 60 MT/s) < f (f bin - 40 MT/s),
                              00011=(f bin - 80 MT/s) < f (f bin - 60 MT/s),
                              00100=(f bin - 100 MT/s) < f (f bin - 80 MT/s),
                              00101=(f bin - 120 MT/s) < f (f bin - 100 MT/s),
                              00110=(f bin - 140 MT/s) < f (f bin - 120 MT/s),
                              00111=(f bin - 160 MT/s) < f (f bin - 140 MT/s),
                              01000=(f bin - 180 MT/s) < f (f bin - 160 MT/s),
                              01001=(f bin - 200 MT/s) < f (f bin - 180 MT/s),
                              01010=(f bin - 220 MT/s) < f (f bin - 200 MT/s),
                              01011=(f bin - 240 MT/s) < f (f bin - 220 MT/s),
                              01100=(f bin - 260 MT/s) < f (f bin - 240 MT/s),
                              01101=(f bin - 280 MT/s) < f (f bin - 260 MT/s),
                              01110=(f bin - 300 MT/s) < f (f bin - 280 MT/s),
                              01111=(f bin - 320 MT/s) < f (f bin - 300 MT/s),
                              10000=(f bin - 340 MT/s) < f (f bin - 320 MT/s),
                              10001=(f bin - 360 MT/s) < f (f bin - 340 MT/s),
                              10010=(f bin - 380 MT/s) < f (f bin - 360 MT/s),
                              10011=(f bin - 400 MT/s) < f (f bin - 380 MT/s),
                              10100=(f bin - 420 MT/s) < f (f bin - 400 MT/s),
                              1..._..._..._..._...reserved=(f bin - 420 MT/s) < f (f bin - 400 MT/s),
                              1..._..._..._..._...reserved=(f bin - 420 MT/s) < f (f bin - 400 MT/s),
                              11111 = Reserved
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW85_STRUCT;

/**
  RW[86] - DQS RTT Park Termination Control Word
**/
#define DDR5_DATA_BUFFER_RW86_REG 0x86
typedef union {
  struct {
    UINT8 dqs_rtt_park : 3;
                            /* DQS_RTT_PARK - Bits[2:0]
                              000 = RTT_OFF Default,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 ),
                              100 = RZQ/4 (60 ),
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW86_STRUCT;

/**
  RW[87] - Host Interface DQ RTT Termination Control Word
**/
#define DDR5_DATA_BUFFER_RW87_REG 0x87
typedef union {
  struct {
    UINT8 dq_rtt_park : 3;
                            /* DQ RTT_PARK - Bits[2:0]
                              000 = RTT_OFF Default,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 ),
                              100 = RZQ/4 (60 ),
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 dq_rtt_wr : 3;
                            /* DQ RTT_WR - Bits[5:3]
                              000 = RTT_OFF,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 ),
                              100 = RZQ/4 (60 ) Default,
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW87_STRUCT;

/**
  RW[88] - Host Interface DQ RTT NOM Termination Control Word
**/
#define DDR5_DATA_BUFFER_RW88_REG 0x88
typedef union {
  struct {
    UINT8 dq_rtt_nom_wr : 3;
                            /* DQ RTT_NOM_WR - Bits[2:0]
                              000 = RTT_OFF,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 ),
                              100 = RZQ/4 (60 ) Default,
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 dq_rtt_nom_rd : 3;
                            /* DQ RTT_NOM_RD - Bits[5:3]
                              000 = RTT_OFF,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 ) Default,
                              100 = RZQ/4 (60 ),
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW88_STRUCT;

/**
  RW[8A] - Host Interface DQ Driver Control Word
**/
#define DDR5_DATA_BUFFER_RW8A_REG 0x8A
typedef union {
  struct {
    UINT8 host_interface_dq_dqs_output_disable : 1;
                            /* Host Interface DQ/DQS Output Disable - Bits[0]
                              0 = Normal Operation -Default,
                              1 = Outputs Disabled
                            */
    UINT8 host_interface_dq_dqs_output_driver_pull_up : 3;
                            /* Host Interface DQ/DQS Output Driver Pull up Impedance control - Bits[3:1]
                              000 = RZQ/7 (34 ),
                              001 = Reserved,
                              010 = RZQ/5 (48 ),
                              011 = Reserved,
                              100 = Reserved,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 host_interface_dq_dqs_output_driver_pull_down : 3;
                            /* Host Interface DQ/DQS Output Driver Pull Down Impedance control - Bits[6:4]
                              000 = RZQ/7 (34 ),
                              001 = Reserved,
                              010 = RZQ/5 (48 ),
                              011 = Reserved,
                              100 = Reserved,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW8A_STRUCT;

/**
  RW[8B] - DRAM Interface MDQ Driver Control Word
**/
#define DDR5_DATA_BUFFER_RW8B_REG 0x8B
typedef union {
  struct {
    UINT8 dram_interface_mdq_mdqs_output_disable : 1;
                            /* DRAM Interface MDQ/MDQS Output Disable - Bits[0]
                              0 = Normal Operation -Default,
                              1 = Outputs Disabled
                            */
    UINT8 dram_interface_mdq_mdqs_output_driver_pull_up : 3;
                            /* DRAM Interface MDQ/MDQS Output Driver Pull up Impedance control - Bits[3:1]
                              000 = RZQ/tbd,
                              001 = Reserved,
                              010 = RZQ/tbd,
                              011 = Reserved,
                              100 = Reserved,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 dram_interface_mdq_mdqs_output_driver_pull_down : 3;
                            /* DRAM Interface MDQ/MDQS Output Driver Pull Down Impedance control - Bits[6:4]
                              000 = RZQ/tbd,
                              001 = Reserved,
                              010 = RZQ/tbd,
                              011 = Reserved,
                              100 = Reserved,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW8B_STRUCT;

/**
  RW[8C] - MDQS and MDQ Park Termination Control Word
**/
#define DDR5_DATA_BUFFER_RW8C_REG 0x8C
typedef union {
  struct {
    UINT8 mdqs_rtt_park : 3;
                            /* MDQS_RTT_PARK - Bits[2:0]
                              000 = RTT_OFF,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 )Default,
                              100 = RZQ/4 (60 ),
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 mdq_rtt_park : 3;
                            /* MDQ_RTT_PARK - Bits[5:3]
                              000 = RTT_OFF,
                              001 = RZQ/1 (240 ),
                              010 = RZQ/2 (120 ),
                              011 = RZQ/3 (80 ),
                              100 = RZQ/4 (60 ) Default,
                              101 = RZQ/5 (48 ),
                              110 = RZQ/6(40 ),
                              111 = RZQ/7(34 )
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW8C_STRUCT;

/**
  RW[8E] - Loopback RTT and RonControl Word
**/
#define DDR5_DATA_BUFFER_RW8E_REG 0x8E
typedef union {
  struct {
    UINT8 rtt_loopback : 3;
                            /* RTT_Loopback - Bits[2:0]
                              000 = RTT_OFF,
                              001 = Reserved,
                              010 = Reserved,
                              011 = Reserved,
                              100 = Reserved,
                              101 = RZQ/5 (48 ) Default,
                              110 = Reserved,
                              111 = Reserved)
                            */
    UINT8 loopback_driver_strength_settings : 2;
                            /* Loopback Driver Strength Settings - Bits[4:3]
                              00 = RZQ/7 (34 ),
                              01 = Reserved,
                              10 = RZQ/5 (48 ),
                              11 = Reserved
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW8E_STRUCT;

/**
  RW[8F] - Host Interface Read DQS Offset Timing Control Word
**/
#define DDR5_DATA_BUFFER_RW8F_REG 0x8F
typedef union {
  struct {
    UINT8 host_interface_dqs_pre_launch : 3;
                            /* Host Interface DQS Pre-launch - Bits[2:0]
                              000 = 0 tCK (DQS match DQ delay,
                              001 = 1 tCK,
                              010 = 2 tCK,
                              011 = 3 tCK,
                              100 = Reserved,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW8F_STRUCT;

/**
  RW[92] - PBA Enumerate ID Control Word
**/
#define DDR5_DATA_BUFFER_RW92_REG 0x92
typedef union {
  struct {
    UINT8 pba_set_id : 4;
                            /* PBA Set ID - Bits[3:0]
                              0000 = ID = 0,
                              0001 = ID = 1,
                              0010 = ID = 2,
                              0011 = ID = 3,
                              0100 = ID = 4,
                              0101 = ID = 5,
                              0110 = ID = 6,
                              0111 = ID = 7,
                              1000 = ID = 8,
                              1001 = ID = 9,
                              1010 = ID = 10,
                              1011 = ID = 11,
                              1100 = ID = 12,
                              1101 = ID = 13,
                              1110 = ID = 14,
                              1111 = ID = 15 (default)
                            */
    UINT8 reserved : 4;
                            /* Reserved - Bits[7:4]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW92_STRUCT;

/**
  RW[93] - PBA Buffer Select ID Control Word
**/
#define DDR5_DATA_BUFFER_RW93_REG 0x93
typedef union {
  struct {
    UINT8 pba_select_id : 4;
                            /* PBA Select ID - Bits[3:0]
                              0000 = ID = 0,
                              0001 = ID = 1,
                              0010 = ID = 2,
                              0011 = ID = 3,
                              0100 = ID = 4,
                              0101 = ID = 5,
                              0110 = ID = 6,
                              0111 = ID = 7,
                              1000 = ID = 8,
                              1001 = ID = 9,
                              1010 = ID = 10,
                              1011 = ID = 11,
                              1100 = ID = 12,
                              1101 = ID = 13,
                              1110 = ID = 14,
                              1111 = ID = 15 Selects All Buffers
                            */
    UINT8 reserved : 4;
                            /* Reserved - Bits[7:4]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW93_STRUCT;

/**
  RW[97] - Buffer Training Configuration Control Word
**/
#define DDR5_DATA_BUFFER_RW97_REG 0x97
typedef union {
  struct {
    UINT8 clear_feedback_status_bit : 1;
                            /* Clear feedback status bit - Bits[0]
                              0 = Default,
                              1 = Clear feedback status (all DQ feedback returns to HIGH)
                            */
    UINT8 per_ui_filtering : 1;
                            /* Per UI Filtering - Bits[1]
                              0 = Disabled,
                              1 = Enabled
                            */
    UINT8 long_read_pattern_sticky_status_feedback_mode : 1;
                            /* Long Read pattern sticky status feedback mode - Bits[2]
                              0 = Enabled,
                              1 = Disabled
                            */
    UINT8 per_bit_vs_per_transaction : 1;
                            /* per-bit vs. per-transaction - Bits[3]
                              0 = Per-bit (default),
                              1 = Per-transaction (per-nibble)
                            */
    UINT8 select_1_of_16_uis_for_per : 4;
                            /* Select 1 of 16 UIs for Per UI Filtering - Bits[7:4]
                              0000 = UI-0,
                              0001 = UI-1,
                              0010 = UI-2,
                              0011 = UI-3,
                              0100 = UI-4,
                              0101 = UI-5,
                              0110 = UI-6,
                              0111 = UI-7,
                              1000 = UI-8,
                              1001 = UI-9,
                              1010 = UI-10,
                              1011 = UI-11,
                              1100 = UI-12,
                              1101 = UI-13,
                              1110 = UI-14,
                              1111 = UI-15
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW97_STRUCT;

/**
  RW[98] - Buffer Training Status Word
**/
#define DDR5_DATA_BUFFER_RW98_REG 0x98
typedef union {
  struct {
    UINT8 dq_0_status : 1;
                            /* DQ 0 status - Bits[0]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_1_status : 1;
                            /* DQ 1 status - Bits[1]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_2_status : 1;
                            /* DQ 2 status - Bits[2]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_3_status : 1;
                            /* DQ 3 status - Bits[3]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_4_status : 1;
                            /* DQ 4 status - Bits[4]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_5_status : 1;
                            /* DQ 5 status - Bits[5]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_6_status : 1;
                            /* DQ 6 status - Bits[6]
                              x = DQ status based on previous Read comparison
                            */
    UINT8 dq_7_status : 1;
                            /* DQ 7 status - Bits[7]
                              x = DQ status based on previous Read comparison
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW98_STRUCT;

/**
  RW[A0] - DFE Control Word Control Word
**/
#define DDR5_DATA_BUFFER_RWA0_REG 0xA0
typedef union {
  struct {
    UINT8 dfe_feature_enable_control : 1;
                            /* DFE Feature Enable Control - Bits[0]
                              0=(Default) DFE and gain features disabled,
                              1 = DFE and gain features enabled
                            */
    UINT8 dfe_vref_disable : 1;
                            /* DFE_VREF Disable - Bits[1]
                              0 = DFE_VREF Circuitry enabled,
                              1 = Disable DFE_VREF Circuitry disabled
                            */
    UINT8 error_counter_disable : 1;
                            /* ERROR Counter Disable - Bits[2]
                              0 = All ERROR Counters Enable,
                              1 = Disable All Error Counters
                            */
    UINT8 rw_control_word_writes_broadcasted : 1;
                            /* RW Control Word Writes Broadcasted - Bits[3]
                              0 = RW Write to select DQn,
                              1 = RW Writes Broadcast to DQ[7:0]
                            */
    UINT8 tap_1_enable_bit_for_all_dq : 1;
                            /* Tap 1 Enable Bit for all DQ[7:0] - Bits[4]
                              0=(Default) Tap 1 disabled,
                              1 = Tap 1 enabled
                            */
    UINT8 tap_2_enable_bit_for_all_dq : 1;
                            /* Tap 2 Enable Bit for all DQ[7:0] - Bits[5]
                              0=(Default) Tap 2 disabled,
                              1 = Tap 2 enabled
                            */
    UINT8 tap_3_enable_bit_for_all_dq : 1;
                            /* Tap 3 Enable Bit for all DQ[7:0] - Bits[6]
                              0=(Default) Tap 3 disabled,
                              1 = Tap 3 enabled
                            */
    UINT8 tap_4_enable_bit_for_all_dq : 1;
                            /* Tap 4 Enable Bit for all DQ[7:0] - Bits[7]
                              0=(Default) Tap 4 disabled,
                              1 = Tap 4 enabled
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RWA0_STRUCT;

/**
  RW[A1] - DQ[7:0] DFE Training Mode Control Word
**/
#define DDR5_DATA_BUFFER_RWA1_REG 0xA1
typedef union {
  struct {
    UINT8 global_dfe_training_mode_enable : 1;
                            /* Global DFE Training Mode Enable - Bits[0]
                              0 = DFE Training Mode disabled,
                              1 = DFE Training Mode enabled 2,3,
                            */
    UINT8 training_source : 2;
                            /* Training Source - Bits[2:1]
                              00 = Monitor,
                              01 = Monitor XOR Slicer Output,
                              10 = LFSR XOR,
                              11 = Reserved
                            */
    UINT8 dcn_dfe_pin_selection : 3;
                            /* DCn DFE pin selection - Bits[5:3]
                              000 = DQ0,
                              011 = DQ1,
                              000 = DQ2,
                              011 = DQ3,
                              100 = DQ4,
                              111 = DQ5,
                              100 = DQ6,
                              111 = DQ7
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RWA1_STRUCT;

/**
  RW[A2] - DQn DFE pin selection Control Word
**/
#define DDR5_DATA_BUFFER_RWA2_REG 0xA2
typedef union {
  struct {
    UINT8 dqn_dfe_pin_selection : 8;
                            /* DQn DFE pin selection - Bits[7:0]
                              0 = DQ0 - Not Selected,
                              1 = DQ0 - Selected,
                              0 = DQ1 - Not Selected,
                              1 = DQ1 - Selected,
                              0 = DQ2 - Not Selected,
                              1 = DQ2 - Selected,
                              0 = DQ3 - Not Selected,
                              1 = DQ3 - Selected,
                              0 = DQ4 - Not Selected,
                              1 = DQ4 - Selected,
                              0 = DQ5 - Not Selected,
                              1 = DQ5 - Selected,
                              0 = DQ6 - Not Selected,
                              1 = DQ6 - Selected,
                              0 = DQ7 - Not Selected,
                              1 = DQ7 - Selected
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RWA2_STRUCT;

/**
  RW[B0] - DRAM tDQS2DQ Tracking Control Control Word
**/
#define DDR5_DATA_BUFFER_RWB0_REG 0xB0
typedef union {
  struct {
    UINT8 tracking_mode : 2;
                            /* Tracking mode - Bits[1:0]
                              00 = Normal operation mode (default),
                              01 = DRAM tDQS2DQ tracking initialization mode enabled,
                              10 = DRAM tDQS2DQ tracking mode enabled,
                              11 = Reserved
                            */
    UINT8 tracking_mode_exit_option : 1;
                            /* Tracking mode exit option - Bits[2]
                              0 = The DB will exit the tracking mode as a function of OP[1:0] being set to 2b00 by the
                                host (default),
                              1 = The DB will exit the tracking mode automatically by resetting OP[1:0] to 2'b00 when the
                                calculation period for a DRAM DQS OSC Counter pair to a specified rank completes
                            */
    UINT8 db_dqs_interval_oscillator_disable : 1;
                            /* DB DQS interval oscillator disable - Bits[3]
                              0 = The DBs DQS interval oscillator is enabled (default). The DB will run its DQS interval
                                oscillator in response to a related DRAM MPC. The DB will return its DQS OSC Counter
                                values when the corresponding DRAM DQS OSC Counter values are read,
                              1 = The DBs DQS interval oscillator is disabled. The DB will not run its DQS interval
                                oscillator in response to related DRAM MPC. The DB will return all zeros in its data
                                payload when the DRAM DQS OSC Counter values are read.
                            */
    UINT8 reserved : 4;
                            /* Reserved - Bits[7:4]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RWB0_STRUCT;

/**
  RW[B1] - DRAM Interval Oscillator Snoop Value
**/
#define DDR5_DATA_BUFFER_RWB1_REG 0xB1
typedef union {
  struct {
    UINT8 the_dram_dqs_interval_oscillator_control : 1;
                            /* The DRAM DQS interval oscillator control. - Bits[0]
                              0 = The DQS interval timer stops when the related MPC command is received (default). The DQS
                                interval timer stop value is not used,
                              1 = The DQS interval timer stops automatically after the number of clocks specified in OP[7:1]
                            */
    UINT8 the_dram_dqs_interval_timer_stop_value : 2;
                            /* The DRAM DQS interval timer stop value. - Bits[2:1]
                              00 = Stop automatically at (OP[7:1] + 1) * 16tCK after start,
                              01 = Stop automatically at 1024tCK after start,
                              10 = Stop automatically at 2048tCK after start,
                              11 = Stop automatically at 4096tCK after start
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RWB1_STRUCT;

/**
  RW[B3] - DB Interval Oscillator Control
**/
#define DDR5_DATA_BUFFER_RWB3_REG 0xB3
typedef union {
  struct {
    UINT8 the_db_dqs_interval_oscillator_control : 1;
                            /* The DB DQS interval oscillator control - Bits[0]
                              0 = The DQS interval timer stops when the related MPC command is received (default). The DQS
                                interval timer stop value is not used,
                              1 = The DQS interval timer stops automatically after the number of clocks specified in OP[7:1]
                            */
    UINT8 the_db_dqs_interval_timer_stop_value : 2;
                            /* The DB DQS interval timer stop value - Bits[2:1]
                              00 = Stop automatically at (OP[7:1] + 1) * 16tCK after start.,
                              01 = Stop automatically at 1024tCK after start,
                              10 = Stop automatically at 2048tCK after start,
                              11 = Stop automatically at 4096tCK after start
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RWB3_STRUCT;

/**
  PG[0] RW[E0] - Lower/Upper Nibble Additional Cycles DRAM Interface Receive Enable Control Word for all Ranks
**/
#define DDR5_DATA_BUFFER_PG0_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 lower_nibble_additional_cycles_of_dram_interface : 4;
                            /* Lower Nibble Additional Cycles of DRAM Interface Receive Enable Delay - Bits[3:0]
                              0000 = 0 nCK receive enable timing latency adder.,
                              0001=+1 nCK receive enable timing latency adder,
                              0010=+2 nCK receive enable timing latency adder,
                              0011=+3 receive enable timing latency adder,
                              0100=+4 receive enable timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000 = Reserved,
                              1001=-1 nCK receive enable timing latency adder,
                              1010=-2 nCK receive enable timing latency adder,
                              1011=-3 receive enable timing latency adder,
                              1100=-4 receive enable timing latency adder,
                              1101 = Reserved,
                              ....,
                              1111 = Reserved
                            */
    UINT8 upper_nibble_additional_cycles_of_dram_interface : 4;
                            /* Upper Nibble Additional Cycles of DRAM Interface Receive Enable Delay - Bits[7:4]
                              0000 = 0 nCK receive enable timing latency adder.,
                              0001=+1 nCK receive enable timing latency adder,
                              0010=+2 nCK receive enable timing latency adder,
                              0011=+3 receive enable timing latency adder,
                              0100=+4 receive enable timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000xxxx = Reserved,
                              1001xxxx=-1 nCK receive enable timing latency adder,
                              1010xxxx=-2 nCK receive enable timing latency adder,
                              1011xxxx=-3 receive enable timing latency adder,
                              1100xxxx=-4 receive enable timing latency adder,
                              1101xxxx = Reserved,
                              ....,
                              1111xxxx = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE0_STRUCT;

/**
  PG[0] RW[E1] - Lower/Upper Nibble Additional Cycles DRAM Interface Write Leveling Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 lower_nibble_additional_cycles_of_dram_interface : 4;
                            /* Lower Nibble Additional Cycles of DRAM Interface Write Leveling Delay - Bits[3:0]
                              0000 = 0 nCK write leveling timing latency adder.,
                              0001=+1 nCK write leveling timing latency adder,
                              0010=+2 nCK write leveling timing latency adder,
                              0011=+3 nCK write leveling timing latency adder,
                              0100=+4 nCK write leveling timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000 = Reserved,
                              1001=-1 nCK write leveling timing latency adder,
                              1010=-2 nCK write leveling timing latency adder,
                              1011=-3 nCK write leveling timing latency adder,
                              1100=-4 nCK write leveling timing latency adder,
                              1101 = Reserved,
                              ....,
                              1111 = Reserved
                            */
    UINT8 upper_nibble_additional_cycles_of_dram_interface : 4;
                            /* Upper Nibble Additional Cycles of DRAM Interface Write Leveling Delay - Bits[7:4]
                              0000 = 0 nCK write leveling timing latency adder.,
                              0001=+1 nCK write leveling timing latency adder,
                              0010=+2 nCK write leveling timing latency adder,
                              0011=+3 nCK write leveling timing latency adder,
                              0100=+4 nCK write leveling timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000xxxx = Reserved,
                              1001xxxx=-1 nCK write leveling timing latency adder,
                              1010xxxx=-2 nCK write leveling timing latency adder,
                              1011xxxx=-3 nCK write leveling timing latency adder,
                              1100xxxx=-4 nCK write leveling timing latency adder,
                              1101xxxx = Reserved,
                              ....,
                              1111xxxx = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE1_STRUCT;

/**
  PG[0] RW[E2] - Lower Nibble DRAM Interface Receive Enable Training Control
**/
#define DDR5_DATA_BUFFER_PG0_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 dram_interface_receive_enable_timing_phase_control : 6;
                            /* DRAM Interface Receive Enable Timing Phase Control in Steps of (1/ 64) * t CK - Bits[5:0]
                              000000 = Delay MDQS receive enable timing by (0/64) * t CK,
                              000001 = Delay MDQS receive enable timing by (1/64) * t CK,
                              000010 = Delay MDQS receive enable timing by (2/64) * t CK,
                              000011 = Delay MDQS receive enable timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS receive enable timing by (60/64) * t CK,
                              111101 = Delay MDQS receive enable timing by (61/64) * t CK,
                              111110 = Delay MDQS receive enable timing by (62/64) * t CK,
                              111111 = Delay MDQS receive enable timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE2_STRUCT;

/**
  PG[0] RW[E3] - Upper Nibble DRAM Interface Receive Enable Training Control
**/
#define DDR5_DATA_BUFFER_PG0_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 dram_interface_receive_enable_timing_phase_control : 6;
                            /* DRAM Interface Receive Enable Timing Phase Control in Steps of (1/ 64) * t CK - Bits[5:0]
                              000000 = Delay MDQS receive enable timing by (0/64) * t CK,
                              000001 = Delay MDQS receive enable timing by (1/64) * t CK,
                              000010 = Delay MDQS receive enable timing by (2/64) * t CK,
                              000011 = Delay MDQS receive enable timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS receive enable timing by (60/64) * t CK,
                              111101 = Delay MDQS receive enable timing by (61/64) * t CK,
                              111110 = Delay MDQS receive enable timing by (62/64) * t CK,
                              111111 = Delay MDQS receive enable timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE3_STRUCT;

/**
  PG[0] RW[E4] - Lower Nibble MDQS Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWE4_REG 0xE4
typedef union {
  struct {
    UINT8 lower_nibble_mdqs_delay_control_during_read : 5;
                            /* Lower Nibble MDQS Delay Control During Read Transactions in Steps of (1/64) * t CK - Bits[4:0]
                              00000 = Delay MDQS by t CK /4 (Default),
                              00001 = Delay MDQS by t CK /4 + (1/64) * t CK,
                              00010 = Delay MDQS by t CK /4 + (2/64) * t CK,
                              00011 = Delay MDQS by t CK /4 + (3/64) * t CK,
                              00100 = Delay MDQS by t CK /4 + (4/64) * t CK,
                              00101 = Delay MDQS by t CK /4 + (5/64) * t CK,
                              00110 = Delay MDQS by t CK /4 + (6/64) * t CK,
                              00111 = Delay MDQS by t CK /4 + (7/64) * t CK,
                              01000 = Delay MDQS by t CK /4 + (8/64) * t CK,
                              01001 = Delay MDQS by t CK /4 + (9/64) * t CK,
                              01010 = Delay MDQS by t CK /4 + (10/64) * t CK,
                              01011 = Delay MDQS by t CK /4 + (11/64) * t CK,
                              01100 = Delay MDQS by t CK /4 + (12/64) * t CK,
                              01101 = Delay MDQS by t CK /4 + (13/64) * t CK,
                              01110 = Delay MDQS by t CK /4 + (14/64) * t CK,
                              01111 = Delay MDQS by t CK /4 + (15/64) * t CK,
                              10000 = Delay MDQS by t CK /4 (same as default),
                              10001 = Delay MDQS by t CK /4 - (1/64) * t CK,
                              10010 = Delay MDQS by t CK /4 - (2/64) * t CK,
                              10011 = Delay MDQS by t CK /4 - (3/64) * t CK,
                              10100 = Delay MDQS by t CK /4 - (4/64) * t CK,
                              10101 = Delay MDQS by t CK /4 - (5/64) * t CK,
                              10110 = Delay MDQS by t CK /4 - (6/64) * t CK,
                              10111 = Delay MDQS by t CK /4 - (7/64) * t CK,
                              11000 = Delay MDQS by t CK /4 - (8/64) * t CK,
                              11001 = Delay MDQS by t CK /4 - (9/64) * t CK,
                              11010 = Delay MDQS by t CK /4 - (10/64) * t CK,
                              11011 = Delay MDQS by t CK /4 - (11/64) * t CK,
                              11100 = Delay MDQS by t CK /4 - (12/64) * t CK,
                              11101 = Delay MDQS by t CK /4 - (13/64) * t CK,
                              11110 = Delay MDQS by t CK /4 - (14/64) * t CK,
                              11111 = Delay MDQS by t CK /4 - (15/64) * t CK
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE4_STRUCT;

/**
  PG[0] RW[E5] - Upper Nibble MDQS Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWE5_REG 0xE5
typedef union {
  struct {
    UINT8 upper_nibble_mdqs_delay_control_during_read : 5;
                            /* Upper Nibble MDQS Delay Control During Read Transactions in Steps of (1/64) * t CK - Bits[4:0]
                              00000 = Delay MDQS by t CK /4 (Default),
                              00001 = Delay MDQS by t CK /4 + (1/64) * t CK,
                              00010 = Delay MDQS by t CK /4 + (2/64) * t CK,
                              00011 = Delay MDQS by t CK /4 + (3/64) * t CK,
                              00100 = Delay MDQS by t CK /4 + (4/64) * t CK,
                              00101 = Delay MDQS by t CK /4 + (5/64) * t CK,
                              00110 = Delay MDQS by t CK /4 + (6/64) * t CK,
                              00111 = Delay MDQS by t CK /4 + (7/64) * t CK,
                              01000 = Delay MDQS by t CK /4 + (8/64) * t CK,
                              01001 = Delay MDQS by t CK /4 + (9/64) * t CK,
                              01010 = Delay MDQS by t CK /4 + (10/64) * t CK,
                              01011 = Delay MDQS by t CK /4 + (11/64) * t CK,
                              01100 = Delay MDQS by t CK /4 + (12/64) * t CK,
                              01101 = Delay MDQS by t CK /4 + (13/64) * t CK,
                              01110 = Delay MDQS by t CK /4 + (14/64) * t CK,
                              01111 = Delay MDQS by t CK /4 + (15/64) * t CK,
                              10000 = Delay MDQS by t CK /4 (same as default),
                              10001 = Delay MDQS by t CK /4 - (1/64) * t CK,
                              10010 = Delay MDQS by t CK /4 - (2/64) * t CK,
                              10011 = Delay MDQS by t CK /4 - (3/64) * t CK,
                              10100 = Delay MDQS by t CK /4 - (4/64) * t CK,
                              10101 = Delay MDQS by t CK /4 - (5/64) * t CK,
                              10110 = Delay MDQS by t CK /4 - (6/64) * t CK,
                              10111 = Delay MDQS by t CK /4 - (7/64) * t CK,
                              11000 = Delay MDQS by t CK /4 - (8/64) * t CK,
                              11001 = Delay MDQS by t CK /4 - (9/64) * t CK,
                              11010 = Delay MDQS by t CK /4 - (10/64) * t CK,
                              11011 = Delay MDQS by t CK /4 - (11/64) * t CK,
                              11100 = Delay MDQS by t CK /4 - (12/64) * t CK,
                              11101 = Delay MDQS by t CK /4 - (13/64) * t CK,
                              11110 = Delay MDQS by t CK /4 - (14/64) * t CK,
                              11111 = Delay MDQS by t CK /4 - (15/64) * t CK
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE5_STRUCT;

/**
  PG[0] RW[E6] - Lower Nibble MDQ Write Baseline Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWE6_REG 0xE6
typedef union {
  struct {
    UINT8 phase_control_for_lower_nibble_mdq_with : 8;
                            /* Phase Control for Lower Nibble MDQ with respect to MDQS During Write Transactions in
                               Steps of (1/64) * t CK - Bits[7:0]
                              00000000 = MDQ to MDQS_t phase delay = - t CK /2,
                              00000001 = MDQ to MDQS_t phase delay = - t CK /2 + (1/64) * t CK,
                              00000010 = MDQ to MDQS_t phase delay = - t CK /2 + (2/64) * t CK,
                              00000011 = MDQ to MDQS_t phase delay = - t CK /2 + (3/64) * t CK,
                              00000100 = MDQ to MDQS_t phase delay = - t CK /2 + (4/64) * t CK,
                              00000101 = MDQ to MDQS_t phase delay = - t CK /2 + (5/64) * t CK,
                              00000110 = MDQ to MDQS_t phase delay = - t CK /2 + (6/64) * t CK,
                              00000111 = MDQ to MDQS_t phase delay = - t CK /2 + (7/64) * t CK,
                              00001000 = MDQ to MDQS_t phase delay = - t CK /2 + (8/64) * t CK,
                              00001001 = MDQ to MDQS_t phase delay = - t CK /2 + (9/64) * t CK,
                              00001010 = MDQ to MDQS_t phase delay = - t CK /2 + (10/64) * t CK,
                              00001011 = MDQ to MDQS_t phase delay = - t CK /2 + (11/64) * t CK,
                              00001100 = MDQ to MDQS_t phase delay = - t CK /2 + (12/64) * t CK,
                              00001101 = MDQ to MDQS_t phase delay = - t CK /2 + (13/64) * t CK,
                              00001110 = MDQ to MDQS_t phase delay = - t CK /2 + (14/64) * t CK,
                              00001111 = MDQ to MDQS_t phase delay = - t CK /2 + (15/64) * t CK,
                              10000 = MDQ to MDQS_t phase delay = - t CK /2 + (16/64) Default,
                              ...,
                              ...,
                              ...,
                              11110001 = MDQ to MDQS_t phase delay = - t CK /2 + (241/64) * t CK,
                              11110010 = MDQ to MDQS_t phase delay = - t CK /2 + (242/64) * t CK,
                              11110011 = MDQ to MDQS_t phase delay = - t CK /2 + (243/64) * t CK,
                              11110100 = MDQ to MDQS_t phase delay = - t CK /2 + (244/64) * t CK,
                              11110101 = MDQ to MDQS_t phase delay = - t CK /2 + (245/64) * t CK,
                              11110110 = MDQ to MDQS_t phase delay = - t CK /2 + (246/64) * t CK,
                              11110111 = MDQ to MDQS_t phase delay = - t CK /2 + (247/64) * t CK,
                              11111000 = MDQ to MDQS_t phase delay = - t CK /2 + (248/64) * t CK,
                              11111001 = MDQ to MDQS_t phase delay = - t CK /2 + (249/64) * t CK,
                              11111010 = MDQ to MDQS_t phase delay = - t CK /2 + (250/64) * t CK,
                              11111011 = MDQ to MDQS_t phase delay = - t CK /2 + (251/64) * t CK,
                              11111100 = MDQ to MDQS_t phase delay = - t CK /2 + (252/64) * t CK,
                              11111101 = MDQ to MDQS_t phase delay = - t CK /2 + (253/64) * t CK,
                              11111110 = MDQ to MDQS_t phase delay = - t CK /2 + (254/64) * t CK,
                              11111111 = MDQ to MDQS_t phase delay = - t CK /2 + (255/64) * t CK
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE6_STRUCT;

/**
  PG[0] RW[E7] - Upper Nibble MDQWrite Baseline Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWE7_REG 0xE7
typedef union {
  struct {
    UINT8 phase_control_for_upper_nibble_mdq_with : 8;
                            /* Phase Control for Upper Nibble MDQ with respect to MDQS During Write Transactions in
                               Steps of (1/ 64) * t CK1,2 - Bits[7:0]
                              00000000 = MDQ to MDQS_t phase delay = - t CK /2,
                              00000001 = MDQ to MDQS_t phase delay = - t CK /2 + (1/64) * t CK,
                              00000010 = MDQ to MDQS_t phase delay = - t CK /2 + (2/64) * t CK,
                              00000011 = MDQ to MDQS_t phase delay = - t CK /2 + (3/64) * t CK,
                              00000100 = MDQ to MDQS_t phase delay = - t CK /2 + (4/64) * t CK,
                              00000101 = MDQ to MDQS_t phase delay = - t CK /2 + (5/64) * t CK,
                              00000110 = MDQ to MDQS_t phase delay = - t CK /2 + (6/64) * t CK,
                              00000111 = MDQ to MDQS_t phase delay = - t CK /2 + (7/64) * t CK,
                              00001000 = MDQ to MDQS_t phase delay = - t CK /2 + (8/64) * t CK,
                              00001001 = MDQ to MDQS_t phase delay = - t CK /2 + (9/64) * t CK,
                              00001010 = MDQ to MDQS_t phase delay = - t CK /2 + (10/64) * t CK,
                              00001011 = MDQ to MDQS_t phase delay = - t CK /2 + (11/64) * t CK,
                              00001100 = MDQ to MDQS_t phase delay = - t CK /2 + (12/64) * t CK,
                              00001101 = MDQ to MDQS_t phase delay = - t CK /2 + (13/64) * t CK,
                              00001110 = MDQ to MDQS_t phase delay = - t CK /2 + (14/64) * t CK,
                              00001111 = MDQ to MDQS_t phase delay = - t CK /2 + (15/64) * t CK,
                              ...,
                              ...,
                              ...,
                              11110001=...,
                              11110010 = MDQ to MDQS_t phase delay = - t CK /2 + (241/64) * t CK,
                              11110011 = MDQ to MDQS_t phase delay = - t CK /2 + (242/64) * t CK,
                              11110100 = MDQ to MDQS_t phase delay = - t CK /2 + (243/64) * t CK,
                              11110101 = MDQ to MDQS_t phase delay = - t CK /2 + (244/64) * t CK,
                              11110110 = MDQ to MDQS_t phase delay = - t CK /2 + (245/64) * t CK,
                              11110111 = MDQ to MDQS_t phase delay = - t CK /2 + (246/64) * t CK,
                              11111000 = MDQ to MDQS_t phase delay = - t CK /2 + (247/64) * t CK,
                              11111001 = MDQ to MDQS_t phase delay = - t CK /2 + (248/64) * t CK,
                              11111010 = MDQ to MDQS_t phase delay = - t CK /2 + (249/64) * t CK,
                              11111011 = MDQ to MDQS_t phase delay = - t CK /2 + (250/64) * t CK,
                              11111100 = MDQ to MDQS_t phase delay = - t CK /2 + (251/64) * t CK,
                              11111101 = MDQ to MDQS_t phase delay = - t CK /2 + (252/64) * t CK,
                              11111110 = MDQ to MDQS_t phase delay = - t CK /2 + (253/64) * t CK,
                              11111111 = MDQ to MDQS_t phase delay = - t CK /2 + (254/64) * t CK
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE7_STRUCT;

/**
  PG[0] RW[E8] - Lower Nibble DRAM Interface Write Leveling Control Word (per
**/
#define DDR5_DATA_BUFFER_PG0_RWE8_REG 0xE8
typedef union {
  struct {
    UINT8 dram_interface_write_leveling_control_in_steps : 6;
                            /* DRAM Interface Write Leveling Control in Steps of (1/64) * t CK - Bits[5:0]
                              000000 = Delay MDQS write leveling timing by (0/64) * t CK,
                              000001 = Delay MDQS write leveling timing by (1/64) * t CK,
                              000010 = Delay MDQS write leveling timing by (2/64) * t CK,
                              000011 = Delay MDQS write leveling timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS write leveling timing by (60/64) * t CK,
                              111101 = Delay MDQS write leveling timing by (61/64) * t CK,
                              111110 = Delay MDQS write leveling timing by (62/64) * t CK,
                              111111 = Delay MDQS write leveling timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE8_STRUCT;

/**
  PG[0] RW[E9] - Upper Nibble DRAM Interface Write Leveling Control Word (per rank)
**/
#define DDR5_DATA_BUFFER_PG0_RWE9_REG 0xE9
typedef union {
  struct {
    UINT8 dram_interface_write_leveling_timing_phase_control : 6;
                            /* DRAM Interface Write Leveling Timing Phase Control in Steps of (1/ 64) * t CK - Bits[5:0]
                              000000 = Delay MDQS write leveling timing by (0/64) * t CK,
                              000001 = Delay MDQS write leveling timing by (1/64) * t CK,
                              000010 = Delay MDQS write leveling timing by (2/64) * t CK,
                              000011 = Delay MDQS write leveling timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS write leveling timing by (60/64) * t CK,
                              111101 = Delay MDQS write leveling timing by (61/64) * t CK,
                              111110 = Delay MDQS write leveling timing by (62/64) * t CK,
                              111111 = Delay MDQS write leveling timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWE9_STRUCT;

/**
  PG[0] RW[EA] - MDQ0/4 Read Delay Control Word.
**/
#define DDR5_DATA_BUFFER_PG0_RWEA_REG 0xEA
typedef union {
  struct {
    UINT8 phase_control_between_mdq0_and_lower_nibble : 3;
                            /* Phase Control Between MDQ0 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ0 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ0 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ0 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ0 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ0 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ0 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ0 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ0 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq4_and_upper_nibble : 3;
                            /* Phase Control Between MDQ4 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ4 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ4 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ4 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ4 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ4 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ4 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ4 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ4 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWEA_STRUCT;

/**
  PG[0] RW[EB] - MDQ1/5 Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWEB_REG 0xEB
typedef union {
  struct {
    UINT8 phase_control_between_mdq1_and_lower_nibble : 3;
                            /* Phase Control Between MDQ1 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ1 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ1 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ1 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ1 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ1 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ1 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ1 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ1 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq5_and_upper_nibble : 3;
                            /* Phase Control Between MDQ5 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ5 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ5with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ5 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ5 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ5with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ5 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ5 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ5 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWEB_STRUCT;

/**
  PG[0] RW[EC] - MDQ2/6 Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWEC_REG 0xEC
typedef union {
  struct {
    UINT8 phase_control_between_mdq2_and_lower_nibble : 3;
                            /* Phase Control Between MDQ2 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ2 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ2 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ2 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ2 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ2 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ2 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ2 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ2 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq6_and_upper_nibble : 3;
                            /* Phase Control Between MDQ6 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ6 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ6 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ6with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ6 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ6 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ6 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ6 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ6 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWEC_STRUCT;

/**
  PG[0] RW[ED] - MDQ3/7 Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWED_REG 0xED
typedef union {
  struct {
    UINT8 phase_control_between_mdq3_and_lower_nibble : 3;
                            /* Phase Control Between MDQ3 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ3 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ3 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ3 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ3 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ3 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ3 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ3 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ3 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq7_and_upper_nibble : 3;
                            /* Phase Control Between MDQ7 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ7 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ7 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ7 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ7 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ7 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ7 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ7 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ7 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWED_STRUCT;

/**
  PG[0] RW[EE] - MDQ1/5 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWEE_REG 0xEE
typedef union {
  struct {
    UINT8 phase_control_between_mdq1_and_lower_nibble : 3;
                            /* Phase Control Between MDQ1 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ1 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ1 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ1 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ1 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ1 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ1 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ1 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ1 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq5_and_upper_nibble : 3;
                            /* Phase Control Between MDQ5 and Upper Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ5 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ5 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ5 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ5 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ5 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ5 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ5 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ5 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWEE_STRUCT;

/**
  PG[0] RW[EF] - MDQ2/6 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWEF_REG 0xEF
typedef union {
  struct {
    UINT8 phase_control_between_mdq2_and_lower_nibble : 3;
                            /* Phase Control Between MDQ2 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ2 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ2 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ2 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ02with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ2 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ2 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ2 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ2 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq6_and_upper_nibble : 3;
                            /* Phase Control Between MDQ6 and Upper Nibble Baseline MDQ-MDQS Delay During Write
                               Transactions in Steps of (1/ 64) * t CK - Bits[6:4]
                              000 = Delay MDQ6 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ6 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ6 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ6 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ6 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ6 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ6 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ6 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWEF_STRUCT;

/**
  PG[0] RW[F0] - MDQ3/7 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWF0_REG 0xF0
typedef union {
  struct {
    UINT8 phase_control_between_mdq3_and_lower_nibble : 3;
                            /* Phase Control Between MDQ3 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ3 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ3 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ3 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ3with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ3 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ3 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ3 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ3 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq7_and_upper_nibble : 3;
                            /* Phase Control Between MDQ7 and Upper Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ7 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ7 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ7 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ7 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ7 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ7 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ7 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ7 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWF0_STRUCT;

/**
  PG[0] RW[F1] - MDQ3/7 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG0_RWF1_REG 0xF1
typedef union {
  struct {
    UINT8 phase_control_between_mdq3_and_lower_nibble : 3;
                            /* Phase Control Between MDQ3 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ3 with respect to baseline by +0/64 * t CK (Default),
                              001 = Delay MDQ3 with respect to baseline by +1/64 * t CK,
                              010 = Delay MDQ3 with respect to baseline by +2/64 * t CK,
                              011 = Delay MDQ3with respect to baseline by +3/64 * t CK,
                              100 = Delay MDQ3 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ3 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ3 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ3 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq7_and_upper_nibble : 3;
                            /* Phase Control Between MDQ7 and Upper Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ7 with respect to baseline by +0/64 * t CK (Default),
                              001 = Delay MDQ7 with respect to baseline by +1/64 * t CK,
                              010 = Delay MDQ7 with respect to baseline by +2/64 * t CK,
                              011 = Delay MDQ7 with respect to baseline by +3/64 * t CK,
                              100 = Delay MDQ7 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ7 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ7 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ7 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG0_RWF1_STRUCT;

/**
  PG[1] RW[E0] - Lower/Upper Nibble Additional Cycles DRAM Interface Receive Enable Control Word for all Ranks
**/
#define DDR5_DATA_BUFFER_PG1_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 lower_nibble_additional_cycles_of_dram_interface : 4;
                            /* Lower Nibble Additional Cycles of DRAM Interface Receive Enable Delay - Bits[3:0]
                              0000 = 0 nCK receive enable timing latency adder.,
                              0001=+1 nCK receive enable timing latency adder,
                              0010=+2 nCK receive enable timing latency adder,
                              0011=+3 receive enable timing latency adder,
                              0100=+4 receive enable timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000 = Reserved,
                              1001=-1 nCK receive enable timing latency adder,
                              1010=-2 nCK receive enable timing latency adder,
                              1011=-3 receive enable timing latency adder,
                              1100=-4 receive enable timing latency adder,
                              1101 = Reserved,
                              ....,
                              1111 = Reserved
                            */
    UINT8 upper_nibble_additional_cycles_of_dram_interface : 4;
                            /* Upper Nibble Additional Cycles of DRAM Interface Receive Enable Delay - Bits[7:4]
                              0000 = 0 nCK receive enable timing latency adder.,
                              0001=+1 nCK receive enable timing latency adder,
                              0010=+2 nCK receive enable timing latency adder,
                              0011=+3 receive enable timing latency adder,
                              0100=+4 receive enable timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000xxxx = Reserved,
                              1001xxxx=-1 nCK receive enable timing latency adder,
                              1010xxxx=-2 nCK receive enable timing latency adder,
                              1011xxxx=-3 receive enable timing latency adder,
                              1100xxxx=-4 receive enable timing latency adder,
                              1101xxxx = Reserved,
                              ....,
                              1111xxxx = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE0_STRUCT;

/**
  PG[1] RW[E1] - Lower/Upper Nibble Additional Cycles DRAM Interface Write Leveling Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 lower_nibble_additional_cycles_of_dram_interface : 4;
                            /* Lower Nibble Additional Cycles of DRAM Interface Write Leveling Delay - Bits[3:0]
                              0000 = 0 nCK write leveling timing latency adder.,
                              0001=+1 nCK write leveling timing latency adder,
                              0010=+2 nCK write leveling timing latency adder,
                              0011=+3 nCK write leveling timing latency adder,
                              0100=+4 nCK write leveling timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000 = Reserved,
                              1001=-1 nCK write leveling timing latency adder,
                              1010=-2 nCK write leveling timing latency adder,
                              1011=-3 nCK write leveling timing latency adder,
                              1100=-4 nCK write leveling timing latency adder,
                              1101 = Reserved,
                              ....,
                              1111 = Reserved
                            */
    UINT8 upper_nibble_additional_cycles_of_dram_interface : 4;
                            /* Upper Nibble Additional Cycles of DRAM Interface Write Leveling Delay - Bits[7:4]
                              0000 = 0 nCK write leveling timing latency adder.,
                              0001=+1 nCK write leveling timing latency adder,
                              0010=+2 nCK write leveling timing latency adder,
                              0011=+3 nCK write leveling timing latency adder,
                              0100=+4 nCK write leveling timing latency adder,
                              0101 = Reserved,
                              ....,
                              1000xxxx = Reserved,
                              1001xxxx=-1 nCK write leveling timing latency adder,
                              1010xxxx=-2 nCK write leveling timing latency adder,
                              1011xxxx=-3 nCK write leveling timing latency adder,
                              1100xxxx=-4 nCK write leveling timing latency adder,
                              1101xxxx = Reserved,
                              ....,
                              1111xxxx = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE1_STRUCT;

/**
  PG[1] RW[E2] - Lower Nibble DRAM Interface Receive Enable Training Control
**/
#define DDR5_DATA_BUFFER_PG1_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 dram_interface_receive_enable_timing_phase_control : 6;
                            /* DRAM Interface Receive Enable Timing Phase Control in Steps of (1/ 64) * t CK - Bits[5:0]
                              000000 = Delay MDQS receive enable timing by (0/64) * t CK,
                              000001 = Delay MDQS receive enable timing by (1/64) * t CK,
                              000010 = Delay MDQS receive enable timing by (2/64) * t CK,
                              000011 = Delay MDQS receive enable timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS receive enable timing by (60/64) * t CK,
                              111101 = Delay MDQS receive enable timing by (61/64) * t CK,
                              111110 = Delay MDQS receive enable timing by (62/64) * t CK,
                              111111 = Delay MDQS receive enable timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE2_STRUCT;

/**
  PG[1] RW[E3] - Upper Nibble DRAM Interface Receive Enable Training Control
**/
#define DDR5_DATA_BUFFER_PG1_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 dram_interface_receive_enable_timing_phase_control : 6;
                            /* DRAM Interface Receive Enable Timing Phase Control in Steps of (1/ 64) * t CK - Bits[5:0]
                              000000 = Delay MDQS receive enable timing by (0/64) * t CK,
                              000001 = Delay MDQS receive enable timing by (1/64) * t CK,
                              000010 = Delay MDQS receive enable timing by (2/64) * t CK,
                              000011 = Delay MDQS receive enable timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS receive enable timing by (60/64) * t CK,
                              111101 = Delay MDQS receive enable timing by (61/64) * t CK,
                              111110 = Delay MDQS receive enable timing by (62/64) * t CK,
                              111111 = Delay MDQS receive enable timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE3_STRUCT;

/**
  PG[1] RW[E4] - Lower Nibble MDQS Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWE4_REG 0xE4
typedef union {
  struct {
    UINT8 lower_nibble_mdqs_delay_control_during_read : 5;
                            /* Lower Nibble MDQS Delay Control During Read Transactions in Steps of (1/64) * t CK - Bits[4:0]
                              00000 = Delay MDQS by t CK /4 (Default),
                              00001 = Delay MDQS by t CK /4 + (1/64) * t CK,
                              00010 = Delay MDQS by t CK /4 + (2/64) * t CK,
                              00011 = Delay MDQS by t CK /4 + (3/64) * t CK,
                              00100 = Delay MDQS by t CK /4 + (4/64) * t CK,
                              00101 = Delay MDQS by t CK /4 + (5/64) * t CK,
                              00110 = Delay MDQS by t CK /4 + (6/64) * t CK,
                              00111 = Delay MDQS by t CK /4 + (7/64) * t CK,
                              01000 = Delay MDQS by t CK /4 + (8/64) * t CK,
                              01001 = Delay MDQS by t CK /4 + (9/64) * t CK,
                              01010 = Delay MDQS by t CK /4 + (10/64) * t CK,
                              01011 = Delay MDQS by t CK /4 + (11/64) * t CK,
                              01100 = Delay MDQS by t CK /4 + (12/64) * t CK,
                              01101 = Delay MDQS by t CK /4 + (13/64) * t CK,
                              01110 = Delay MDQS by t CK /4 + (14/64) * t CK,
                              01111 = Delay MDQS by t CK /4 + (15/64) * t CK,
                              10000 = Delay MDQS by t CK /4 (same as default),
                              10001 = Delay MDQS by t CK /4 - (1/64) * t CK,
                              10010 = Delay MDQS by t CK /4 - (2/64) * t CK,
                              10011 = Delay MDQS by t CK /4 - (3/64) * t CK,
                              10100 = Delay MDQS by t CK /4 - (4/64) * t CK,
                              10101 = Delay MDQS by t CK /4 - (5/64) * t CK,
                              10110 = Delay MDQS by t CK /4 - (6/64) * t CK,
                              10111 = Delay MDQS by t CK /4 - (7/64) * t CK,
                              11000 = Delay MDQS by t CK /4 - (8/64) * t CK,
                              11001 = Delay MDQS by t CK /4 - (9/64) * t CK,
                              11010 = Delay MDQS by t CK /4 - (10/64) * t CK,
                              11011 = Delay MDQS by t CK /4 - (11/64) * t CK,
                              11100 = Delay MDQS by t CK /4 - (12/64) * t CK,
                              11101 = Delay MDQS by t CK /4 - (13/64) * t CK,
                              11110 = Delay MDQS by t CK /4 - (14/64) * t CK,
                              11111 = Delay MDQS by t CK /4 - (15/64) * t CK
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE4_STRUCT;

/**
  PG[1] RW[E5] - Upper Nibble MDQS Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWE5_REG 0xE5
typedef union {
  struct {
    UINT8 upper_nibble_mdqs_delay_control_during_read : 5;
                            /* Upper Nibble MDQS Delay Control During Read Transactions in Steps of (1/64) * t CK - Bits[4:0]
                              00000 = Delay MDQS by t CK /4 (Default),
                              00001 = Delay MDQS by t CK /4 + (1/64) * t CK,
                              00010 = Delay MDQS by t CK /4 + (2/64) * t CK,
                              00011 = Delay MDQS by t CK /4 + (3/64) * t CK,
                              00100 = Delay MDQS by t CK /4 + (4/64) * t CK,
                              00101 = Delay MDQS by t CK /4 + (5/64) * t CK,
                              00110 = Delay MDQS by t CK /4 + (6/64) * t CK,
                              00111 = Delay MDQS by t CK /4 + (7/64) * t CK,
                              01000 = Delay MDQS by t CK /4 + (8/64) * t CK,
                              01001 = Delay MDQS by t CK /4 + (9/64) * t CK,
                              01010 = Delay MDQS by t CK /4 + (10/64) * t CK,
                              01011 = Delay MDQS by t CK /4 + (11/64) * t CK,
                              01100 = Delay MDQS by t CK /4 + (12/64) * t CK,
                              01101 = Delay MDQS by t CK /4 + (13/64) * t CK,
                              01110 = Delay MDQS by t CK /4 + (14/64) * t CK,
                              01111 = Delay MDQS by t CK /4 + (15/64) * t CK,
                              10000 = Delay MDQS by t CK /4 (same as default),
                              10001 = Delay MDQS by t CK /4 - (1/64) * t CK,
                              10010 = Delay MDQS by t CK /4 - (2/64) * t CK,
                              10011 = Delay MDQS by t CK /4 - (3/64) * t CK,
                              10100 = Delay MDQS by t CK /4 - (4/64) * t CK,
                              10101 = Delay MDQS by t CK /4 - (5/64) * t CK,
                              10110 = Delay MDQS by t CK /4 - (6/64) * t CK,
                              10111 = Delay MDQS by t CK /4 - (7/64) * t CK,
                              11000 = Delay MDQS by t CK /4 - (8/64) * t CK,
                              11001 = Delay MDQS by t CK /4 - (9/64) * t CK,
                              11010 = Delay MDQS by t CK /4 - (10/64) * t CK,
                              11011 = Delay MDQS by t CK /4 - (11/64) * t CK,
                              11100 = Delay MDQS by t CK /4 - (12/64) * t CK,
                              11101 = Delay MDQS by t CK /4 - (13/64) * t CK,
                              11110 = Delay MDQS by t CK /4 - (14/64) * t CK,
                              11111 = Delay MDQS by t CK /4 - (15/64) * t CK
                            */
    UINT8 reserved : 3;
                            /* Reserved - Bits[7:5]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE5_STRUCT;

/**
  PG[1] RW[E6] - Lower Nibble MDQ Write Baseline Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWE6_REG 0xE6
typedef union {
  struct {
    UINT8 phase_control_for_lower_nibble_mdq_with : 8;
                            /* Phase Control for Lower Nibble MDQ with respect to MDQS During Write Transactions in
                               Steps of (1/64) * t CK - Bits[7:0]
                              00000000 = MDQ to MDQS_t phase delay = - t CK /2,
                              00000001 = MDQ to MDQS_t phase delay = - t CK /2 + (1/64) * t CK,
                              00000010 = MDQ to MDQS_t phase delay = - t CK /2 + (2/64) * t CK,
                              00000011 = MDQ to MDQS_t phase delay = - t CK /2 + (3/64) * t CK,
                              00000100 = MDQ to MDQS_t phase delay = - t CK /2 + (4/64) * t CK,
                              00000101 = MDQ to MDQS_t phase delay = - t CK /2 + (5/64) * t CK,
                              00000110 = MDQ to MDQS_t phase delay = - t CK /2 + (6/64) * t CK,
                              00000111 = MDQ to MDQS_t phase delay = - t CK /2 + (7/64) * t CK,
                              00001000 = MDQ to MDQS_t phase delay = - t CK /2 + (8/64) * t CK,
                              00001001 = MDQ to MDQS_t phase delay = - t CK /2 + (9/64) * t CK,
                              00001010 = MDQ to MDQS_t phase delay = - t CK /2 + (10/64) * t CK,
                              00001011 = MDQ to MDQS_t phase delay = - t CK /2 + (11/64) * t CK,
                              00001100 = MDQ to MDQS_t phase delay = - t CK /2 + (12/64) * t CK,
                              00001101 = MDQ to MDQS_t phase delay = - t CK /2 + (13/64) * t CK,
                              00001110 = MDQ to MDQS_t phase delay = - t CK /2 + (14/64) * t CK,
                              00001111 = MDQ to MDQS_t phase delay = - t CK /2 + (15/64) * t CK,
                              10000 = MDQ to MDQS_t phase delay = - t CK /2 + (16/64) Default,
                              ...,
                              ...,
                              ...,
                              11110001 = MDQ to MDQS_t phase delay = - t CK /2 + (241/64) * t CK,
                              11110010 = MDQ to MDQS_t phase delay = - t CK /2 + (242/64) * t CK,
                              11110011 = MDQ to MDQS_t phase delay = - t CK /2 + (243/64) * t CK,
                              11110100 = MDQ to MDQS_t phase delay = - t CK /2 + (244/64) * t CK,
                              11110101 = MDQ to MDQS_t phase delay = - t CK /2 + (245/64) * t CK,
                              11110110 = MDQ to MDQS_t phase delay = - t CK /2 + (246/64) * t CK,
                              11110111 = MDQ to MDQS_t phase delay = - t CK /2 + (247/64) * t CK,
                              11111000 = MDQ to MDQS_t phase delay = - t CK /2 + (248/64) * t CK,
                              11111001 = MDQ to MDQS_t phase delay = - t CK /2 + (249/64) * t CK,
                              11111010 = MDQ to MDQS_t phase delay = - t CK /2 + (250/64) * t CK,
                              11111011 = MDQ to MDQS_t phase delay = - t CK /2 + (251/64) * t CK,
                              11111100 = MDQ to MDQS_t phase delay = - t CK /2 + (252/64) * t CK,
                              11111101 = MDQ to MDQS_t phase delay = - t CK /2 + (253/64) * t CK,
                              11111110 = MDQ to MDQS_t phase delay = - t CK /2 + (254/64) * t CK,
                              11111111 = MDQ to MDQS_t phase delay = - t CK /2 + (255/64) * t CK
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE6_STRUCT;

/**
  PG[1] RW[E7] - Upper Nibble MDQWrite Baseline Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWE7_REG 0xE7
typedef union {
  struct {
    UINT8 phase_control_for_upper_nibble_mdq_with : 8;
                            /* Phase Control for Upper Nibble MDQ with respect to MDQS During Write Transactions in
                               Steps of (1/ 64) * t CK1,2 - Bits[7:0]
                              00000000 = MDQ to MDQS_t phase delay = - t CK /2,
                              00000001 = MDQ to MDQS_t phase delay = - t CK /2 + (1/64) * t CK,
                              00000010 = MDQ to MDQS_t phase delay = - t CK /2 + (2/64) * t CK,
                              00000011 = MDQ to MDQS_t phase delay = - t CK /2 + (3/64) * t CK,
                              00000100 = MDQ to MDQS_t phase delay = - t CK /2 + (4/64) * t CK,
                              00000101 = MDQ to MDQS_t phase delay = - t CK /2 + (5/64) * t CK,
                              00000110 = MDQ to MDQS_t phase delay = - t CK /2 + (6/64) * t CK,
                              00000111 = MDQ to MDQS_t phase delay = - t CK /2 + (7/64) * t CK,
                              00001000 = MDQ to MDQS_t phase delay = - t CK /2 + (8/64) * t CK,
                              00001001 = MDQ to MDQS_t phase delay = - t CK /2 + (9/64) * t CK,
                              00001010 = MDQ to MDQS_t phase delay = - t CK /2 + (10/64) * t CK,
                              00001011 = MDQ to MDQS_t phase delay = - t CK /2 + (11/64) * t CK,
                              00001100 = MDQ to MDQS_t phase delay = - t CK /2 + (12/64) * t CK,
                              00001101 = MDQ to MDQS_t phase delay = - t CK /2 + (13/64) * t CK,
                              00001110 = MDQ to MDQS_t phase delay = - t CK /2 + (14/64) * t CK,
                              00001111 = MDQ to MDQS_t phase delay = - t CK /2 + (15/64) * t CK,
                              ...,
                              ...,
                              ...,
                              11110001=...,
                              11110010 = MDQ to MDQS_t phase delay = - t CK /2 + (241/64) * t CK,
                              11110011 = MDQ to MDQS_t phase delay = - t CK /2 + (242/64) * t CK,
                              11110100 = MDQ to MDQS_t phase delay = - t CK /2 + (243/64) * t CK,
                              11110101 = MDQ to MDQS_t phase delay = - t CK /2 + (244/64) * t CK,
                              11110110 = MDQ to MDQS_t phase delay = - t CK /2 + (245/64) * t CK,
                              11110111 = MDQ to MDQS_t phase delay = - t CK /2 + (246/64) * t CK,
                              11111000 = MDQ to MDQS_t phase delay = - t CK /2 + (247/64) * t CK,
                              11111001 = MDQ to MDQS_t phase delay = - t CK /2 + (248/64) * t CK,
                              11111010 = MDQ to MDQS_t phase delay = - t CK /2 + (249/64) * t CK,
                              11111011 = MDQ to MDQS_t phase delay = - t CK /2 + (250/64) * t CK,
                              11111100 = MDQ to MDQS_t phase delay = - t CK /2 + (251/64) * t CK,
                              11111101 = MDQ to MDQS_t phase delay = - t CK /2 + (252/64) * t CK,
                              11111110 = MDQ to MDQS_t phase delay = - t CK /2 + (253/64) * t CK,
                              11111111 = MDQ to MDQS_t phase delay = - t CK /2 + (254/64) * t CK
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE7_STRUCT;

/**
  PG[1] RW[E8] - Lower Nibble DRAM Interface Write Leveling Control Word (per
**/
#define DDR5_DATA_BUFFER_PG1_RWE8_REG 0xE8
typedef union {
  struct {
    UINT8 dram_interface_write_leveling_control_in_steps : 6;
                            /* DRAM Interface Write Leveling Control in Steps of (1/64) * t CK - Bits[5:0]
                              000000 = Delay MDQS write leveling timing by (0/64) * t CK,
                              000001 = Delay MDQS write leveling timing by (1/64) * t CK,
                              000010 = Delay MDQS write leveling timing by (2/64) * t CK,
                              000011 = Delay MDQS write leveling timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS write leveling timing by (60/64) * t CK,
                              111101 = Delay MDQS write leveling timing by (61/64) * t CK,
                              111110 = Delay MDQS write leveling timing by (62/64) * t CK,
                              111111 = Delay MDQS write leveling timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE8_STRUCT;

/**
  PG[1] RW[E9] - Upper Nibble DRAM Interface Write Leveling Control Word (per rank)
**/
#define DDR5_DATA_BUFFER_PG1_RWE9_REG 0xE9
typedef union {
  struct {
    UINT8 dram_interface_write_leveling_timing_phase_control : 6;
                            /* DRAM Interface Write Leveling Timing Phase Control in Steps of (1/ 64) * t CK - Bits[5:0]
                              000000 = Delay MDQS write leveling timing by (0/64) * t CK,
                              000001 = Delay MDQS write leveling timing by (1/64) * t CK,
                              000010 = Delay MDQS write leveling timing by (2/64) * t CK,
                              000011 = Delay MDQS write leveling timing by (3/64) * t CK,
                              ...,
                              111100 = Delay MDQS write leveling timing by (60/64) * t CK,
                              111101 = Delay MDQS write leveling timing by (61/64) * t CK,
                              111110 = Delay MDQS write leveling timing by (62/64) * t CK,
                              111111 = Delay MDQS write leveling timing by (63/64) * t CK
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[7:6]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWE9_STRUCT;

/**
  PG[1] RW[EA] - MDQ0/4 Read Delay Control Word.
**/
#define DDR5_DATA_BUFFER_PG1_RWEA_REG 0xEA
typedef union {
  struct {
    UINT8 phase_control_between_mdq0_and_lower_nibble : 3;
                            /* Phase Control Between MDQ0 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ0 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ0 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ0 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ0 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ0 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ0 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ0 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ0 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq4_and_upper_nibble : 3;
                            /* Phase Control Between MDQ4 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ4 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ4 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ4 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ4 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ4 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ4 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ4 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ4 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWEA_STRUCT;

/**
  PG[1] RW[EB] - MDQ1/5 Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWEB_REG 0xEB
typedef union {
  struct {
    UINT8 phase_control_between_mdq1_and_lower_nibble : 3;
                            /* Phase Control Between MDQ1 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ1 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ1 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ1 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ1 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ1 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ1 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ1 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ1 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq5_and_upper_nibble : 3;
                            /* Phase Control Between MDQ5 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ5 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ5with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ5 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ5 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ5with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ5 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ5 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ5 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWEB_STRUCT;

/**
  PG[1] RW[EC] - MDQ2/6 Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWEC_REG 0xEC
typedef union {
  struct {
    UINT8 phase_control_between_mdq2_and_lower_nibble : 3;
                            /* Phase Control Between MDQ2 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ2 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ2 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ2 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ2 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ2 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ2 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ2 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ2 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq6_and_upper_nibble : 3;
                            /* Phase Control Between MDQ6 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ6 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ6 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ6with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ6 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ6 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ6 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ6 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ6 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWEC_STRUCT;

/**
  PG[1] RW[ED] - MDQ3/7 Read Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWED_REG 0xED
typedef union {
  struct {
    UINT8 phase_control_between_mdq3_and_lower_nibble : 3;
                            /* Phase Control Between MDQ3 and Lower Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ3 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ3 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ3 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ3 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ3 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ3 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ3 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ3 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq7_and_upper_nibble : 3;
                            /* Phase Control Between MDQ7 and Upper Nibble Baseline MDQS Delay During Read Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ7 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ7 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ7 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ7 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ7 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ7 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ7 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ7 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWED_STRUCT;

/**
  PG[1] RW[EE] - MDQ1/5 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWEE_REG 0xEE
typedef union {
  struct {
    UINT8 phase_control_between_mdq1_and_lower_nibble : 3;
                            /* Phase Control Between MDQ1 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ1 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ1 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ1 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ1 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ1 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ1 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ1 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ1 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq5_and_upper_nibble : 3;
                            /* Phase Control Between MDQ5 and Upper Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ5 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ5 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ5 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ5 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ5 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ5 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ5 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ5 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWEE_STRUCT;

/**
  PG[1] RW[EF] - MDQ2/6 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWEF_REG 0xEF
typedef union {
  struct {
    UINT8 phase_control_between_mdq2_and_lower_nibble : 3;
                            /* Phase Control Between MDQ2 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ2 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ2 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ2 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ02with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ2 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ2 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ2 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ2 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq6_and_upper_nibble : 3;
                            /* Phase Control Between MDQ6 and Upper Nibble Baseline MDQ-MDQS Delay During Write
                               Transactions in Steps of (1/ 64) * t CK - Bits[6:4]
                              000 = Delay MDQ6 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ6 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ6 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ6 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ6 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ6 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ6 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ6 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWEF_STRUCT;

/**
  PG[1] RW[F0] - MDQ3/7 Write Delay Control Word
**/
#define DDR5_DATA_BUFFER_PG1_RWF0_REG 0xF0
typedef union {
  struct {
    UINT8 phase_control_between_mdq3_and_lower_nibble : 3;
                            /* Phase Control Between MDQ3 and Lower Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[2:0]
                              000 = Delay MDQ3 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ3 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ3 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ3with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ3 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ3 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ3 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ3 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 phase_control_between_mdq7_and_upper_nibble : 3;
                            /* Phase Control Between MDQ7 and Upper Nibble Baseline MDQ Delay During Write Transactions
                               in Steps of (1/64) * t CK - Bits[6:4]
                              000 = Delay MDQ7 with respect to baseline by + 0/64 * t CK (Default),
                              001 = Delay MDQ7 with respect to baseline by + 1/64 * t CK,
                              010 = Delay MDQ7 with respect to baseline by + 2/64 * t CK,
                              011 = Delay MDQ7 with respect to baseline by + 3/64 * t CK,
                              100 = Delay MDQ7 with respect to baseline by -0/64 * t CK (same as default),
                              101 = Delay MDQ7 with respect to baseline by -1/64 * t CK,
                              110 = Delay MDQ7 with respect to baseline by -2/64 * t CK,
                              111 = Delay MDQ7 with respect to baseline by -3/64 * t CK
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG1_RWF0_STRUCT;

/**
  PG[2] RW[E0] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE0_STRUCT;

/**
  PG[2] RW[E1] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE1_STRUCT;

/**
  PG[2] RW[E2] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE2_STRUCT;

/**
  PG[2] RW[E3] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE3_STRUCT;

/**
  PG[2] RW[E4] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE4_REG 0xE4
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE4_STRUCT;

/**
  PG[2] RW[E5] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE5_REG 0xE5
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE5_STRUCT;

/**
  PG[2] RW[E6] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE6_REG 0xE6
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE6_STRUCT;

/**
  PG[2] RW[E7] - Host Interface Internal VrefDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWE7_REG 0xE7
typedef union {
  struct {
    UINT8 vref_dq : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWE7_STRUCT;

/**
  PG[2] RW[F0] - DRAM Interface Internal VrefMDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWF0_REG 0xF0
typedef union {
  struct {
    UINT8 Vref_MDQ : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWF0_STRUCT;

/**
  PG[2] RW[F1] - DRAM Interface Internal VrefMDQ Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWF1_REG 0xF1
typedef union {
  struct {
    UINT8 Vref_MDQ : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWF1_STRUCT;

/**
  PG[2] RW[FA] - Internal BVref Control Word
**/
#define DDR5_DATA_BUFFER_PG2_RWFA_REG 0xFA
typedef union {
  struct {
    UINT8 B_Vref : 7;
                            /* VrefDQ as% of VDD - Bits[6:0]
                              0000000 = 97.5%,
                              0000001 = 97.0%,
                              0000010 = 96.5%,
                              0000011 = 96.0%,
                              0000100 = 95.5%,
                              0000101 = 95.0%,
                              0000110 = 94.5%,
                              0000111 = 94.0%,
                              ...,
                              0101101 = 75% (Default),
                              ...,
                              1100001 = 49.0%,
                              1100010 = 48.5%
                              1100011 = 48.0%,
                              1100100 = 47.5%,
                              1100101 = 47.0%,
                              1100110 = 46.5%,
                              1101011 = 46.0%,
                              1101000 = 45.5%,
                              1101001 = 45.0%,
                              1101010 - 1111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG2_RWFA_STRUCT;

/**
  PG[4] RW[E1] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE1_STRUCT;

/**
  PG[4] RW[E9] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWE9_REG 0xE9
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE9_STRUCT;

/**
  PG[4] RW[F1] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWF1_REG 0xF1
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF1_STRUCT;

/**
  PG[4] RW[F9] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWF9_REG 0xF9
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF9_STRUCT;

/**
  PG[4] RW[E2] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE2_STRUCT;

/**
  PG[4] RW[EA] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWEA_REG 0xEA
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWEA_STRUCT;

/**
  PG[4] RW[F2] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWF2_REG 0xF2
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF2_STRUCT;

/**
  PG[4] RW[FA] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWFA_REG 0xFA
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWFA_STRUCT;

/**
  PG[4] RW[E3] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG4_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE3_STRUCT;

/**
  PG[4] RW[EB] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG4_RWEB_REG 0xEB
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWEB_STRUCT;

/**
  PG[4] RW[F3] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG4_RWF3_REG 0xF3
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF3_STRUCT;

/**
  PG[4] RW[FB] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG4_RWFB_REG 0xFB
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWFB_STRUCT;

/**
  PG[4] RW[E4] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWE4_REG 0xE4
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE4_STRUCT;

/**
  PG[4] RW[EC] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWEC_REG 0xEC
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWEC_STRUCT;

/**
  PG[4] RW[F4] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWF4_REG 0xF4
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF4_STRUCT;

/**
  PG[4] RW[FC] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG4_RWFC_REG 0xFC
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWFC_STRUCT;

/**
  PG[4] RW[E0] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG4_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE0_STRUCT;

/**
  PG[4] RW[E8] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG4_RWE8_REG 0xE8
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWE8_STRUCT;

/**
  PG[4] RW[F0] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG4_RWF0_REG 0xF0
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF0_STRUCT;

/**
  PG[4] RW[F8] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG4_RWF8_REG 0xF8
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG4_RWF8_STRUCT;

/**
  PG[5] RW[E1] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE1_STRUCT;

/**
  PG[5] RW[E9] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWE9_REG 0xE9
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE9_STRUCT;

/**
  PG[5] RW[F1] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWF1_REG 0xF1
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF1_STRUCT;

/**
  PG[5] RW[F9] - DQ [7:0] Receiver DFE Tap 1 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWF9_REG 0xF9
typedef union {
  struct {
    UINT8 tap_1_dfe_coefficient : 6;
                            /* Tap 1 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 1 DFE bias = 0 mV,
                              000001 = Tap 1 DFE bias +1 Tap Step,
                              000010 = Tap 1 DFE bias +2 Tap Steps,
                              000011 = Tap 1 DFE bias +3 Tap Step,
                              000100 = Tap 1 DFE bias +4 Tap Steps,
                              000101 = Tap 1 DFE bias +5 Tap Steps,
                              000110 = Tap 1 DFE bias +6 Tap Steps,
                              000111 = Tap 1 DFE bias +7 Tap Steps,
                              001000 = Tap 1 DFE bias +8 Tap Steps,
                              001001 = Tap 1 DFE bias +9 Tap Steps,
                              001010 = Tap 1 DFE bias +10 Tap Steps,
                              001011 = Tap 1 DFE bias +11 Tap Steps,
                              001100 = Tap 1 DFE bias +12 Tap Steps,
                              001101 = Tap 1 DFE bias +13 Tap Steps,
                              001110 = Tap 1 DFE bias +14 Tap Steps,
                              001111 = Tap 1 DFE bias +15 Tap Steps,
                              010000 = Tap 1 DFE bias +16 Tap Steps,
                              010001 = Tap 1 DFE bias +17 Tap Steps,
                              010010 = Tap 1 DFE bias +18 Tap Steps,
                              010011 = Tap 1 DFE bias +19 Tap Steps,
                              010100 = Tap 1 DFE bias +20 Tap Steps,
                              010101 = Tap 1 DFE bias +21 Tap Steps,
                              010110 = Tap 1 DFE bias +22 Tap Steps,
                              010111 = Tap 1 DFE bias +23 Tap Steps,
                              011000 = Tap 1 DFE bias +24 Tap Steps,
                              011001 = Tap 1 DFE bias +25 Tap Steps,
                              011010 = Tap 1 DFE bias +26 Tap Steps,
                              011011 = Tap 1 DFE bias +27 Tap Steps,
                              011100 = Tap 1 DFE bias +28 Tap Steps,
                              011101 = Tap 1 DFE bias +29 Tap Steps,
                              011110 = Tap 1 DFE bias +30 Tap Steps,
                              011111 = Tap 1 DFE bias +31 Tap Steps,
                              100000 = Tap 1 DFE bias +32 Tap Steps,
                              100001 = Tap 1 DFE bias +33 Tap Steps,
                              100010 = Tap 1 DFE bias +34 Tap Steps,
                              100011 = Tap 1 DFE bias +35 Tap Steps,
                              100100 = Tap 1 DFE bias +36 Tap Steps,
                              100101 = Tap 1 DFE bias +37 Tap Steps,
                              100110 = Tap 1 DFE bias +38 Tap Steps,
                              100111 = Tap 1 DFE bias +39 Tap Steps,
                              101000 = Tap 1 DFE bias +40 Tap Steps,
                              101001 = Tap 1 DFE bias +41 Tap Steps,
                              101010 = Tap 1 DFE bias +42 Tap Steps,
                              101011 = Tap 1 DFE bias +43 Tap Steps,
                              101100 = Tap 1 DFE bias +44 Tap Steps,
                              101101 = Tap 1 DFE bias +45 Tap Steps,
                              101110 = Tap 1 DFE bias +46 Tap Steps,
                              101111 = Tap 1 DFE bias +47 Tap Steps,
                              110000 = Tap 1 DFE bias +48 Tap Steps,
                              110001 = Tap 1 DFE bias +49 Tap Steps,
                              110010 = Tap 1 DFE bias +50 Tap Steps,
                              110011 = Reserved,
                              110100 = Reserved,
                              110101 = Reserved,
                              110110 = Reserved
                              110111 = Reserved,
                              111000 = Reserved,
                              111001 = Reserved,
                              111010 = Reserved,
                              111011 = Reserved,
                              111100 = Reserved,
                              111101 = Reserved,
                              111110 = Reserved,
                              111111 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_1_coefficient_sign_bit : 1;
                            /* Tap 1 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 1 DFE bias when Tap 1 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF9_STRUCT;

/**
  PG[5] RW[E2] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE2_STRUCT;

/**
  PG[5] RW[EA] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWEA_REG 0xEA
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWEA_STRUCT;

/**
  PG[5] RW[F2] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWF2_REG 0xF2
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF2_STRUCT;

/**
  PG[5] RW[FA] - DQ[7:0] Receiver DFE Tap 2 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWFA_REG 0xFA
typedef union {
  struct {
    UINT8 tap_2_dfe_coefficient : 6;
                            /* Tap 2 DFE Coefficient - Bits[5:0]
                              000000 = (Default) Tap 2 DFE bias = 0 mV,
                              000001 = Tap 2 DFE bias+1 Tap Step,
                              000010 = Tap 2 DFE bias +2 Tap Steps,
                              000011 = Tap 2 DFE bias +3 Tap Steps,
                              000100 = Tap 2 DFE bias +4 Tap Steps,
                              000101 = Tap 2 DFE bias +5 Tap Steps,
                              000110 = Tap 2 DFE bias +6 Tap Steps,
                              000111 = Tap 2 DFE bias +7 Tap Steps,
                              001000 = Tap 2 DFE bias +8 Tap Steps,
                              001001 = Tap 2 DFE bias +9 Tap Steps,
                              001010 = Tap 2 DFE bias +10 Tap Steps,
                              001011 = Tap 2 DFE bias +11 Tap Steps,
                              001100 = Tap 2 DFE bias +12 Tap Steps,
                              001101 = Tap 2 DFE bias +13 Tap Steps,
                              001110 = Tap 2 DFE bias +14 Tap Steps,
                              001111 = Tap 2 DFE bias +15 Tap Steps,
                              010000 = Tap 2 DFE bias +16 Tap Steps,
                              010001 = Tap 2 DFE bias +17 Tap Steps,
                              010010 = Tap 2 DFE bias +18 Tap Steps,
                              010011 = Tap 2 DFE bias +19 Tap Steps,
                              010100 = Tap 2 DFE bias +20 Tap Steps,
                              010101 = Reserved,
                              010110 = Reserved,
                              010111 = Reserved,
                              011000 = Reserved,
                              011001 = Reserved,
                              011010 = Reserved,
                              011011 = Reserved,
                              011100 = Reserved,
                              011101 = Reserved,
                              011110 = Reserved,
                              001111 = Reserved,
                              100001 = Reserved,
                              100000 = Reserved,
                              100011 = Reserved,
                              100010 = Reserved,
                              100101 = Reserved,
                              100100 = Reserved,
                              100111 = Reserved,
                              100110 = Reserved,
                              101001 = Reserved,
                              101000 = Reserved
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[6]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 tap_2_coefficient_sign_bit : 1;
                            /* Tap 2 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 2 DFE bias when Tap 2 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWFA_STRUCT;

/**
  PG[5] RW[E3] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG5_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE3_STRUCT;

/**
  PG[5] RW[EB] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG5_RWEB_REG 0xEB
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWEB_STRUCT;

/**
  PG[5] RW[F3] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG5_RWF3_REG 0xF3
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF3_STRUCT;

/**
  PG[5] RW[FB] - DQ [7:0] Receiver DFE Tap 3 Coefficient
**/
#define DDR5_DATA_BUFFER_PG5_RWFB_REG 0xFB
typedef union {
  struct {
    UINT8 tap_3_dfe_coefficient : 5;
                            /* Tap 3 DFE Coefficient - Bits[4:0]
                              00000 = (Default) Tap 3 DFE bias = 0 mV,
                              00001 = Tap 3 DFE bias +1 Tap Step,
                              00010 = Tap 3 DFE bias +2 Tap Steps,
                              00011 = Tap 3 DFE bias +3 Tap Steps,
                              00100 = Tap 3DFE bias +4 Tap Steps,
                              00101 = Tap 3 DFE bias +5 Tap Steps,
                              00110 = Tap 3 DFE bias +6 Tap Steps,
                              00111 = Tap 3 DFE bias +7 Tap Steps,
                              01000 = Tap 3 DFE bias +8 Tap Steps,
                              01001 = Tap 3 DFE bias +9 Tap Steps,
                              01010 = Tap 3 DFE bias +10 Tap Steps,
                              01011 = Tap 3DFE bias +11 Tap Steps,
                              01100 = Tap 3 DFE bias +12 Tap Steps,
                              01101 = Tap 3DFE bias +13 Tap Steps,
                              01110 = Tap 3 DFE bias +14 Tap Steps,
                              01111 = Tap 3 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_3_coefficient_sign_bit : 1;
                            /* Tap 3 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 3 DFE bias when Tap 3 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWFB_STRUCT;

/**
  PG[5] RW[E4] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWE4_REG 0xE4
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE4_STRUCT;

/**
  PG[5] RW[EC] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWEC_REG 0xEC
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWEC_STRUCT;

/**
  PG[5] RW[F4] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWF4_REG 0xF4
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF4_STRUCT;

/**
  PG[5] RW[FC] - DQ[7:0] Receiver DFE Tap 4 Coefficients
**/
#define DDR5_DATA_BUFFER_PG5_RWFC_REG 0xFC
typedef union {
  struct {
    UINT8 tap_4_dfe_coefficient : 5;
                            /* Tap 4 DFE Coefficient , - Bits[4:0]
                              00000 = (Default) Tap 4 DFE bias = 0 mV,
                              00001 = Tap 4 DFE bias +1 Tap Steps,
                              00010 = Tap 4 DFE bias +2 Tap Steps,
                              00011 = Tap 4 DFE bias +3 Tap Steps,
                              00100 = Tap 4 DFE bias +4 Tap Steps,
                              00101 = Tap 4 DFE bias +5 Tap Steps,
                              00110 = Tap 4 DFE bias +6 Tap Steps,
                              00111 = Tap 4 DFE bias +7 Tap Steps,
                              01000 = Tap 4 DFE bias +8 Tap Steps,
                              01001 = Tap 4 DFE bias +9 Tap Steps,
                              01010 = Tap 4 DFE bias +10 Tap Steps,
                              01011 = Tap 4 DFE bias +11 Tap Steps,
                              01100 = Tap 4 DFE bias +12 Tap Steps,
                              01101 = Tap 4 DFE bias +13 Tap Steps,
                              01110 = Tap 4 DFE bias +14 Tap Steps,
                              01111 = Tap 4 DFE bias +15 Tap Steps,
                              10000 = Reserved,
                              10001 = Reserved,
                              10010 = Reserved,
                              10011 = Reserved,
                              10100 = Reserved,
                              10101 = Reserved,
                              10110 = Reserved,
                              10111 = Reserved,
                              11000 = Reserved,
                              11001 = Reserved,
                              11010 = Reserved,
                              11011 = Reserved,
                              11100 = Reserved,
                              11101 = Reserved,
                              11110 = Reserved,
                              11111 = Reserved
                            */
    UINT8 reserved : 2;
                            /* Reserved - Bits[6:5]
                            */
    UINT8 tap_4_coefficient_sign_bit : 1;
                            /* Tap 4 Coefficient Sign Bit - Bits[7]
                              0 = (Default) Positive Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Negative bias for
                                Logic 0 Tap 1 post-cursor),
                              1 = Negative Tap 4 DFE bias when Tap 4 post-cursor is Logic 1 (Positive bias for Logic 0 Tap
                                1 post-cursor)
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWFC_STRUCT;

/**
  PG[5] RW[E0] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG5_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE0_STRUCT;

/**
  PG[5] RW[E8] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG5_RWE8_REG 0xE8
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWE8_STRUCT;

/**
  PG[5] RW[F0] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG5_RWF0_REG 0xF0
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF0_STRUCT;

/**
  PG[5] RW[F8] - DQ[7:0] Receiver DFE Gain Offset Adjustment
**/
#define DDR5_DATA_BUFFER_PG5_RWF8_REG 0xF8
typedef union {
  struct {
    UINT8 flat_band_dc_gain_adjustment : 3;
                            /* Flat-band (DC) gain adjustment - Bits[2:0]
                              000 = Gain Adjustment = 0 dB (default),
                              001 = Gain Adjustment = +6 dB,
                              010 = Gain Adjustment = +4 dB,
                              011 = Gain Adjustment = +2 dB,
                              100 = Gain Adjustment = 0 dB (same as default),
                              101 = Gain Adjustment = -2 dB,
                              110 = Gain Adjustment = -4 dB,
                              111 = Gain Adjustment = -6 dB
                            */
    UINT8 reserved : 5;
                            /* Reserved - Bits[7:3]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG5_RWF8_STRUCT;

/**
  PG[6] RW[E3] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWE3_STRUCT;

/**
  PG[6] RW[E7] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWE7_REG 0xE7
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWE7_STRUCT;

/**
  PG[6] RW[EB] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWEB_REG 0xEB
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWEB_STRUCT;

/**
  PG[6] RW[EF] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWEF_REG 0xEF
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWEF_STRUCT;

/**
  PG[6] RW[F3] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWF3_REG 0xF3
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWF3_STRUCT;

/**
  PG[6] RW[F7] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWF7_REG 0xF7
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWF7_STRUCT;

/**
  PG[6] RW[FB] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWFB_REG 0xFB
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWFB_STRUCT;

/**
  PG[6] RW[FF] - Sign Bit Dn DFE_VREF Control Word
**/
#define DDR5_DATA_BUFFER_PG6_RWFF_REG 0xFF
typedef union {
  struct {
    UINT8 dn_sign_dfe_vref : 1;
                            /* Dn Sign DFE_VREF - Bits[0]
                              0 = Positive,
                              1 = Negative
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG6_RWFF_STRUCT;

/**
  PG[7] RW[70] - MDQ Error counters Reset Control Word
**/
#define DDR5_DATA_BUFFER_PG7_RW70_REG 0x70
typedef union {
  struct {
    UINT8 mdq_error_counters_reset_1_2 : 1;
                            /* MDQ Error counters Reset 1,2 - Bits[0]
                              0000 = All per-bit MDQ read error counters operate normally.,
                              0001 = All per-bit MDQ read error counters are reset to zero. This bit is self-clearing in
                                   the next cycle
                            */
    UINT8 reserved : 7;
                            /* Reserved - Bits[7:1]
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG7_RW70_STRUCT;

/**
  PG[9] RW[60] - DFETA Training Mode Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW60_REG 0x60
typedef union {
  struct {
    UINT8 dfe_training_accelerator : 1;
                            /* DFE Training Accelerator - Bits[0]
                              0 = Stop DFE Training Accelerator (default),
                              1 = Start DFE Training Accelerator
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[1]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 select_inner_loop_parameter : 2;
                            /* Select Inner Loop Parameter - Bits[3:2]
                              00 = Select Inner Loop Parameter - NULL,
                              01 = Select Inner Loop Parameter - DFE_Vref,
                              10 = Select Inner Loop Parameter - VrefCA,
                              11 = Reserved
                            */
    UINT8 select_outer_loop_parameter : 3;
                            /* Select Outer Loop Parameter - Bits[6:4]
                              000 = Select Outer Loop Parameter - NULL,
                              001 = Select Outer Loop Parameter - Tap 1,
                              010 = Select Outer Loop Parameter - Tap 2,
                              011 = Select Outer Loop Parameter - Tap 3,
                              100 = Select Outer Loop Parameter - Tap 4,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW60_STRUCT;

/**
  PG[9] RW[61] - DFETA Inner Loop Start Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW61_REG 0x61
typedef union {
  struct {
    UINT8 inner_loop_start_value : 8;
                            /* Inner Loop Start Value - Bits[7:0]
                              00000000 = Inner Loop Start Value = 0x00,
                              00000001 = Inner Loop Start Value = 0x01,
                              00000010 = Inner Loop Start Value = 0x02,
                              ...,
                              11111100 = Inner Loop Start Value = 0xFC,
                              11111101 = Inner Loop Start Value = 0xFD,
                              11111110 = Inner Loop Start Value = 0xFE,
                              11111111 = Inner Loop Start Value = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW61_STRUCT;

/**
  PG[9] RW[62] - DFETA Inner Loop Start Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW62_REG 0x62
typedef union {
  struct {
    UINT8 inner_loop_start_value : 1;
                            /* Inner Loop Start Value - Bits[0]
                              0 = Inner Loop Start Value = 0x0,
                              1 = Inner Loop Start Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW62_STRUCT;

/**
  PG[9] RW[63] - DFETA Outer Loop Start Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW63_REG 0x63
typedef union {
  struct {
    UINT8 outer_loop_start_value_1_2 : 7;
                            /* Outer Loop Start Value 1,2 - Bits[6:0]
                              0000000 = Outer Loop Start Value = 0x00,
                              0000001 = Outer Loop Start Value = 0x01,
                              0000010 = Outer Loop Start Value = 0x02,
                              ...,
                              1111100 = Outer Loop Start Value = 0x7C,
                              1111101 = Outer Loop Start Value = 0x7D,
                              1111110 = Outer Loop Start Value = 0x7E,
                              1111111 = Outer Loop Start Value = 0x7F
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW63_STRUCT;

/**
  PG[9] RW[64] - DFETA Inner Loop Current Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW64_REG 0x64
typedef union {
  struct {
    UINT8 inner_loop_current_value_read_only : 8;
                            /* Inner Loop Current Value. (Read Only) - Bits[7:0]
                              00000000 = Inner Loop Current Value = 0x00,
                              00000001 = Inner Loop Current Value = 0x01,
                              00000010 = Inner Loop Current Value = 0x02,
                              ...,
                              11111100 = Inner Loop Current Value = 0xFC,
                              11111101 = Inner Loop Current Value = 0xFD,
                              11111110 = Inner Loop Current Value = 0xFE,
                              11111111 = Inner Loop Current Value = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW64_STRUCT;

/**
  PG[9] RW[65] - DFETA Inner Loop Current Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW65_REG 0x65
typedef union {
  struct {
    UINT8 inner_loop_start_value_read_only : 1;
                            /* Inner Loop Start Value (Read Only) - Bits[0]
                              0 = Inner Loop Start Value = 0x0,
                              1 = Inner Loop Start Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW65_STRUCT;

/**
  PG[9] RW[66] - DFETA Outer Loop Current Value Control Word
**/
#define DDR5_DATA_BUFFER_RW66_REG 0x66
typedef union {
  struct {
    UINT8 outer_loop_current_value_read_only : 7;
                            /* Outer Loop Current Value. (Read Only) - Bits[6:0]
                              0000000 = Outer Loop Current Value = 0x00,
                              0000001 = Outer Loop Current Value = 0x01,
                              0000010 = Outer Loop Current Value = 0x02,
                              ...,
                              1111100 = Outer Loop Current Value = 0x7C,
                              1111101 = Outer Loop Current Value = 0x7D,
                              1111110 = Outer Loop Current Value = 0x7E,
                              1111111 = Outer Loop Current Value = 0x7F
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_RW66_STRUCT;

/**
  PG[9] RW[67] - DFETA Inner and Outer Loop Step Size Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW67_REG 0x67
typedef union {
  struct {
    UINT8 inner_loop_step_size : 4;
                            /* Inner Loop Step Size - Bits[3:0]
                              0000 = Inner Loop Step Size - 1,
                              0001 = Inner Loop Step Size - 2,
                              0010 = Inner Loop Step Size - 3,
                              0011 = Inner Loop Step Size - 4,
                              0100 = Inner Loop Step Size - 5,
                              0101 = Inner Loop Step Size - 6,
                              0110 = Inner Loop Step Size - 7,
                              0111 = Inner Loop Step Size - 8,
                              1000 = Inner Loop Step Size - 9,
                              1001 = Inner Loop Step Size - 10,
                              1010 = Inner Loop Step Size - 11,
                              1011 = Inner Loop Step Size - 12,
                              1100 = Inner Loop Step Size - 13,
                              1101 = Inner Loop Step Size - 14,
                              1110 = Inner Loop Step Size - 15,
                              1111 = Inner Loop Step Size - 16
                            */
    UINT8 outer_loop_step_size : 4;
                            /* Outer Loop Step Size - Bits[7:4]
                              0000 = Outer Loop Step Size - 1,
                              0001 = Outer Loop Step Size - 2,
                              0010 = Outer Loop Step Size - 3,
                              0011 = Outer Loop Step Size - 4,
                              0100 = Outer Loop Step Size - 5,
                              0101 = Outer Loop Step Size - 6,
                              0110 = Outer Loop Step Size - 7,
                              0111 = Outer Loop Step Size - 8,
                              1000 = Outer Loop Step Size - 9,
                              1001 = Outer Loop Step Size - 10,
                              1010 = Outer Loop Step Size - 11,
                              1011 = Outer Loop Step Size - 12,
                              1100 = Outer Loop Step Size - 13,
                              1101 = Outer Loop Step Size - 14,
                              1110 = Outer Loop Step Size - 15,
                              1111 = Outer Loop Step Size - 16
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW67_STRUCT;

/**
  PG[9] RW[68] - DFETA Inter Loop Number of Increments Bit [7:0] Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW68_REG 0x68
typedef union {
  struct {
    UINT8 inner_loop_number_of_increments : 8;
                            /* Inner Loop Number of Increments - Bits[7:0]
                              00000000 = Inner Loop Number of Increments = 0x00,
                              00000001 = Inner Loop Number of Increments = 0x01,
                              00000010 = Inner Loop Number of Increments = 0x02,
                              ...,
                              11111100 = Inner Loop Number of Increments = 0xFC,
                              11111101 = Inner Loop Number of Increments = 0xFD,
                              11111110 = Inner Loop Number of Increments = 0xFE,
                              11111111 = Inner Loop Number of Increments = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW68_STRUCT;

/**
  PG[9] RW[69] - DFETA Inner Loop Start Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW69_REG 0x69
typedef union {
  struct {
    UINT8 inner_loop_start_value : 1;
                            /* Inner Loop Start Value - Bits[0]
                              0 = Inner Loop Start Value = 0x0,
                              1 = Inner Loop Start Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW69_STRUCT;

/**
  PG[9] RW[6A] - DFETA Outer Loop Number of Increments Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW6A_REG 0x6A
typedef union {
  struct {
    UINT8 outer_loop_number_of_increments : 7;
                            /* Outer Loop Number of Increments - Bits[6:0]
                              0000000 = Outer Loop Number of Increments = 0x00,
                              0000001 = Outer Loop Number of Increments = 0x01,
                              0000010 = Outer Loop Number of Increments = 0x02,
                              ...,
                              1111100 = Outer Loop Number of Increments = 0x7C,
                              1111101 = Outer Loop Number of Increments = 0x7D,
                              1111110 = Outer Loop Number of Increments = 0x7E,
                              1111111 = Outer Loop Number of Increments = 0x7F
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW6A_STRUCT;

/**
  PG[9] RW[6B] - DFETA Inner Loop Current Increment Bit [7:0] Status Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW6B_REG 0x6B
typedef union {
  struct {
    UINT8 inner_loop_current_increment_read_only : 8;
                            /* Inner Loop Current Increment (Read Only) - Bits[7:0]
                              00000000 = Inner Loop Current Increment = 0x00,
                              00000001 = Inner Loop Current Increment = 0x01,
                              00000010 = Inner Loop Current Increment = 0x02,
                              ...,
                              11111100 = Inner Loop Current Increment = 0xFC,
                              11111101 = Inner Loop Current Increment = 0xFD,
                              11111110 = Inner Loop Current Increment = 0xFE,
                              11111111 = Inner Loop Current Increment = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW6B_STRUCT;

/**
  PG[9] RW[6C] - DFETA Inter Loop Number of Increments Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW6C_REG 0x6C
typedef union {
  struct {
    UINT8 inner_loop_start_value : 1;
                            /* Inner Loop Start Value - Bits[0]
                              0 = Inner Loop Start Value = 0x0,
                              1 = Inner Loop Start Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW6C_STRUCT;

/**
  PG[9] RW[6D] - DFETA Outer Loop Current Increment Status Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW6D_REG 0x6D
typedef union {
  struct {
    UINT8 outer_loop_current_increment_read_only : 7;
                            /* Outer Loop Current Increment (Read Only) - Bits[6:0]
                              0000000 = Inner Loop Current Increment = 0x00,
                              0000001 = Inner Loop Current Increment = 0x01,
                              0000010 = Inner Loop Current Increment = 0x02,
                              ...,
                              1111100 = Inner Loop Current Increment = 0x7C,
                              1111101 = Inner Loop Current Increment = 0x7D,
                              1111110 = Inner Loop Current Increment = 0x7E,
                              1111111 = Inner Loop Current Increment = 0x7F
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW6D_STRUCT;

/**
  PG[9] RW[72] - DFETA Inner Loop Value Status Bit [7:0] Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW72_REG 0x72
typedef union {
  struct {
    UINT8 inner_loop_value_read_only : 8;
                            /* Inner Loop Value (Read Only) - Bits[7:0]
                              00000000 = Inner Loop Current Increment = 0x00,
                              00000001 = Inner Loop Current Increment = 0x01,
                              00000010 = Inner Loop Current Increment = 0x02,
                              ...,
                              11111100 = Inner Loop Current Increment = 0xFC,
                              11111101 = Inner Loop Current Increment = 0xFD,
                              11111110 = Inner Loop Current Increment = 0xFE,
                              11111111 = Inner Loop Current Increment = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW72_STRUCT;

/**
  PG[9] RW[73] - DFETA Inner Loop Value Status Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW73_REG 0x73
typedef union {
  struct {
    UINT8 inner_loop_value : 1;
                            /* Inner Loop Value - Bits[0]
                              0 = Inner Loop Start Value = 0x0,
                              1 = Inner Loop Start Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW73_STRUCT;

/**
  PG[9] RW[74] - DFETA Outer Loop Value Status Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RW74_REG 0x74
typedef union {
  struct {
    UINT8 outer_loop_current_increment_read_only : 7;
                            /* Outer Loop Current Increment (Read Only) - Bits[6:0]
                              0000000 = Inner Loop Current Increment = 0x00,
                              0000001 = Inner Loop Current Increment = 0x01,
                              0000010 = Inner Loop Current Increment = 0x02,
                              ...,
                              1111100 = Inner Loop Current Increment = 0x7C,
                              1111101 = Inner Loop Current Increment = 0x7D,
                              1111110 = Inner Loop Current Increment = 0x7E,
                              1111111 = Inner Loop Current Increment = 0x7F
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RW74_STRUCT;
/**
  PG[9] RW[E0] - DFETA Training Mode Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 dfe_training_accelerator : 1;
                            /* DFE Training Accelerator - Bits[0]
                              0 = Stop DFE Training Accelerator (default),
                              1 = Start DFE Training Accelerator
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[1]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 select_inner_loop_parameter : 2;
                            /* Select Inner Loop Parameter - Bits[3:2]
                              00 = Select Inner Loop Parameter - NULL,
                              01 = Select Inner Loop Parameter - DFE_Vref,
                              10 = Select Inner Loop Parameter - VrefDQ,
                              11 = Reserved
                            */
    UINT8 select_outer_loop_parameter : 3;
                            /* Select Outer Loop Parameter - Bits[6:4]
                              000 = Select Outer Loop Parameter - NULL,
                              001 = Select Outer Loop Parameter - Tap 1,
                              010 = Select Outer Loop Parameter - Tap 2,
                              011 = Select Outer Loop Parameter - Tap 3,
                              100 = Select Outer Loop Parameter - Tap 4,
                              101 = Reserved,
                              110 = Reserved,
                              111 = Reserved
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE0_STRUCT;

/**
  PG[9] RW[E1] - DFETA Inner Loop Start Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 inner_loop_start_value : 8;
                            /* Inner Loop Start Value - Bits[7:0]
                              00000000 = Inner Loop Start Value = 0x00,
                              00000001 = Inner Loop Start Value = 0x01,
                              00000010 = Inner Loop Start Value = 0x02,
                              ...,
                              11111100 = Inner Loop Start Value = 0xFC,
                              11111101 = Inner Loop Start Value = 0xFD,
                              11111110 = Inner Loop Start Value = 0xFE,
                              11111111 = Inner Loop Start Value = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE1_STRUCT;

/**
  PG[9] RW[E2] - DFETA Inner Loop Start Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 inner_loop_start_value : 1;
                            /* Inner Loop Start Value - Bits[0]
                              0 = Inner Loop Start Value = 0x0,
                              1 = Inner Loop Start Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE2_STRUCT;

/**
  PG[9] RW[E3] - DFETA Outer Loop Start Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 outer_loop_start_value : 7;
                            /* Outer Loop Start Value - Bits[6:0]
                              0000000 = Outer Loop Start Value = 0x00,
                              0000001 = Outer Loop Start Value = 0x01,
                              0000010 = Outer Loop Start Value = 0x02,
                              ...,
                              1111100 = Outer Loop Start Value = 0x7C,
                              1111101 = Outer Loop Start Value = 0x7D,
                              1111110 = Outer Loop Start Value = 0x7E,
                              1111111 = Outer Loop Start Value = 0x7F
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE3_STRUCT;

/**
  PG[9] RW[E4] - DFETA Inner Loop Current Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE4_REG 0xE4
typedef union {
  struct {
    UINT8 inner_loop_current_value_read_only : 8;
                            /* Inner Loop Current Value. (Read Only) - Bits[7:0]
                              00000000 = Inner Loop Current Value = 0x00,
                              00000001 = Inner Loop Current Value = 0x01,
                              00000010 = Inner Loop Current Value = 0x02,
                              ...,
                              11111100 = Inner Loop Current Value = 0xFC,
                              11111101 = Inner Loop Current Value = 0xFD,
                              11111110 = Inner Loop Current Value = 0xFE,
                              11111111 = Inner Loop Current Value = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE4_STRUCT;

/**
  PG[9] RW[E5] - DFETA Inner Loop Current Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE5_REG 0xE5
typedef union {
  struct {
    UINT8 inner_loop_current_value_read_only : 1;
                            /* Inner Loop Current Value (Read Only) - Bits[0]
                              0 = Inner Loop Current Value = 0x0,
                              1 = Inner Loop Current Value = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE5_STRUCT;

/**
  PG[9] RW[E6] - DFETA Outer Loop Current Value Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE6_REG 0xE6
typedef union {
  struct {
    UINT8 outer_loop_current_value_read_only : 7;
                            /* Outer Loop Current Value. (Read Only) - Bits[6:0]
                              0000000 = Outer Loop Current Value = 0x00,
                              0000001 = Outer Loop Current Value = 0x01,
                              0000010 = Outer Loop Current Value = 0x02,
                              ...,
                              1111100 = Outer Loop Current Value = 0x7C,
                              1111101 = Outer Loop Current Value = 0x7D,
                              1111110 = Outer Loop Current Value = 0x7E,
                              1111111 = Outer Loop Current Value = 0x7F
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE6_STRUCT;

/**
  PG[9] RW[E7] - DFETA Inner and Outer Loop Step Size Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE7_REG 0xE7
typedef union {
  struct {
    UINT8 inner_loop_step_size : 4;
                            /* Inner Loop Step Size - Bits[3:0]
                              0000 = Inner Loop Step Size - 1,
                              0001 = Inner Loop Step Size - 2,
                              0010 = Inner Loop Step Size - 3,
                              0011 = Inner Loop Step Size - 4,
                              0100 = Inner Loop Step Size - 5,
                              0101 = Inner Loop Step Size - 6,
                              0110 = Inner Loop Step Size - 7,
                              0111 = Inner Loop Step Size - 8,
                              1000 = Inner Loop Step Size - 9,
                              1001 = Inner Loop Step Size - 10,
                              1010 = Inner Loop Step Size - 11,
                              1011 = Inner Loop Step Size - 12,
                              1100 = Inner Loop Step Size - 13,
                              1101 = Inner Loop Step Size - 14,
                              1110 = Inner Loop Step Size - 15,
                              1111 = Inner Loop Step Size - 16
                            */
    UINT8 outer_loop_step_size : 4;
                            /* Outer Loop Step Size - Bits[7:4]
                              0000 = Outer Loop Step Size - 1,
                              0001 = Outer Loop Step Size - 2,
                              0010 = Outer Loop Step Size - 3,
                              0011 = Outer Loop Step Size - 4,
                              0100 = Outer Loop Step Size - 5,
                              0101 = Outer Loop Step Size - 6,
                              0110 = Outer Loop Step Size - 7,
                              0111 = Outer Loop Step Size - 8,
                              1000 = Outer Loop Step Size - 9,
                              1001 = Outer Loop Step Size - 10,
                              1010 = Outer Loop Step Size - 11,
                              1011 = Outer Loop Step Size - 12,
                              1100 = Outer Loop Step Size - 13,
                              1101 = Outer Loop Step Size - 14,
                              1110 = Outer Loop Step Size - 15,
                              1111 = Outer Loop Step Size - 16
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE7_STRUCT;

/**
  PG[9] RW[E8] - DFETA Inter Loop Number of Increments Bit [7:0] Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE8_REG 0xE8
typedef union {
  struct {
    UINT8 inner_loop_number_of_increments : 8;
                            /* Inner Loop Number of Increments - Bits[7:0]
                              00000000 = Inner Loop Number of Increments = 0x00,
                              00000001 = Inner Loop Number of Increments = 0x01,
                              00000010 = Inner Loop Number of Increments = 0x02,
                              ...,
                              11111100 = Inner Loop Number of Increments = 0xFC,
                              11111101 = Inner Loop Number of Increments = 0xFD,
                              11111110 = Inner Loop Number of Increments = 0xFE,
                              11111111 = Inner Loop Number of Increments = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE8_STRUCT;

/**
  PG[9] RW[E9] - DFETA Inner Loop Start Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWE9_REG 0xE9
typedef union {
  struct {
    UINT8 inner_loop_number_of_increments : 1;
                            /* Inner Loop Number of Increments - Bits[0]
                              0 = Inner Loop Number of Increments = 0x0,
                              1 = Inner Loop Number of Increments = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWE9_STRUCT;

/**
  PG[9] RW[EA] - DFETA Outer Loop Number of Increments Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWEA_REG 0xEA
typedef union {
  struct {
    UINT8 outer_loop_number_of_increments : 7;
                            /* Outer Loop Number of Increments - Bits[6:0]
                              0000000 = Outer Loop Number of Increments = 0x00,
                              0000001 = Outer Loop Number of Increments = 0x01,
                              0000010 = Outer Loop Number of Increments = 0x02,
                              ...,
                              1111100 = Outer Loop Number of Increments = 0x7C,
                              1111101 = Outer Loop Number of Increments = 0x7D,
                              1111110 = Outer Loop Number of Increments = 0x7E,
                              1111111 = Outer Loop Number of Increments = 0x7F
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWEA_STRUCT;

/**
  PG[9] RW[EB] - DFETA Inner Loop Current Increment Bit [7:0] Status Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWEB_REG 0xEB
typedef union {
  struct {
    UINT8 inner_loop_current_increment_read_only : 8;
                            /* Inner Loop Current Increment (Read Only) - Bits[7:0]
                              00000000 = Inner Loop Current Increment = 0x00,
                              00000001 = Inner Loop Current Increment = 0x01,
                              00000010 = Inner Loop Current Increment = 0x02,
                              ...,
                              11111100 = Inner Loop Current Increment = 0xFC,
                              11111101 = Inner Loop Current Increment = 0xFD,
                              11111110 = Inner Loop Current Increment = 0xFE,
                              11111111 = Inner Loop Current Increment = 0xFF
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWEB_STRUCT;

/**
  PG[9] RW[EC] - DFETA Inner Loop Start Value Bit [8]Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWEC_REG 0xEC
typedef union {
  struct {
    UINT8 inner_loop_current_increment : 1;
                            /* Inner Loop Current Increment - Bits[0]
                              0 = Inner Loop Current Increment = 0x0,
                              1 = Inner Loop Current Increment = 0x1
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWEC_STRUCT;

/**
  PG[9] RW[ED] - DFETA Outer Loop Current Increment Status Control Word
**/
#define DDR5_DATA_BUFFER_PG9_RWED_REG 0xED
typedef union {
  struct {
    UINT8 outer_loop_current_increment_read_only : 7;
                            /* Outer Loop Current Increment (Read Only) - Bits[6:0]
                              0000000 = Inner Loop Current Increment = 0x00,
                              0000001 = Inner Loop Current Increment = 0x01,
                              0000010 = Inner Loop Current Increment = 0x02,
                              ...,
                              1111100 = Inner Loop Current Increment = 0x7C,
                              1111101 = Inner Loop Current Increment = 0x7D,
                              1111110 = Inner Loop Current Increment = 0x7E,
                              1111111 = Inner Loop Current Increment = 0x7F
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PG9_RWED_STRUCT;

/**
  PG[A] RW[60] - Lower-Nibble, Rank-0 Initial DRAM DQS Oscillator Counter LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW60_REG 0x60
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_initial_dram : 8;
                            /* The lower-nibble, rank-0 initial DRAM DQS oscillator counter LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the lower-nibble, rank-0 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter LSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW60_STRUCT;

/**
  PG[A] RW[61] - Lower-Nibble, Rank-0 Initial DRAM DQS Oscillator Counter MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW61_REG 0x61
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_initial_dram : 8;
                            /* The lower-nibble, rank-0 initial DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the lower-nibble, rank-0 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter MSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00.
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW61_STRUCT;

/**
  PG[A] RW[62] - Lower-Nibble, Rank-1 Initial DRAM DQS Oscillator Counter LSB Table 148 Value
**/
#define DDR5_DATA_BUFFER_PGA_RW62_REG 0x62
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_initial_dram : 8;
                            /* The lower-nibble, rank-1 initial DRAM DQS oscillator counter LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the lower-nibble, rank-1 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter LSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW62_STRUCT;

/**
  PG[A] RW[63] - Lower-Nibble, Rank-1 Initial DRAM DQS Oscillator Counter MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW63_REG 0x63
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_initial_dram : 8;
                            /* The lower-nibble, rank-1 initial DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the lower-nibble, rank-1 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter MSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00.
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW63_STRUCT;

/**
  PG[A] RW[64] - Upper-Nibble, Rank-0 Initial DRAM DQS Oscillator Counter LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW64_REG 0x64
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_initial_dram : 8;
                            /* The upper-nibble, rank-0 initial DRAM DQS oscillator counter LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the upper-nibble, rank-0 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter LSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00.
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW64_STRUCT;

/**
  PG[A] RW[65] - Upper-Nibble, Rank-0 Initial DRAM DQS Oscillator Counter MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW65_REG 0x65
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_initial_dram : 8;
                            /* The upper-nibble, rank-0 initial DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the upper-nibble, rank-0 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter MSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW65_STRUCT;

/**
  PG[A] RW[66] - Upper-Nibble, Rank-1 Initial DRAM DQS Oscillator Counter LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW66_REG 0x66
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_1_initial_dram : 8;
                            /* The upper-nibble, rank-1 initial DRAM DQS oscillator counter LSB value. - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the upper-nibble, rank-1 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter LSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW66_STRUCT;

/**
  PG[A] RW[67] - Upper-Nibble, Rank-1 Initial DRAM DQS Oscillator Counter MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW67_REG 0x67
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_1_initial_dram : 8;
                            /* The upper-nibble, rank-1 initial DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written with the
                              read data that is captured from the upper-nibble, rank-1 DRAM as a result of a DRAM-space
                              MRR to its DQS OSC Counter MSB register. Otherwise, it is unaltered. This register can
                              also be written by a DB-space MRW operation and read by a DB-space MRR operation. This
                              register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW67_STRUCT;

/**
  PG[A] RW[68] - Lower-Nibble, Rank-0 Initial DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW68_REG 0x68
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_initial_dram : 8;
                            /* The lower-nibble, rank-0 initial DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay LSB value for the lower-nibble,
                              rank-0 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW68_STRUCT;

/**
  PG[A] RW[69] - Lower-Nibble, Rank-0 Initial DRAM DQS Clock Tree Delay MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW69_REG 0x69
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_initial_dram : 8;
                            /* The lower-nibble, rank-0 initial DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay MSB value for the lower-nibble,
                              rank-0 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00.
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW69_STRUCT;

/**
  PG[A] RW[6A] - Lower-Nibble, Rank-1 Initial DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW6A_REG 0x6A
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_initial_dram : 8;
                            /* The lower-nibble, rank-1 initial DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay LSB value for the lower-nibble,
                              rank-1 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00.
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW6A_STRUCT;

/**
  PG[A] RW[6B] - Lower-Nibble, Rank-1 Initial DRAM DQS Clock Tree Delay MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW6B_REG 0x6B
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_initial_dram : 8;
                            /* The lower-nibble, rank-1 initial DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay MSB value for the lower-nibble,
                              rank-1 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW6B_STRUCT;

/**
  PG[A] RW[6C] - Upper-Nibble, Rank-0 Initial DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW6C_REG 0x6C
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_initial_dram : 8;
                            /* The upper-nibble, rank-0 initial DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay LSB value for the upper-nibble,
                              rank-0 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW6C_STRUCT;

/**
  PG[A] RW[6D] - Upper-Nibble, Rank-0 Initial DRAM DQS Clock Tree Delay MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW6D_REG 0x6D
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_initial_dram : 8;
                            /* The upper-nibble, rank-0 initial DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay MSB value for the upper-nibble,
                              rank-0 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW6D_STRUCT;

/**
  PG[A] RW[6E] - Upper-Nibble, Rank-1 Initial DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW6E_REG 0x6E
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_1_initial_dram : 8;
                            /* The upper-nibble, rank-1 initial DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay LSB value for the upper-nibble,
                              rank-1 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW6E_STRUCT;

/**
  PG[A] RW[6F] - Upper-Nibble, Rank-0 Initial DRAM DQS Clock Tree Delay MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW6F_REG 0x6F
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_1_initial_dram : 8;
                            /* The upper-nibble, rank-1 initial DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking initialization mode is enabled, this register is written by the
                              DB hardware with the calculated DQS clock tree delay MSB value for the upper-nibble,
                              rank-1 DRAM. Otherwise, it is unaltered. This register is read only and can be read by a
                              DB-space MRR operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW6F_STRUCT;

/**
  PG[A] RW[70] - Upper-Nibble, Rank-1 Initial DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW70_REG 0x70
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_current_dram : 8;
                            /* The upper-nibble, rank-0 current DRAM DQS oscillator counter LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written with the read data that
                              is captured from the upper-nibble, rank-0 DRAM as a result of a DRAM-space MRR to its DQS
                              OSC Counter LSB register. Otherwise, it is unaltered. This register can also be written by
                              a DB-space MRW operation and read by a DB-space MRR operation. This register defaults to
                              8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW70_STRUCT;

/**
  PG[A] RW[71] - Lower-Nibble, Rank-0, Current DRAM DQS Oscillator Counter MSB
**/
#define DDR5_DATA_BUFFER_PGA_RW71_REG 0x71
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_current_dram : 8;
                            /* The lower-nibble, rank-0 current DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written with the read data that
                              is captured from the lower-nibble, rank-0 DRAM as a result of a DRAM-space MRR to its DQS
                              OSC Counter MSB register. Otherwise, it is unaltered. This register can also be written by
                              a DB-space MRW operation and read by a DB-space MRR operation. This register defaults to
                              8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW71_STRUCT;

/**
  PG[A] RW[72] - Lower-Nibble, Rank-1, Current DRAM DQS Oscillator CounterLSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW72_REG 0x72
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_current_dram : 8;
                            /* The lower-nibble, rank-1 current DRAM DQS oscillator counter LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written with the read data that
                              is captured from the lower-nibble, rank-1 DRAM as a result of a DRAM-space MRR to its DQS
                              OSC Counter LSB register. Otherwise, it is unaltered. This register can also be written by
                              a DB-space MRW operation and read by a DB-space MRR operation. This register defaults to
                              8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW72_STRUCT;

/**
  PG[A] RW[73] - Lower-Nibble, Rank-1, Current DRAM DQS Oscillator Counter MSB
**/
#define DDR5_DATA_BUFFER_PGA_RW73_REG 0x73
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_current_dram : 8;
                            /* The lower-nibble, rank-1 current DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written with the read data that
                              is captured from the lower-nibble, rank-1 DRAM as a result of a DRAM-space MRR to its DQS
                              OSC Counter MSB register. Otherwise, it is unaltered. This register can also be written by
                              a DB-space MRW operation and read by a DB-space MRR operation. This register defaults to
                              8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW73_STRUCT;

/**
  PG[A] RW[74] - Upper-Nibble, Rank-0, Current DRAM DQS Oscillator Counter LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW74_REG 0x74
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_current_dram : 8;
                            /* The upper-nibble, rank-0 current DRAM DQS oscillator counter LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written with the read data that
                              is captured from the upper-nibble, rank-0 DRAM as a result of a DRAM-space MRR to its DQS
                              OSC Counter LSB register. Otherwise, it is unaltered. This register can also be written by
                              a DB-space MRW operation and read by a DB-space MRR operation. This register defaults to
                              8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW74_STRUCT;

/**
  PG[A] RW[75] - Upper-Nibble, Rank-0, Current DRAM DQS Oscillator Counter MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW75_REG 0x75
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_current_dram : 8;
                            /* The upper-nibble, rank-0 current DRAM DQS oscillator counter MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written with the read data that
                              is captured from the upper-nibble, rank-0 DRAM as a result of a DRAM-space MRR to its DQS
                              OSC Counter MSB register. Otherwise, it is unaltered. This register can also be written by
                              a DB-space MRW operation and read by a DB-space MRR operation. This register defaults to
                              8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW75_STRUCT;

/**
  PG[A] RW[76] - Lower-Nibble, Rank-0 Current DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW76_REG 0x76
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_current_dram : 8;
                            /* The lower-nibble, rank-0 current DRAM DQS clock tree delay LSB value. - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay LSB value for the lowernibble, rank0 DRAM. Otherwise,
                              it is unaltered. This register is read only and can be read by a DBspace MRR operation.
                              This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW76_STRUCT;

/**
  PG[A] RW[77] - Lower-Nibble, Rank-0 Current DRAM DQS Clock Tree Delay MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW77_REG 0x77
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_0_current_dram : 8;
                            /* The lower-nibble, rank-0 current DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay MSB value for the lowernibble, rank0 DRAM. Otherwise,
                              it is unaltered. This register is read only and can be read by a DBspace MRR operation.
                              This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW77_STRUCT;

/**
  PG[A] RW[78] - Lower-Nibble, Rank-1 Current DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW78_REG 0x78
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_current_dram : 8;
                            /* The lower-nibble, rank-1 current DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay LSB value for the lower nibble, rank1 DRAM. Otherwise,
                              it is unaltered. This register is read only and can be read by a DB space MRR operation.
                              This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW78_STRUCT;

/**
  PG[A] RW[79] - Lower-Nibble, Rank-1 Current DRAM DQS Clock Tree Delay MSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW79_REG 0x79
typedef union {
  struct {
    UINT8 the_lower_nibble_rank_1_current_dram : 8;
                            /* The lower-nibble, rank-1 current DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay MSB value for the lower nibble, rank-1 DRAM.
                              Otherwise, it is unaltered. This register is read only and can be read by a DB space MRR
                              operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW79_STRUCT;

/**
  PG[A] RW[7A] - Upper-Nibble, Rank-0 Current DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW7A_REG 0x7A
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_current_dram : 8;
                            /* The upper-nibble, rank-0 current DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay LSB value for the upper nibble, rank0 DRAM. Otherwise,
                              it is unaltered. This register is read only and can be read by a DB-space MRR operation.
                              This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW7A_STRUCT;

/**
  PG[A] RW[7B] - Upper-Nibble, Rank-0 Current DRAM DQS Clock Tree Delay MSB
**/
#define DDR5_DATA_BUFFER_PGA_RW7B_REG 0x7B
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_0_current_dram : 8;
                            /* The upper-nibble, rank-0 current DRAM DQS clock tree delay MSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay MSB value for the upper nibble, rank-0 DRAM.
                              Otherwise, it is unaltered. This register is read only and can be read by a DB-space MRR
                              operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW7B_STRUCT;

/**
  PG[A] RW[7C] - Upper-Nibble, Rank-1 Current DRAM DQS Clock Tree Delay LSB Value
**/
#define DDR5_DATA_BUFFER_PGA_RW7C_REG 0x7C
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_1_current_dram : 8;
                            /* The upper-nibble, rank-1 current DRAM DQS clock tree delay LSB value - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay LSB value for the uppernibble, rank1 DRAM. Otherwise,
                              it is unaltered.This register is read only and can be read by a DBspace MRR operation.
                              This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW7C_STRUCT;

/**
  PG[A] RW[7D] - Upper-Nibble, Rank-1 Current DRAM DQS Clock Tree Delay MSB
**/
#define DDR5_DATA_BUFFER_PGA_RW7D_REG 0x7D
typedef union {
  struct {
    UINT8 the_upper_nibble_rank_1_current_dram : 8;
                            /* The upper-nibble, rank-1 current DRAM DQS clock tree delay MSB value. - Bits[8:0]
                              If DRAM tDQS2DQ tracking mode is enabled, this register is written by the DB hardware with
                              the calculated DQS clock tree delay MSB value for the upper nibble, rank-1 DRAM.
                              Otherwise, it is unaltered.This register is read only and can be read by a DBspace MRR
                              operation. This register defaults to 8h00
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGA_RW7D_STRUCT;

/**
  PG[B] RW[E0] - Rank0 ODTLon_WR_offset Control Word
**/
#define DDR5_DATA_BUFFER_PGB_RWE0_REG 0xE0
typedef union {
  struct {
    UINT8 odtlon_wr_n0_r1_offset_the_offset : 3;
                            /* ODTLon_WR_N0_R1_offset (the offset for the lower nibble of Rank1) - Bits[2:0]
                              000 = Reserved,
                              001 = -4 Clocks,
                              010 = -3 Clocks,
                              011 = -2 Clocks,
                              100 = -1 Clocks (default),
                              101 = 0 Clocks (,
                              110 = +1 Clocks,
                              111 = +2 Clocks
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 odtlon_wr_n1_r1_offset_the_offset : 3;
                            /* ODTLon_WR_N1_R1_offset (the offset for the upper nibble of Rank1) - Bits[6:4]
                              000 = Reserved,
                              001 = -4 Clocks,
                              010 = -3 Clocks,
                              011 = -2 Clocks,
                              100 = -1 Clocks (default),
                              101 = 0 Clocks,
                              110 = +1 Clocks,
                              111 = +2 Clocks
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGB_RWE0_STRUCT;

/**
  PG[B] RW[E1] - Rank1 ODTLon_WR_offset Control Word
**/
#define DDR5_DATA_BUFFER_PGB_RWE1_REG 0xE1
typedef union {
  struct {
    UINT8 odtloff_wr_n0_r0_offset_the_offset : 3;
                            /* ODTLoff_WR_N0_R0_offset (the offset for the lower nibble of Rank0) - Bits[2:0]
                              000 = Reserved,
                              001 = +4 Clocks,
                              010 = +3 Clocks,
                              011 = +2 Clocks,
                              100 = +1 Clocks,
                              101 = 0 Clocks (default),
                              110 = -1 Clocks,
                              111 = -2 Clocks
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 odtloff_wr_n1_r0_offset_the_offset : 3;
                            /* ODTLoff_WR_N1_R0_offset (the offset for the upper nibble of Rank0) - Bits[6:4]
                              000 = Reserved,
                              001 = +4 Clocks,
                              010 = +3 Clocks,
                              011 = +2 Clocks,
                              100 = +1 Clocks,
                              101 = 0 Clocks (default),
                              110 = -1 Clocks,
                              111 = -2 Clocks
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGB_RWE1_STRUCT;

/**
  PG[B] RW[E2] - Rank0 ODTLoff_WR_offset Control Word
**/
#define DDR5_DATA_BUFFER_PGB_RWE2_REG 0xE2
typedef union {
  struct {
    UINT8 odtloff_wr_n0_r1_offset_the_offset : 3;
                            /* ODTLoff_WR_N0_R1_offset (the offset for the lower nibble of Rank1) - Bits[2:0]
                              000 = Reserved,
                              001 = +4 Clocks,
                              010 = +3 Clocks,
                              011 = +2 Clocks,
                              100 = +1 Clocks,
                              101 = 0 Clocks (default),
                              110 = -1 Clocks,
                              111 = -2 Clocks
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 odtloff_wr_n1_r1_offset_the_offset : 3;
                            /* ODTLoff_WR_N1_R1_offset (the offset for the upper nibble of Rank1) - Bits[6:4]
                              000 = Reserved,
                              001 = +4 Clocks,
                              010 = +3 Clocks,
                              011 = +2 Clocks,
                              100 = +1 Clocks,
                              101 = 0 Clocks (default),
                              110 = -1 Clocks,
                              111 = -2 Clocks
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGB_RWE2_STRUCT;

/**
  PG[B] RW[E3] - Rank1 ODTLoff_WR_offset Control Word
**/
#define DDR5_DATA_BUFFER_PGB_RWE3_REG 0xE3
typedef union {
  struct {
    UINT8 odtloff_wr_n0_r1_offset_the_offset : 3;
                            /* ODTLoff_WR_N0_R1_offset (the offset for the lower nibble of Rank1) - Bits[2:0]
                              000 = Reserved,
                              001 = +4 Clocks,
                              010 = +3 Clocks,
                              011 = +2 Clocks,
                              100 = +1 Clocks,
                              101 = 0 Clocks (default),
                              110 = -1 Clocks,
                              111 = -2 Clocks
                            */
    UINT8 reserved : 1;
                            /* Reserved - Bits[3]
                              0 = Reserved,
                              1 = Reserved
                            */
    UINT8 odtloff_wr_n1_r1_offset_the_offset : 3;
                            /* ODTLoff_WR_N1_R1_offset (the offset for the upper nibble of Rank1) - Bits[6:4]
                              000 = Reserved,
                              001 = +4 Clocks,
                              010 = +3 Clocks,
                              011 = +2 Clocks,
                              100 = +1 Clocks,
                              101 = 0 Clocks (default),
                              110 = -1 Clocks,
                              111 = -2 Clocks
                            */
    UINT8 reserved_1 : 1;
                            /* Reserved - Bits[7]
                              0 = Reserved,
                              1 = Reserved
                            */
  } Bits;
  UINT8 Data;
} DDR5_DATA_BUFFER_PGB_RWE3_STRUCT;


#endif  // _DDR5_DB_REGS_H_
