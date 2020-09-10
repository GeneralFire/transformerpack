
/** @file
  FNV_DPA_MAPPED_0.h

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


#ifndef _FNV_DPA_MAPPED_0_h
#define _FNV_DPA_MAPPED_0_h
#include "DataTypes.h"







/* MB_SMM_CMD_FNV_DPA_MAPPED_0_REG supported on:                                */
/*      BWV (0x40000204)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_SMM_CMD_FNV_DPA_MAPPED_0_REG 0x1B040204

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

} MB_SMM_CMD_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_0_FNV_DPA_MAPPED_0_REG supported on:                            */
/*      BWV (0x40000208)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMM_NONCE_0_FNV_DPA_MAPPED_0_REG 0x1B040208

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

} MB_SMM_NONCE_0_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_1_FNV_DPA_MAPPED_0_REG supported on:                            */
/*      BWV (0x4000020c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMM_NONCE_1_FNV_DPA_MAPPED_0_REG 0x1B04020C

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

} MB_SMM_NONCE_1_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_2_FNV_DPA_MAPPED_0_REG supported on:                            */
/*      BWV (0x40000210)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reserved for future use.
*/


#define MB_SMM_NONCE_2_FNV_DPA_MAPPED_0_REG 0x1B040210

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

} MB_SMM_NONCE_2_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_3_FNV_DPA_MAPPED_0_REG supported on:                            */
/*      BWV (0x40000214)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reserved for future use.
*/


#define MB_SMM_NONCE_3_FNV_DPA_MAPPED_0_REG 0x1B040214

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

} MB_SMM_NONCE_3_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000218)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG 0x1B040218

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

} MB_SMM_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000021c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG 0x1B04021C

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

} MB_SMM_INPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000220)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG 0x1B040220

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

} MB_SMM_INPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000224)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG 0x1B040224

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

} MB_SMM_INPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000228)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG 0x1B040228

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

} MB_SMM_INPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000022c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG 0x1B04022C

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

} MB_SMM_INPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000230)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG 0x1B040230

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

} MB_SMM_INPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000234)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG 0x1B040234

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

} MB_SMM_INPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000238)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG 0x1B040238

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

} MB_SMM_INPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000023c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG 0x1B04023C

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

} MB_SMM_INPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000240)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG 0x1B040240

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

} MB_SMM_INPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000244)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG 0x1B040244

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

} MB_SMM_INPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000248)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG 0x1B040248

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

} MB_SMM_INPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000024c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG 0x1B04024C

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

} MB_SMM_INPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000250)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG 0x1B040250

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

} MB_SMM_INPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000254)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG 0x1B040254

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

} MB_SMM_INPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000258)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG 0x1B040258

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

} MB_SMM_INPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000025c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG 0x1B04025C

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

} MB_SMM_INPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000260)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG 0x1B040260

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

} MB_SMM_INPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000264)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG 0x1B040264

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

} MB_SMM_INPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000268)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG 0x1B040268

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

} MB_SMM_INPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000026c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG 0x1B04026C

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

} MB_SMM_INPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000270)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG 0x1B040270

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

} MB_SMM_INPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000274)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG 0x1B040274

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

} MB_SMM_INPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000278)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG 0x1B040278

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

} MB_SMM_INPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000027c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG 0x1B04027C

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

} MB_SMM_INPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000280)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG 0x1B040280

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

} MB_SMM_INPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000284)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG 0x1B040284

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

} MB_SMM_INPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000288)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG 0x1B040288

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

} MB_SMM_INPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000028c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG 0x1B04028C

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

} MB_SMM_INPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000290)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG 0x1B040290

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

} MB_SMM_INPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000294)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG 0x1B040294

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

} MB_SMM_INPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_STATUS_FNV_DPA_MAPPED_0_REG supported on:                             */
/*      BWV (0x40000298)                                                        */
/* Register default value on BWV: 0x00000001                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_SMM_STATUS_FNV_DPA_MAPPED_0_REG 0x1B040298

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

} MB_SMM_STATUS_FNV_DPA_MAPPED_0_STRUCT;


/* MB_SMM_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002a0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG 0x1B0402A0

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

} MB_SMM_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002a4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG 0x1B0402A4

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

} MB_SMM_OUTPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002a8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG 0x1B0402A8

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

} MB_SMM_OUTPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002ac)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG 0x1B0402AC

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

} MB_SMM_OUTPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002b0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG 0x1B0402B0

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

} MB_SMM_OUTPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002b4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG 0x1B0402B4

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

} MB_SMM_OUTPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002b8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG 0x1B0402B8

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

} MB_SMM_OUTPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002bc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG 0x1B0402BC

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

} MB_SMM_OUTPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002c0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG 0x1B0402C0

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

} MB_SMM_OUTPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400002c4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG 0x1B0402C4

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

} MB_SMM_OUTPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002c8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG 0x1B0402C8

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

} MB_SMM_OUTPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002cc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG 0x1B0402CC

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

} MB_SMM_OUTPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002d0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG 0x1B0402D0

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

} MB_SMM_OUTPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002d4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG 0x1B0402D4

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

} MB_SMM_OUTPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002d8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG 0x1B0402D8

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

} MB_SMM_OUTPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002dc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG 0x1B0402DC

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

} MB_SMM_OUTPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002e0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG 0x1B0402E0

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

} MB_SMM_OUTPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002e4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG 0x1B0402E4

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

} MB_SMM_OUTPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002e8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG 0x1B0402E8

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

} MB_SMM_OUTPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002ec)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG 0x1B0402EC

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

} MB_SMM_OUTPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002f0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG 0x1B0402F0

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

} MB_SMM_OUTPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002f4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG 0x1B0402F4

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

} MB_SMM_OUTPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002f8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG 0x1B0402F8

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

} MB_SMM_OUTPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x400002fc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG 0x1B0402FC

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

} MB_SMM_OUTPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x40000300)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG 0x1B040300

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

} MB_SMM_OUTPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x40000304)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG 0x1B040304

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

} MB_SMM_OUTPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x40000308)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG 0x1B040308

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

} MB_SMM_OUTPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x4000030c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG 0x1B04030C

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

} MB_SMM_OUTPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x40000310)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG 0x1B040310

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

} MB_SMM_OUTPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x40000314)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG 0x1B040314

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

} MB_SMM_OUTPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x40000318)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG 0x1B040318

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

} MB_SMM_OUTPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG supported on:                  */
/*      BWV (0x4000031c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG 0x1B04031C

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

} MB_SMM_OUTPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_ABORT_FNV_DPA_MAPPED_0_REG supported on:                              */
/*      BWV (0x40000320)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Abort.
*/


