
/** @file
  FNV_DA_UNIT_0.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2018 Intel Corporation. <BR>
  
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

/* BWV Security Policy Groups:                                                  */


#ifndef _FNV_DA_UNIT_0_h
#define _FNV_DA_UNIT_0_h
#include "DataTypes.h"

/* DA_DDRT_TRAINING_MR0_FNV_DA_UNIT_0_REG supported on:                         */
/*      BWV (0x40002000)                                                        */
/* Register default value on BWV: 0x00000020                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define DA_DDRT_TRAINING_MR0_FNV_DA_UNIT_0_REG 0x1B240000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcl_gnt_data_0 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               CAS Latency measured from GNT to read data (in
                               number of dclks). Low bit.[br] 0x0-0x3: 9-12
                               [br] 0x4-0x7: 13-16 [br] 0x8: 18[br] 0x9: 20[br]
                               0xA: 22[br] 0xB: 24[br] 0xC: rsvd[br] 0xD:
                               17[br] 0xE: 19[br] 0xF: 21[br] Note: This
                               register field is used with bit[6:4] of this
                               register to form TCL GNT to Data register
                               field[br] tcl setting must satisfy FNV min
                               latency of 12 clocks for tcl + tcl_add [br] tcl
                               + tcl_add >= 12
                            */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcl_gnt_data : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000002*/

                            /*
                               CAS Latency measured from GNT to read data (in
                               number of dclks). High bits.[br] 0x0-0x3: 9-12
                               [br] 0x4-0x7: 13-16 [br] 0x8: 18[br] 0x9: 20[br]
                               0xA: 22[br] 0xB: 24[br] 0xC: rsvd[br] 0xD:
                               17[br] 0xE: 19[br] 0xF: 21[br] Note: This is
                               combined with bit[2] of this register to form
                               TCL GNT to Data register field [br] tcl setting
                               must satisfy FNV min latency of 12 clocks for
                               tcl + tcl_add [br] tcl + tcl_add >= 12
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_MR0_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_MR1_FNV_DA_UNIT_0_REG supported on:                         */
/*      BWV (0x40002004)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define DA_DDRT_TRAINING_MR1_FNV_DA_UNIT_0_REG 0x1B240004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rlvl_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Not used. */
    UINT32 wlvl_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Write Leveling Enable [br] 0 : disable 1 :
                               enable
                            */
    UINT32 rsvd_8 : 3;

                            /* Bits[10:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_MR1_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_MR2_FNV_DA_UNIT_0_REG supported on:                         */
/*      BWV (0x40002008)                                                        */
/* Register default value on BWV: 0x00000020                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define DA_DDRT_TRAINING_MR2_FNV_DA_UNIT_0_REG 0x1B240008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcwl : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000004*/

                            /*
                               CAS write latency (in number of dclks)[br]
                               3'b000 : 9, tcwl_add max of 15 (FNV Does NOT
                               Support!) [br] 3'b001 : 10, tcwl_add max of 15
                               (FNV Does NOT Support!)[br] 3'b010 : 11,
                               tcwl_add max of 15 (FNV Does NOT Support!)[br]
                               3'b011 : 12, tcwl_add max of 14 (Supported only
                               with cwl_add of at least '1' when par_flopen =
                               '1')[br] 3'b100 : 14, tcwl_add max of 13 [br]
                               3'b101 : 16, tcwl_add max of 11 [br] 3'b110 :
                               18, tcwl_add max of 9 [br] 3'b111 : 20, tcwl_add
                               max of 7 [br] The effective write latency
                               observed will be 12+tcwl_add+1 when CAS write
                               latency is set to 3'h3.[br] Note: tcwl setting
                               must satisfy FNV min latency of 12 clocks for
                               tcwl + tcwl_add [br] tcwl + tcwl_add >= 12
                            */
    UINT32 cmd_2n_en : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               DDRT Command 1N/2N Timing [br] 2N is onlly
                               supported during training; 2N timing is NOT
                               supported in functional mode[br] 00 = 1N
                               (default)[br] 01 = 2N [br] 1x = Reserved
                            */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_9 : 4;

                            /* Bits[12:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_MR2_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_MR3_FNV_DA_UNIT_0_REG supported on:                         */
/*      BWV (0x4000200c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define DA_DDRT_TRAINING_MR3_FNV_DA_UNIT_0_REG 0x1B24000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               MPR page selection[br] 0x0 : page 0[br] 0x1 :
                               page 1[br] 0x2 : page 2[br] 0x3 : page 3
                            */
    UINT32 mpr_op : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               MPR operation[br] 0 : normal[br] 1 : data flow
                               from/to MPR
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_5 : 6;

                            /* Bits[10:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mpr_rd_frmt : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /*
                               MPR read format; FNV supports both serial and
                               parallel MPR read format 00 = Serial [br] 01 =
                               Parallel [br] others = reserved
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_MR3_FNV_DA_UNIT_0_STRUCT;





/* DA_DDRT_MPR3_PAGE0_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002020)                                                        */
/* Register default value on BWV: 0x000F3355                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Training Pattern
*/


