
/** @file
  CWV_DDRIO_HSIO.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>
  
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
  
  This file contains Silicon register definitions.
  
  This is a generated file; please do not modify it directly.
  
**/

/* The following security policy groups are used by registers in this file:     */

/* CWV Security Policy Groups:                                                  */


#ifndef _CWV_DDRIO_HSIO_h
#define _CWV_DDRIO_HSIO_h
#include <DataTypes.h>

/* DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG supported on:                       */
/*      CWV (0x40807000)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Training Enable
*/


#define DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG 0x1BA42000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               FMV general training mode Needs to be set for
                               all training steps
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Early CMD-CK Training FMC Parity-only Training
                               mode Does not submit commands to ingress queue -
                               only computes parity
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Advanced CMD-CK Training FMC block CSR writes
                               training mode Used during advanced cmd-clk
                               training. Protects the FMC states from being
                               changed as CMD/CTL signals are margined.
                            */
    UINT32 basic_req_trn : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* DDRT2 REQ Training Enable */
    UINT32 recen_dq_dqs_mpr_trn : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* DDRT2 HSIO RD Training Pattern Enable */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_STRUCT;

/* RW00_GLOBAL_FEATURES_CWV_DDRIO_HSIO_REG supported on:                        */
/*      CWV (0x40807004)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* RW00 - Global Features Control Word
*/


#define RW00_GLOBAL_FEATURES_CWV_DDRIO_HSIO_REG 0x1BA42004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 bcom_pass_thru_mode : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               BCOM pass Through mode 0 - Normal operation 1 -
                               Enabled
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW00_GLOBAL_FEATURES_CWV_DDRIO_HSIO_STRUCT;

/* RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_REG supported on:                         */
/*      CWV (0x40807008)                                                        */
/* Register default value on CWV: 0x00000002                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* RW01 - Parity Control Word
*/


#define RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_REG 0x1BA42008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 par_chk : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Parity Checking 0 - Parity checking disabled 1 -
                               Parity checking enabled Function not supported
                            */
    UINT32 blk_cmd_dram : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               DRAM Interface Block All CMDs 0 - Block 1 -
                               Forward Function not supported
                            */
    UINT32 rsvd_0 : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 blk_cmd_dq_buffer : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - Block 1 - Forward Default of this CSR is to
                               block commands to DQ Buffer. BIOS will program
                               this CSR to a "1" when BCOM interface is trained
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_STRUCT;

/* RW02_HOST_INTERFACE_TRAINING_MODE_CWV_DDRIO_HSIO_REG supported on:           */
/*      CWV (0x4080700c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* RW02 - Host Interface Training Mode Global Control Word
*/


#define RW02_HOST_INTERFACE_TRAINING_MODE_CWV_DDRIO_HSIO_REG 0x1BA4200C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_select_cha : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - Normal operating mode 1 - Clock to DCA
                               training mode 2 - DCS0_n training mode 3 -
                               DCS1_n training mode(not supported)
                            */
    UINT32 tm_select_chb : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - Normal operating mode 1 - Clock to DCA
                               training mode 2 - DCS0_n training mode 3 -
                               DCS1_n training mode(not supported)
                            */
    UINT32 dca_trn_xor_sampling_edge : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - Both rising and falling edge 1 - Rising edge
                               (even samples) 2 - Falling edge (odd sampled) 3
                               - Reserved
                            */
    UINT32 vrefcabroadcast : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* Broadcast not supported */
    UINT32 vrefcsbroadcast : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Broadcast not supported */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW02_HOST_INTERFACE_TRAINING_MODE_CWV_DDRIO_HSIO_STRUCT;

/* RW03_DB_INTF_TRAINING_MODE_CWV_DDRIO_HSIO_REG supported on:                  */
/*      CWV (0x40807010)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* RW03 - Data Buffer Interface Training Mode Control Word
*/


