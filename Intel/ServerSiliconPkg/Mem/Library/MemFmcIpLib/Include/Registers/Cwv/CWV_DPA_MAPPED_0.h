
/** @file
  CWV_DPA_MAPPED_0.h

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


#ifndef _CWV_DPA_MAPPED_0_h
#define _CWV_DPA_MAPPED_0_h
#include <DataTypes.h>

/* SCRATCH_0_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006000)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define SCRATCH_0_CWV_DPA_MAPPED_0_REG 0x1B040000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCRATCH_0_CWV_DPA_MAPPED_0_STRUCT;

/* SCRATCH_1_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006004)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define SCRATCH_1_CWV_DPA_MAPPED_0_REG 0x1B040004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCRATCH_1_CWV_DPA_MAPPED_0_STRUCT;

/* SCRATCH_2_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006008)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define SCRATCH_2_CWV_DPA_MAPPED_0_REG 0x1B040008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCRATCH_2_CWV_DPA_MAPPED_0_STRUCT;

/* SCRATCH_3_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x4000600c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define SCRATCH_3_CWV_DPA_MAPPED_0_REG 0x1B04000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCRATCH_3_CWV_DPA_MAPPED_0_STRUCT;

/* FW_REVID_CWV_DPA_MAPPED_0_REG supported on:                                  */
/*      CWV (0x40006020)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register is programmed with the Firmware Revision ID value
*/


#define FW_REVID_CWV_DPA_MAPPED_0_REG 0x1B040020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_REVID_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_0_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006030)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_0_CWV_DPA_MAPPED_0_REG 0x1B040030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_0_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_1_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006034)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_1_CWV_DPA_MAPPED_0_REG 0x1B040034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_1_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_2_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006038)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_2_CWV_DPA_MAPPED_0_REG 0x1B040038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_2_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_3_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x4000603c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_3_CWV_DPA_MAPPED_0_REG 0x1B04003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_3_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_4_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006040)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_4_CWV_DPA_MAPPED_0_REG 0x1B040040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_4_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_5_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006044)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_5_CWV_DPA_MAPPED_0_REG 0x1B040044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_5_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_6_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006048)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_6_CWV_DPA_MAPPED_0_REG 0x1B040048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_6_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_7_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x4000604c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_7_CWV_DPA_MAPPED_0_REG 0x1B04004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_7_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_8_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006050)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_8_CWV_DPA_MAPPED_0_REG 0x1B040050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_8_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_9_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006054)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_9_CWV_DPA_MAPPED_0_REG 0x1B040054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_9_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_10_CWV_DPA_MAPPED_0_REG supported on:                                */
/*      CWV (0x40006058)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_10_CWV_DPA_MAPPED_0_REG 0x1B040058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_10_CWV_DPA_MAPPED_0_STRUCT;

/* FW_MISC_11_CWV_DPA_MAPPED_0_REG supported on:                                */
/*      CWV (0x4000605c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These bits have no effect on the operation of Valley HW. They are intended to be available for use by FW for testing/debug, etc.
*/


