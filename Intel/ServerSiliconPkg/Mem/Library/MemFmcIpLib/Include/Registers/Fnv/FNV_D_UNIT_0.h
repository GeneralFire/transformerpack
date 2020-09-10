
/** @file
  FNV_D_UNIT_0.h

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


#ifndef _FNV_D_UNIT_0_h
#define _FNV_D_UNIT_0_h
#include "DataTypes.h"

/* D_FW_STATUS_FNV_D_UNIT_0_REG supported on:                                   */
/*      BWV (0x40004000)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* BIOS will read this register to determine FNV Firmware Status during boot.
*/


#define D_FW_STATUS_FNV_D_UNIT_0_REG 0x1B440000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fw_status : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Firmware status. */

  } Bits;
  UINT32 Data;

} D_FW_STATUS_FNV_D_UNIT_0_STRUCT;

/* D_FW_STATUS_H_FNV_D_UNIT_0_REG supported on:                                 */
/*      BWV (0x40004004)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* BIOS will read this register to determine FNV Firmware Status during boot.
*/


#define D_FW_STATUS_H_FNV_D_UNIT_0_REG 0x1B440004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fw_status : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Firmware status. */

  } Bits;
  UINT32 Data;

} D_FW_STATUS_H_FNV_D_UNIT_0_STRUCT;

/* D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG supported on:                            */
/*      BWV (0x40004008)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Training  Enable
*/


#define D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG 0x1B440008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               DDRT Training Mode Enable [br] Setting this bit
                               will block read and write command from sending
                               to memory
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               EARLY CMD-CK Training Enable [br] Parity only,
                               commands are not written into ingress command
                               queue
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Advanced CMD-CK Training Enable[br] MRS,
                               extended MRS and Memory Mapped config access are
                               ignored
                            */
    UINT32 reserved_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Reserved (3) */
    UINT32 reserved_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Reserved (4) */
    UINT32 disable_rid_feedback : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the RID bits are no longer returned
                               within the data packet,[br] allowing for all
                               data in the buffer to be returned as it was
                               written
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

} D_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT;

/* D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x4000400c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Other Training  Enable
*/


#define D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG 0x1B44000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reset_trn : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Reset Training Enable [br] Set this bit to reset
                               training FSM TBD
                            */
    UINT32 rsvd_3 : 27;

                            /* Bits[29:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pche_inv_set0_only : 1;

                            /* Bits[30:30], Access Type=RWV, default=0x00000000*/

                            /*
                               When this bit is asserted, DUNIT Prefetch Cache
                               will invalidate SET 0 Only. Hardware will clear
                               CSR, once done. Use this CSR right after
                               Training. Prefetch SET 0 is used for training
                               and must be invalidated when switching to normal
                               mode.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_STRUCT;




/* D_DDRT_TRAINING_MPR_FNV_D_UNIT_0_REG supported on:                           */
/*      BWV (0x4000401c)                                                        */
/* Register default value on BWV: 0x000F3355                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* MPR Training Pattern
*/


#define D_DDRT_TRAINING_MPR_FNV_D_UNIT_0_REG 0x1B44001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000055*/

                            /* MPR training pattern 0 */
    UINT32 mpr1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000033*/

                            /* MPR training pattern 1 */
    UINT32 mpr2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x0000000F*/

                            /* MPR training pattern 2 */
    UINT32 mpr3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* MPR training pattern 3 */

  } Bits;
  UINT32 Data;

} D_DDRT_TRAINING_MPR_FNV_D_UNIT_0_STRUCT;