#define RW03_DB_INTF_TRAINING_MODE_CWV_DDRIO_HSIO_REG 0x1BA42010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 bcom0_val : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Static Control Value for BCOM0 when Static BCOM
                               control enabled 0 - Output driven statically Low
                               1 - Output driven statically High
                            */
    UINT32 bcom1_val : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Static Control Value for BCOM1 when Static BCOM
                               control enabled 0 - Output driven statically Low
                               1 - Output driven statically High
                            */
    UINT32 bcom2_val : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               Static Control Value for BCOM2 when Static BCOM
                               control enabled 0 - Output driven statically Low
                               1 - Output driven statically High
                            */
    UINT32 bcs_n_val : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Static Control Value for BCS_n when Static BCOM
                               control enabled 0 - Output driven statically Low
                               1 - Output driven statically High
                            */
    UINT32 bcom_static_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Static BCOM Control enable */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW03_DB_INTF_TRAINING_MODE_CWV_DDRIO_HSIO_STRUCT;

/* RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG supported on:                         */
/*      CWV (0x40807014)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* RW04 - Command Space Control Word
*/


#define RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG 0x1BA42014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmdnum : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field should be program along with
                               rw03_db_intf_training_mode.bcom_static_en, for
                               bcom reset to assert to dq buffer [br] 0x0 - NOP
                               [br] 0x1 - CH_A DB Reset [Swicthes BRST_A _n to
                               active which is LOW] [br] 0x2 - Clear CH_A_DB
                               _Reset [Switches BRST_A_n to inactive which is
                               HIGH] [br] 0x3 - CH_B_DB _Reset [Switches
                               BRST_B_n to active which is LOW] [br] 0x4 -
                               Clear CH_B DB_Reset [Switches BRST_B_n to
                               inactive which is HIGH] [br] rest - reserved
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_STRUCT;

/* DDRT_HSIO_TRAINING_MR0_CWV_DDRIO_HSIO_REG supported on:                      */
/*      CWV (0x40807018)                                                        */
/* Register default value on CWV: 0x00000003                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register mimics DDR5 Mode Registers
*/


#define DDRT_HSIO_TRAINING_MR0_CWV_DDRIO_HSIO_REG 0x1BA42018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tcl_gnt_data_0 : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               tcl_gnt_data_0: CAS latency measured from GNT
                               command to Read Data [in number of DCLKs] Note:
                               This register field is combined with bits [3:1]
                               of this register to form TCL GNT2DATA[4:0]
                            */
    UINT32 tcl_gnt_data : 4;

                            /* Bits[4:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               tcl_gnt_data: CAS latency measured from GNT
                               command to Read Data [in number of DCLKs] Note:
                               This register field is combined with bit [0] of
                               this register to form TCL GNT2DATA[4:0]
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_TRAINING_MR0_CWV_DDRIO_HSIO_STRUCT;

/* DDRT_HSIO_TRAINING_MR2_CWV_DDRIO_HSIO_REG supported on:                      */
/*      CWV (0x4080701c)                                                        */
/* Register default value on CWV: 0x00000006                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register mimics DDR5 Mode Registers
*/


#define DDRT_HSIO_TRAINING_MR2_CWV_DDRIO_HSIO_REG 0x1BA4201C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tcwl : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000006*/

                            /*
                               tcwl: CAS lWrite atency measured from WR command
                               to Write Data [in number of DCLKs]
                            */
    UINT32 write_leveling : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* 0: Normal Mode [default] 1: Write Leveling */
    UINT32 cmd_2n_en : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Command Timing DDRT2 Command 1N/2N timing 00: 1N
                               [default] 01: 2N [Not supported] 1X: Reserved
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_TRAINING_MR2_CWV_DDRIO_HSIO_STRUCT;


/* MR25_RD_TRN_MODE_CWV_DDRIO_HSIO_REG supported on:                            */
/*      CWV (0x40807024)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - Read Training Mode Settings
*/


#define MR25_RD_TRN_MODE_CWV_DDRIO_HSIO_REG 0x1BA42024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pat_format : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Read Training Pattern Format 0 - Serial 1 - LFSR */
    UINT32 lfsr0_pat_sel : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* LFSR0 Pattern Option 0 - LFSR 1 - Clock */
    UINT32 lfsr1_pat_sel : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* LFSR1 Pattern Option 0 - LFSR 1 - Clock */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR25_RD_TRN_MODE_CWV_DDRIO_HSIO_STRUCT;


/* MR26_RD_PAT_D0_LFSR0_CWV_DDRIO_HSIO_REG supported on:                        */
/*      CWV (0x4080702c)                                                        */
/* Register default value on CWV: 0x0000005A                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - Read Pattern Data0 or LFSR0 seed
*/


