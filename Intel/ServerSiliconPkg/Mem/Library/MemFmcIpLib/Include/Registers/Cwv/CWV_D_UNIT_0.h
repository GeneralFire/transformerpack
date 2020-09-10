
/** @file
  CWV_D_UNIT_0.h

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


#ifndef _CWV_D_UNIT_0_h
#define _CWV_D_UNIT_0_h
#include <DataTypes.h>

/* D_FW_STATUS_CWV_D_UNIT_0_REG supported on:                                   */
/*      CWV (0x40026000)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* BIOS will read this register to determine Valley Controller Firmware Status during boot.
*/


#define D_FW_STATUS_CWV_D_UNIT_0_REG 0x1B240000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fw_status : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Firmware status. */

  } Bits;
  UINT32 Data;

} D_FW_STATUS_CWV_D_UNIT_0_STRUCT;

/* D_FW_STATUS_H_CWV_D_UNIT_0_REG supported on:                                 */
/*      CWV (0x40026004)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* BIOS will read this register to determine Valley Controller Firmware Status during boot.
*/


#define D_FW_STATUS_H_CWV_D_UNIT_0_REG 0x1B240004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fw_status : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Firmware status. */

  } Bits;
  UINT32 Data;

} D_FW_STATUS_H_CWV_D_UNIT_0_STRUCT;


/* DN_ECC_ENABLE_CWV_D_UNIT_0_REG supported on:                                 */
/*      CWV (0x40026010)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Read ECC Control Registers
*/


#define DN_ECC_ENABLE_CWV_D_UNIT_0_REG 0x1B240010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecc_rd_chk_en : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated: Enable read ECC generation[br]
                               0 = ecc check disabled[br] 1 = ecc check enabled
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_ECC_ENABLE_CWV_D_UNIT_0_STRUCT;

/* D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG supported on:                            */
/*      CWV (0x40026020)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Training  Enable
*/


#define D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG 0x1B240020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: DDRT Training Mode Enable [br]
                               Setting this bit will block read and write
                               command from sending to memory
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: EARLY CMD-CK Training Enable
                               [br] Parity only, commands are not written into
                               ingress command queue
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: Advanced CMD-CK Training
                               Enable[br] MRS, extended MRS and Memory Mapped
                               config access are ignored
                            */
    UINT32 reserved_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* TODO: Deprecated: Reserved (3) */
    UINT32 reserved_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* TODO: Deprecated: Reserved (4) */
    UINT32 disable_rid_feedback : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: When set, the RID bits are no
                               longer returned within the data packet,[br]
                               allowing for all data in the buffer to be
                               returned as it was written
                            */
    UINT32 reserved_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* TODO: Deprecated: Reserved (6) */
    UINT32 reserved_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* TODO: Deprecated: Reserved (7) */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_DDRT_TRAINING_EN_CWV_D_UNIT_0_STRUCT;

/* D_DDRT_TRAINING_OTHER_EN_CWV_D_UNIT_0_REG supported on:                      */
/*      CWV (0x40026024)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Other Training  Enable
*/


#define D_DDRT_TRAINING_OTHER_EN_CWV_D_UNIT_0_REG 0x1B240024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reset_trn : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: Reset Training Enable [br] Set
                               this bit to reset training FSM TBD
                            */
    UINT32 rsvd_3 : 27;

                            /* Bits[29:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pche_inv_set0_only : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: When this bit is asserted,
                               DUNIT Prefetch Cache will invalidate SET 0 Only.
                               Hardware will clear CSR, once done. Use this CSR
                               right after Training. Prefetch SET 0 is used for
                               training and must be invalidated when switching
                               to normal mode.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_DDRT_TRAINING_OTHER_EN_CWV_D_UNIT_0_STRUCT;




/* D_DDRT_TRAINING_MPR_CWV_D_UNIT_0_REG supported on:                           */
/*      CWV (0x40026034)                                                        */
/* Register default value on CWV: 0x000F3355                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* MPR Training Pattern
*/


#define D_DDRT_TRAINING_MPR_CWV_D_UNIT_0_REG 0x1B240034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr0 : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000055*/

                            /* TODO: Deprecated: MPR training pattern 0 */
    UINT32 mpr1 : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000033*/

                            /* TODO: Deprecated: MPR training pattern 1 */
    UINT32 mpr2 : 8;

                            /* Bits[23:16], Access Type=RO, default=0x0000000F*/

                            /* TODO: Deprecated: MPR training pattern 2 */
    UINT32 mpr3 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* TODO: Deprecated: MPR training pattern 3 */

  } Bits;
  UINT32 Data;

} D_DDRT_TRAINING_MPR_CWV_D_UNIT_0_STRUCT;