/* D_DDRT_TRAINING_MR3_FNV_D_UNIT_0_REG supported on:                           */
/*      BWV (0x40004020)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define D_DDRT_TRAINING_MR3_FNV_D_UNIT_0_REG 0x1B440020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr_sel : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               MPR page selection[br] 0x0 : Select MPR0[br] 0x1
                               : Select MPR1[br] 0x2 : Select MPR2[br] 0x3 :
                               Select MPR3
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
    UINT32 per_dram_addr_ability : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

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

} D_DDRT_TRAINING_MR3_FNV_D_UNIT_0_STRUCT;

/* D_MC_WDATA_FNV_D_UNIT_0_REG supported on:                                    */
/*      BWV (0x40004030)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register holds write data for Message Channel windowed write transaction. This should be programmed prior to starting a window transaction.
*/


#define D_MC_WDATA_FNV_D_UNIT_0_REG 0x1B440030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_wdata : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Write Data for MC transaction. */

  } Bits;
  UINT32 Data;

} D_MC_WDATA_FNV_D_UNIT_0_STRUCT;

/* D_MC_CTRL_FNV_D_UNIT_0_REG supported on:                                     */
/*      BWV (0x40004034)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register includes address and command for indirect access to DDRT IO and DDRT Agent CSRs, which live on Message Channel.
*/


#define D_MC_CTRL_FNV_D_UNIT_0_REG 0x1B440034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addr : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Destination Address for Message Channel
                               transaction.
                            */
    UINT32 mc_port : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Destination Port ID for Message Channel
                               transaction.[br] Daunit CSRs PortID: 8'd72[br]
                               Unicast IO CSRs PortID: 8'h71 (or 113
                               decimal)[br] Broadcast IO CSRs PortID: 8'h78 (or
                               120 decimal)
                            */
    UINT32 mc_be : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /*
                               Byte enable for Message Channel transaction.
                               Corresponds to First BE[3:0].
                            */
    UINT32 rsvd : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mc_posted : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates whether the Message Channel message is
                               posted or nonposted. Non-posted messages require
                               a Message Channel Completion message. Posted
                               message do not require a Message Channel
                               Completion message. All reads are nonposted
                               because data returns in the completion
                               message.[br] 1: Posted[br] 0: Nonposted
                            */
    UINT32 mc_opcode : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Opcode bit for Message Channel transaction. (0 =
                               Read, 1 = Write)
                            */
    UINT32 mc_start : 1;

                            /* Bits[31:31], Access Type=RWV, default=0x00000000*/

                            /*
                               AHB start bit for Message Channel window control
                               transaction. This bit initiates a command on to
                               the Message Channel.
                            */

  } Bits;
  UINT32 Data;

} D_MC_CTRL_FNV_D_UNIT_0_STRUCT;

/* D_MC_ACC_INFO_FNV_D_UNIT_0_REG supported on:                                 */
/*      BWV (0x40004038)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This holds addition information for MC transactions
*/


#define D_MC_ACC_INFO_FNV_D_UNIT_0_REG 0x1B440038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_acc_info : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /*
                               Holds additional info required for IO MC
                               transaction.[br] IO register access:[br] [br]
                               Bits [11] - IA [br] Bits [10:8] - Bus [br] Bits
                               [7:3] - Device [br] Bits [2:0] - Function [br]
                               Note: Bus is always 2 for IO MC access.[br] For
                               non-IO MC CSR access, this register should be
                               ignored, because it is not used.
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_MC_ACC_INFO_FNV_D_UNIT_0_STRUCT;

/* D_MC_TIMEOUT_FNV_D_UNIT_0_REG supported on:                                  */
/*      BWV (0x4000403c)                                                        */
/* Register default value on BWV: 0x80000040                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register controls the timeout setting of when to trigger a timeout.
*/


#define D_MC_TIMEOUT_FNV_D_UNIT_0_REG 0x1B44003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_timeout : 31;

                            /* Bits[30:0], Access Type=RW, default=0x00000040*/

                            /*
                               After a non-posted Message Channel transaction
                               has been sent and timeout counter is enabled,
                               the counter will trigger a timeout and the
                               status is reported in the Message Channel status
                               register. The counter increments per core clock
                               cycle.
                            */
    UINT32 mc_timeout_vld : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               After a non-posted Message Channel transaction
                               has been sent and timeout counter is enabled,
                               the counter will trigger a timeout and the
                               status is reported in the Message Channel status
                               register.[br] 1'b1 - Enable timeout.
                               (Default)[br] 1'b0 - Disable timeout
                            */

  } Bits;
  UINT32 Data;

} D_MC_TIMEOUT_FNV_D_UNIT_0_STRUCT;