#define MR26_RD_PAT_D0_LFSR0_CWV_DDRIO_HSIO_REG 0x1BA4202C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rdp0_lfsr0_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x0000005A*/

                            /*
                               Read Training Pattern Data0 or LFSR0 Seed Serial
                               Mode: (selected in MR25) Bit [0] - Data in 1UI
                               Bit [1] - Data in 2UI Bit [2] - Data in 3UI Bit
                               [3] - Data in 4UI Bit [4] - Data in 5UI Bit [5]
                               - Data in 6UI Bit [6] - Data in 7UI Bit [7] -
                               Data in 8UI LFSR Mode: (selected in MR25) LFSR0
                               seed
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR26_RD_PAT_D0_LFSR0_CWV_DDRIO_HSIO_STRUCT;

/* MR27_RD_PAT_D1_LFSR1_CWV_DDRIO_HSIO_REG supported on:                        */
/*      CWV (0x40807030)                                                        */
/* Register default value on CWV: 0x0000003C                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - Read Pattern Data1 or LFSR1 seed
*/


#define MR27_RD_PAT_D1_LFSR1_CWV_DDRIO_HSIO_REG 0x1BA42030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rdp1_lfsr1_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x0000003C*/

                            /*
                               Read Training Pattern Data1 or LFSR1 Seed Serial
                               Mode: (selected in MR25) Bit [0] - Data in 9UI
                               Bit [1] - Data in 10UI Bit [2] - Data in 11UI
                               Bit [3] - Data in 12UI Bit [4] - Data in 13UI
                               Bit [5] - Data in 14UI Bit [6] - Data in 15UI
                               Bit [7] - Data in 16UI LFSR Mode: (selected in
                               MR25) LFSR1 seed
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR27_RD_PAT_D1_LFSR1_CWV_DDRIO_HSIO_STRUCT;

/* MR28_RD_PAT_INV_LOWER_CWV_DDRIO_HSIO_REG supported on:                       */
/*      CWV (0x40807034)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - Read Pattern Invert Lower - DQL[7:0]
*/


#define MR28_RD_PAT_INV_LOWER_CWV_DDRIO_HSIO_REG 0x1BA42034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dql : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert Lower Data bits in Read Pattern Bit[0] =
                               1 {Invert DQ0] Bit[1] = 1 {Invert DQ1] Bit[2] =
                               1 {Invert DQ2] Bit[3] = 1 {Invert DQ3] Bit[4] =
                               1 {Invert DQ4] Bit[5] = 1 {Invert DQ5] Bit[6] =
                               1 {Invert DQ6] Bit[7] = 1 {Invert DQ7]
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR28_RD_PAT_INV_LOWER_CWV_DDRIO_HSIO_STRUCT;


/* MR30_RD_LFSR_MAP_CWV_DDRIO_HSIO_REG supported on:                            */
/*      CWV (0x4080703c)                                                        */
/* Register default value on CWV: 0x000000FE                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - Read LFSR Assignments
*/


#define MR30_RD_LFSR_MAP_CWV_DDRIO_HSIO_REG 0x1BA4203C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dql_dqu : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x000000FE*/

                            /*
                               LFSR 0/1 output assignments to DQ Bit[0] - LFSR
                               assignnment for DQL0/DQU0 {0 = LFSR0, 1 = LFSR1}
                               Bit[1] - LFSR assignnment for DQL1/DQU1 {0 =
                               LFSR0, 1 = LFSR1} Bit[2] - LFSR assignnment for
                               DQL2/DQU2 {0 = LFSR0, 1 = LFSR1} Bit[3] - LFSR
                               assignnment for DQL3/DQU3 {0 = LFSR0, 1 = LFSR1}
                               Bit[4] - LFSR assignnment for DQL4/DQU4 {0 =
                               LFSR0, 1 = LFSR1} Bit[5] - LFSR assignnment for
                               DQL5/DQU5 {0 = LFSR0, 1 = LFSR1} Bit[6] - LFSR
                               assignnment for DQL6/DQU6 {0 = LFSR0, 1 = LFSR1}
                               Bit[7] - LFSR assignnment for DQL7/DQU7 {0 =
                               LFSR0, 1 = LFSR1}
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR30_RD_LFSR_MAP_CWV_DDRIO_HSIO_STRUCT;

/* RW31_DFE_CONFIG_CWV_DDRIO_HSIO_REG supported on:                             */
/*      CWV (0x40807040)                                                        */
/* Register default value on CWV: 0x00000008                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* RW31 RCD control word-Far memory Controller DFE configuration Control Word

*/