/* D_DDRT_TRAINING_MR3_CWV_D_UNIT_0_REG supported on:                           */
/*      CWV (0x40026038)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define D_DDRT_TRAINING_MR3_CWV_D_UNIT_0_REG 0x1B240038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr_sel : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: MPR page selection[br] 0x0 :
                               Select MPR0[br] 0x1 : Select MPR1[br] 0x2 :
                               Select MPR2[br] 0x3 : Select MPR3
                            */
    UINT32 mpr_op : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Deprecated: MPR operation[br] 0 :
                               normal[br] 1 : data flow from/to MPR
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 per_dram_addr_ability : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* TODO: Deprecated: Reserved */
    UINT32 rsvd_5 : 6;

                            /* Bits[10:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mpr_rd_frmt : 2;

                            /* Bits[12:11], Access Type=RO, default=0x00000000*/

                            /*
                               Obsolete: (do not use) [br] MPR read format;
                               Valley Controller supports both serial and
                               parallel MPR read format 00 = Serial [br] 01 =
                               Parallel [br] others = reserved
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_DDRT_TRAINING_MR3_CWV_D_UNIT_0_STRUCT;

/* DRDP_WCRD_CONFIG_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026040)                                                        */
/* Register default value on CWV: 0x00000080                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Controls the number of write credits allocated for DDRT
*/


#define DRDP_WCRD_CONFIG_CWV_D_UNIT_0_REG 0x1B240040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcrd_cfg : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000080*/

                            /*
                               Setting this will specify the number of write
                               credits allocated for DDRT. FW must program this
                               to 80 (CWV Max Supported Write Credits), as the
                               default of 128 write credits is not supported.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRDP_WCRD_CONFIG_CWV_D_UNIT_0_STRUCT;

/* DN_CRD_CNT_CWV_D_UNIT_0_REG supported on:                                    */
/*      CWV (0x40026044)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Indicates status of write and read credit counts
*/


#define DN_CRD_CNT_CWV_D_UNIT_0_REG 0x1B240044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcrd_rcyc_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Indicates the number of write credits waiting to
                               return to host
                            */
    UINT32 rcrd_cnt : 7;

                            /* Bits[14:8], Access Type=RO, default=0x00000000*/

                            /* DEPRICATED: Use Subchannel specific from DAUNIT */
    UINT32 wcrd_cnt : 8;

                            /* Bits[22:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               Indicates the number of write credits free
                               within Valley Controller
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_CRD_CNT_CWV_D_UNIT_0_STRUCT;

/* D_READ_CREDIT_CWV_D_UNIT_0_REG supported on:                                 */
/*      CWV (0x40026048)                                                        */
/* Register default value on CWV: 0x00000030                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the default value of read credits. Will be loaded when value of CSR changes
*/


#define D_READ_CREDIT_CWV_D_UNIT_0_REG 0x1B240048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 val : 6;

                            /* Bits[5:0], Access Type=RO, default=0x00000030*/

                            /*
                               TODO: Depricated:Value of Read credits. Default
                               value is 48
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_READ_CREDIT_CWV_D_UNIT_0_STRUCT;


/* D_FUNC_DEFEATURE1_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x40026050)                                                        */
/* Register default value on CWV: 0xF4051E00                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Defeature 1
*/


#define D_FUNC_DEFEATURE1_CWV_D_UNIT_0_REG 0x1B240050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 21;

                            /* Bits[20:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wcrd_init : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

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

} D_FUNC_DEFEATURE1_CWV_D_UNIT_0_STRUCT;

/* D_FUNC_DEFEATURE2_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x40026054)                                                        */
/* Register default value on CWV: 0x0011F21E                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Defeature 2
*/


#define D_FUNC_DEFEATURE2_CWV_D_UNIT_0_REG 0x1B240054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 err_pche_miss : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:DDRT_ERR# is asserted if read
                               misses prefetch cache during training [br] 0:
                               Disable - (default)[br] 1: DDRT_ERR# is asserted
                               if read misses prefetch cache during DDRT
                               training
                            */
    UINT32 rsvd_1 : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} D_FUNC_DEFEATURE2_CWV_D_UNIT_0_STRUCT;