#define FW_MISC_11_CWV_DPA_MAPPED_0_REG 0x1B04005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} FW_MISC_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_CMD_SPARE_CWV_DPA_MAPPED_0_REG supported on:                          */
/*      CWV (0x40006200)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_SMM_CMD_SPARE_CWV_DPA_MAPPED_0_REG 0x1B040200

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

} MB_SMM_CMD_SPARE_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_CMD_CWV_DPA_MAPPED_0_REG supported on:                                */
/*      CWV (0x40006204)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_SMM_CMD_CWV_DPA_MAPPED_0_REG 0x1B040204

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

} MB_SMM_CMD_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_0_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x40006208)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMM_NONCE_0_CWV_DPA_MAPPED_0_REG 0x1B040208

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

} MB_SMM_NONCE_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_1_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x4000620c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMM_NONCE_1_CWV_DPA_MAPPED_0_REG 0x1B04020C

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

} MB_SMM_NONCE_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_2_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x40006210)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMM_NONCE_2_CWV_DPA_MAPPED_0_REG 0x1B040210

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

} MB_SMM_NONCE_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_NONCE_3_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x40006214)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_SMM_NONCE_3_CWV_DPA_MAPPED_0_REG 0x1B040214

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

} MB_SMM_NONCE_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006218)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG 0x1B040218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000621c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG 0x1B04021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006220)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG 0x1B040220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006224)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG 0x1B040224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006228)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG 0x1B040228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000622c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG 0x1B04022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006230)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG 0x1B040230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006234)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG 0x1B040234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006238)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG 0x1B040238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000623c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG 0x1B04023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006240)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG 0x1B040240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006244)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG 0x1B040244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006248)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG 0x1B040248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000624c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG 0x1B04024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006250)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG 0x1B040250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006254)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG 0x1B040254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006258)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG 0x1B040258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000625c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG 0x1B04025C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006260)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG 0x1B040260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006264)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG 0x1B040264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006268)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG 0x1B040268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000626c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG 0x1B04026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006270)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG 0x1B040270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006274)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG 0x1B040274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006278)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG 0x1B040278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000627c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG 0x1B04027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006280)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG 0x1B040280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006284)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG 0x1B040284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006288)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG 0x1B040288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000628c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG 0x1B04028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006290)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG 0x1B040290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006294)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG 0x1B040294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_SMM_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_STATUS_CWV_DPA_MAPPED_0_REG supported on:                             */
/*      CWV (0x40006298)                                                        */
/* Register default value on CWV: 0x00000001                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_SMM_STATUS_CWV_DPA_MAPPED_0_REG 0x1B040298

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

} MB_SMM_STATUS_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_SPARE_STATUS_CWV_DPA_MAPPED_0_REG supported on:                       */
/*      CWV (0x4000629c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_SMM_SPARE_STATUS_CWV_DPA_MAPPED_0_REG 0x1B04029C

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

} MB_SMM_SPARE_STATUS_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062a0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG 0x1B0402A0

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

} MB_SMM_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062a4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG 0x1B0402A4

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

} MB_SMM_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062a8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG 0x1B0402A8

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

} MB_SMM_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062ac)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG 0x1B0402AC

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

} MB_SMM_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062b0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG 0x1B0402B0

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

} MB_SMM_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062b4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG 0x1B0402B4

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

} MB_SMM_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062b8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG 0x1B0402B8

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

} MB_SMM_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062bc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG 0x1B0402BC

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

} MB_SMM_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062c0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG 0x1B0402C0

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

} MB_SMM_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400062c4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG 0x1B0402C4

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

} MB_SMM_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062c8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG 0x1B0402C8

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

} MB_SMM_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062cc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG 0x1B0402CC

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

} MB_SMM_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062d0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG 0x1B0402D0

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

} MB_SMM_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG 0x1B0402D4

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

} MB_SMM_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062d8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG 0x1B0402D8

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

} MB_SMM_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062dc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG 0x1B0402DC

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

} MB_SMM_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062e0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG 0x1B0402E0

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

} MB_SMM_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062e4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG 0x1B0402E4

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

} MB_SMM_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062e8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG 0x1B0402E8

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

} MB_SMM_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062ec)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG 0x1B0402EC

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

} MB_SMM_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062f0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG 0x1B0402F0

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

} MB_SMM_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062f4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG 0x1B0402F4

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

} MB_SMM_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG 0x1B0402F8

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

} MB_SMM_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400062fc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG 0x1B0402FC

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

} MB_SMM_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006300)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG 0x1B040300

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

} MB_SMM_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006304)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG 0x1B040304

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

} MB_SMM_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006308)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG 0x1B040308

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

} MB_SMM_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x4000630c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG 0x1B04030C

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

} MB_SMM_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006310)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG 0x1B040310

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

} MB_SMM_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006314)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG 0x1B040314

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

} MB_SMM_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006318)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG 0x1B040318

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

} MB_SMM_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x4000631c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_SMM_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG 0x1B04031C

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

} MB_SMM_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_STRUCT;

/* MB_SMM_ABORT_CWV_DPA_MAPPED_0_REG supported on:                              */
/*      CWV (0x40006320)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MB_SMM_ABORT_CWV_DPA_MAPPED_0_REG 0x1B040320

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

} MB_SMM_ABORT_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_CMD_SPARE_CWV_DPA_MAPPED_0_REG supported on:                           */
/*      CWV (0x40006400)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_OS_CMD_SPARE_CWV_DPA_MAPPED_0_REG 0x1B040400

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

} MB_OS_CMD_SPARE_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_CMD_CWV_DPA_MAPPED_0_REG supported on:                                 */
/*      CWV (0x40006404)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_OS_CMD_CWV_DPA_MAPPED_0_REG 0x1B040404

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

} MB_OS_CMD_CWV_DPA_MAPPED_0_STRUCT;

/* SXEXTFLUSHSTATE_0_CWV_DPA_MAPPED_0_REG supported on:                         */
/*      CWV (0x40006408)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* CSR for BIOS/FW fADR and AD-WB use
*/