/* D_MC_STAT_FNV_D_UNIT_0_REG supported on:                                     */
/*      BWV (0x40004040)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register holds the status of the Message Channel.
*/


#define D_MC_STAT_FNV_D_UNIT_0_REG 0x1B440040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_sts : 1;

                            /* Bits[0:0], Access Type=ROV, default=0x00000000*/

                            /*
                               This bit signifies MC Window Control is busy and
                               cannot process another transaction yet.
                            */
    UINT32 mc_addr_err : 1;

                            /* Bits[1:1], Access Type=ROV, default=0x00000000*/

                            /*
                               This indicates that the provided Message Channel
                               address and/or PortID is invalid.[br] 1'b1:
                               Invalid[br] 1'b0: No errors
                            */
    UINT32 mc_to : 1;

                            /* Bits[2:2], Access Type=RW1CV, default=0x00000000*/

                            /*
                               This indicates that the provided Message Channel
                               access exceeded the timeout value while waiting
                               for a response.[br] 1'b1: Timeout[br] 1'b0:
                               Default
                            */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mc_rdata_vld : 1;

                            /* Bits[8:8], Access Type=RW1CV, default=0x00000000*/

                            /* Valid bit for MC read data. */
    UINT32 rsvd_9 : 7;

                            /* Bits[15:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mc_rdata_src : 1;

                            /* Bits[16:16], Access Type=RW1CV, default=0x00000000*/

                            /*
                               This holds the rdata source (0 = IO, 1 =
                               Daunit).
                            */
    UINT32 rsvd_17 : 7;

                            /* Bits[23:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mc_resp : 1;

                            /* Bits[24:24], Access Type=RW1CV, default=0x00000000*/

                            /*
                               Message Channel message responded. Either the
                               write completed or there was a miss.
                            */
    UINT32 mc_err_resp : 1;

                            /* Bits[25:25], Access Type=RW1CV, default=0x00000000*/

                            /*
                               Message Channel message responded with AHB
                               ERROR. Either generated a PCR error, or
                               targetted an unimplemented region.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_MC_STAT_FNV_D_UNIT_0_STRUCT;

/* D_MC_RDATA_FNV_D_UNIT_0_REG supported on:                                    */
/*      BWV (0x40004044)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Holds the read data from MC transaction.
*/


#define D_MC_RDATA_FNV_D_UNIT_0_REG 0x1B440044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_rdata : 32;

                            /* Bits[31:0], Access Type=RWV, default=0x00000000*/

                            /*
                               This holds the returned read data from the
                               Message Channel.
                            */

  } Bits;
  UINT32 Data;

} D_MC_RDATA_FNV_D_UNIT_0_STRUCT;

/* DN_ECC_ENABLE_FNV_D_UNIT_0_REG supported on:                                 */
/*      BWV (0x40004048)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Read ECC Control Registers
*/


#define DN_ECC_ENABLE_FNV_D_UNIT_0_REG 0x1B440048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecc_rd_chk_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable read ECC generation[br] 0 = ecc check
                               disabled[br] 1 = ecc check enabled
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_ECC_ENABLE_FNV_D_UNIT_0_STRUCT;

/* DRDP_WCRD_CONFIG_FNV_D_UNIT_0_REG supported on:                              */
/*      BWV (0x40004050)                                                        */
/* Register default value on BWV: 0x00000050                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Controls the number of write credits allocated for DDRT
*/