/* D_RST_MASK_CWV_D_UNIT_0_REG supported on:                                    */
/*      CWV (0x40026058)                                                        */
/* Register default value on CWV: 0x00000003                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Reset Interrupt Mask
*/


#define D_RST_MASK_CWV_D_UNIT_0_REG 0x1B240058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sprs_rst_int : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

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

} D_RST_MASK_CWV_D_UNIT_0_STRUCT;









































/* DN_EMASK_CWV_D_UNIT_0_REG supported on:                                      */
/*      CWV (0x4002614c)                                                        */
/* Register default value on CWV: 0x0000BFF7                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Masking errors per source
*/


#define DN_EMASK_CWV_D_UNIT_0_REG 0x1B24014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wr_credit : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               Mask Error [br] 1 = Mask Error [br] 0 = Allow
                               Error [br] Default is 1
                            */
    UINT32 rsvd_7 : 11;

                            /* Bits[17:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_18 : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_EMASK_CWV_D_UNIT_0_STRUCT;




























/* DN_PERM_ERR_LOG_CWV_D_UNIT_0_REG supported on:                               */
/*      CWV (0x40026268)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br] This register stores which AHB CSR endpoint saw a CSR access violation
*/


#define DN_PERM_ERR_LOG_CWV_D_UNIT_0_REG 0x1B240268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dn_csr_vld : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access
                               violation
                            */
    UINT32 dn_csr_write : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the CSR
                               Access Violation.[br] 1'b1 - write[br] 1'b0 -
                               read
                            */
    UINT32 dn_csr_unimpl_vld : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access to
                               an unimplemented CSR
                            */
    UINT32 dm_bwcmd_csr_vld : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access
                               violation
                            */
    UINT32 dm_bwcmd_csr_write : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the CSR
                               Access Violation.[br] 1'b1 - write[br] 1'b0 -
                               read
                            */
    UINT32 dm_bwcmd_csr_unimpl_vld : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access to
                               an unimplemented CSR
                            */
    UINT32 dm_bwsts_csr_vld : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw a PCR access
                               violation
                            */
    UINT32 dm_bwsts_csr_write : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the CSR
                               Access Violation.[br] 1'b1 - write[br] 1'b0 -
                               read
                            */
    UINT32 dm_bwsts_csr_unimpl_vld : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access to
                               an unimplemented CSR
                            */
    UINT32 dm_fwmb_csr_vld : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access
                               violation
                            */
    UINT32 dm_fwmb_csr_write : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the CSR
                               Access Violation.[br] 1'b1 - write[br] 1'b0 -
                               read
                            */
    UINT32 dm_fwmb_csr_unimpl_vld : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access to
                               an unimplemented CSR
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_PERM_ERR_LOG_CWV_D_UNIT_0_STRUCT;





































/* DN0_ASC_RSP_CTL0_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026370)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 0
*/


#define DN0_ASC_RSP_CTL0_CWV_D_UNIT_0_REG 0x1B240370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asc0outsel : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 output select mode for ascout[0] */
    UINT32 asc1mode : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 mode Select, 0-select MBP, 1-select MBPQual */
    UINT32 asc1mbpselin : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Select */
    UINT32 asc0mbpselin : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Select */
    UINT32 asc1mbpqual : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Qualification */
    UINT32 asc0mbpqual : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Qualification */
    UINT32 asc1enable : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 enable */
    UINT32 asc0enable : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 enable */
    UINT32 asc1clronstop : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 clear on stop */
    UINT32 asc0clronstop : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 clear on stop */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN0_ASC_RSP_CTL0_CWV_D_UNIT_0_STRUCT;

/* DN0_ASC_RSP_CTL1_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026374)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 1
*/