#define MB_SMM_ABORT_FNV_DPA_MAPPED_0_REG 0x1B040320

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

} MB_SMM_ABORT_FNV_DPA_MAPPED_0_STRUCT;


/* MB_OS_CMD_FNV_DPA_MAPPED_0_REG supported on:                                 */
/*      BWV (0x40000404)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_OS_CMD_FNV_DPA_MAPPED_0_REG 0x1B040404

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

} MB_OS_CMD_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_NONCE_0_FNV_DPA_MAPPED_0_REG supported on:                             */
/*      BWV (0x40000408)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_OS_NONCE_0_FNV_DPA_MAPPED_0_REG 0x1B040408

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

} MB_OS_NONCE_0_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_NONCE_1_FNV_DPA_MAPPED_0_REG supported on:                             */
/*      BWV (0x4000040c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_OS_NONCE_1_FNV_DPA_MAPPED_0_REG 0x1B04040C

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

} MB_OS_NONCE_1_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_NONCE_2_FNV_DPA_MAPPED_0_REG supported on:                             */
/*      BWV (0x40000410)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reserved for future use.
*/


#define MB_OS_NONCE_2_FNV_DPA_MAPPED_0_REG 0x1B040410

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

} MB_OS_NONCE_2_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_NONCE_3_FNV_DPA_MAPPED_0_REG supported on:                             */
/*      BWV (0x40000414)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* Reserved for future use.
*/