#define RW31_DFE_CONFIG_CWV_DDRIO_HSIO_REG 0x1BA42040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_gain_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - Disabled 1 - DFE and Gain features enabled
                               Feature Not supported
                            */
    UINT32 dfe_vref_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - DFE Vref Circuitry disabled 1 - DFE Vref
                               Circtuitry enabled Feature Not supported.
                            */
    UINT32 err_cntr_dis : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0-All error counters enabled 1-All error
                               counters disbaled
                            */
    UINT32 rw_control_word_writes_broadcasted : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* Broadcast not supported here. */
    UINT32 tap_en_4_1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0- Tap[4:1] disabled (default ) 1- Tap [4:1]
                               Enabled Feature Not supported.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW31_DFE_CONFIG_CWV_DDRIO_HSIO_STRUCT;

/* RW32_DFETM_CTL_CWV_DDRIO_HSIO_REG supported on:                              */
/*      CWV (0x40807044)                                                        */
/* Register default value on CWV: 0x00000080                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DPAR and DCA [6:0]DFE training mode (DFETM)

*/


#define RW32_DFETM_CTL_CWV_DDRIO_HSIO_REG 0x1BA42044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 glb_dfe_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - DFE Training mode disabled (DFETM) 1 - DEFTM
                               Enabled
                            */
    UINT32 trn_src : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 -Monitor or Raw 1 - Monitor XOR Slicer Output
                               2- LFSR XOR (8 bit shift register) Note that "0"
                               or "1" are not supported, and setting value to
                               "3" has same impact as setting to "2".
                            */
    UINT32 dcn_dfe_sel : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /* 6:0-DCA [6:0] 7-DPAR */
    UINT32 rwupd_en : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Do not support RWUPD scheme [BIOS will use SMBUS
                               to program registers during CA DFE acceleration]
                            */
    UINT32 exit_rwupd_mode : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               Do not support RWUPD scheme [BIOS will use SMBUS
                               to program registers during CA DFE acceleration]
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW32_DFETM_CTL_CWV_DDRIO_HSIO_STRUCT;


/* RW34_LFSR_DFETM_CWV_DDRIO_HSIO_REG supported on:                             */
/*      CWV (0x4080704c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DFE training accelerator
DFE LFSR Training mode control word

*/


#define RW34_LFSR_DFETM_CWV_DDRIO_HSIO_REG 0x1BA4204C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_acc_fsm_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0- DFE Training Accelerator Stop DFE Training
                               Accelerator 1- Start DFE Training Accelerator
                            */
    UINT32 lfsr_load : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* "1" - Loads all LFSRs with LFSR seed */
    UINT32 inner_loop_param : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               "0-NULL/No change 1-Even Sampler Offset 2-VrefCA
                               3-Odd Sampler Offset" If even_odd_sampler_offset
                               = 1, it will be inner loop parameter regardless
                               of value here.
                            */
    UINT32 outer_loop_param : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               "0-NULL/ No change 1-Tap1 2-Tap2 3-Tap3 4-Tap4
                               5, 6,7-Reserved"
                            */
    UINT32 even_odd_sampler_offset : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               "1 - Both samplers adjusted 0 - single sampler
                               adjusted" If even_odd_sampler_offset = 1, it
                               will be inner loop parameter regardless of value
                               in inner_loop_param[3:2].
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW34_LFSR_DFETM_CWV_DDRIO_HSIO_STRUCT;


/* DDRT_HSIO_DDRIO_CMD_EN_CWV_DDRIO_HSIO_REG supported on:                      */
/*      CWV (0x40807054)                                                        */
/* Register default value on CWV: 0x00000099                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Enable IO controls
*/