#define DRDP_WCRD_CONFIG_FNV_D_UNIT_0_REG 0x1B440050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcrd_cfg : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000050*/

                            /*
                               Setting this will specify the number of write
                               credits allocated for DDRT. Default is 80 write
                               credits
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRDP_WCRD_CONFIG_FNV_D_UNIT_0_STRUCT;

/* DN_CRD_CNT_FNV_D_UNIT_0_REG supported on:                                    */
/*      BWV (0x40004054)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Indicates status of write and read credit counts
*/


#define DN_CRD_CNT_FNV_D_UNIT_0_REG 0x1B440054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcrd_rcyc_cnt : 7;

                            /* Bits[6:0], Access Type=ROV, default=0x00000000*/

                            /*
                               Indicates the number of write credits waiting to
                               return to host
                            */
    UINT32 rcrd_cnt : 7;

                            /* Bits[13:7], Access Type=ROV, default=0x00000000*/

                            /*
                               Indicates the number of read credits free within
                               FNV
                            */
    UINT32 wcrd_cnt : 7;

                            /* Bits[20:14], Access Type=ROV, default=0x00000000*/

                            /*
                               Indicates the number of write credits free
                               within FNV
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_CRD_CNT_FNV_D_UNIT_0_STRUCT;

/* D_READ_CREDIT_FNV_D_UNIT_0_REG supported on:                                 */
/*      BWV (0x40004058)                                                        */
/* Register default value on BWV: 0x00000030                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the default value of read credits. Will be loaded when value of CSR changes
*/


#define D_READ_CREDIT_FNV_D_UNIT_0_REG 0x1B440058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 val : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000030*/

                            /* Value of Read credits. Default value is 48 */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_READ_CREDIT_FNV_D_UNIT_0_STRUCT;

























/* D_FUNC_DEFEATURE1_FNV_D_UNIT_0_REG supported on:                             */
/*      BWV (0x40004130)                                                        */
/* Register default value on BWV: 0xF4041E00                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Defeature 1
*/


#define D_FUNC_DEFEATURE1_FNV_D_UNIT_0_REG 0x1B440130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pf_rd_drp_dis : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               If the bit is set, DDRT reads are NOT dropped
                               during ADR [br] Reads are still dropped during
                               power fail to remove write address conflict with
                               read [br] 0: Reads are dropped in ADR [br] 1:
                               Reads are NOT dropped in ADR
                            */
    UINT32 rsvd_17 : 4;

                            /* Bits[20:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wcrd_init : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Initialize credit to max setting [br] Write 0
                               and then 1 to initialize write credit [br] 0:
                               default [br] 1: Initial write credit
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_FUNC_DEFEATURE1_FNV_D_UNIT_0_STRUCT;

/* D_FUNC_DEFEATURE2_FNV_D_UNIT_0_REG supported on:                             */
/*      BWV (0x40004134)                                                        */
/* Register default value on BWV: 0x0011FA1E                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Defeature 2
*/


#define D_FUNC_DEFEATURE2_FNV_D_UNIT_0_REG 0x1B440134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 err_pche_miss : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               DDRT_ERR# is asserted if read misses prefetch
                               cache during training [br] 0: Disable -
                               (default)[br] 1: DDRT_ERR# is asserted if read
                               misses prefetch cache during DDRT training
                            */
    UINT32 rsvd_1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_FUNC_DEFEATURE2_FNV_D_UNIT_0_STRUCT;






/* D_RST_MASK_FNV_D_UNIT_0_REG supported on:                                    */
/*      BWV (0x4000414c)                                                        */
/* Register default value on BWV: 0x00000003                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reset Interrupt Mask
*/


#define D_RST_MASK_FNV_D_UNIT_0_REG 0x1B44014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sprs_rst_int : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Surprise reset interrupt mask [br] 0: Generate
                               interrupt after surprise reset is detected
                               (ddrt_rst_n=0)[br] Surprise reset will generate
                               interrupt to FW only in functional mode; [br] in
                               training mode, surprise reset will not generate
                               interrupt to FW [br] 1: default
                            */
    UINT32 rsvd_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_RST_MASK_FNV_D_UNIT_0_STRUCT;