#define DA_DDRT_MPR3_PAGE0_FNV_DA_UNIT_0_REG 0x1B240020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000055*/

                            /* MPR training pattern 3 (from ddrt_training_mpr3) */
    UINT32 mpr1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000033*/

                            /* MPR training pattern 1 (from ddrt_training_mpr1) */
    UINT32 mpr2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x0000000F*/

                            /* MPR training pattern 2 (from ddrt_training_mpr2) */
    UINT32 mpr3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* MPR training pattern 3 (from ddrt_training_mpr3) */

  } Bits;
  UINT32 Data;

} DA_DDRT_MPR3_PAGE0_FNV_DA_UNIT_0_STRUCT;




/* DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG supported on:                          */
/*      BWV (0x40002030)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Training Enable
*/


#define DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG 0x1B240030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               FMC general training mode - [br] needs to be set
                               for all training steps. [br] Setting this bit
                               will block read and write command from sending
                               to memory
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               FMC parity-only training mode. [br] Does not
                               submit commands to ingress queue - only computes
                               parity
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               FMC block CSR writes training mode.[br] Used
                               during advanced cmd-clk training. Protects the
                               FMC states from being changed as CMD/CTL signals
                               are margined.[br] MRS, extended MRS and Memory
                               Mapped config access are ignored
                            */
    UINT32 basic_erid_trn : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               This enables the generation of this constant
                               pattern sourced from FMC [br] during early read
                               ID Fine Training
                            */
    UINT32 recen_dq_dqs_mpr_trn : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               RECEN DQ DQS using MPR Training Enable [br] MPR
                               pattern is returned for GNT even when REQ was
                               not asserted [br] and no Read Command was
                               originally sent.
                            */
    UINT32 disable_rid_feedback : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the RID bits are no longer returned
                               within the data packet, allowing for all data in
                               the buffer to be returned as it was written
                            */
    UINT32 reserved_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Reserved (6) */
    UINT32 reserved_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Reserved (7) */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_OTHER_EN_FNV_DA_UNIT_0_REG supported on:                    */
/*      BWV (0x40002034)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Other Training Enable
*/


#define DA_DDRT_TRAINING_OTHER_EN_FNV_DA_UNIT_0_REG 0x1B240034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clkadj_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable IO to core dclk ajustment[br] Set this
                               bit to enable io to core dclk CK adjustment FSM
                            */
    UINT32 rsvd_3 : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reset_trn : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Reset Training Enable [br] Set this bit to reset
                               training FSM TBD
                            */
    UINT32 rsvd_6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_OTHER_EN_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_RX_DQ_SWZ0_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002038)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ15 through DQ0 Swizzle
*/


#define DA_DDRT_RX_DQ_SWZ0_FNV_DA_UNIT_0_REG 0x1B240038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq0_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               RX DQ0 swizzling[br] 00 = IO dq0 to core dq0[br]
                               01 = IO dq1 to core dq0[br] 10 = IO dq2 to core
                               dq0[br] 11 = IO dq3 to core dq0
                            */
    UINT32 dq1_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ1 swizzling */
    UINT32 dq2_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ2 swizzling */
    UINT32 dq3_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ3 swizzling */
    UINT32 dq4_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ4 swizzling */
    UINT32 dq5_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ5 swizzling */
    UINT32 dq6_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ6 swizzling */
    UINT32 dq7_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ7 swizzling */
    UINT32 dq8_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ8 swizzling */
    UINT32 dq9_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ9 swizzling */
    UINT32 dq10_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ10 swizzling */
    UINT32 dq11_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ11 swizzling */
    UINT32 dq12_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ12 swizzling */
    UINT32 dq13_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ13 swizzling */
    UINT32 dq14_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ14 swizzling */
    UINT32 dq15_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ15 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_RX_DQ_SWZ0_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_RX_DQ_SWZ1_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x4000203c)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ31 through DQ16 Swizzle
*/


#define DA_DDRT_RX_DQ_SWZ1_FNV_DA_UNIT_0_REG 0x1B24003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq16_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ16 swizzling */
    UINT32 dq17_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ17 swizzling */
    UINT32 dq18_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ18 swizzling */
    UINT32 dq19_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ19 swizzling */
    UINT32 dq20_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ20 swizzling */
    UINT32 dq21_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ21 swizzling */
    UINT32 dq22_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ22 swizzling */
    UINT32 dq23_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ23 swizzling */
    UINT32 dq24_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ24 swizzling */
    UINT32 dq25_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ25 swizzling */
    UINT32 dq26_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ26 swizzling */
    UINT32 dq27_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ27 swizzling */
    UINT32 dq28_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ28 swizzling */
    UINT32 dq29_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ29 swizzling */
    UINT32 dq30_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ30 swizzling */
    UINT32 dq31_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ31 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_RX_DQ_SWZ1_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_RX_DQ_SWZ2_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002040)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ47 through DQ32 Swizzle