#define DDRT_HSIO_DDRIO_CMD_EN_CWV_DDRIO_HSIO_REG 0x1BA42054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gnt_n_mask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               When set, DA PHY does not decode/use the CS from
                               IO Clear CSR when IO is initialized
                            */
    UINT32 mc2ddrcsoutputenable : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               BCS, BCOM control: Output Enable 0: Disable 1:
                               Enable
                            */
    UINT32 mc2ddrodtoutputenable : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               REQ, ERR control: Output Enable 0: Disable 1:
                               Enable
                            */
    UINT32 bcs_invert : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               BCS Inversion BCS_N is active low in standard DQ
                               buffer For CWB, BCS_N is active high 0: Active
                               High 1: Active Low
                            */
    UINT32 brst_invert : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               BRST Inversion BRST_N is active low in standard
                               DQ buffer For CWB, BRST_N is active high 0:
                               Active High 1: Active Low
                            */
    UINT32 mc2ddrtcmdoutputenable_bclk : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               CMD Output Enable for BCLK_DP & BCLK_DN 0:
                               Disable 1: Enable
                            */
    UINT32 ddrt_bclk_invert : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               (Deprecated) BCLK Invertion: Inverts
                               xxDDRT_BCLKs
                            */
    UINT32 fnvbclkpark1 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* BCLK PARK 1: Parks BCLK at One */
    UINT32 fnvbclkpark0 : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* BCLK PARK 0: Parks BCLK at Zero */
    UINT32 ddrtio_par_flopen : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mux select on io2dap_cmd and io2dap_cmd_par 1:
                               flopped 0: bypass
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_DDRIO_CMD_EN_CWV_DDRIO_HSIO_STRUCT;


/* DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_REG supported on:                       */
/*      CWV (0x40807058)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Write Data Scrambling Control
*/


#define DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_REG 0x1BA42058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lfsr_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Scrambling LFSR Enable */
    UINT32 lfsr_seed_ordersel : 16;

                            /* Bits[16:1], Access Type=RW/P, default=0x00000000*/

                            /* Scrambling LFSR Seed Order Select */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_STRUCT;


/* DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG supported on:                       */
/*      CWV (0x4080705c)                                                        */
/* Register default value on CWV: 0x0003FFFC                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Read Data Scrambling Control
*/


#define DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG 0x1BA4205C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lfsr_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Scrambling LFSR Enable */
    UINT32 lfsr_load : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* Scrambling LFSR Load */
    UINT32 lfsr_seed : 16;

                            /* Bits[17:2], Access Type=RW/P, default=0x0000FFFF*/

                            /* Scrambling LFSR Seed */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_STRUCT;


/* DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_REG supported on:                   */
/*      CWV (0x40807060)                                                        */
/* Register default value on CWV: 0x00400FF8                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Functional Defeature Bits
*/


#define DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_REG 0x1BA42060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clkstp_csdet : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000008*/

                            /*
                               Number of clocks [DCLKs] CS needs to be asserted
                               for Clock Stop Entry detection
                            */
    UINT32 noclkstp_exitdet : 16;

                            /* Bits[19:4], Access Type=RW/P, default=0x000000FF*/

                            /*
                               Number of clocks [DCLKs] after SRE Power Down
                               [without Clock Stop] entry is detected to SRE
                               Power Down Exit detection.
                            */
    UINT32 wdata_dq_swiz : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Inversion Disabled 1: Invert Even and Odd
                               Data Transfers
                            */
    UINT32 rdata_dq_swiz : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Inversion Disabled 1: Invert Even and Odd
                               Data Transfers
                            */
    UINT32 dq_spare_lane_used : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* 0: Disable spare lane used 1: Enable spare lane */
    UINT32 clear_ddrt_err : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Clear DDRT ERR: Clear ddrt_err_n; this bit needs
                               to be clear for the next ddrt_err_n error
                               assertion This bit is only used during CTL-CK
                               training; setting the bit will de-assert
                               DDRT_ERR# pin without following the normal error
                               flow sequence 0 = normal 1 = deasssert
                               ddrt_err_n
                            */
    UINT32 zqcal_latch : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Defeature ZQCal Latch command when set = 1. */
    UINT32 zqcal_start : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* Defeature ZQCal Start command when set = 1. */
    UINT32 pde_dis : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Defeature Disable PDE When the bit is set, PDE
                               is disabled. PDE command is ignored
                            */
    UINT32 mpc_broadcast : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Defeature Enable MPC broadcast mode When the bit
                               is set, MPC send to SA will be broadcast to SB.
                               Default is have MPC in SA and SB independant
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_FUNC_DEFEATURE0_CWV_DDRIO_HSIO_STRUCT;

































/* RW63_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x408070ec)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca0_lfsr_seed

*/