#define DN0_ASC_RSP_CTL1_CWV_D_UNIT_0_REG 0x1B240374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 glbrsp0startsel : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 start Select */
    UINT32 glbrsp0stopsel : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 glbrsp1startsel : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 start Select */
    UINT32 glbrsp1stopsel : 3;

                            /* Bits[11:9], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 stop Select */
    UINT32 glbrsp2startsel : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 start Select */
    UINT32 glbrsp2stopsel : 3;

                            /* Bits[17:15], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 stop Select */
    UINT32 glbrsp3startsel : 3;

                            /* Bits[20:18], Access Type=RW/P, default=0x00000000*/

                            /* Global response 3 start Select */
    UINT32 glbrsp3stopsel : 3;

                            /* Bits[23:21], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN0_ASC_RSP_CTL1_CWV_D_UNIT_0_STRUCT;

/* DN0_ASC0_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026378)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN0_ASC0_LDHIVAL_CWV_D_UNIT_0_REG 0x1B240378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN0_ASC0_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN0_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x4002637c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN0_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B24037C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN0_ASC0_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN0_ASC1_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026380)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN0_ASC1_LDHIVAL_CWV_D_UNIT_0_REG 0x1B240380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN0_ASC1_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN0_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x40026384)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN0_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B240384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN0_ASC1_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN0_CTS_ASC_RSP_CWV_D_UNIT_0_REG supported on:                               */
/*      CWV (0x40026388)                                                        */
/* Register default value on CWV: 0x00000008                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* 
*/


#define DN0_CTS_ASC_RSP_CWV_D_UNIT_0_REG 0x1B240388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsp_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 rsp_sel : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 respdone_dis : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN0_CTS_ASC_RSP_CWV_D_UNIT_0_STRUCT;

/* DN1_ASC_RSP_CTL0_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x4002638c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 0
*/


#define DN1_ASC_RSP_CTL0_CWV_D_UNIT_0_REG 0x1B24038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asc0outsel : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 output select mode for ascout[0] */
    UINT32 asc1mode : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 mode Select, 0-select MBP, 1-select MBPQual */
    UINT32 asc1mbpselin : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Select */
    UINT32 asc0mbpselin : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Select */
    UINT32 asc1mbpqual : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Qualification */
    UINT32 asc0mbpqual : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Qualification */
    UINT32 asc1enable : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 enable */
    UINT32 asc0enable : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 enable */
    UINT32 asc1clronstop : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 clear on stop */
    UINT32 asc0clronstop : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 clear on stop */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN1_ASC_RSP_CTL0_CWV_D_UNIT_0_STRUCT;

/* DN1_ASC_RSP_CTL1_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026390)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 1
*/


#define DN1_ASC_RSP_CTL1_CWV_D_UNIT_0_REG 0x1B240390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 glbrsp0startsel : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 start Select */
    UINT32 glbrsp0stopsel : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 glbrsp1startsel : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 start Select */
    UINT32 glbrsp1stopsel : 3;

                            /* Bits[11:9], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 stop Select */
    UINT32 glbrsp2startsel : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 start Select */
    UINT32 glbrsp2stopsel : 3;

                            /* Bits[17:15], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 stop Select */
    UINT32 glbrsp3startsel : 3;

                            /* Bits[20:18], Access Type=RW/P, default=0x00000000*/

                            /* Global response 3 start Select */
    UINT32 glbrsp3stopsel : 3;

                            /* Bits[23:21], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN1_ASC_RSP_CTL1_CWV_D_UNIT_0_STRUCT;

/* DN1_ASC0_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x40026394)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN1_ASC0_LDHIVAL_CWV_D_UNIT_0_REG 0x1B240394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN1_ASC0_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN1_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x40026398)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN1_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B240398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN1_ASC0_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN1_ASC1_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x4002639c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN1_ASC1_LDHIVAL_CWV_D_UNIT_0_REG 0x1B24039C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN1_ASC1_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN1_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x400263a0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN1_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B2403A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN1_ASC1_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN1_CTS_ASC_RSP_CWV_D_UNIT_0_REG supported on:                               */
/*      CWV (0x400263a4)                                                        */
/* Register default value on CWV: 0x00000008                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* 
*/


#define DN1_CTS_ASC_RSP_CWV_D_UNIT_0_REG 0x1B2403A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsp_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 rsp_sel : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 respdone_dis : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN1_CTS_ASC_RSP_CWV_D_UNIT_0_STRUCT;

/* DN2_ASC_RSP_CTL0_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263a8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 0
*/


#define DN2_ASC_RSP_CTL0_CWV_D_UNIT_0_REG 0x1B2403A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asc0outsel : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 output select mode for ascout[0] */
    UINT32 asc1mode : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 mode Select, 0-select MBP, 1-select MBPQual */
    UINT32 asc1mbpselin : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Select */
    UINT32 asc0mbpselin : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Select */
    UINT32 asc1mbpqual : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Qualification */
    UINT32 asc0mbpqual : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Qualification */
    UINT32 asc1enable : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 enable */
    UINT32 asc0enable : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 enable */
    UINT32 asc1clronstop : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 clear on stop */
    UINT32 asc0clronstop : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 clear on stop */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN2_ASC_RSP_CTL0_CWV_D_UNIT_0_STRUCT;