*/


#define DA_DDRT_RX_DQ_SWZ2_FNV_DA_UNIT_0_REG 0x1B240040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq32_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ32 swizzling */
    UINT32 dq33_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ33 swizzling */
    UINT32 dq34_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ34 swizzling */
    UINT32 dq35_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ35 swizzling */
    UINT32 dq36_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ36 swizzling */
    UINT32 dq37_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ37 swizzling */
    UINT32 dq38_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ38 swizzling */
    UINT32 dq39_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ39 swizzling */
    UINT32 dq40_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ40 swizzling */
    UINT32 dq41_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ41 swizzling */
    UINT32 dq42_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ42 swizzling */
    UINT32 dq43_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ43 swizzling */
    UINT32 dq44_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ44 swizzling */
    UINT32 dq45_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ45 swizzling */
    UINT32 dq46_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ46 swizzling */
    UINT32 dq47_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ47 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_RX_DQ_SWZ2_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_RX_DQ_SWZ3_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002044)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ63 through DQ48 Swizzle
*/


#define DA_DDRT_RX_DQ_SWZ3_FNV_DA_UNIT_0_REG 0x1B240044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq48_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ48 swizzling */
    UINT32 dq49_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ49 swizzling */
    UINT32 dq50_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ50 swizzling */
    UINT32 dq51_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ51 swizzling */
    UINT32 dq52_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ52 swizzling */
    UINT32 dq53_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ53 swizzling */
    UINT32 dq54_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ54 swizzling */
    UINT32 dq55_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ55 swizzling */
    UINT32 dq56_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ56 swizzling */
    UINT32 dq57_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ57 swizzling */
    UINT32 dq58_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ58 swizzling */
    UINT32 dq59_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ59 swizzling */
    UINT32 dq60_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ60 swizzling */
    UINT32 dq61_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ61 swizzling */
    UINT32 dq62_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ62 swizzling */
    UINT32 dq63_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ63 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_RX_DQ_SWZ3_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_RX_DQ_SWZ4_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002048)                                                        */
/* Register default value on BWV: 0x0000E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ71 through DQ64 Swizzle
*/


#define DA_DDRT_RX_DQ_SWZ4_FNV_DA_UNIT_0_REG 0x1B240048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq64_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ64 swizzling */
    UINT32 dq65_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ65 swizzling */
    UINT32 dq66_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ66 swizzling */
    UINT32 dq67_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ67 swizzling */
    UINT32 dq68_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ68 swizzling */
    UINT32 dq69_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ69 swizzling */
    UINT32 dq70_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ70 swizzling */
    UINT32 dq71_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ71 swizzling */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_RX_DQ_SWZ4_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TX_DQ_SWZ0_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x4000204c)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ15 through DQ0 Swizzle
*/


#define DA_DDRT_TX_DQ_SWZ0_FNV_DA_UNIT_0_REG 0x1B24004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq0_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               RX DQ0 swizzling[br] 00 = IO dq0 to core dq0[br]
                               01 = IO dq1 to core dq0[br] 10 = IO dq2 to core
                               dq0[br] 11 = IO dq3 to core dq0
                            */
    UINT32 dq1_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ1 swizzling */
    UINT32 dq2_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ2 swizzling */
    UINT32 dq3_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ3 swizzling */
    UINT32 dq4_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ4 swizzling */
    UINT32 dq5_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ5 swizzling */
    UINT32 dq6_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ6 swizzling */
    UINT32 dq7_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ7 swizzling */
    UINT32 dq8_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ8 swizzling */
    UINT32 dq9_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ9 swizzling */
    UINT32 dq10_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ10 swizzling */
    UINT32 dq11_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ11 swizzling */
    UINT32 dq12_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ12 swizzling */
    UINT32 dq13_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ13 swizzling */
    UINT32 dq14_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ14 swizzling */
    UINT32 dq15_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ15 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_TX_DQ_SWZ0_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TX_DQ_SWZ1_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002050)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ31 through DQ16 Swizzle
*/


#define DA_DDRT_TX_DQ_SWZ1_FNV_DA_UNIT_0_REG 0x1B240050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq16_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ16 swizzling */
    UINT32 dq17_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ17 swizzling */
    UINT32 dq18_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ18 swizzling */
    UINT32 dq19_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ19 swizzling */
    UINT32 dq20_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ20 swizzling */
    UINT32 dq21_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ21 swizzling */
    UINT32 dq22_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ22 swizzling */
    UINT32 dq23_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ23 swizzling */
    UINT32 dq24_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ24 swizzling */
    UINT32 dq25_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ25 swizzling */
    UINT32 dq26_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ26 swizzling */
    UINT32 dq27_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ27 swizzling */
    UINT32 dq28_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ28 swizzling */
    UINT32 dq29_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ29 swizzling */
    UINT32 dq30_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ30 swizzling */
    UINT32 dq31_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ31 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_TX_DQ_SWZ1_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TX_DQ_SWZ2_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002054)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ47 through DQ32 Swizzle
*/