#define RW63_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA420EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca0_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW63_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW67_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x408070f0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca1_lfsr_seed
*/


#define RW67_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA420F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca1_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW67_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW6B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x408070f4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca2_lfsr_seed
*/


#define RW6B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA420F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca2_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW6B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW6F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x408070f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca3_lfsr_seed
*/


#define RW6F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA420F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca3_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW6F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW73_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x408070fc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca4_lfsr_seed
*/


#define RW73_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA420FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca4_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW73_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW77_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x40807100)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca5_lfsr_seed
*/


#define RW77_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA42100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca5_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW77_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW7B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x40807104)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca6_lfsr_seed
*/


#define RW7B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA42104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca6_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW7B_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* RW7F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x40807108)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* ca7_lfsr_seed
*/


#define RW7F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_REG 0x1BA42108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ca7_lfsr_seed : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* LFSR]=0 x00:0xFF */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW7F_DFE_LFSR_SEED_CWV_DDRIO_HSIO_STRUCT;

/* MR8_WR_RD_PRE_POSTAMBLE_CWV_DDRIO_HSIO_REG supported on:                     */
/*      CWV (0x4080710c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - MR8- Preamble/Postamble
*/


#define MR8_WR_RD_PRE_POSTAMBLE_CWV_DDRIO_HSIO_REG 0x1BA4210C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 read_preamble : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               000: 1 tCK - 10 Pattern 001: 2 tCK - 0010
                               Pattern 010: 2 tCK - 1110 Pattern (DDR4 Style)
                               011: 3 tCK - 000010 Pattern 100: 4 tCK -
                               00001010 Pattern 101: Reserved 110: Reserved
                               111: Reserved
                            */
    UINT32 write_preamble : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               00: Reserved 01: 2 tCK - 0010 Pattern 10: 3 tCK
                               - 000010 Pattern 11: 4 tCK - 00001010 Pattern
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 rd_postamble : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* 0: 0.5 tCK - 0 Pattern 1: 1.5 tCK - 010 Pattern */
    UINT32 wr_postamble : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* 0: 0.5 tCK - 0 Pattern 1: 1.5 tCK - 000 Pattern */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR8_WR_RD_PRE_POSTAMBLE_CWV_DDRIO_HSIO_STRUCT;

/* MR40_RD_DQS_OFFSET_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x40807110)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* DDR5 Mode Register - MR40 -Read DQS Offset Timing
*/


#define MR40_RD_DQS_OFFSET_CWV_DDRIO_HSIO_REG 0x1BA42110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_dqs_offset : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               000: 0 Clock (DEFAULT) 001: 1 Clock 010: 2
                               Clocks 011: 3 Clocks 100: RFU 101: RFU 110: RFU
                               111: RFU
                            */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MR40_RD_DQS_OFFSET_CWV_DDRIO_HSIO_STRUCT;










/* DDRT_HSIO_CCC_CFG5_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x40807138)                                                        */
/* Register default value on CWV: 0x2003E01F                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* PI control for clk2 and clk3 module, CCC0 buf0 and buf1
*/


#define DDRT_HSIO_CCC_CFG5_CWV_DDRIO_HSIO_REG 0x1BA42138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sel_pi_clk2 : 13;

                            /* Bits[12:0], Access Type=RW/P, default=0x0000001F*/

                            /*
                               (ccc1/ccctop0) 1: cmdbuf uses PI group 0, 0:
                               cmdbuf uses alternate PI group determined by
                               floorplan
                            */
    UINT32 sel_pi_clk3 : 13;

                            /* Bits[25:13], Access Type=RW/P, default=0x0000001F*/

                            /*
                               (ccc1/ccctop1) 1: cmdbuf uses PI group 0, 0:
                               cmdbuf uses alternate PI group determined by
                               floorplan
                            */
    UINT32 txtype_ccc0_buf0 : 3;

                            /* Bits[28:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               (ccc0/ccctop0/buf0) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc0_buf1 : 3;

                            /* Bits[31:29], Access Type=RW/P, default=0x00000001*/

                            /*
                               (ccc0/ccctop0/buf1) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_CCC_CFG5_CWV_DDRIO_HSIO_STRUCT;



/* DDRT_HSIO_CCC_CFG8_CWV_DDRIO_HSIO_REG supported on:                          */
/*      CWV (0x40807144)                                                        */
/* Register default value on CWV: 0x2EE48B6D                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* PI control for ccc1 buf9 to buf12, ccc2 buf0 to buf5
*/