/* DN2_ASC_RSP_CTL1_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263ac)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 1
*/


#define DN2_ASC_RSP_CTL1_CWV_D_UNIT_0_REG 0x1B2403AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 glbrsp0startsel : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 start Select */
    UINT32 glbrsp0stopsel : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 glbrsp1startsel : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 start Select */
    UINT32 glbrsp1stopsel : 3;

                            /* Bits[11:9], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 stop Select */
    UINT32 glbrsp2startsel : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 start Select */
    UINT32 glbrsp2stopsel : 3;

                            /* Bits[17:15], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 stop Select */
    UINT32 glbrsp3startsel : 3;

                            /* Bits[20:18], Access Type=RW/P, default=0x00000000*/

                            /* Global response 3 start Select */
    UINT32 glbrsp3stopsel : 3;

                            /* Bits[23:21], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN2_ASC_RSP_CTL1_CWV_D_UNIT_0_STRUCT;

/* DN2_ASC0_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263b0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN2_ASC0_LDHIVAL_CWV_D_UNIT_0_REG 0x1B2403B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN2_ASC0_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN2_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x400263b4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN2_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B2403B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN2_ASC0_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN2_ASC1_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263b8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN2_ASC1_LDHIVAL_CWV_D_UNIT_0_REG 0x1B2403B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN2_ASC1_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN2_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x400263bc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN2_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B2403BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN2_ASC1_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN2_CTS_ASC_RSP_CWV_D_UNIT_0_REG supported on:                               */
/*      CWV (0x400263c0)                                                        */
/* Register default value on CWV: 0x00000008                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* 
*/


#define DN2_CTS_ASC_RSP_CWV_D_UNIT_0_REG 0x1B2403C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsp_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 rsp_sel : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 respdone_dis : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN2_CTS_ASC_RSP_CWV_D_UNIT_0_STRUCT;

/* DN3_ASC_RSP_CTL0_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263c4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 0
*/


#define DN3_ASC_RSP_CTL0_CWV_D_UNIT_0_REG 0x1B2403C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asc0outsel : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 output select mode for ascout[0] */
    UINT32 asc1mode : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 mode Select, 0-select MBP, 1-select MBPQual */
    UINT32 asc1mbpselin : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Select */
    UINT32 asc0mbpselin : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Select */
    UINT32 asc1mbpqual : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 MPB Qualification */
    UINT32 asc0mbpqual : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 MPB Qualification */
    UINT32 asc1enable : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 enable */
    UINT32 asc0enable : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 enable */
    UINT32 asc1clronstop : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* ASC1 clear on stop */
    UINT32 asc0clronstop : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* ASC0 clear on stop */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN3_ASC_RSP_CTL0_CWV_D_UNIT_0_STRUCT;

/* DN3_ASC_RSP_CTL1_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263c8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC and RESP control 1
*/


#define DN3_ASC_RSP_CTL1_CWV_D_UNIT_0_REG 0x1B2403C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 glbrsp0startsel : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 start Select */
    UINT32 glbrsp0stopsel : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 glbrsp1startsel : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 start Select */
    UINT32 glbrsp1stopsel : 3;

                            /* Bits[11:9], Access Type=RW/P, default=0x00000000*/

                            /* Global response 1 stop Select */
    UINT32 glbrsp2startsel : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 start Select */
    UINT32 glbrsp2stopsel : 3;

                            /* Bits[17:15], Access Type=RW/P, default=0x00000000*/

                            /* Global response 2 stop Select */
    UINT32 glbrsp3startsel : 3;

                            /* Bits[20:18], Access Type=RW/P, default=0x00000000*/

                            /* Global response 3 start Select */
    UINT32 glbrsp3stopsel : 3;

                            /* Bits[23:21], Access Type=RW/P, default=0x00000000*/

                            /* Global response 0 stop Select */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN3_ASC_RSP_CTL1_CWV_D_UNIT_0_STRUCT;

/* DN3_ASC0_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263cc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN3_ASC0_LDHIVAL_CWV_D_UNIT_0_REG 0x1B2403CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN3_ASC0_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN3_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x400263d0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN3_ASC0_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B2403D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN3_ASC0_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN3_ASC1_LDHIVAL_CWV_D_UNIT_0_REG supported on:                              */
/*      CWV (0x400263d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN3_ASC1_LDHIVAL_CWV_D_UNIT_0_REG 0x1B2403D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN3_ASC1_LDHIVAL_CWV_D_UNIT_0_STRUCT;

/* DN3_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x400263d8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* ASC count value.
*/