#define DA_DDRT_TX_DQ_SWZ2_FNV_DA_UNIT_0_REG 0x1B240054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq32_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ32 swizzling */
    UINT32 dq33_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ33 swizzling */
    UINT32 dq34_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ34 swizzling */
    UINT32 dq35_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ35 swizzling */
    UINT32 dq36_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ36 swizzling */
    UINT32 dq37_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ37 swizzling */
    UINT32 dq38_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ38 swizzling */
    UINT32 dq39_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ39 swizzling */
    UINT32 dq40_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ40 swizzling */
    UINT32 dq41_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ41 swizzling */
    UINT32 dq42_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ42 swizzling */
    UINT32 dq43_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ43 swizzling */
    UINT32 dq44_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ44 swizzling */
    UINT32 dq45_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ45 swizzling */
    UINT32 dq46_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ46 swizzling */
    UINT32 dq47_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ47 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_TX_DQ_SWZ2_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TX_DQ_SWZ3_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x40002058)                                                        */
/* Register default value on BWV: 0xE4E4E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ63 through DQ48 Swizzle
*/


#define DA_DDRT_TX_DQ_SWZ3_FNV_DA_UNIT_0_REG 0x1B240058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq48_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ48 swizzling */
    UINT32 dq49_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ49 swizzling */
    UINT32 dq50_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ50 swizzling */
    UINT32 dq51_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ51 swizzling */
    UINT32 dq52_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ52 swizzling */
    UINT32 dq53_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ53 swizzling */
    UINT32 dq54_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ54 swizzling */
    UINT32 dq55_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ55 swizzling */
    UINT32 dq56_swz_sel : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* DQ56 swizzling */
    UINT32 dq57_swz_sel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /* DQ57 swizzling */
    UINT32 dq58_swz_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000002*/

                            /* DQ58 swizzling */
    UINT32 dq59_swz_sel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* DQ59 swizzling */
    UINT32 dq60_swz_sel : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* DQ60 swizzling */
    UINT32 dq61_swz_sel : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000001*/

                            /* DQ61 swizzling */
    UINT32 dq62_swz_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /* DQ62 swizzling */
    UINT32 dq63_swz_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* DQ63 swizzling */

  } Bits;
  UINT32 Data;

} DA_DDRT_TX_DQ_SWZ3_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TX_DQ_SWZ4_FNV_DA_UNIT_0_REG supported on:                           */
/*      BWV (0x4000205c)                                                        */
/* Register default value on BWV: 0x0000E4E4                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ71 through DQ64 Swizzle
*/


#define DA_DDRT_TX_DQ_SWZ4_FNV_DA_UNIT_0_REG 0x1B24005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq64_swz_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* DQ64 swizzling */
    UINT32 dq65_swz_sel : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000001*/

                            /* DQ65 swizzling */
    UINT32 dq66_swz_sel : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000002*/

                            /* DQ66 swizzling */
    UINT32 dq67_swz_sel : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /* DQ67 swizzling */
    UINT32 dq68_swz_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* DQ68 swizzling */
    UINT32 dq69_swz_sel : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000001*/

                            /* DQ69 swizzling */
    UINT32 dq70_swz_sel : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000002*/

                            /* DQ70 swizzling */
    UINT32 dq71_swz_sel : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000003*/

                            /* DQ71 swizzling */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TX_DQ_SWZ4_FNV_DA_UNIT_0_STRUCT;




/* DA_DDRIO_INIT_CONTROL_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x4000206c)                                                        */
/* Register default value on BWV: 0x00684000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Connects to DDR IO Interface Initialization 
*/


#define DA_DDRIO_INIT_CONTROL_FNV_DA_UNIT_0_REG 0x1B24006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 9;

                            /* Bits[8:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_9 : 11;

                            /* Bits[19:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrtio_par_flopen : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               When this is set (1'b1 by default), Command
                               Parity Bit path across FNVIO has an extra dclk
                               latency impact.
                            */
    UINT32 rsvd_22 : 5;

                            /* Bits[26:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRIO_INIT_CONTROL_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRIO_CMDEN_FNV_DA_UNIT_0_REG supported on:                               */
/*      BWV (0x40002070)                                                        */
/* Register default value on BWV: 0x800000C0                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Output Enable for REQ, ERR, ERID, BCOM, BCLK, BCKE, BODT.
*/


#define DA_DDRIO_CMDEN_FNV_DA_UNIT_0_REG 0x1B240070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fnvbcomflopen : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Enables an additional Flops (2dclks) in BCOM
                               latency for RD & WR bcom only.[br] BCOM Flop
                               enable should be set to 1 to use EKV Core bcom
                               logic. [br] When set to 0, Bcom RD&WR only will
                               use FNVIO BCOM (save 2dclks).
                            */
    UINT32 rsvd_7 : 24;

                            /* Bits[30:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fnvbcomrden : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               When set: Core always drives bcom on gnts(read)
                               when fnvbcomflopen is set to zero, or set to
                               one. [br] When cleared: FNVIO will drive bcom on
                               gnts(read) only when fnvbcomflopen is also set
                               to zero. [br] default : 1'b1 (Core drives).
                            */

  } Bits;
  UINT32 Data;

} DA_DDRIO_CMDEN_FNV_DA_UNIT_0_STRUCT;