/* DN_EMASK_FNV_D_UNIT_0_REG supported on:                                      */
/*      BWV (0x40004180)                                                        */
/* Register default value on BWV: 0x7FFFFFFF                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Masking errors per source
*/


#define DN_EMASK_FNV_D_UNIT_0_REG 0x1B440180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wr_credit : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Mask Error [br] 1 = Mask Error [br] 0 = Allow
                               Error [br] Default is 1
                            */
    UINT32 rsvd_3 : 28;

                            /* Bits[30:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_EMASK_FNV_D_UNIT_0_STRUCT;






























































































































/* DN_SPARE_CTRL_FNV_D_UNIT_0_REG supported on:                                 */
/*      BWV (0x40004704)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains spare
*/


#define DN_SPARE_CTRL_FNV_D_UNIT_0_REG 0x1B440704

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spare_1 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /*
                               [0] - Disable DN Message Channel Arbitration
                               timeout. Otherwise default is to timeout if no
                               response is seen from source.[br] [1:13] -
                               Reserved for future use.[br] [14] -
                               Reserved.[br] [15] - Reserved.
                            */

  } Bits;
  UINT32 Data;

} DN_SPARE_CTRL_FNV_D_UNIT_0_STRUCT;






































/* MB_SMBUS_CMD_FNV_D_UNIT_0_REG supported on:                                  */
/*      BWV (0x40004804)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_SMBUS_CMD_FNV_D_UNIT_0_REG 0x1B440804

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 opcode : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               FW MB OPCODE. Refer to FIS for detailed
                               explaination.
                            */
    UINT32 doorbell : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* FW MB Doorbell */
    UINT32 spares : 15;

                            /* Bits[31:17], Access Type=RW, default=0x00000000*/

                            /* Refer to FIS for details */

  } Bits;
  UINT32 Data;

} MB_SMBUS_CMD_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_0_FNV_D_UNIT_0_REG supported on:                              */
/*      BWV (0x40004808)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMBUS_NONCE_0_FNV_D_UNIT_0_REG 0x1B440808

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_0_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_1_FNV_D_UNIT_0_REG supported on:                              */
/*      BWV (0x4000480c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMBUS_NONCE_1_FNV_D_UNIT_0_REG 0x1B44080C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_1_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_2_FNV_D_UNIT_0_REG supported on:                              */
/*      BWV (0x40004810)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reserved for future use.
*/


#define MB_SMBUS_NONCE_2_FNV_D_UNIT_0_REG 0x1B440810

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_2_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_3_FNV_D_UNIT_0_REG supported on:                              */
/*      BWV (0x40004814)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reserved for future use.
*/