#define DDRT_HSIO_CCC_CFG8_CWV_DDRIO_HSIO_REG 0x1BA42144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txtype_ccc1_buf9 : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000005*/

                            /*
                               (ccc0/ccctop1/buf9) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc1_buf10 : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000005*/

                            /*
                               (ccc0/ccctop1/buf10) 0x0: CA, 0x1: CLK, 0x2:
                               CKE, 0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE,
                               0x7: ODT
                            */
    UINT32 txtype_ccc1_buf11 : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000005*/

                            /*
                               (ccc0/ccctop1/buf11) 0x0: CA, 0x1: CLK, 0x2:
                               CKE, 0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE,
                               0x7: ODT
                            */
    UINT32 txtype_ccc1_buf12 : 3;

                            /* Bits[11:9], Access Type=RW/P, default=0x00000005*/

                            /*
                               (ccc0/ccctop1/buf12) 0x0: CA, 0x1: CLK, 0x2:
                               CKE, 0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE,
                               0x7: ODT
                            */
    UINT32 txtype_ccc2_buf0 : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               (ccc1/ccctop0/buf0) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc2_buf1 : 3;

                            /* Bits[17:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               (ccc1/ccctop0/buf1) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc2_buf2 : 3;

                            /* Bits[20:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               (ccc1/ccctop0/buf2) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc2_buf3 : 3;

                            /* Bits[23:21], Access Type=RW/P, default=0x00000007*/

                            /*
                               (ccc1/ccctop0/buf3) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc2_buf4 : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000006*/

                            /*
                               (ccc1/ccctop0/buf4) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 txtype_ccc2_buf5 : 3;

                            /* Bits[29:27], Access Type=RW/P, default=0x00000005*/

                            /*
                               (ccc1/ccctop0/buf5) 0x0: CA, 0x1: CLK, 0x2: CKE,
                               0x3: CS, 0x4: RSVD, 0x5: WCK, 0x6: CS_CKE, 0x7:
                               ODT
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_HSIO_CCC_CFG8_CWV_DDRIO_HSIO_STRUCT;



/* RW36_DFETM_INNER_LOOP_START_CWV_DDRIO_HSIO_REG supported on:                 */
/*      CWV (0x40807150)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DFE training accelerator inner loop start value
*/


#define RW36_DFETM_INNER_LOOP_START_CWV_DDRIO_HSIO_REG 0x1BA42150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 inner_loop_start_value : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 0 0 0 0 0 0 0 Inner Loop Start Value = 0x00 0
                               0 0 0 0 0 0 1 Inner Loop Start Value = 0x01 0 0
                               0 0 0 0 1 0 Inner Loop Start Value = 0x02 ... 1
                               1 1 1 1 1 0 0 Inner Loop Start Value = 0xFC 1 1
                               1 1 1 1 0 1 Inner Loop Start Value = 0xFD 1 1 1
                               1 1 1 1 0 Inner Loop Start Value = 0xFE 1 1 1 1
                               1 1 1 1 Inner Loop Start Value = 0xFF
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW36_DFETM_INNER_LOOP_START_CWV_DDRIO_HSIO_STRUCT;

/* RW37_DFETM_OUTER_LOOP_START_CWV_DDRIO_HSIO_REG supported on:                 */
/*      CWV (0x40807154)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DFE training accelerator outer loop start value
*/


#define RW37_DFETM_OUTER_LOOP_START_CWV_DDRIO_HSIO_REG 0x1BA42154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 outer_loop_start_value : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 0 0 0 0 0 0 Outer Loop Start Value = 0x00 0 0
                               0 0 0 0 1 Outer Loop Start Value = 0x01 0 0 0 0
                               0 1 0 Outer Loop Start Value = 0x02 ... 1 1 1 1
                               1 0 0 Outer Loop Start Value = 0x7C 1 1 1 1 1 0
                               1 Outer Loop Start Value = 0x7D 1 1 1 1 1 1 0
                               Outer Loop Start Value = 0x7E 1 1 1 1 1 1 1
                               Outer Loop Start Value = 0x7F
                            */
    UINT32 reserved : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* reserved */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW37_DFETM_OUTER_LOOP_START_CWV_DDRIO_HSIO_STRUCT;