/* DA_ECC_ENABLE_FNV_DA_UNIT_0_REG supported on:                                */
/*      BWV (0x4000207c)                                                        */
/* Register default value on BWV: 0x00001F00                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Write ECC Control
*/


#define DA_ECC_ENABLE_FNV_DA_UNIT_0_REG 0x1B24007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecc_wr_chk_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable write ECC generation[br] 0 = ecc check
                               disabled[br] 1 = ecc check enabled
                            */
    UINT32 ecc_corr_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Write ECC Correction[br] 0 = ecc
                               correction disabled.[br] 1 = ecc correction
                               enabled.[br] ECC Correction must be enabled to
                               run Flow B. [br] ECC Correcation can be disabled
                               to run Flow A. [br]
                            */
    UINT32 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 trnreseterrexit_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Enable assertion of DDRT IO Train Reset on DDRT
                               Err Exit for progammable number of dclks[br] 0 =
                               disabled.[br] 1 = enabled. [br]
                            */
    UINT32 trnreseterrexit_cntr_termval : 7;

                            /* Bits[15:9], Access Type=RW, default=0x0000000F*/

                            /*
                               Determines the pulse length of DDRT IO Train
                               Reset on DDRT Err Exit for progammable number of
                               dclks[br] default - 16 dclks .[br]
                            */
    UINT32 rsvd_16 : 13;

                            /* Bits[28:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errflow_a_alwayscorr : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               With this bit set, HW will always correct Write
                               ECC Error in Flow A, if error is correctable[br]
                               0 = WECC Flow A Always Correct disabled[br] 1 =
                               WECC Flow A Always Correct enabled[br] Note**
                               Field is Deprecated. Do not change value from
                               default
                            */
    UINT32 ecc_poison_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               With this bit set, HW will poison on WECC
                               Correction Fail[br] 0 = WECC Poison disabled[br]
                               1 = WECC Poison enabled
                            */
    UINT32 errflow_a_exitcorr : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Write ECC Flow A Correction on Retry Flow
                               Exit[br] 0 = ecc Flow A correction on retry flow
                               exit disabled[br] 1 = ecc Flow A correction on
                               retry flow exit enabled
                            */

  } Bits;
  UINT32 Data;

} DA_ECC_ENABLE_FNV_DA_UNIT_0_STRUCT;









/* DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x400020a0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Command Space Control Word
*/


#define DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG 0x1B2400A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 misc_ctrl : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               0000 = reserved (SoftReset)[br] 0001 = DB Reset
                               [br] 0010 = DRAM Reset[br] 0011 = Clear DRAM
                               Reset [br] 0100 = CW Read operation [br] 0101 =
                               CW Write operation [br] 0110 = Clear parity
                               error[br] 0111 = reserved (Geardown Mode delay
                               CS)[br] 1000 = reserved (Geardown mode entry
                               )[br] others = reserved
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_STRUCT;






/* DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x400020b8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Training Control Word
*/


#define DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG 0x1B2400B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 training_mode : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Training mode selection[br] 000 = Normal
                               operating [br] 001 = CK-CA training[br] 010 =
                               CS0_n loopback[br] 011 = GNT loopback (CS1_n
                               loopback)[br] 100 = CKE0 loopback[br] 101 =
                               reserved (CKE1 loopback; DDRT REQ)[br] 110 =
                               ODT0 loopback[br] 111 = ERID loopback (ODT1
                               loopback, DDRT ERR)
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_STRUCT;


/* DA_DDRT_TRAINING_RC0E_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x400020c0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Parity Control Word
*/


#define DA_DDRT_TRAINING_RC0E_FNV_DA_UNIT_0_REG 0x1B2400C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 par_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Parity Enable [br] If parity is enabled, command
                               parity emask is n/a
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_2 : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_RC0E_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_RC0F_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x400020c4)                                                        */
/* Register default value on BWV: 0x00000003                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Command Latency Adder Control Word
*/


#define DA_DDRT_TRAINING_RC0F_FNV_DA_UNIT_0_REG 0x1B2400C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcmd_add : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000003*/

                            /*
                               Latency adder measured from the 2nd UI of DDRT
                               write command to BCOM write command [br]
                               0x0-0x3: +0 dclk [br] 0x4-0xF: +1 to +12 dclk
                               [br]
                            */
    UINT32 tcl_add_3 : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               tcl_add[4:0]: 0x0-0x1f: 0-31 dclk [br] Note:
                               This register field is used with tcl_add[2:0] to
                               form tcl_add[4:0] [br] tcl setting must satisfy
                               FNV min latency of 12 clocks for tcl + tcl_add
                               [br]
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_RC0F_FNV_DA_UNIT_0_STRUCT;




/* DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x400020d4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* CW Source Selection Control Word
*/


#define DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_REG 0x1B2400D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_12_8 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /* Content of A[12:8] for BCW write/read */
    UINT32 func : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000000*/

                            /* Function select for BCW access */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_STRUCT;


/* DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_REG supported on:                        */
/*      BWV (0x400020dc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* CW Data Control Word
*/


#define DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_REG 0x1B2400DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_7_0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Content of A[7:0] for BCW write/read */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_STRUCT;










/* DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG supported on:                               */
/*      BWV (0x40002104)                                                        */
/* Register default value on BWV: 0x0000FFFF                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Initial LFSR seed for read data scrambling
*/


#define DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG 0x1B240104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 seed : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /*
                               Initial LFSR seed for read data scrambling[br]
                               x^16 + x^13 + x^10 + x^9 + x^8 + x^4 + 1
                            */
    UINT32 en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Enable read data scrambling[br] 0: Read data is
                               NOT scrambled[br] 1: Enable read data scrambler;
                               read data is scrambled
                            */
    UINT32 lfsr_load : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Load new LFSR seed, this bit needs to be clear
                               first and then set to load a new lfsr seed; this
                               bit is used to load both lower and upper wr lfsr
                               seed
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_RD_SCR_LFSR_FNV_DA_UNIT_0_STRUCT;

/* DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG supported on:                             */
/*      BWV (0x40002108)                                                        */
/* Register default value on BWV: 0x0000FFFF                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Initial LFSR seed for write data scrambling. Low register
*/


#define DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG 0x1B240108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 seed_lower : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /*
                               Initial LFSR seed for read data scrambling[br]
                               x^16 + x^13 + x^10 + x^9 + x^8 + x^4 + 1
                            */
    UINT32 en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               ENable write data scrambling[br] 0: Write data
                               is NOT scrambled[br] 1: Enable write data
                               scrambler; write data is scrambled
                            */
    UINT32 lfsr_load : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Load new LFSR seed, this bit needs to be clear
                               first and then set to load a new lfsr seed; this
                               bit is used to load both lower and upper wr lfsr
                               seed
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_STRUCT;

/* DA_WR_SCR_LFSR_H_FNV_DA_UNIT_0_REG supported on:                             */
/*      BWV (0x4000210c)                                                        */
/* Register default value on BWV: 0x0000FFFF                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Initial LFSR seed for write data scrambling. High register
*/


#define DA_WR_SCR_LFSR_H_FNV_DA_UNIT_0_REG 0x1B24010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 seed_upper : 16;

                            /* Bits[15:0], Access Type=RW, default=0x0000FFFF*/

                            /*
                               Initial LFSR seed for read data scrambling[br]
                               x^16 + x^13 + x^10 + x^9 + x^8 + x^4 + 1
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WR_SCR_LFSR_H_FNV_DA_UNIT_0_STRUCT;

/* DA_WR_SCRAMBLE_SEED_SEL_FNV_DA_UNIT_0_REG supported on:                      */
/*      BWV (0x40002110)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Write Data Scrambler Seed Reordering
*/


#define DA_WR_SCRAMBLE_SEED_SEL_FNV_DA_UNIT_0_REG 0x1B240110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ordersel : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               select for scrambler seed re-ordering[br]
                               default - no re-ordering
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WR_SCRAMBLE_SEED_SEL_FNV_DA_UNIT_0_STRUCT;

/* DA_DDRT_LATENCY_FNV_DA_UNIT_0_REG supported on:                              */
/*      BWV (0x40002114)                                                        */
/* Register default value on BWV: 0x00000006                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Performance
*/


#define DA_DDRT_LATENCY_FNV_DA_UNIT_0_REG 0x1B240114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tcl_gnt_erid : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000006*/

                            /*
                               ERID CAS latency measured from GNT to ERID (in
                               number of dclks + 6)[br] 0x0 : RSVD (FNV does
                               not support 6 dclks) [br] BWV supports min
                               setting of 12 dclks [br] 0x1-0x9: 7-15[br]
                               (Write recovery and read to precharge for DDR4)
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_LATENCY_FNV_DA_UNIT_0_STRUCT;



/* DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG supported on:                         */
/*      BWV (0x40002120)                                                        */
/* Register default value on BWV: 0x0063C601                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* DQ buffer BCOM interface control
*/