#define SXEXTFLUSHSTATE_0_CWV_DPA_MAPPED_0_REG 0x1B040408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sxextflushstatelow : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               To enable OSPM initiated cache-flush from
                               software triggered flows, the SxExtFlushState
                               (low) CSR is used for latching the extended
                               flush status. SxExtFlushState DPA address is DPA
                               0x100040.
                            */

  } Bits;
  UINT32 Data;

} SXEXTFLUSHSTATE_0_CWV_DPA_MAPPED_0_STRUCT;

/* SXEXTFLUSHSTATE_1_CWV_DPA_MAPPED_0_REG supported on:                         */
/*      CWV (0x4000640c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* CSR for BIOS/FW fADR and AD-WB use
*/


#define SXEXTFLUSHSTATE_1_CWV_DPA_MAPPED_0_REG 0x1B04040C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sxextflushstatehigh : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               To enable OSPM initiated cache-flush from
                               software triggered flows, the SxExtFlushState
                               (High) CSR is used for latching the extended
                               flush status. SxExtFlushState DPA address is DPA
                               0x100040.
                            */

  } Bits;
  UINT32 Data;

} SXEXTFLUSHSTATE_1_CWV_DPA_MAPPED_0_STRUCT;

/* EXTFLUSHSTATE_0_CWV_DPA_MAPPED_0_REG supported on:                           */
/*      CWV (0x40006410)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* CSR for BIOS/FW fADR and AD-WB use
*/


#define EXTFLUSHSTATE_0_CWV_DPA_MAPPED_0_REG 0x1B040410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extflushstatelow : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               CWV to sample FL[3:0] and capture into
                               ExtFlushState (low) (for fADR/AD-WB) for FW
                               access. ExtFlushState DPA address is DPA
                               0x100080.
                            */

  } Bits;
  UINT32 Data;

} EXTFLUSHSTATE_0_CWV_DPA_MAPPED_0_STRUCT;

/* EXTFLUSHSTATE_1_CWV_DPA_MAPPED_0_REG supported on:                           */
/*      CWV (0x40006414)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* CSR for BIOS/FW fADR and AD-WB use
*/