#define MB_SMBUS_NONCE_3_FNV_D_UNIT_0_REG 0x1B440814

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_3_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_0_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004818)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_0_FNV_D_UNIT_0_REG 0x1B440818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_0_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_1_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x4000481c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_1_FNV_D_UNIT_0_REG 0x1B44081C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_1_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_2_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004820)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_2_FNV_D_UNIT_0_REG 0x1B440820

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_2_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_3_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004824)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_3_FNV_D_UNIT_0_REG 0x1B440824

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_3_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_4_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004828)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_4_FNV_D_UNIT_0_REG 0x1B440828

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_4_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_5_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x4000482c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_5_FNV_D_UNIT_0_REG 0x1B44082C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_5_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_6_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004830)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_6_FNV_D_UNIT_0_REG 0x1B440830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_6_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_7_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004834)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_7_FNV_D_UNIT_0_REG 0x1B440834

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_7_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_8_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x40004838)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_8_FNV_D_UNIT_0_REG 0x1B440838

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_8_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_9_FNV_D_UNIT_0_REG supported on:                      */
/*      BWV (0x4000483c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_9_FNV_D_UNIT_0_REG 0x1B44083C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_9_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_10_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004840)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_10_FNV_D_UNIT_0_REG 0x1B440840

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_10_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_11_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004844)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_11_FNV_D_UNIT_0_REG 0x1B440844

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_11_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_12_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004848)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_12_FNV_D_UNIT_0_REG 0x1B440848

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_12_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_13_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x4000484c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_13_FNV_D_UNIT_0_REG 0x1B44084C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_13_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_14_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004850)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_14_FNV_D_UNIT_0_REG 0x1B440850

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_14_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_15_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004854)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_15_FNV_D_UNIT_0_REG 0x1B440854

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_15_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_16_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004858)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_16_FNV_D_UNIT_0_REG 0x1B440858

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_16_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_17_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x4000485c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_17_FNV_D_UNIT_0_REG 0x1B44085C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_17_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_18_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004860)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_18_FNV_D_UNIT_0_REG 0x1B440860

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_18_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_19_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004864)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_19_FNV_D_UNIT_0_REG 0x1B440864

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_19_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_20_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004868)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_20_FNV_D_UNIT_0_REG 0x1B440868

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_20_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_21_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x4000486c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_21_FNV_D_UNIT_0_REG 0x1B44086C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_21_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_22_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004870)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_22_FNV_D_UNIT_0_REG 0x1B440870

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_22_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_23_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004874)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_23_FNV_D_UNIT_0_REG 0x1B440874

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_23_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_24_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004878)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_24_FNV_D_UNIT_0_REG 0x1B440878

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_24_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_25_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x4000487c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_25_FNV_D_UNIT_0_REG 0x1B44087C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_25_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_26_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004880)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_26_FNV_D_UNIT_0_REG 0x1B440880

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_26_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_27_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004884)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_27_FNV_D_UNIT_0_REG 0x1B440884

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_27_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_28_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004888)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_28_FNV_D_UNIT_0_REG 0x1B440888

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_28_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_29_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x4000488c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_29_FNV_D_UNIT_0_REG 0x1B44088C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_29_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_30_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004890)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_30_FNV_D_UNIT_0_REG 0x1B440890

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_30_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_31_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x40004894)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_31_FNV_D_UNIT_0_REG 0x1B440894

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_31_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_STATUS_FNV_D_UNIT_0_REG supported on:                               */
/*      BWV (0x40004898)                                                        */
/* Register default value on BWV: 0x00000001                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_SMBUS_STATUS_FNV_D_UNIT_0_REG 0x1B440898

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp : 1;

                            /* Bits[0:0], Access Type=RWV, default=0x00000001*/

                            /*
                               FW Mailbox Completion:[br] 0 = Not Complete[br]
                               1 = Complete[br] HW will clear this bit when
                               doorbell bit is set.
                            */
    UINT32 fail : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               FW Mailbox Nonce Fail:[br] 0 = Success[br] 1 =
                               Failure
                            */
    UINT32 spare0 : 6;

                            /* Bits[7:2], Access Type=RW, default=0x00000000*/

                            /* See FIS for details. */
    UINT32 stat : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /* FW Mailbox Status Code. See FIS for details */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MB_SMBUS_STATUS_FNV_D_UNIT_0_STRUCT;