#define DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG 0x1B240120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Enable Data Buffer (DB) [br] 0: Disable DB,
                               commands are not generated for BCOM (default)
                               [br] 1: Enable DB; rd, wr, mrs, bcwrd, bcwwr
                               command are generated for BCOM
                            */
    UINT32 rsvd_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcl_add : 3;

                            /* Bits[4:2], Access Type=RW, default=0x00000000*/

                            /*
                               tcl_add[3:0]: 0x0-0x7: 0-7 dclk [br] Note: This
                               is combined with tcl_add_3 to form tcl_add[3:0]
                               [br] tcl setting must satisfy FNV min latency of
                               12 clocks for tcl + tcl_add [br]
                            */
    UINT32 tcwl_add : 4;

                            /* Bits[8:5], Access Type=RW, default=0x00000000*/

                            /* 0x0-0xf: 0-15 dclk */
    UINT32 rsvd_9 : 4;

                            /* Bits[12:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bcw_mpr_rd_req_en : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               DDRT REQ# is generated for MEMRD when DQ buffer
                               is set to MPR read override mode[br] Default is
                               not to generate REQ#, since MEMRD is converted
                               to BCW read and sent to DQ buffer [br] 0:
                               Disabled (default) [br] 1: Enabled
                            */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_STRUCT;


/* DA_WRITE_CREDIT_FNV_DA_UNIT_0_REG supported on:                              */
/*      BWV (0x40002128)                                                        */
/* Register default value on BWV: 0x00000050                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the default value of write credits. Will be loaded when value of CSR changes
*/


#define DA_WRITE_CREDIT_FNV_DA_UNIT_0_REG 0x1B240128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 val : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000050*/

                            /*
                               Value of write credits. Default value is 80. Max
                               is 80.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WRITE_CREDIT_FNV_DA_UNIT_0_STRUCT;




/* DA_WCRD_CNT_FNV_DA_UNIT_0_REG supported on:                                  */
/*      BWV (0x4000213c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Indicates status of write credit availability
*/


#define DA_WCRD_CNT_FNV_DA_UNIT_0_REG 0x1B24013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcrd_cnt : 8;

                            /* Bits[7:0], Access Type=ROV, default=0x00000000*/

                            /*
                               Indicates the number of write credits available
                               in FNV
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WCRD_CNT_FNV_DA_UNIT_0_STRUCT;








/* DA_DDRT_TRAINING_DEFEATURE0_FNV_DA_UNIT_0_REG supported on:                  */
/*      BWV (0x40002300)                                                        */
/* Register default value on BWV: 0x00000108                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Training Defeature 0
*/


#define DA_DDRT_TRAINING_DEFEATURE0_FNV_DA_UNIT_0_REG 0x1B240300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nodata_trn_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               No data packet is returned if GNT# is received
                               too early during training[br] 0 = Normal [br] 1
                               = Enable nodata packet during training (default)
                            */
    UINT32 rsvd_4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_DEFEATURE0_FNV_DA_UNIT_0_STRUCT;


//#ifdef SV_HOOKS

/* DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_REG supported on:                      */
/*      BWV (0x40002308)                                                        */
/* Register default value on BWV: 0x00469C01                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Functional Defeature 1
*/