#define EXTFLUSHSTATE_1_CWV_DPA_MAPPED_0_REG 0x1B040414

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extflushstatehigh : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               CWV to sample FL[3:0] and capture into
                               ExtFlushState (high) (for fADR/AD-WB) for FW
                               access. ExtFlushState DPA address is DPA
                               0x100080.
                            */

  } Bits;
  UINT32 Data;

} EXTFLUSHSTATE_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006418)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG 0x1B040418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x4000641c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG 0x1B04041C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006420)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG 0x1B040420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006424)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG 0x1B040424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006428)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG 0x1B040428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x4000642c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG 0x1B04042C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006430)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG 0x1B040430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006434)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG 0x1B040434

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x40006438)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG 0x1B040438

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG supported on:                     */
/*      CWV (0x4000643c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG 0x1B04043C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006440)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG 0x1B040440

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006444)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG 0x1B040444

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006448)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG 0x1B040448

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000644c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG 0x1B04044C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006450)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG 0x1B040450

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006454)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG 0x1B040454

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006458)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG 0x1B040458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000645c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG 0x1B04045C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006460)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG 0x1B040460

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006464)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG 0x1B040464

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006468)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG 0x1B040468

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000646c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG 0x1B04046C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006470)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG 0x1B040470

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006474)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG 0x1B040474

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006478)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG 0x1B040478

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000647c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG 0x1B04047C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006480)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG 0x1B040480

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006484)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG 0x1B040484

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006488)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG 0x1B040488

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000648c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG 0x1B04048C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006490)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG 0x1B040490

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006494)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG 0x1B040494

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OS_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_STATUS_CWV_DPA_MAPPED_0_REG supported on:                              */
/*      CWV (0x40006498)                                                        */
/* Register default value on CWV: 0x00000001                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_OS_STATUS_CWV_DPA_MAPPED_0_REG 0x1B040498

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

} MB_OS_STATUS_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_SPARE_STATUS_CWV_DPA_MAPPED_0_REG supported on:                        */
/*      CWV (0x4000649c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_OS_SPARE_STATUS_CWV_DPA_MAPPED_0_REG 0x1B04049C

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

} MB_OS_SPARE_STATUS_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064a0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG 0x1B0404A0

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

} MB_OS_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064a4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG 0x1B0404A4

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

} MB_OS_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064a8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG 0x1B0404A8

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

} MB_OS_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064ac)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG 0x1B0404AC

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

} MB_OS_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064b0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG 0x1B0404B0

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

} MB_OS_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064b4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG 0x1B0404B4

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

} MB_OS_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064b8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG 0x1B0404B8

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

} MB_OS_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064bc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG 0x1B0404BC

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

} MB_OS_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064c0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG 0x1B0404C0

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

} MB_OS_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x400064c4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG 0x1B0404C4

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

} MB_OS_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064c8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG 0x1B0404C8

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

} MB_OS_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064cc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG 0x1B0404CC

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

} MB_OS_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064d0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG 0x1B0404D0

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

} MB_OS_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG 0x1B0404D4

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

} MB_OS_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064d8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG 0x1B0404D8

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

} MB_OS_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064dc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG 0x1B0404DC

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

} MB_OS_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064e0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG 0x1B0404E0

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

} MB_OS_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064e4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG 0x1B0404E4

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

} MB_OS_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064e8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG 0x1B0404E8

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

} MB_OS_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064ec)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG 0x1B0404EC

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

} MB_OS_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064f0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG 0x1B0404F0

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

} MB_OS_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064f4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG 0x1B0404F4

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

} MB_OS_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG 0x1B0404F8

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

} MB_OS_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400064fc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG 0x1B0404FC

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

} MB_OS_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006500)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG 0x1B040500

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

} MB_OS_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006504)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG 0x1B040504

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

} MB_OS_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006508)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG 0x1B040508

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

} MB_OS_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000650c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG 0x1B04050C

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

} MB_OS_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006510)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG 0x1B040510

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

} MB_OS_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006514)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG 0x1B040514

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

} MB_OS_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006518)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG 0x1B040518

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

} MB_OS_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000651c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OS_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG 0x1B04051C

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

} MB_OS_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OS_ABORT_CWV_DPA_MAPPED_0_REG supported on:                               */
/*      CWV (0x40006520)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MB_OS_ABORT_CWV_DPA_MAPPED_0_REG 0x1B040520

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

} MB_OS_ABORT_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_CMD_SPARE_CWV_DPA_MAPPED_0_REG supported on:                          */
/*      CWV (0x40006600)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_OOB_CMD_SPARE_CWV_DPA_MAPPED_0_REG 0x1B040600

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

} MB_OOB_CMD_SPARE_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_CMD_CWV_DPA_MAPPED_0_REG supported on:                                */
/*      CWV (0x40006604)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Command.
*/


