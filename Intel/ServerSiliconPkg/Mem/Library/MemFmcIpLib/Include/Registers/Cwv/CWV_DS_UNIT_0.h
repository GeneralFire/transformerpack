
/** @file
  CWV_DS_UNIT_0.h

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


#ifndef _CWV_DS_UNIT_0_h
#define _CWV_DS_UNIT_0_h
#include <DataTypes.h>

/* MB_SMBUS_CMD_SPARE_CWV_DS_UNIT_0_REG supported on:                           */
/*      CWV (0x4002e000)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_SMBUS_CMD_SPARE_CWV_DS_UNIT_0_REG 0x1B340000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spare : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* See FIS for details. */

  } Bits;
  UINT32 Data;

} MB_SMBUS_CMD_SPARE_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_CMD_CWV_DS_UNIT_0_REG supported on:                                 */
/*      CWV (0x4002e004)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_SMBUS_CMD_CWV_DS_UNIT_0_REG 0x1B340004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 opcode : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               FW MB OPCODE. Refer to FIS for detailed
                               explaination.
                            */
    UINT32 doorbell : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Doorbell */
    UINT32 spares : 15;

                            /* Bits[31:17], Access Type=RW/P, default=0x00000000*/

                            /* Refer to FIS for details */

  } Bits;
  UINT32 Data;

} MB_SMBUS_CMD_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_0_CWV_DS_UNIT_0_REG supported on:                             */
/*      CWV (0x4002e008)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMBUS_NONCE_0_CWV_DS_UNIT_0_REG 0x1B340008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_0_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_1_CWV_DS_UNIT_0_REG supported on:                             */
/*      CWV (0x4002e00c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMBUS_NONCE_1_CWV_DS_UNIT_0_REG 0x1B34000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_1_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_2_CWV_DS_UNIT_0_REG supported on:                             */
/*      CWV (0x4002e010)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMBUS_NONCE_2_CWV_DS_UNIT_0_REG 0x1B340010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_2_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_NONCE_3_CWV_DS_UNIT_0_REG supported on:                             */
/*      CWV (0x4002e014)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMBUS_NONCE_3_CWV_DS_UNIT_0_REG 0x1B340014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nonce : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Firmware Mailbox Nonce */

  } Bits;
  UINT32 Data;

} MB_SMBUS_NONCE_3_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e018)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG 0x1B340018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_0_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_1_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e01c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_1_CWV_DS_UNIT_0_REG 0x1B34001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_1_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_2_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e020)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_2_CWV_DS_UNIT_0_REG 0x1B340020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_2_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_3_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e024)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_3_CWV_DS_UNIT_0_REG 0x1B340024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_3_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_4_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e028)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_4_CWV_DS_UNIT_0_REG 0x1B340028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_4_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_5_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e02c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_5_CWV_DS_UNIT_0_REG 0x1B34002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_5_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_6_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e030)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_6_CWV_DS_UNIT_0_REG 0x1B340030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_6_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_7_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e034)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_7_CWV_DS_UNIT_0_REG 0x1B340034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_7_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_8_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e038)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_8_CWV_DS_UNIT_0_REG 0x1B340038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_8_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_9_CWV_DS_UNIT_0_REG supported on:                     */
/*      CWV (0x4002e03c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_9_CWV_DS_UNIT_0_REG 0x1B34003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_9_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_10_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e040)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_10_CWV_DS_UNIT_0_REG 0x1B340040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_10_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_11_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e044)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_11_CWV_DS_UNIT_0_REG 0x1B340044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_11_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_12_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e048)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_12_CWV_DS_UNIT_0_REG 0x1B340048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_12_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_13_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e04c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_13_CWV_DS_UNIT_0_REG 0x1B34004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_13_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_14_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e050)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_14_CWV_DS_UNIT_0_REG 0x1B340050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_14_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_15_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e054)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_15_CWV_DS_UNIT_0_REG 0x1B340054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_15_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_16_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e058)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_16_CWV_DS_UNIT_0_REG 0x1B340058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_16_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_17_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e05c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_17_CWV_DS_UNIT_0_REG 0x1B34005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_17_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_18_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e060)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_18_CWV_DS_UNIT_0_REG 0x1B340060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_18_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_19_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e064)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_19_CWV_DS_UNIT_0_REG 0x1B340064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_19_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_20_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e068)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_20_CWV_DS_UNIT_0_REG 0x1B340068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_20_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_21_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e06c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_21_CWV_DS_UNIT_0_REG 0x1B34006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_21_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_22_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e070)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_22_CWV_DS_UNIT_0_REG 0x1B340070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_22_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_23_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e074)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_23_CWV_DS_UNIT_0_REG 0x1B340074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_23_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_24_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e078)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_24_CWV_DS_UNIT_0_REG 0x1B340078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_24_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_25_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e07c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_25_CWV_DS_UNIT_0_REG 0x1B34007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_25_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_26_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e080)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_26_CWV_DS_UNIT_0_REG 0x1B340080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_26_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_27_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e084)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_27_CWV_DS_UNIT_0_REG 0x1B340084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_27_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_28_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e088)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_28_CWV_DS_UNIT_0_REG 0x1B340088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_28_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_29_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e08c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_29_CWV_DS_UNIT_0_REG 0x1B34008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_29_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_30_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e090)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_30_CWV_DS_UNIT_0_REG 0x1B340090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_30_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_INPUT_PAYLOAD_31_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e094)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_INPUT_PAYLOAD_31_CWV_DS_UNIT_0_REG 0x1B340094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_INPUT_PAYLOAD_31_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_STATUS_CWV_DS_UNIT_0_REG supported on:                              */
/*      CWV (0x4002e098)                                                        */
/* Register default value on CWV: 0x00000001                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_SMBUS_STATUS_CWV_DS_UNIT_0_REG 0x1B340098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000001*/

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
    UINT32 spare1 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* See FIS for details. */

  } Bits;
  UINT32 Data;

} MB_SMBUS_STATUS_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_SPARE_STATUS_CWV_DS_UNIT_0_REG supported on:                        */
/*      CWV (0x4002e09c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_SMBUS_SPARE_STATUS_CWV_DS_UNIT_0_REG 0x1B34009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 status : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* See FIS for details. */

  } Bits;
  UINT32 Data;

} MB_SMBUS_SPARE_STATUS_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0a0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG 0x1B3400A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_0_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_1_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0a4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_1_CWV_DS_UNIT_0_REG 0x1B3400A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_1_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_2_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0a8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_2_CWV_DS_UNIT_0_REG 0x1B3400A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_2_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_3_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0ac)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_3_CWV_DS_UNIT_0_REG 0x1B3400AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_3_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_4_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0b0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_4_CWV_DS_UNIT_0_REG 0x1B3400B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_4_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_5_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0b4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_5_CWV_DS_UNIT_0_REG 0x1B3400B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_5_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_6_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0b8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_6_CWV_DS_UNIT_0_REG 0x1B3400B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_6_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_7_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0bc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_7_CWV_DS_UNIT_0_REG 0x1B3400BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_7_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_8_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0c0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_8_CWV_DS_UNIT_0_REG 0x1B3400C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_8_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_9_CWV_DS_UNIT_0_REG supported on:                    */
/*      CWV (0x4002e0c4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_9_CWV_DS_UNIT_0_REG 0x1B3400C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_9_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_10_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0c8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_10_CWV_DS_UNIT_0_REG 0x1B3400C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_10_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_11_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0cc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_11_CWV_DS_UNIT_0_REG 0x1B3400CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_11_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_12_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0d0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_12_CWV_DS_UNIT_0_REG 0x1B3400D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_12_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_13_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_13_CWV_DS_UNIT_0_REG 0x1B3400D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_13_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_14_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0d8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_14_CWV_DS_UNIT_0_REG 0x1B3400D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_14_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_15_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0dc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_15_CWV_DS_UNIT_0_REG 0x1B3400DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_15_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_16_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0e0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_16_CWV_DS_UNIT_0_REG 0x1B3400E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_16_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_17_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0e4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_17_CWV_DS_UNIT_0_REG 0x1B3400E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_17_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_18_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0e8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_18_CWV_DS_UNIT_0_REG 0x1B3400E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_18_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_19_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0ec)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_19_CWV_DS_UNIT_0_REG 0x1B3400EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_19_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_20_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0f0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_20_CWV_DS_UNIT_0_REG 0x1B3400F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_20_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_21_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0f4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_21_CWV_DS_UNIT_0_REG 0x1B3400F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_21_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_22_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_22_CWV_DS_UNIT_0_REG 0x1B3400F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_22_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_23_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e0fc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_23_CWV_DS_UNIT_0_REG 0x1B3400FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_23_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_24_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e100)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_24_CWV_DS_UNIT_0_REG 0x1B340100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_24_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_25_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e104)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_25_CWV_DS_UNIT_0_REG 0x1B340104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_25_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_26_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e108)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_26_CWV_DS_UNIT_0_REG 0x1B340108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_26_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_27_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e10c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_27_CWV_DS_UNIT_0_REG 0x1B34010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_27_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_28_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e110)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_28_CWV_DS_UNIT_0_REG 0x1B340110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_28_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_29_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e114)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_29_CWV_DS_UNIT_0_REG 0x1B340114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_29_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_30_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e118)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_30_CWV_DS_UNIT_0_REG 0x1B340118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_30_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_OUTPUT_PAYLOAD_31_CWV_DS_UNIT_0_REG supported on:                   */
/*      CWV (0x4002e11c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMBUS_OUTPUT_PAYLOAD_31_CWV_DS_UNIT_0_REG 0x1B34011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMBUS_OUTPUT_PAYLOAD_31_CWV_DS_UNIT_0_STRUCT;

/* MB_SMBUS_ABORT_CWV_DS_UNIT_0_REG supported on:                               */
/*      CWV (0x4002e120)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MB_SMBUS_ABORT_CWV_DS_UNIT_0_REG 0x1B340120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 abort : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MB_SMBUS_ABORT_CWV_DS_UNIT_0_STRUCT;

/* DS_DDRT_TRAINING_EN_CWV_DS_UNIT_0_REG supported on:                          */
/*      CWV (0x4002e128)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Training  Enable
*/