#define DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_REG 0x1B240308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 entr_dckg_tmr : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000001*/

                            /*
                               Number of clocks (in 16-clk interval) to wait
                               before gating dclk [br] Value should not be
                               changed except for Debug purposes [br] Else
                               leave at default value [br] 000: rsvd [br] 001:
                               16 clocks [br] 010: 32 clocks [br] 011: 48
                               clocks [br] 100 - 111 : rsvd
                            */
    UINT32 eack_dsrt_tmr : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               Number of clocks (in 64-clk interval) to wait
                               before de-asserting DDRT_ERR# [br] 000: 64
                               (default) [br] 001: 128 [br] ... [br] 111: 512
                               clocks
                            */
    UINT32 req_err_asrt : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               The minimum number of DDRT clocks to wait before
                               REQ can assert after DDRT error exit[br] 00: 16
                               [br] 01: 32 [br] 10: 48 [br] 11: 64
                            */
    UINT32 err_asrt_flsh : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of clocks to wait after DDRT ERR
                               assertion before flushing read data [br] 00: 64
                               (default) [br] 01: 128 [br] 10: 192 [br] 11: 256
                            */
    UINT32 iot_pkt_allwd : 4;

                            /* Bits[13:10], Access Type=RW, default=0x00000007*/

                            /*
                               Number of IOT packet allowed for REDB [br] If
                               IOT is set to any other numbers other than
                               default 7, [br] then the number of read credits
                               advertised to Host must be adjusted accordingly
                               such that IOT packet + DDRT read = 47. This CSR
                               must be programmed between values 7-15
                               (decimal). [br] 00: rsvd [br] 7: (default)
                            */
    UINT32 gnt_32_cont_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit will enable gnt_cnt to be
                               updated from rfill_cnt when gnt_cnt is equal to
                               1 [br] Data is filled continuously from REDB to
                               RXD for 32 B2B GNTs read DQ/DQS training [br]
                               This bit needs to be set prior to the training
                               step that requires 32 B2B GNTs without
                               NODATA[br] this bit might be set in normal mode
                               [br]
                            */
    UINT32 nodata_pkt_allwd : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000001*/

                            /*
                               Number of Nodata packet allowed for REDB [br]
                               The number of read credits advertised to Host
                               must be adjusted accordingly such that [br]
                               Nodata + IOT + Interrupt + DDRT read = 47 [br]
                               d_func_defeature1.nodata_pkt_allwd has to be set
                               to the same value [br] 00: rsvd [br] 01: 1 no
                               data (default)[br] 10: rsvd [br] 11: rsvd
                            */
    UINT32 wait_rd_thrsh : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Wait for read threshold before servicing reqs
                               [br] 0 = Req is generated after asserted [br] 1
                               = Wait for read threshold (default)
                            */
    UINT32 bcom_1st_ui : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               BCOM command is generated from the 1st UI of
                               DDRT command packet [br] and parity error forced
                               if parity erro ris detected in the 2nd UI of
                               DDRT command packet[br] 0 = BCOM cmd generated
                               from 2nd UI NOT supported [br] 1 = BCOM cmd
                               generated from 1st UI (default)
                            */
    UINT32 cmd_ff_bypass : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bypass IO command FF stage [br] CMD bypass is
                               NOT supported and needs to be kept at default
                               [br] At lower frequency, set to bypass command
                               FF to improve read and write command latency[br]
                               0 = Disable cmd bypass (default) [br] 1 = Bypass
                               cmd FF stage NOT supported
                            */
    UINT32 gnt_ff_bypass : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bypass GNT FF stage [br] GNT bypass is NOT
                               supported and needs to be kept at default [br]
                               At lower frequency, set to bypass GNT FF to
                               improve read data latency[br] 0 = Disable GNT
                               bypass (default)[br] 1 = Bypass GNT FF NOT
                               supported
                            */
    UINT32 erid_drv0_sel : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Select ERID output default level 0/1 [br] If
                               there is NO read data to be returned, ERID is
                               driven to default 0/1 logic level [br] 0 = Drive
                               default level 1 (default) [br] 1 = Drive default
                               level 0
                            */
    UINT32 ddrt_err_wait_balert : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               If BALERT is still asserted after DDRT error
                               flow is ready to exit, DDRT_ERR# will remain
                               asserted [br] After BALERT is deasserted,
                               DDRT_ERR# will be deasserted [br] 0 = Exit DDRT
                               error without waiting for BALERT deassertion
                               [br] 1 = Wait for BALERT deassertion before
                               exiting DDRT error flow (default)
                            */
    UINT32 spare_cxfifo_hwmark : 3;

                            /* Bits[25:23], Access Type=RW, default=0x00000000*/

                            /*
                               CXFIFO High water mark is no longer used.
                               Reserved for future use.[br] Bit [2:0] : Write
                               data CXFIFO High water mark; DMA write is
                               granted only if the number of data entries is
                               less than HWM to prevent CXFIFO overflow[br]
                               000: 96 (default) [br] 001: 84 [br] 010: 88 [br]
                               011: 92 [br] 100: 100[br] 101: 104[br] 110:
                               108[br] 111: disabled
                            */
    UINT32 encsgating : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Enable IO to gate CS/GNT; this bit is reserved
                               for future use; [br] 0 = Core does CS/GNT gating
                               (default)[br] 1 = Enable IO to gate CS/GNT
                            */
    UINT32 selfastclkidle : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Select CLKIDLE source[br] 0 = Normal clkidle is
                               used to gate spurious CS#/GNT# (default)[br] 1 =
                               Fast clkidle is used to gate spurious CS#/GNT#
                            */
    UINT32 bodt_low_pwrdown_dis : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               In BODT core mode (ddrt_bodt_ovrd=1), BODT is
                               driven LOW during powerdown [br] Setting this
                               bit will also require ODT from IO rx to be
                               enabled (DdrCrCmdControls2_2.FnvCrCmdIsCke =
                               0x4) [br] 0 = Core drives BODT=0 during
                               powerdown (default)[br] 1 = Core routes
                               ddrt2mccommandqnnnh_odt to BODT during powerdown
                            */
    UINT32 spare : 3;

                            /* Bits[31:29], Access Type=RW, default=0x00000000*/

                            /* Reserved for future use */

  } Bits;
  UINT32 Data;

} DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_STRUCT;

//#endif 



























/* DA_EMASK_FNV_DA_UNIT_0_REG supported on:                                     */
/*      BWV (0x40002510)                                                        */
/* Register default value on BWV: 0xFFFEBFFF                                    */
/* BWV Register File:    0_0_2_BWV                                              */
/* Struct generated from BWV BDF: 0_0_2                                         */
/* BWV Security PolicyGroup:                                                    */
/* Masking errors per source
*/


#define DA_EMASK_FNV_DA_UNIT_0_REG 0x1B240510

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wr_credit : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Mask error [br] 1 = Mask error [br] 0 = Allow
                               error [br] Default is 1
                            */
    UINT32 rsvd_4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_EMASK_FNV_DA_UNIT_0_STRUCT;



























































#endif /* _FNV_DA_UNIT_0_h */
