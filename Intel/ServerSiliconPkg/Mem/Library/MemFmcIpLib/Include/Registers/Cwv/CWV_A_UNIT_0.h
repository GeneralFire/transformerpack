
/** @file
  CWV_A_UNIT_0.h

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


#ifndef _CWV_A_UNIT_0_h
#define _CWV_A_UNIT_0_h
#include <DataTypes.h>














































































/* A_PERM_ERR_LOG_CWV_A_UNIT_0_REG supported on:                                */
/*      CWV (0x40036140)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br] register stores which AHB CSR endpoint saw a CSR access violation
*/


#define A_PERM_ERR_LOG_CWV_A_UNIT_0_REG 0x1B440140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 a_csr_vld : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw a PCR access
                               violation
                            */
    UINT32 a_csr_write : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the CSR
                               Access Violation.[br] 1'b1 - write[br] 1'b0 -
                               read
                            */
    UINT32 a_csr_unimpl_vld : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access to
                               an unimplemented register
                            */
    UINT32 cpgc_csr_vld : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CHN CPGC CSR endpoint saw an
                               access violation
                            */
    UINT32 cpgc_csr_write : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the CHN
                               CPGC CSR Access Violation.[br] 1'b1 - write[br]
                               1'b0 - read
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} A_PERM_ERR_LOG_CWV_A_UNIT_0_STRUCT;




































/* A_FFW_DIGEST1_CSR_0_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362a0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_0_CWV_A_UNIT_0_REG 0x1B4402A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_0_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_1_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362a4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_1_CWV_A_UNIT_0_REG 0x1B4402A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_1_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_2_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362a8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_2_CWV_A_UNIT_0_REG 0x1B4402A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_2_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_3_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362ac)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_3_CWV_A_UNIT_0_REG 0x1B4402AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_3_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_4_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362b0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_4_CWV_A_UNIT_0_REG 0x1B4402B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_4_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_5_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362b4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_5_CWV_A_UNIT_0_REG 0x1B4402B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_5_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_6_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362b8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_6_CWV_A_UNIT_0_REG 0x1B4402B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_6_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_7_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362bc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_7_CWV_A_UNIT_0_REG 0x1B4402BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_7_CWV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_8_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362c0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_8_CWV_A_UNIT_0_REG 0x1B4402C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_8_CWV_A_UNIT_0_STRUCT;



/* A_FFW_DIGEST1_CSR_9_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362c4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_9_CWV_A_UNIT_0_REG 0x1B4402C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_9_CWV_A_UNIT_0_STRUCT;



/* A_FFW_DIGEST1_CSR_10_CWV_A_UNIT_0_REG supported on:                          */
/*      CWV (0x400362c8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_10_CWV_A_UNIT_0_REG 0x1B4402C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_10_CWV_A_UNIT_0_STRUCT;



/* A_FFW_DIGEST1_CSR_11_CWV_A_UNIT_0_REG supported on:                          */
/*      CWV (0x400362cc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST1_CSR_11_CWV_A_UNIT_0_REG 0x1B4402CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_11_CWV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_0_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362d0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_0_CWV_A_UNIT_0_REG 0x1B4402D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_0_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_1_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_1_CWV_A_UNIT_0_REG 0x1B4402D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_1_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_2_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362d8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_2_CWV_A_UNIT_0_REG 0x1B4402D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_2_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_3_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362dc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_3_CWV_A_UNIT_0_REG 0x1B4402DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_3_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_4_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362e0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_4_CWV_A_UNIT_0_REG 0x1B4402E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_4_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_5_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362e4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_5_CWV_A_UNIT_0_REG 0x1B4402E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_5_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_6_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362e8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_6_CWV_A_UNIT_0_REG 0x1B4402E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_6_CWV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_7_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362ec)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_7_CWV_A_UNIT_0_REG 0x1B4402EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_7_CWV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_8_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362f0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_8_CWV_A_UNIT_0_REG 0x1B4402F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_8_CWV_A_UNIT_0_STRUCT;



/* A_FFW_DIGEST_CSR_9_CWV_A_UNIT_0_REG supported on:                            */
/*      CWV (0x400362f4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_9_CWV_A_UNIT_0_REG 0x1B4402F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_9_CWV_A_UNIT_0_STRUCT;



/* A_FFW_DIGEST_CSR_10_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_10_CWV_A_UNIT_0_REG 0x1B4402F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_10_CWV_A_UNIT_0_STRUCT;



/* A_FFW_DIGEST_CSR_11_CWV_A_UNIT_0_REG supported on:                           */
/*      CWV (0x400362fc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These write once CSRs hold a FFW digest (384 bit HASH value) that is computed by the ROM code during FFW image verification.  ROM writes the 384 bit hash value ONLY for PRQ type FFW. For other FFW types (DBG, DFX, RAW) ROM code does write or lock digest CSRs.   See ROM Code UG for additional details.
*/


#define A_FFW_DIGEST_CSR_11_CWV_A_UNIT_0_REG 0x1B4402FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               These write once CSRs hold a FFW digest (384 bit
                               HASH value) that is computed by the ROM code
                               during FFW image verification. ROM writes the
                               384 bit hash value ONLY for PRQ type FFW. For
                               other FFW types (DBG, DFX, RAW) ROM code does
                               write or lock digest CSRs. See ROM Code UG for
                               additional details.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_11_CWV_A_UNIT_0_STRUCT;



/* A_DRLD_START_IDX_CWV_A_UNIT_0_REG supported on:                              */
/*      CWV (0x40036300)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/aunit_csr                          */
/* Struct generated from CWV BDF: 0_6_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Starting DRAM Index to load.
*/


#define A_DRLD_START_IDX_CWV_A_UNIT_0_REG 0x1B440300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 idx : 26;

                            /* Bits[25:0], Access Type=RW, default=0x00000000*/

                            /*
                               Starting DRAM Index to load. Each index
                               corresponds to 8 AIT Entries.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} A_DRLD_START_IDX_CWV_A_UNIT_0_STRUCT;




















































































































































































#endif /* _CWV_A_UNIT_0_h */