/* RW3A_DFETM_LOOP_STEP_SIZE_CWV_DDRIO_HSIO_REG supported on:                   */
/*      CWV (0x40807160)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DFE training accelerator step sizes
If the next iteration's step size would cause an overflow, one final iteration is performed at the parameter's max value.
*/


#define RW3A_DFETM_LOOP_STEP_SIZE_CWV_DDRIO_HSIO_REG 0x1BA42160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 inner_loop_step_size : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 0 0 0 Inner Loop Step Size - 1 0 0 0 1 Inner
                               Loop Step Size - 2 0 0 1 0 Inner Loop Step Size
                               - 3 0 0 1 1 Inner Loop Step Size - 4 0 1 0 0
                               Inner Loop Step Size - 5 0 1 0 1 Inner Loop Step
                               Size - 6 0 1 1 0 Inner Loop Step Size - 7 0 1 1
                               1 Inner Loop Step Size - 8 1 0 0 0 Inner Loop
                               Step Size - 9 1 0 0 1 Inner Loop Step Size - 10
                               1 0 1 0 Inner Loop Step Size - 11 1 0 1 1 Inner
                               Loop Step Size - 12 1 1 0 0 Inner Loop Step Size
                               - 13 1 1 0 1 Inner Loop Step Size - 14 1 1 1 0
                               Inner Loop Step Size - 15 1 1 1 1 Inner Loop
                               Step Size - 16
                            */
    UINT32 outer_loop_step_size : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 0 0 0 Inner Loop Step Size - 1 0 0 0 1 Inner
                               Loop Step Size - 2 0 0 1 0 Inner Loop Step Size
                               - 3 0 0 1 1 Inner Loop Step Size - 4 0 1 0 0
                               Inner Loop Step Size - 5 0 1 0 1 Inner Loop Step
                               Size - 6 0 1 1 0 Inner Loop Step Size - 7 0 1 1
                               1 Inner Loop Step Size - 8 1 0 0 0 Inner Loop
                               Step Size - 9 1 0 0 1 Inner Loop Step Size - 10
                               1 0 1 0 Inner Loop Step Size - 11 1 0 1 1 Inner
                               Loop Step Size - 12 1 1 0 0 Inner Loop Step Size
                               - 13 1 1 0 1 Inner Loop Step Size - 14 1 1 1 0
                               Inner Loop Step Size - 15 1 1 1 1 Inner Loop
                               Step Size - 16
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW3A_DFETM_LOOP_STEP_SIZE_CWV_DDRIO_HSIO_STRUCT;

/* RW3B_DFETM_INNER_LOOP_INCR_CWV_DDRIO_HSIO_REG supported on:                  */
/*      CWV (0x40807164)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DFE training accelerator expected inner increments
*/


#define RW3B_DFETM_INNER_LOOP_INCR_CWV_DDRIO_HSIO_REG 0x1BA42164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 inner_loop_number_of_increments : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of Increments = value - 1. For example,
                               set the value to 5 results in 4 inner loop
                               increments. When the number of increments =0x00
                               or 0x01 the Start value will be applied for a
                               single iteration of the loop Programmed number
                               of increments will not be performed if max value
                               has been reached.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW3B_DFETM_INNER_LOOP_INCR_CWV_DDRIO_HSIO_STRUCT;

/* RW3C_DFETM_OUTER_LOOP_INCR_CWV_DDRIO_HSIO_REG supported on:                  */
/*      CWV (0x40807168)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrt_hsio/ddrphy_ddrt_hsio/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup:                                                    */
/* Far Memory Controller DFE training accelerator expected outer increments
*/


#define RW3C_DFETM_OUTER_LOOP_INCR_CWV_DDRIO_HSIO_REG 0x1BA42168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 outer_loop_number_of_increments : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Outer Loop Number of Increments = value - 1. For
                               example, set the value to 5 results in 4 outer
                               loop increments. When the number of increments
                               =0x00 or 0x01 the Start value will be applied
                               for a single iteration of the loop Programmed
                               number of increments will not be performed if
                               max value has been reached.
                            */
    UINT32 reserved : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* reserved */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RW3C_DFETM_OUTER_LOOP_INCR_CWV_DDRIO_HSIO_STRUCT;















#endif /* _CWV_DDRIO_HSIO_h */