#define DS_DDRT_TRAINING_EN_CWV_DS_UNIT_0_REG 0x1B340128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tm_en : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:DDRT Training Mode Enable (not
                               used) [br] Setting this bit will block write
                               data from sending to memory
                            */
    UINT32 early_cmd_ck_trn : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:EARLY CMD-CK Training Enable
                               [br] Parity only, commands are not written into
                               ingress command queue
                            */
    UINT32 adv_cmd_ck_trn : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:Advanced CMD-CK Training
                               Enable[br] MRS, extended MRS and Memory Mapped
                               config access are ignored
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DS_DDRT_TRAINING_EN_CWV_DS_UNIT_0_STRUCT;







































/* DS_PERM_ERR_LOG_CWV_DS_UNIT_0_REG supported on:                              */
/*      CWV (0x4002e1d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsunit_csr                         */
/* Struct generated from CWV BDF: 0_5_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br] This register stores which AHB CSR endpoint saw a CSR access violation
*/


#define DS_PERM_ERR_LOG_CWV_DS_UNIT_0_REG 0x1B3401D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ds_csr_vld : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access
                               violation
                            */
    UINT32 ds_csr_write : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:The transaction type of the
                               access that caused the error.[br] 1'b1 -
                               write[br] 1'b0 - read
                            */
    UINT32 ds_csr_unimpl_vld : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               TODO: Depricated:CSR endpoint saw an access to
                               an unimplemented CSR
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DS_PERM_ERR_LOG_CWV_DS_UNIT_0_STRUCT;










































































#endif /* _CWV_DS_UNIT_0_h */