#define DN3_ASC1_LDLOWVAL_CWV_D_UNIT_0_REG 0x1B2403D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 asccountval : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* ASC Count Value */

  } Bits;
  UINT32 Data;

} DN3_ASC1_LDLOWVAL_CWV_D_UNIT_0_STRUCT;

/* DN3_CTS_ASC_RSP_CWV_D_UNIT_0_REG supported on:                               */
/*      CWV (0x400263dc)                                                        */
/* Register default value on CWV: 0x00000008                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* 
*/


#define DN3_CTS_ASC_RSP_CWV_D_UNIT_0_REG 0x1B2403DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsp_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 rsp_sel : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 respdone_dis : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN3_CTS_ASC_RSP_CWV_D_UNIT_0_STRUCT;


























































































































/* DDRT_PERM_ERR_LOG_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x400266f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* The AHB address of the access that triggered an error.
*/


#define DDRT_PERM_ERR_LOG_CWV_D_UNIT_0_REG 0x1B2406F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vld : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Valid indicator, FW should write 1 to clear when
                               serviced.
                            */
    UINT32 rsvd : 4;

                            /* Bits[4:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 err_type : 2;

                            /* Bits[6:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Error Type 2:Unsuccesful 1: DDRT SAI Error 0:
                               PCR Violation.
                            */
    UINT32 write : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* Violation Type 1: Write 0: Read. */
    UINT32 offset : 16;

                            /* Bits[23:8], Access Type=RO/V, default=0x00000000*/

                            /* Bits [15:0] of the AHB Addrs */
    UINT32 base_addr : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* Bits [31:24] of the AHB Addrs violation */

  } Bits;
  UINT32 Data;

} DDRT_PERM_ERR_LOG_CWV_D_UNIT_0_STRUCT;



/* DN_SPARE_CTRL_CWV_D_UNIT_0_REG supported on:                                 */
/*      CWV (0x40026704)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains spare
*/


#define DN_SPARE_CTRL_CWV_D_UNIT_0_REG 0x1B240704

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spare_1 : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               [0] - Disable DN Message Channel Arbitration
                               timeout. Otherwise default is to timeout if no
                               response is seen from source.[br] [1:13] -
                               Reserved for future use.[br] [14] -
                               Reserved.[br] [15] - Reserved.
                            */

  } Bits;
  UINT32 Data;

} DN_SPARE_CTRL_CWV_D_UNIT_0_STRUCT;



















/* DN_DMA_STS_CWV_D_UNIT_0_REG supported on:                                    */
/*      CWV (0x40026750)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Bit Vector that will indicate where in the design DMA transactions are active
*/


#define DN_DMA_STS_CWV_D_UNIT_0_REG 0x1B240750

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dma_wr_acq : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA write command is in acq */
    UINT32 dma_rd_acq : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with any ID */
    UINT32 dma_rd0_acq : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 0 */
    UINT32 dma_rd1_acq : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 1 */
    UINT32 dma_rd2_acq : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 2 */
    UINT32 dma_rd3_acq : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 3 */
    UINT32 dma_rd4_acq : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 4 */
    UINT32 dma_rd5_acq : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 5 */
    UINT32 dma_rd6_acq : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 6 */
    UINT32 dma_rd7_acq : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in acq with ID 7 */
    UINT32 dma_wr_glb : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA write command is in glb */
    UINT32 dma_rd_glb : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with any ID */
    UINT32 dma_rd0_glb : 1;

                            /* Bits[12:12], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 0 */
    UINT32 dma_rd1_glb : 1;

                            /* Bits[13:13], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 1 */
    UINT32 dma_rd2_glb : 1;

                            /* Bits[14:14], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 2 */
    UINT32 dma_rd3_glb : 1;

                            /* Bits[15:15], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 3 */
    UINT32 dma_rd4_glb : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 4 */
    UINT32 dma_rd5_glb : 1;

                            /* Bits[17:17], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 5 */
    UINT32 dma_rd6_glb : 1;

                            /* Bits[18:18], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 6 */
    UINT32 dma_rd7_glb : 1;

                            /* Bits[19:19], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in glb with ID 7 */
    UINT32 dma_wr_chn : 1;

                            /* Bits[20:20], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA write command is in chn */
    UINT32 dma_rd_chn : 1;

                            /* Bits[21:21], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with any ID */
    UINT32 dma_rd0_chn : 1;

                            /* Bits[22:22], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 0 */
    UINT32 dma_rd1_chn : 1;

                            /* Bits[23:23], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 1 */
    UINT32 dma_rd2_chn : 1;

                            /* Bits[24:24], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 2 */
    UINT32 dma_rd3_chn : 1;

                            /* Bits[25:25], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 3 */
    UINT32 dma_rd4_chn : 1;

                            /* Bits[26:26], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 4 */
    UINT32 dma_rd5_chn : 1;

                            /* Bits[27:27], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 5 */
    UINT32 dma_rd6_chn : 1;

                            /* Bits[28:28], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 6 */
    UINT32 dma_rd7_chn : 1;

                            /* Bits[29:29], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is in chn with ID 7 */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_DMA_STS_CWV_D_UNIT_0_STRUCT;