#define MB_OOB_CMD_CWV_DPA_MAPPED_0_REG 0x1B040604

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

} MB_OOB_CMD_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_NONCE_0_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x40006608)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_OOB_NONCE_0_CWV_DPA_MAPPED_0_REG 0x1B040608

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

} MB_OOB_NONCE_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_NONCE_1_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x4000660c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_OOB_NONCE_1_CWV_DPA_MAPPED_0_REG 0x1B04060C

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

} MB_OOB_NONCE_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_NONCE_2_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x40006610)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_OOB_NONCE_2_CWV_DPA_MAPPED_0_REG 0x1B040610

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

} MB_OOB_NONCE_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_NONCE_3_CWV_DPA_MAPPED_0_REG supported on:                            */
/*      CWV (0x40006614)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Nonce.
*/


#define MB_OOB_NONCE_3_CWV_DPA_MAPPED_0_REG 0x1B040614

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

} MB_OOB_NONCE_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006618)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG 0x1B040618

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000661c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG 0x1B04061C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006620)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG 0x1B040620

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006624)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG 0x1B040624

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006628)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG 0x1B040628

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000662c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG 0x1B04062C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006630)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG 0x1B040630

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006634)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG 0x1B040634

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x40006638)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG 0x1B040638

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG supported on:                    */
/*      CWV (0x4000663c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG 0x1B04063C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006640)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG 0x1B040640

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006644)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG 0x1B040644

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006648)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG 0x1B040648

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000664c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG 0x1B04064C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006650)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG 0x1B040650

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006654)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG 0x1B040654

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006658)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG 0x1B040658

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000665c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG 0x1B04065C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006660)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG 0x1B040660

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006664)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG 0x1B040664

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006668)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG 0x1B040668

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000666c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG 0x1B04066C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006670)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG 0x1B040670

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006674)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG 0x1B040674

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006678)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG 0x1B040678

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000667c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG 0x1B04067C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006680)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG 0x1B040680

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006684)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG 0x1B040684

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006688)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG 0x1B040688

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x4000668c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG 0x1B04068C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006690)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG 0x1B040690

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x40006694)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG 0x1B040694

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 payload : 32;

                            /* Bits[31:0], Access Type=RW/L, default=0x00000000*/

                            /* FW MB Payload */

  } Bits;
  UINT32 Data;

} MB_OOB_INPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_STATUS_CWV_DPA_MAPPED_0_REG supported on:                             */
/*      CWV (0x40006698)                                                        */
/* Register default value on CWV: 0x00000001                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_OOB_STATUS_CWV_DPA_MAPPED_0_REG 0x1B040698

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

} MB_OOB_STATUS_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_SPARE_STATUS_CWV_DPA_MAPPED_0_REG supported on:                       */
/*      CWV (0x4000669c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the FW Mailbox Status.
*/