/* MB_SMBUS_OUTPUT_PAYLOAD_0_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048a0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_0_FNV_D_UNIT_0_REG 0x1B4408A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_0_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_1_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048a4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_1_FNV_D_UNIT_0_REG 0x1B4408A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_1_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_2_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048a8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_2_FNV_D_UNIT_0_REG 0x1B4408A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_2_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_3_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048ac)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_3_FNV_D_UNIT_0_REG 0x1B4408AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_3_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_4_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048b0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_4_FNV_D_UNIT_0_REG 0x1B4408B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_4_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_5_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048b4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_5_FNV_D_UNIT_0_REG 0x1B4408B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_5_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_6_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048b8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_6_FNV_D_UNIT_0_REG 0x1B4408B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_6_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_7_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048bc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_7_FNV_D_UNIT_0_REG 0x1B4408BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_7_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_8_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048c0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_8_FNV_D_UNIT_0_REG 0x1B4408C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_8_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_9_FNV_D_UNIT_0_REG supported on:                     */
/*      BWV (0x400048c4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_9_FNV_D_UNIT_0_REG 0x1B4408C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_9_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_10_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048c8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_10_FNV_D_UNIT_0_REG 0x1B4408C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_10_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_11_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048cc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_11_FNV_D_UNIT_0_REG 0x1B4408CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_11_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_12_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048d0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_12_FNV_D_UNIT_0_REG 0x1B4408D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_12_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_13_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048d4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_13_FNV_D_UNIT_0_REG 0x1B4408D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_13_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_14_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048d8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_14_FNV_D_UNIT_0_REG 0x1B4408D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_14_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_15_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048dc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_15_FNV_D_UNIT_0_REG 0x1B4408DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_15_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_16_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048e0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_16_FNV_D_UNIT_0_REG 0x1B4408E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_16_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_17_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048e4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_17_FNV_D_UNIT_0_REG 0x1B4408E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_17_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_18_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048e8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_18_FNV_D_UNIT_0_REG 0x1B4408E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_18_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_19_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048ec)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_19_FNV_D_UNIT_0_REG 0x1B4408EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_19_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_20_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048f0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_20_FNV_D_UNIT_0_REG 0x1B4408F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_20_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_21_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048f4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_21_FNV_D_UNIT_0_REG 0x1B4408F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_21_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_22_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048f8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_22_FNV_D_UNIT_0_REG 0x1B4408F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_22_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_23_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x400048fc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_23_FNV_D_UNIT_0_REG 0x1B4408FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_23_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_24_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x40004900)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_24_FNV_D_UNIT_0_REG 0x1B440900

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_24_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_25_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x40004904)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_25_FNV_D_UNIT_0_REG 0x1B440904

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_25_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_26_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x40004908)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_26_FNV_D_UNIT_0_REG 0x1B440908

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_26_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_27_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x4000490c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_27_FNV_D_UNIT_0_REG 0x1B44090C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_27_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_28_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x40004910)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_28_FNV_D_UNIT_0_REG 0x1B440910

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_28_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_29_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x40004914)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_29_FNV_D_UNIT_0_REG 0x1B440914

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_29_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_30_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x40004918)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_30_FNV_D_UNIT_0_REG 0x1B440918

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_30_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_31_FNV_D_UNIT_0_REG supported on:                    */
/*      BWV (0x4000491c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_31_FNV_D_UNIT_0_REG 0x1B44091C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_31_FNV_D_UNIT_0_STRUCT;

/* MB_SMBUS_ABORT_FNV_D_UNIT_0_REG supported on:                                */
/*      BWV (0x40004920)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Abort.
*/


#define MB_SMBUS_ABORT_FNV_D_UNIT_0_REG 0x1B440920

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 abort : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               FW MB Abort:[br] 1 = Abort Command in Command
                               Register
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MB_SMBUS_ABORT_FNV_D_UNIT_0_STRUCT;

/* DS_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG supported on:                           */
/*      BWV (0x40004928)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_4_BWV                                              */
/* Struct generated from BWV BDF: 0_0_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Training  Enable
*/


#define DS_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG 0x1B440928

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               DDRT Training Mode Enable (not used) [br]
                               Setting this bit will block write data from
                               sending to memory
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               EARLY CMD-CK Training Enable [br] Parity only,
                               commands are not written into ingress command
                               queue
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Advanced CMD-CK Training Enable[br] MRS,
                               extended MRS and Memory Mapped config access are
                               ignored
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DS_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT;





































































































































#endif /* _FNV_D_UNIT_0_h */