/* DN_DMA_STS_MERGED_CWV_D_UNIT_0_REG supported on:                             */
/*      CWV (0x40026754)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Merged Bit Vector that will indicate DMA transactions are active
*/


#define DN_DMA_STS_MERGED_CWV_D_UNIT_0_REG 0x1B240754

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dma_wr_actv : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA write command is active in glb/chn */
    UINT32 dma_rd_actv : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* Indicates DMA Read command is active in glb/chn */
    UINT32 dma_rd0_actv : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               0
                            */
    UINT32 dma_rd1_actv : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               1
                            */
    UINT32 dma_rd2_actv : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               2
                            */
    UINT32 dma_rd3_actv : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               3
                            */
    UINT32 dma_rd4_actv : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               4
                            */
    UINT32 dma_rd5_actv : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               5
                            */
    UINT32 dma_rd6_actv : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               6
                            */
    UINT32 dma_rd7_actv : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates DMA Read command is in glb/chn with ID
                               7
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DN_DMA_STS_MERGED_CWV_D_UNIT_0_STRUCT;

/* TXN_TRK_DDRT_CWV_D_UNIT_0_REG supported on:                                  */
/*      CWV (0x40026758)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Sets up which dpa and commands to log
*/


#define TXN_TRK_DDRT_CWV_D_UNIT_0_REG 0x1B240758

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_addr_lower : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* Starting address bit for lower DPA */
    UINT32 start_addr_upper : 6;

                            /* Bits[10:5], Access Type=RW/P, default=0x00000000*/

                            /* Starting address bit for upper DPA */
    UINT32 addr_splice_sel : 3;

                            /* Bits[13:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Sets up how many bits to divide between upper
                               and lower DPA
                            */
    UINT32 cmd_en : 16;

                            /* Bits[29:14], Access Type=RW/P, default=0x00000000*/

                            /* Sets up which commands to log */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXN_TRK_DDRT_CWV_D_UNIT_0_STRUCT;

/* TXN_TRK_RTRY_CWV_D_UNIT_0_REG supported on:                                  */
/*      CWV (0x4002675c)                                                        */
/* Register default value on CWV: 0xC0000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Sets up which dpa and commands to log
*/


#define TXN_TRK_RTRY_CWV_D_UNIT_0_REG 0x1B24075C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_addr_lower : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* Starting address bit for lower DPA */
    UINT32 start_addr_upper : 6;

                            /* Bits[10:5], Access Type=RW/P, default=0x00000000*/

                            /* Starting address bit for upper DPA */
    UINT32 addr_splice_sel : 3;

                            /* Bits[13:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Sets up how many bits to divide between upper
                               and lower DPA
                            */
    UINT32 cmd_en : 14;

                            /* Bits[27:14], Access Type=RW/P, default=0x00000000*/

                            /* Sets up which commands to log */
    UINT32 src_en : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000000*/

                            /* Sets up which command sources to log */
    UINT32 nrtry_en : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000001*/

                            /* Sets up whether to log nonretried commands */
    UINT32 rtry_en : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /* Sets up whether to log retried commands only */

  } Bits;
  UINT32 Data;

} TXN_TRK_RTRY_CWV_D_UNIT_0_STRUCT;

/* TXN_TRK_D2P_CWV_D_UNIT_0_REG supported on:                                   */
/*      CWV (0x40026760)                                                        */
/* Register default value on CWV: 0xF0000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Sets up which dpa and commands to log
*/


