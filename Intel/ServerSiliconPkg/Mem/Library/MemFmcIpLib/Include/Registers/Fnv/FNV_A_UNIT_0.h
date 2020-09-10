
/** @file
  FNV_A_UNIT_0.h

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


#ifndef _FNV_A_UNIT_0_h
#define _FNV_A_UNIT_0_h
#include "DataTypes.h"
















































































































/* A_FFW_DIGEST1_CSR_0_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063c0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_0_FNV_A_UNIT_0_REG 0x1B6403C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_0_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_1_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063c4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_1_FNV_A_UNIT_0_REG 0x1B6403C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_1_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_2_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063c8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_2_FNV_A_UNIT_0_REG 0x1B6403C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_2_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_3_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063cc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_3_FNV_A_UNIT_0_REG 0x1B6403CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_3_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_4_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063d0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_4_FNV_A_UNIT_0_REG 0x1B6403D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_4_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_5_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063d4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_5_FNV_A_UNIT_0_REG 0x1B6403D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_5_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST1_CSR_6_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063d8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_6_FNV_A_UNIT_0_REG 0x1B6403D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_6_FNV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST1_CSR_7_FNV_A_UNIT_0_REG supported on:                           */
/*      BWV (0x400063dc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold additional functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle. (CCB 135761)
*/


#define A_FFW_DIGEST1_CSR_7_FNV_A_UNIT_0_REG 0x1B6403DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest1 : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold additional functional FW digest
                               (256 bit HASH value) and are updated by ROM code
                               for each boot cycle. (CCB 135761)
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST1_CSR_7_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_0_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063e0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_0_FNV_A_UNIT_0_REG 0x1B6403E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_0_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_1_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063e4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_1_FNV_A_UNIT_0_REG 0x1B6403E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_1_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_2_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063e8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_2_FNV_A_UNIT_0_REG 0x1B6403E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_2_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_3_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063ec)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_3_FNV_A_UNIT_0_REG 0x1B6403EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_3_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_4_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063f0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_4_FNV_A_UNIT_0_REG 0x1B6403F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_4_FNV_A_UNIT_0_STRUCT;


/* A_FFW_DIGEST_CSR_5_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063f4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_5_FNV_A_UNIT_0_REG 0x1B6403F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_5_FNV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_6_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063f8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_6_FNV_A_UNIT_0_REG 0x1B6403F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_6_FNV_A_UNIT_0_STRUCT;

/* A_FFW_DIGEST_CSR_7_FNV_A_UNIT_0_REG supported on:                            */
/*      BWV (0x400063fc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_6_BWV                                              */
/* Struct generated from BWV BDF: 0_0_6                                         */
/* BWV Security PolicyGroup:                                                    */
/* These CSRs hold functional FW digest (256 bit HASH value) and are updated by ROM code for each boot cycle.
*/


#define A_FFW_DIGEST_CSR_7_FNV_A_UNIT_0_REG 0x1B6403FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 digest : 32;

                            /* Bits[31:0], Access Type=RWO, default=0x00000000*/

                            /*
                               These CSRs hold functional FW digest (256 bit
                               HASH value) and are updated by ROM code for each
                               boot cycle.
                            */

  } Bits;
  UINT32 Data;

} A_FFW_DIGEST_CSR_7_FNV_A_UNIT_0_STRUCT;







































































































































































































































































































#endif /* _FNV_A_UNIT_0_h */