#define MB_OOB_SPARE_STATUS_CWV_DPA_MAPPED_0_REG 0x1B04069C

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

} MB_OOB_SPARE_STATUS_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066a0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_REG 0x1B0406A0

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

} MB_OOB_OUTPUT_PAYLOAD_0_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066a4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_REG 0x1B0406A4

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

} MB_OOB_OUTPUT_PAYLOAD_1_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066a8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_REG 0x1B0406A8

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

} MB_OOB_OUTPUT_PAYLOAD_2_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066ac)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_REG 0x1B0406AC

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

} MB_OOB_OUTPUT_PAYLOAD_3_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066b0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_REG 0x1B0406B0

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

} MB_OOB_OUTPUT_PAYLOAD_4_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066b4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_REG 0x1B0406B4

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

} MB_OOB_OUTPUT_PAYLOAD_5_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066b8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_REG 0x1B0406B8

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

} MB_OOB_OUTPUT_PAYLOAD_6_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066bc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_REG 0x1B0406BC

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

} MB_OOB_OUTPUT_PAYLOAD_7_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066c0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_REG 0x1B0406C0

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

} MB_OOB_OUTPUT_PAYLOAD_8_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG supported on:                   */
/*      CWV (0x400066c4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_REG 0x1B0406C4

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

} MB_OOB_OUTPUT_PAYLOAD_9_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066c8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_REG 0x1B0406C8

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

} MB_OOB_OUTPUT_PAYLOAD_10_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066cc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_REG 0x1B0406CC

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

} MB_OOB_OUTPUT_PAYLOAD_11_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066d0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_REG 0x1B0406D0

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

} MB_OOB_OUTPUT_PAYLOAD_12_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066d4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_REG 0x1B0406D4

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

} MB_OOB_OUTPUT_PAYLOAD_13_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066d8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_REG 0x1B0406D8

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

} MB_OOB_OUTPUT_PAYLOAD_14_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066dc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_REG 0x1B0406DC

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

} MB_OOB_OUTPUT_PAYLOAD_15_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066e0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_REG 0x1B0406E0

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

} MB_OOB_OUTPUT_PAYLOAD_16_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066e4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_REG 0x1B0406E4

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

} MB_OOB_OUTPUT_PAYLOAD_17_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066e8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_REG 0x1B0406E8

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

} MB_OOB_OUTPUT_PAYLOAD_18_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066ec)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_REG 0x1B0406EC

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

} MB_OOB_OUTPUT_PAYLOAD_19_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066f0)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_REG 0x1B0406F0

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

} MB_OOB_OUTPUT_PAYLOAD_20_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066f4)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_REG 0x1B0406F4

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

} MB_OOB_OUTPUT_PAYLOAD_21_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066f8)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_REG 0x1B0406F8

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

} MB_OOB_OUTPUT_PAYLOAD_22_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x400066fc)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_REG 0x1B0406FC

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

} MB_OOB_OUTPUT_PAYLOAD_23_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006700)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_REG 0x1B040700

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

} MB_OOB_OUTPUT_PAYLOAD_24_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006704)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_REG 0x1B040704

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

} MB_OOB_OUTPUT_PAYLOAD_25_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006708)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_REG 0x1B040708

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

} MB_OOB_OUTPUT_PAYLOAD_26_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x4000670c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_REG 0x1B04070C

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

} MB_OOB_OUTPUT_PAYLOAD_27_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006710)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_REG 0x1B040710

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

} MB_OOB_OUTPUT_PAYLOAD_28_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006714)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_REG 0x1B040714

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

} MB_OOB_OUTPUT_PAYLOAD_29_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x40006718)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_REG 0x1B040718

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

} MB_OOB_OUTPUT_PAYLOAD_30_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG supported on:                  */
/*      CWV (0x4000671c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* These registers contains the opcode specific FW Mailbox Short Payload.
*/


#define MB_OOB_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_REG 0x1B04071C

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

} MB_OOB_OUTPUT_PAYLOAD_31_CWV_DPA_MAPPED_0_STRUCT;

/* MB_OOB_ABORT_CWV_DPA_MAPPED_0_REG supported on:                              */
/*      CWV (0x40006720)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_glb/dsmunit_fwmb_csr                   */
/* Struct generated from CWV BDF: 0_0_6                                         */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MB_OOB_ABORT_CWV_DPA_MAPPED_0_REG 0x1B040720

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

} MB_OOB_ABORT_CWV_DPA_MAPPED_0_STRUCT;
#endif /* _CWV_DPA_MAPPED_0_h */