#define MB_OS_NONCE_3_FNV_DPA_MAPPED_0_REG 0x1B040414

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

} MB_OS_NONCE_3_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000418)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG 0x1B040418

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

} MB_OS_INPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x4000041c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG 0x1B04041C

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

} MB_OS_INPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000420)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG 0x1B040420

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

} MB_OS_INPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000424)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG 0x1B040424

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

} MB_OS_INPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000428)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG 0x1B040428

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

} MB_OS_INPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x4000042c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG 0x1B04042C

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

} MB_OS_INPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000430)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG 0x1B040430

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

} MB_OS_INPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000434)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG 0x1B040434

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

} MB_OS_INPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x40000438)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG 0x1B040438

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

} MB_OS_INPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG supported on:                     */
/*      BWV (0x4000043c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG 0x1B04043C

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

} MB_OS_INPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000440)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG 0x1B040440

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

} MB_OS_INPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000444)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG 0x1B040444

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

} MB_OS_INPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000448)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG 0x1B040448

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

} MB_OS_INPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000044c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG 0x1B04044C

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

} MB_OS_INPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000450)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG 0x1B040450

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

} MB_OS_INPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000454)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG 0x1B040454

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

} MB_OS_INPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000458)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG 0x1B040458

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

} MB_OS_INPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000045c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG 0x1B04045C

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

} MB_OS_INPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000460)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG 0x1B040460

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

} MB_OS_INPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000464)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG 0x1B040464

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

} MB_OS_INPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000468)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG 0x1B040468

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

} MB_OS_INPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000046c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG 0x1B04046C

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

} MB_OS_INPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000470)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG 0x1B040470

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

} MB_OS_INPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000474)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG 0x1B040474

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

} MB_OS_INPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000478)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG 0x1B040478

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

} MB_OS_INPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000047c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG 0x1B04047C

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

} MB_OS_INPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000480)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG 0x1B040480

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

} MB_OS_INPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000484)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG 0x1B040484

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

} MB_OS_INPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000488)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG 0x1B040488

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

} MB_OS_INPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x4000048c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG 0x1B04048C

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

} MB_OS_INPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000490)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG 0x1B040490

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

} MB_OS_INPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x40000494)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG 0x1B040494

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

} MB_OS_INPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_STATUS_FNV_DPA_MAPPED_0_REG supported on:                              */
/*      BWV (0x40000498)                                                        */
/* Register default value on BWV: 0x00000001                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_OS_STATUS_FNV_DPA_MAPPED_0_REG 0x1B040498

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

} MB_OS_STATUS_FNV_DPA_MAPPED_0_STRUCT;


/* MB_OS_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004a0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_REG 0x1B0404A0

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

} MB_OS_OUTPUT_PAYLOAD_0_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004a4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_REG 0x1B0404A4

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

} MB_OS_OUTPUT_PAYLOAD_1_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004a8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_REG 0x1B0404A8

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

} MB_OS_OUTPUT_PAYLOAD_2_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004ac)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_REG 0x1B0404AC

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

} MB_OS_OUTPUT_PAYLOAD_3_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004b0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_REG 0x1B0404B0

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

} MB_OS_OUTPUT_PAYLOAD_4_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004b4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_REG 0x1B0404B4

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

} MB_OS_OUTPUT_PAYLOAD_5_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004b8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_REG 0x1B0404B8

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

} MB_OS_OUTPUT_PAYLOAD_6_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004bc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_REG 0x1B0404BC

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

} MB_OS_OUTPUT_PAYLOAD_7_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004c0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_REG 0x1B0404C0

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

} MB_OS_OUTPUT_PAYLOAD_8_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG supported on:                    */
/*      BWV (0x400004c4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_REG 0x1B0404C4

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

} MB_OS_OUTPUT_PAYLOAD_9_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004c8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_REG 0x1B0404C8

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

} MB_OS_OUTPUT_PAYLOAD_10_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004cc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_REG 0x1B0404CC

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

} MB_OS_OUTPUT_PAYLOAD_11_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004d0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_REG 0x1B0404D0

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

} MB_OS_OUTPUT_PAYLOAD_12_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004d4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_REG 0x1B0404D4

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

} MB_OS_OUTPUT_PAYLOAD_13_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004d8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_REG 0x1B0404D8

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

} MB_OS_OUTPUT_PAYLOAD_14_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004dc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_REG 0x1B0404DC

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

} MB_OS_OUTPUT_PAYLOAD_15_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004e0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_REG 0x1B0404E0

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

} MB_OS_OUTPUT_PAYLOAD_16_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004e4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_REG 0x1B0404E4

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

} MB_OS_OUTPUT_PAYLOAD_17_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004e8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_REG 0x1B0404E8

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

} MB_OS_OUTPUT_PAYLOAD_18_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004ec)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_REG 0x1B0404EC

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

} MB_OS_OUTPUT_PAYLOAD_19_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004f0)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_REG 0x1B0404F0

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

} MB_OS_OUTPUT_PAYLOAD_20_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004f4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_REG 0x1B0404F4

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

} MB_OS_OUTPUT_PAYLOAD_21_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004f8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_REG 0x1B0404F8

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

} MB_OS_OUTPUT_PAYLOAD_22_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x400004fc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_REG 0x1B0404FC

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

} MB_OS_OUTPUT_PAYLOAD_23_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000500)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_REG 0x1B040500

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

} MB_OS_OUTPUT_PAYLOAD_24_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000504)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_REG 0x1B040504

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

} MB_OS_OUTPUT_PAYLOAD_25_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000508)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_REG 0x1B040508

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

} MB_OS_OUTPUT_PAYLOAD_26_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000050c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_REG 0x1B04050C

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

} MB_OS_OUTPUT_PAYLOAD_27_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000510)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_REG 0x1B040510

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

} MB_OS_OUTPUT_PAYLOAD_28_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000514)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_REG 0x1B040514

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

} MB_OS_OUTPUT_PAYLOAD_29_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x40000518)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_REG 0x1B040518

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

} MB_OS_OUTPUT_PAYLOAD_30_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG supported on:                   */
/*      BWV (0x4000051c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_REG 0x1B04051C

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

} MB_OS_OUTPUT_PAYLOAD_31_FNV_DPA_MAPPED_0_STRUCT;

/* MB_OS_ABORT_FNV_DPA_MAPPED_0_REG supported on:                               */
/*      BWV (0x40000520)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_0_BWV                                              */
/* Struct generated from BWV BDF: 0_0_0                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Abort.
*/


#define MB_OS_ABORT_FNV_DPA_MAPPED_0_REG 0x1B040520

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

} MB_OS_ABORT_FNV_DPA_MAPPED_0_STRUCT;






#endif /* _FNV_DPA_MAPPED_0_h */