#define TXN_TRK_D2P_CWV_D_UNIT_0_REG 0x1B240760

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_addr_lower : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* Starting address bit for lower DPA */
    UINT32 start_addr_upper : 6;

                            /* Bits[10:5], Access Type=RW/P, default=0x00000000*/

                            /* Starting address bit for upper DPA */
    UINT32 addr_splice_sel : 3;

                            /* Bits[13:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Sets up how many bits to divide between upper
                               and lower DPA
                            */
    UINT32 cmd_en : 14;

                            /* Bits[27:14], Access Type=RW/P, default=0x00000000*/

                            /* Sets up which commands to log */
    UINT32 src_en : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000003*/

                            /* Sets up which command sources to log */
    UINT32 nrtry_en : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000001*/

                            /* Sets up whether to log nonretried commands */
    UINT32 rtry_en : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /* Sets up whether to log retried commands */

  } Bits;
  UINT32 Data;

} TXN_TRK_D2P_CWV_D_UNIT_0_STRUCT;

/* GLB_TXN_TRK_SEL_CWV_D_UNIT_0_REG supported on:                               */
/*      CWV (0x40026764)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Picks which entries to read from fifo
*/


#define GLB_TXN_TRK_SEL_CWV_D_UNIT_0_REG 0x1B240764

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrt_fifo : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* Picks which pair of entries to output */
    UINT32 rtry_fifo : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x00000000*/

                            /* Picks which pair of entries to output */
    UINT32 d2p_fifo : 5;

                            /* Bits[14:10], Access Type=RW/P, default=0x00000000*/

                            /* Picks which pair of entries to output */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GLB_TXN_TRK_SEL_CWV_D_UNIT_0_STRUCT;

/* GLB_TXN_TRK_DATA_DDRT_CWV_D_UNIT_0_REG supported on:                         */
/*      CWV (0x40026768)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* gives 2 entries of the fifo to read
*/


#define GLB_TXN_TRK_DATA_DDRT_CWV_D_UNIT_0_REG 0x1B240768

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* data out */

  } Bits;
  UINT32 Data;

} GLB_TXN_TRK_DATA_DDRT_CWV_D_UNIT_0_STRUCT;

/* GLB_TXN_TRK_DATA_RTRY_CWV_D_UNIT_0_REG supported on:                         */
/*      CWV (0x4002676c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* gives 2 entries of the fifo to read
*/


#define GLB_TXN_TRK_DATA_RTRY_CWV_D_UNIT_0_REG 0x1B24076C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* data out */

  } Bits;
  UINT32 Data;

} GLB_TXN_TRK_DATA_RTRY_CWV_D_UNIT_0_STRUCT;

/* GLB_TXN_TRK_DATA_D2P_CWV_D_UNIT_0_REG supported on:                          */
/*      CWV (0x40026770)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* gives 2 entries of the fifo to read
*/


#define GLB_TXN_TRK_DATA_D2P_CWV_D_UNIT_0_REG 0x1B240770

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_data : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* data out */

  } Bits;
  UINT32 Data;

} GLB_TXN_TRK_DATA_D2P_CWV_D_UNIT_0_STRUCT;


/* DDRT_DN_CFG_UNIMPL_LOG_CWV_D_UNIT_0_REG supported on:                        */
/*      CWV (0x40026778)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dnunit_csr                         */
/* Struct generated from CWV BDF: 0_4_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* The AHB address of the access that triggered an error.
*/


#define DDRT_DN_CFG_UNIMPL_LOG_CWV_D_UNIT_0_REG 0x1B240778

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vld : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Valid indicator, FW should write 1 to clear when
                               serviced.
                            */
    UINT32 rsvd : 4;

                            /* Bits[4:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 err_type : 2;

                            /* Bits[6:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Error Type 2:Unsuccesful 1: DDRT SAI Error 0:
                               PCR Violation.
                            */
    UINT32 write : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* Violation Type 1: Write 0: Read. */
    UINT32 offset : 16;

                            /* Bits[23:8], Access Type=RO/V, default=0x00000000*/

                            /* Bits [15:0] of the AHB Addrs */
    UINT32 base_addr : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* Bits [31:24] of the AHB Addrs violation */

  } Bits;
  UINT32 Data;

} DDRT_DN_CFG_UNIMPL_LOG_CWV_D_UNIT_0_STRUCT;
#endif /* _CWV_D_UNIT_0_h */
