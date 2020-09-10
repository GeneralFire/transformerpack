
/** @file
  CWV_DA_UNIT_0.h

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


#ifndef _CWV_DA_UNIT_0_h
#define _CWV_DA_UNIT_0_h
#include <DataTypes.h>


/* DA_ECC_ENABLE_CWV_DA_UNIT_0_REG supported on:                                */
/*      CWV (0x4000e010)                                                        */
/* Register default value on CWV: 0x00001F00                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Write ECC Control
*/


#define DA_ECC_ENABLE_CWV_DA_UNIT_0_REG 0x1B140010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecc_rd_chk_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable read ECC generation[br] 0 = ecc check
                               disabled[br] 1 = ecc check enabled
                            */
    UINT32 ecc_wr_chk_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable write ECC generation[br] 0 = ecc check
                               disabled[br] 1 = ecc check enabled
                            */
    UINT32 ecc_corr_en : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Write ECC Correction[br] 0 = ecc
                               correction disabled.[br] 1 = ecc correction
                               enabled.[br] ECC Correction must be enabled to
                               run Flow B. [br] ECC Correcation can be disabled
                               to run Flow A. [br]
                            */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 trnreseterrexit_en : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               Enable assertion of DDRT IO Train Reset on DDRT
                               Err Exit for progammable number of dclks[br] 0 =
                               disabled.[br] 1 = enabled. [br]
                            */
    UINT32 trnreseterrexit_cntr_termval : 7;

                            /* Bits[15:9], Access Type=RW/P, default=0x0000000F*/

                            /*
                               Determines the pulse length of DDRT IO Train
                               Reset on DDRT Err Exit for progammable number of
                               dclks[br] default - 16 dclks .[br]
                            */
    UINT32 rsvd_16 : 13;

                            /* Bits[28:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errflow_a_alwayscorr : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               With this bit set, HW will always correct Write
                               ECC Error in Flow A, if error is correctable[br]
                               0 = WECC Flow A Always Correct disabled[br] 1 =
                               WECC Flow A Always Correct enabled[br] Note**
                               Field is Deprecated. Do not change value from
                               default
                            */
    UINT32 ecc_poison_en : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               With this bit set, HW will poison on WECC
                               Correction Fail[br] 0 = WECC Poison disabled[br]
                               1 = WECC Poison enabled
                            */
    UINT32 errflow_a_exitcorr : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Write ECC Flow A Correction on Retry Flow
                               Exit[br] 0 = ecc Flow A correction on retry flow
                               exit disabled[br] 1 = ecc Flow A correction on
                               retry flow exit enabled
                            */

  } Bits;
  UINT32 Data;

} DA_ECC_ENABLE_CWV_DA_UNIT_0_STRUCT;





/* DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG supported on:                          */
/*      CWV (0x4000e030)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Training  Enable
*/


#define DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG 0x1B140030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               DDRT Training Mode Enable [br] Setting this bit
                               will block read and write command from sending
                               to memory
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               EARLY CMD-CK Training Enable [br] Parity only,
                               commands are not written into ingress command
                               queue
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Advanced CMD-CK Training Enable[br] MRS,
                               extended MRS and Memory Mapped config access are
                               ignored
                            */
    UINT32 rsvd : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_rid_feedback : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, the RID bits are no longer returned
                               within the data packet,[br] allowing for all
                               data in the buffer to be returned as it was
                               written
                            */
    UINT32 rsvd_6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_MPR_CWV_DA_UNIT_0_REG supported on:                         */
/*      CWV (0x4000e034)                                                        */
/* Register default value on CWV: 0x000F3355                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* MPR Training Pattern
*/


#define DA_DDRT_TRAINING_MPR_CWV_DA_UNIT_0_REG 0x1B140034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr0 : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000055*/

                            /* MPR training pattern 0 */
    UINT32 mpr1 : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000033*/

                            /* MPR training pattern 1 */
    UINT32 mpr2 : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x0000000F*/

                            /* MPR training pattern 2 */
    UINT32 mpr3 : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /* MPR training pattern 3 */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_MPR_CWV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_MR3_CWV_DA_UNIT_0_REG supported on:                         */
/*      CWV (0x4000e038)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register mimics DDR4 SRAM Mode Registers.
*/


#define DA_DDRT_TRAINING_MR3_CWV_DA_UNIT_0_REG 0x1B140038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpr_sel : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               MPR page selection[br] 0x0 : Select MPR0[br] 0x1
                               : Select MPR1[br] 0x2 : Select MPR2[br] 0x3 :
                               Select MPR3
                            */
    UINT32 mpr_op : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               MPR operation[br] 0 : normal[br] 1 : data flow
                               from/to MPR
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 per_dram_addr_ability : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd_5 : 6;

                            /* Bits[10:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mpr_rd_frmt : 2;

                            /* Bits[12:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               MPR read format; Valley Controller supports both
                               serial and parallel MPR read format 00 = Serial
                               [br] 01 = Parallel [br] others = reserved
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_MR3_CWV_DA_UNIT_0_STRUCT;

/* DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_REG supported on:                    */
/*      CWV (0x4000e03c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Other Training  Enable
*/


#define DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_REG 0x1B14003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_pche_inv_set0_only : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               SubChannel A- When this bit is asserted, DUNIT
                               Prefetch Cache will invalidate SET 0 Only.
                               Hardware will clear CSR, once done. Use this CSR
                               right after Training. Prefetch SET 0 is used for
                               training and must be invalidated when switching
                               to normal mode.
                            */

    UINT32 sa_pche_inv_allsets : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               SubChannel A- When this bit is asserted, DUNIT
                               Prefetch Cache will invalidate first 32 entries
                               only. Hardware will clear CSR, once done. Use
                               this CSR in Normal Operation or DFX Mode.
                            */

    UINT32 sb_pche_inv_set0_only : 1;

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /*
                               SubChannel B- When this bit is asserted, DUNIT
                               Prefetch Cache will invalidate SET 0 Only.
                               Hardware will clear CSR, once done. Use this CSR
                               right after Training. Prefetch SET 0 is used for
                               training and must be invalidated when switching
                               to normal mode.
                            */

    UINT32 sb_pche_inv_allsets : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*
                               SubChannel B- When this bit is asserted, DUNIT
                               Prefetch Cache will invalidate first 32 entries
                               only. Hardware will clear CSR, once done. Use
                               this CSR in Normal Operation or DFX Mode.
                            */

    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_STRUCT;


/* DA_READ_CREDIT_CWV_DA_UNIT_0_REG supported on:                               */
/*      CWV (0x4000e044)                                                        */
/* Register default value on CWV: 0x00000030                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the default value of read credits. Will be loaded when value of CSR changes
*/


#define DA_READ_CREDIT_CWV_DA_UNIT_0_REG 0x1B140044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 val : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000030*/

                            /* Value of Read credits. Default value is 48 */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_READ_CREDIT_CWV_DA_UNIT_0_STRUCT;

/* DA_CRD_CNT_CWV_DA_UNIT_0_REG supported on:                                   */
/*      CWV (0x4000e048)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Indicates status of write and read credit counts
*/


#define DA_CRD_CNT_CWV_DA_UNIT_0_REG 0x1B140048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sb_rcrd_cnt : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Indicates the number of read credits free within
                               Valley Controller
                            */
    UINT32 sa_rcrd_cnt : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               Indicates the number of read credits free within
                               Valley Controller
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_CRD_CNT_CWV_DA_UNIT_0_STRUCT;







/* DA_WCRD_CNT_CWV_DA_UNIT_0_REG supported on:                                  */
/*      CWV (0x4000e098)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Indicates status of write credit availability
*/


#define DA_WCRD_CNT_CWV_DA_UNIT_0_REG 0x1B140098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wcrd_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Indicates the number of write credits available
                               in Valley Controller
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WCRD_CNT_CWV_DA_UNIT_0_STRUCT;

/* DA_WRITE_CREDIT_CWV_DA_UNIT_0_REG supported on:                              */
/*      CWV (0x4000e09c)                                                        */
/* Register default value on CWV: 0x00000080                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/daunit_cclk_csr                    */
/* Struct generated from CWV BDF: 0_1_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the default value of write credits. Will be loaded when value of CSR changes
*/


#define DA_WRITE_CREDIT_CWV_DA_UNIT_0_REG 0x1B14009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 val : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000080*/

                            /*
                               Value of write credits. Default value is 80. Max
                               is 80.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_WRITE_CREDIT_CWV_DA_UNIT_0_STRUCT;























































#endif /* _CWV_DA_UNIT_0_h */
