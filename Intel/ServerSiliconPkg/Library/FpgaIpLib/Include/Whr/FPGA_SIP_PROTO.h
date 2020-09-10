
/** @file
  FPGA_SIP_PROTO.h

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

/* WHR Security Policy Groups:                                                  */


#ifndef _FPGA_SIP_PROTO_h
#define _FPGA_SIP_PROTO_h
#include <Base.h>

/* PCIE_00_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000000)                                                        */
/* Register default value on WHR: 0xBCC68086                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Device ID and Vendor ID
*/


#define PCIE_00_FPGA_SIP_PROTO_REG 0x1A420000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vendorid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* Vendor ID */
    UINT32 deviceid : 16;

                            /* Bits[31:16], Access Type=RO, default=0x0000BCC6*/

                            /* Device ID */

  } Bits;
  UINT32 Data;

} PCIE_00_FPGA_SIP_PROTO_STRUCT;

/* PCIE_04_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000004)                                                        */
/* Register default value on WHR: 0x00100000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Status and Command
*/


#define PCIE_04_FPGA_SIP_PROTO_REG 0x1A420004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 command : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* Command */
    UINT32 status : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000010*/

                            /* Status */

  } Bits;
  UINT32 Data;

} PCIE_04_FPGA_SIP_PROTO_STRUCT;

/* PCIE_08_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000008)                                                        */
/* Register default value on WHR: 0x08800000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Class code and Revision ID
*/


#define PCIE_08_FPGA_SIP_PROTO_REG 0x1A420008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revisionid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Revision ID */
    UINT32 classcode : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00088000*/

                            /* Class code */

  } Bits;
  UINT32 Data;

} PCIE_08_FPGA_SIP_PROTO_STRUCT;

/* PCIE_0C_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x2000000c)                                                        */
/* Register default value on WHR: 0x00000010                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Latency Timer and CL size
*/


#define PCIE_0C_FPGA_SIP_PROTO_REG 0x1A42000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clsize : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /* Cache line size */
    UINT32 latencytimer : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Latency Timer */
    UINT32 headertype : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Header Type */
    UINT32 bist : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* BIST */

  } Bits;
  UINT32 Data;

} PCIE_0C_FPGA_SIP_PROTO_STRUCT;

/* PCIE_10_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000010)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #0
*/


#define PCIE_10_FPGA_SIP_PROTO_REG 0x1A420010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bar0 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address Register #0 */

  } Bits;
  UINT32 Data;

} PCIE_10_FPGA_SIP_PROTO_STRUCT;

/* PCIE_14_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000014)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #1
*/


#define PCIE_14_FPGA_SIP_PROTO_REG 0x1A420014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bar1 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address Register #1 */

  } Bits;
  UINT32 Data;

} PCIE_14_FPGA_SIP_PROTO_STRUCT;

/* PCIE_18_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000018)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #2
*/


#define PCIE_18_FPGA_SIP_PROTO_REG 0x1A420018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bar2 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address Register #2 */

  } Bits;
  UINT32 Data;

} PCIE_18_FPGA_SIP_PROTO_STRUCT;

/* PCIE_1C_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x2000001c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #3
*/


#define PCIE_1C_FPGA_SIP_PROTO_REG 0x1A42001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bar3 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address Register #3 */

  } Bits;
  UINT32 Data;

} PCIE_1C_FPGA_SIP_PROTO_STRUCT;

/* PCIE_20_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000020)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #4
*/


#define PCIE_20_FPGA_SIP_PROTO_REG 0x1A420020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bar4 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address Register #4 */

  } Bits;
  UINT32 Data;

} PCIE_20_FPGA_SIP_PROTO_STRUCT;

/* PCIE_24_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000024)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #5
*/


#define PCIE_24_FPGA_SIP_PROTO_REG 0x1A420024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bar5 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Base Address Register #5 */

  } Bits;
  UINT32 Data;

} PCIE_24_FPGA_SIP_PROTO_STRUCT;

/* PCIE_28_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000028)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Cardbus CIS Pointer
*/


#define PCIE_28_FPGA_SIP_PROTO_REG 0x1A420028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cardbuspntr : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Cardbus CIS Pointer */

  } Bits;
  UINT32 Data;

} PCIE_28_FPGA_SIP_PROTO_STRUCT;

/* PCIE_2C_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x2000002c)                                                        */
/* Register default value on WHR: 0x00008086                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Subsystem and Subsystem Vendor ID
*/


#define PCIE_2C_FPGA_SIP_PROTO_REG 0x1A42002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 subsystemvendorid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* Subsystem Vendor ID */
    UINT32 subsystemid : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Subsystem ID */

  } Bits;
  UINT32 Data;

} PCIE_2C_FPGA_SIP_PROTO_STRUCT;

/* PCIE_30_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000030)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Expansion ROM base address
*/


#define PCIE_30_FPGA_SIP_PROTO_REG 0x1A420030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 exprombar : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Expansion ROM base address */

  } Bits;
  UINT32 Data;

} PCIE_30_FPGA_SIP_PROTO_STRUCT;

/* PCIE_34_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000034)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Capabilities pointer
*/


#define PCIE_34_FPGA_SIP_PROTO_REG 0x1A420034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capabpntr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Capabilities pointer */
    UINT32 reserved8 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCIE_34_FPGA_SIP_PROTO_STRUCT;

/* PCIE_38_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000038)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE_Reserved
*/


#define PCIE_38_FPGA_SIP_PROTO_REG 0x1A420038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PCIE_38_FPGA_SIP_PROTO_STRUCT;

/* PCIE_3C_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x2000003c)                                                        */
/* Register default value on WHR: 0x00000100                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Latency and Interrupt
*/


#define PCIE_3C_FPGA_SIP_PROTO_REG 0x1A42003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 interruptline : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Interrupt Line */
    UINT32 interruptpin : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000001*/

                            /* Interrupt Pin */
    UINT32 mingnt : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Minimum Grant */
    UINT32 maxlat : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Maximum Latency */

  } Bits;
  UINT32 Data;

} PCIE_3C_FPGA_SIP_PROTO_STRUCT;

/* WHR_BUSNO_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x20000104)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* WHR Bus number configuration
*/


#define WHR_BUSNO_FPGA_SIP_PROTO_REG 0x1A420104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 busnumb0 : 13;

                            /* Bits[12:0], Access Type=RWSL, default=0x00000000*/

                            /* WHR base bus/device decode */
    UINT32 reserved13 : 18;

                            /* Bits[30:13], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 busnumbprog : 1;

                            /* Bits[31:31], Access Type=RWSL, default=0x00000000*/

                            /* Set by BIOS when BusNumb0 is valid */

  } Bits;
  UINT32 Data;

} WHR_BUSNO_FPGA_SIP_PROTO_STRUCT;

/* KTI_SCRATCH0_FPGA_SIP_PROTO_REG supported on:                                */
/*      WHR (0x20000210)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI scratchpad register 0
*/


#define KTI_SCRATCH0_FPGA_SIP_PROTO_REG 0x1A420210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratchpad : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Scratchpad register 0 */

  } Bits;
  UINT32 Data;

} KTI_SCRATCH0_FPGA_SIP_PROTO_STRUCT;

/* KTI_SCRATCH1_FPGA_SIP_PROTO_REG supported on:                                */
/*      WHR (0x20000214)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI scratchpad register 1
*/


#define KTI_SCRATCH1_FPGA_SIP_PROTO_REG 0x1A420214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratchpad : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Scratchpad register 1 */

  } Bits;
  UINT32 Data;

} KTI_SCRATCH1_FPGA_SIP_PROTO_STRUCT;

/* KTI_STKYSCRATCH0_FPGA_SIP_PROTO_REG supported on:                            */
/*      WHR (0x20000218)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI sticky scratchpad register 0
*/


#define KTI_STKYSCRATCH0_FPGA_SIP_PROTO_REG 0x1A420218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stickyscratchpad : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Sticky scratchpad register 0 */

  } Bits;
  UINT32 Data;

} KTI_STKYSCRATCH0_FPGA_SIP_PROTO_STRUCT;

/* KTI_STKYSCRATCH1_FPGA_SIP_PROTO_REG supported on:                            */
/*      WHR (0x2000021c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI sticky scratchpad register 1
*/


#define KTI_STKYSCRATCH1_FPGA_SIP_PROTO_REG 0x1A42021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stickyscratchpad : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Sticky scratchpad register 1 */

  } Bits;
  UINT32 Data;

} KTI_STKYSCRATCH1_FPGA_SIP_PROTO_STRUCT;

/* PEI_LOCK_FPGA_SIP_PROTO_REG supported on:                                    */
/*      WHR (0x2000022c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* PEI Lock
*/


#define PEI_LOCK_FPGA_SIP_PROTO_REG 0x1A42022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 peilock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Controls write access to PEI region locked
                               registers (1 - Lock, 0 - Unlock). Once this
                               register written, it gets locked. Warm-reset
                               clears the lock.
                            */
    UINT32 reserved1 : 31;

                            /* Bits[31:1], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PEI_LOCK_FPGA_SIP_PROTO_STRUCT;

/* DRAM_RULE0_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000230)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Coherency configuration control 0
*/


#define DRAM_RULE0_FPGA_SIP_PROTO_REG 0x1A420230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ruleenable : 1;

                            /* Bits[0:0], Access Type=RWSL, default=0x00000000*/

                            /* 0, disable this rule 1, enable this rule */
    UINT32 reserved1 : 1;

                            /* Bits[1:1], Access Type=RsvdP, default=0x00000001*/

                            /*
                               Reserved (Xeon Interleave_Mode, unsupported on
                               FPGA. FPGA only interleaves on address bits
                               {8,7,6})
                            */
    UINT32 reserved2 : 2;

                            /* Bits[3:2], Access Type=RsvdP, default=0x00000000*/

                            /*
                               Reserved (Xeon Attr, unsupported on FPGA. FPGA
                               only generates transactions to DRAM, not MMCFG
                               space)
                            */
    UINT32 reserved4 : 3;

                            /* Bits[6:4], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 addrlimit : 20;

                            /* Bits[26:7], Access Type=RWSL, default=0x00000000*/

                            /*
                               This field corresponds to ADDR[45:26] of DRAM
                               top address limit (Lower limit is 0)
                            */
    UINT32 reserved27 : 5;

                            /* Bits[31:27], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE0_FPGA_SIP_PROTO_STRUCT;

/* INTERLEAVE_LIST0_FPGA_SIP_PROTO_REG supported on:                            */
/*      WHR (0x20000234)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* DRAM Interleave list 0
*/


#define INTERLEAVE_LIST0_FPGA_SIP_PROTO_REG 0x1A420234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST0_FPGA_SIP_PROTO_STRUCT;

/* DRAM_RULE1_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000238)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Coherency configuration control 1
*/


#define DRAM_RULE1_FPGA_SIP_PROTO_REG 0x1A420238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ruleenable : 1;

                            /* Bits[0:0], Access Type=RWSL, default=0x00000000*/

                            /* 0, disable this rule 1, enable this rule */
    UINT32 reserved1 : 1;

                            /* Bits[1:1], Access Type=RsvdP, default=0x00000001*/

                            /*
                               Reserved (Xeon Interleave_Mode, unsupported on
                               FPGA. FPGA only interleaves on address bits
                               {8,7,6})
                            */
    UINT32 reserved2 : 2;

                            /* Bits[3:2], Access Type=RsvdP, default=0x00000000*/

                            /*
                               Reserved (Xeon Attr, unsupported on FPGA. FPGA
                               only generates transactions to DRAM, not MMCFG
                               space)
                            */
    UINT32 reserved4 : 3;

                            /* Bits[6:4], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 addrlimit : 20;

                            /* Bits[26:7], Access Type=RWSL, default=0x00000000*/

                            /*
                               This field corresponds to ADDR[45:26] of DRAM
                               top address limit. Must be strictly greater than
                               previous rule even if this rule is disabled.
                            */
    UINT32 reserved27 : 5;

                            /* Bits[31:27], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RULE1_FPGA_SIP_PROTO_STRUCT;

/* INTERLEAVE_LIST1_FPGA_SIP_PROTO_REG supported on:                            */
/*      WHR (0x2000023c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* DRAM Interleave list 1
*/


#define INTERLEAVE_LIST1_FPGA_SIP_PROTO_REG 0x1A42023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 package0 : 4;

                            /* Bits[3:0], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package1 : 4;

                            /* Bits[7:4], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package2 : 4;

                            /* Bits[11:8], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package3 : 4;

                            /* Bits[15:12], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package4 : 4;

                            /* Bits[19:16], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package5 : 4;

                            /* Bits[23:20], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package6 : 4;

                            /* Bits[27:24], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */
    UINT32 package7 : 4;

                            /* Bits[31:28], Access Type=RWSL, default=0x00000000*/

                            /* Node ID of interleave list target */

  } Bits;
  UINT32 Data;

} INTERLEAVE_LIST1_FPGA_SIP_PROTO_STRUCT;

/* FPGA_BUSNO_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000240)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* FPGA Bus number configuration
*/


#define FPGA_BUSNO_FPGA_SIP_PROTO_REG 0x1A420240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 busnumb0 : 8;

                            /* Bits[7:0], Access Type=RWSL, default=0x00000000*/

                            /*
                               FPGA base bus decode (FPGA only decodes one
                               segment - 32 busses)
                            */
    UINT32 reserved8 : 23;

                            /* Bits[30:8], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 busnumbprog : 1;

                            /* Bits[31:31], Access Type=RWSL, default=0x00000000*/

                            /* Set by BIOS when BusNumb0 is valid */

  } Bits;
  UINT32 Data;

} FPGA_BUSNO_FPGA_SIP_PROTO_STRUCT;

/* INT_APIC_ID_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x20000270)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Interrupt APICID
*/


#define INT_APIC_ID_FPGA_SIP_PROTO_REG 0x1A420270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 31;

                            /* Bits[30:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 busnumbprog : 1;

                            /* Bits[31:31], Access Type=RW-L, default=0x00000000*/

                            /* Interrupt APICID */

  } Bits;
  UINT32 Data;

} INT_APIC_ID_FPGA_SIP_PROTO_STRUCT;

/* INT_CONTROL_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x20000274)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Interrupt Control
*/


#define INT_CONTROL_FPGA_SIP_PROTO_REG 0x1A420274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 interruptvector : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Interrupt Vector */
    UINT32 triggermode : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* 0, Edge Triggered 1, Level Triggered */
    UINT32 level : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Only for Level triggered Interrupts 0, deassert
                               1, assert
                            */
    UINT32 deliverymode : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /* 0, Directed or Fixed 1, Redirectable 2, SMI */
    UINT32 destinationmode : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* 0, Logical 1, Physical */
    UINT32 redirectionhint : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* 0, directed 1, redirectable */
    UINT32 injectinterrupt : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Write 1 to inject an interrupt over UPI. Clear
                               it before injecting one more interrupt
                            */
    UINT32 reserved16 : 16;

                            /* Bits[31:16], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} INT_CONTROL_FPGA_SIP_PROTO_STRUCT;

/* CIPUCTL_FPGA_SIP_PROTO_REG supported on:                                     */
/*      WHR (0x20000278)                                                        */
/* Register default value on WHR: 0x00000008                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* CCI PU control register
*/


#define CIPUCTL_FPGA_SIP_PROTO_REG 0x1A420278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enautoflush : 1;

                            /* Bits[0:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Enable cache auto flush for Snoop Assist */
    UINT32 flushinterval : 8;

                            /* Bits[8:1], Access Type=RsvdZ, default=0x00000004*/

                            /*
                               Number of waiting clock cycles for cache flush
                               interval
                            */
    UINT32 reserved9 : 23;

                            /* Bits[31:9], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CIPUCTL_FPGA_SIP_PROTO_STRUCT;

/* CIPUSTAT0_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x2000027c)                                                        */
/* Register default value on WHR: 0xC0001702                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* CCI PU-RD status (even channel)
*/


#define CIPUSTAT0_FPGA_SIP_PROTO_REG 0x1A42027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 1;

                            /* Bits[0:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 tginitdone : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /* Tag initialization done */
    UINT32 retryreqt4 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Retry request in T4 */
    UINT32 rbstallrxwr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* RB requested a WR stall */
    UINT32 rbstallrxrd : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* RB requested a RD stall */
    UINT32 reserved5 : 3;

                            /* Bits[7:5], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 txreqdone : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /* TX request done */
    UINT32 tagwrdone : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /* TAG write done */
    UINT32 cachewrdone : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /* Cache write done */
    UINT32 wrfencepend : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* WrFence pending to be completed */
    UINT32 taglookupdone : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /* TAG lookup done */
    UINT32 stallt4 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T4 */
    UINT32 stallt3 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T3 */
    UINT32 stallt2 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T2 */
    UINT32 stallt1 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T1 */
    UINT32 sreqt4 : 3;

                            /* Bits[19:17], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT4[2:0] */
    UINT32 sreqt3 : 3;

                            /* Bits[22:20], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT3[2:0] */
    UINT32 sreqt2 : 3;

                            /* Bits[25:23], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT2[2:0] */
    UINT32 sreqt1 : 3;

                            /* Bits[28:26], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT1[2:0] */
    UINT32 puhdrvalid : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /* Header valid */
    UINT32 allwrreqcomp : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /* All WR requests completed */
    UINT32 allreqcomp : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /* All requests in CCI completed */

  } Bits;
  UINT32 Data;

} CIPUSTAT0_FPGA_SIP_PROTO_STRUCT;

/* CIPUSTAT1_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x20000280)                                                        */
/* Register default value on WHR: 0xC0001702                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* CCI PU-WR status (even channel)
*/


#define CIPUSTAT1_FPGA_SIP_PROTO_REG 0x1A420280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 1;

                            /* Bits[0:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 tginitdone : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /* Tag initialization done */
    UINT32 retryreqt4 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Retry request in T4 */
    UINT32 rbstallrxwr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* RB requested a WR stall */
    UINT32 rbstallrxrd : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* RB requested a RD stall */
    UINT32 chflushdone : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Cache flush done */
    UINT32 reserved6 : 2;

                            /* Bits[7:6], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 txreqdone : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /* TX request done */
    UINT32 tagwrdone : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /* TAG write done */
    UINT32 cachewrdone : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /* Cache write done */
    UINT32 wrfencepend : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* WrFence pending to be completed */
    UINT32 taglookupdone : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /* TAG lookup done */
    UINT32 stallt4 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T4 */
    UINT32 stallt3 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T3 */
    UINT32 stallt2 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T2 */
    UINT32 stallt1 : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* pipeline stall in T1 */
    UINT32 sreqt4 : 3;

                            /* Bits[19:17], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT4[2:0] */
    UINT32 sreqt3 : 3;

                            /* Bits[22:20], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT3[2:0] */
    UINT32 sreqt2 : 3;

                            /* Bits[25:23], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT2[2:0] */
    UINT32 sreqt1 : 3;

                            /* Bits[28:26], Access Type=RO, default=0x00000000*/

                            /* pipeline sreqT1[2:0] */
    UINT32 puhdrvalid : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /* Header valid */
    UINT32 allwrreqcomp : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000001*/

                            /* All WR requests completed */
    UINT32 allreqcomp : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /* All requests in CCI completed */

  } Bits;
  UINT32 Data;

} CIPUSTAT1_FPGA_SIP_PROTO_STRUCT;

/* CIPUSTAT2_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x20000284)                                                        */
/* Register default value on WHR: 0xC0001702                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* CCI PU-RD status (odd channel) (Reserved when SINGLE_CHANNEL)
*/


#define CIPUSTAT2_FPGA_SIP_PROTO_REG 0x1A420284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 1;

                            /* Bits[0:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 tginitdone : 1;

                            /* Bits[1:1], Access Type=RsvdZ, default=0x00000001*/

                            /* Tag initialization done */
    UINT32 retryreqt4 : 1;

                            /* Bits[2:2], Access Type=RsvdZ, default=0x00000000*/

                            /* Retry request in T4 */
    UINT32 rbstallrxwr : 1;

                            /* Bits[3:3], Access Type=RsvdZ, default=0x00000000*/

                            /* RB requested a WR stall */
    UINT32 rbstallrxrd : 1;

                            /* Bits[4:4], Access Type=RsvdZ, default=0x00000000*/

                            /* RB requested a RD stall */
    UINT32 chflushdone : 1;

                            /* Bits[5:5], Access Type=RsvdZ, default=0x00000000*/

                            /* Cache flush done */
    UINT32 reserved6 : 2;

                            /* Bits[7:6], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 txreqdone : 1;

                            /* Bits[8:8], Access Type=RsvdZ, default=0x00000001*/

                            /* TX request done */
    UINT32 tagwrdone : 1;

                            /* Bits[9:9], Access Type=RsvdZ, default=0x00000001*/

                            /* TAG write done */
    UINT32 cachewrdone : 1;

                            /* Bits[10:10], Access Type=RsvdZ, default=0x00000001*/

                            /* Cache write done */
    UINT32 wrfencepend : 1;

                            /* Bits[11:11], Access Type=RsvdZ, default=0x00000000*/

                            /* WrFence pending to be completed */
    UINT32 taglookupdone : 1;

                            /* Bits[12:12], Access Type=RsvdZ, default=0x00000001*/

                            /* TAG lookup done */
    UINT32 stallt4 : 1;

                            /* Bits[13:13], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T4 */
    UINT32 stallt3 : 1;

                            /* Bits[14:14], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T3 */
    UINT32 stallt2 : 1;

                            /* Bits[15:15], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T2 */
    UINT32 stallt1 : 1;

                            /* Bits[16:16], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T1 */
    UINT32 sreqt4 : 3;

                            /* Bits[19:17], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT4[2:0] */
    UINT32 sreqt3 : 3;

                            /* Bits[22:20], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT3[2:0] */
    UINT32 sreqt2 : 3;

                            /* Bits[25:23], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT2[2:0] */
    UINT32 sreqt1 : 3;

                            /* Bits[28:26], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT1[2:0] */
    UINT32 puhdrvalid : 1;

                            /* Bits[29:29], Access Type=RsvdZ, default=0x00000000*/

                            /* Header valid */
    UINT32 allwrreqcomp : 1;

                            /* Bits[30:30], Access Type=RsvdZ, default=0x00000001*/

                            /* All WR requests completed */
    UINT32 allreqcomp : 1;

                            /* Bits[31:31], Access Type=RsvdZ, default=0x00000001*/

                            /* All requests in CCI completed */

  } Bits;
  UINT32 Data;

} CIPUSTAT2_FPGA_SIP_PROTO_STRUCT;

/* CIPUSTAT3_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x20000288)                                                        */
/* Register default value on WHR: 0xC0001702                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* CCI PU-WR status (odd channel) (Reserved when SINGLE_CHANNEL)
*/


#define CIPUSTAT3_FPGA_SIP_PROTO_REG 0x1A420288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 1;

                            /* Bits[0:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 tginitdone : 1;

                            /* Bits[1:1], Access Type=RsvdZ, default=0x00000001*/

                            /* Tag initialization done */
    UINT32 retryreqt4 : 1;

                            /* Bits[2:2], Access Type=RsvdZ, default=0x00000000*/

                            /* Retry request in T4 */
    UINT32 rbstallrxwr : 1;

                            /* Bits[3:3], Access Type=RsvdZ, default=0x00000000*/

                            /* RB requested a WR stall */
    UINT32 rbstallrxrd : 1;

                            /* Bits[4:4], Access Type=RsvdZ, default=0x00000000*/

                            /* RB requested a RD stall */
    UINT32 chflushdone : 1;

                            /* Bits[5:5], Access Type=RsvdZ, default=0x00000000*/

                            /* Cache flush done */
    UINT32 reserved6 : 2;

                            /* Bits[7:6], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 txreqdone : 1;

                            /* Bits[8:8], Access Type=RsvdZ, default=0x00000001*/

                            /* TX request done */
    UINT32 tagwrdone : 1;

                            /* Bits[9:9], Access Type=RsvdZ, default=0x00000001*/

                            /* TAG write done */
    UINT32 cachewrdone : 1;

                            /* Bits[10:10], Access Type=RsvdZ, default=0x00000001*/

                            /* Cache write done */
    UINT32 wrfencepend : 1;

                            /* Bits[11:11], Access Type=RsvdZ, default=0x00000000*/

                            /* WrFence pending to be completed */
    UINT32 taglookupdone : 1;

                            /* Bits[12:12], Access Type=RsvdZ, default=0x00000001*/

                            /* TAG lookup done */
    UINT32 stallt4 : 1;

                            /* Bits[13:13], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T4 */
    UINT32 stallt3 : 1;

                            /* Bits[14:14], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T3 */
    UINT32 stallt2 : 1;

                            /* Bits[15:15], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T2 */
    UINT32 stallt1 : 1;

                            /* Bits[16:16], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline stall in T1 */
    UINT32 sreqt4 : 3;

                            /* Bits[19:17], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT4[2:0] */
    UINT32 sreqt3 : 3;

                            /* Bits[22:20], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT3[2:0] */
    UINT32 sreqt2 : 3;

                            /* Bits[25:23], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT2[2:0] */
    UINT32 sreqt1 : 3;

                            /* Bits[28:26], Access Type=RsvdZ, default=0x00000000*/

                            /* pipeline sreqT1[2:0] */
    UINT32 puhdrvalid : 1;

                            /* Bits[29:29], Access Type=RsvdZ, default=0x00000000*/

                            /* Header valid */
    UINT32 allwrreqcomp : 1;

                            /* Bits[30:30], Access Type=RsvdZ, default=0x00000001*/

                            /* All WR requests completed */
    UINT32 allreqcomp : 1;

                            /* Bits[31:31], Access Type=RsvdZ, default=0x00000001*/

                            /* All requests in CCI completed */

  } Bits;
  UINT32 Data;

} CIPUSTAT3_FPGA_SIP_PROTO_STRUCT;

/* EXTSCRATCH0_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x20000290)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Extended scratchpad register0
*/


#define EXTSCRATCH0_FPGA_SIP_PROTO_REG 0x1A420290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extscratch : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Extended scratchpad register */

  } Bits;
  UINT32 Data;

} EXTSCRATCH0_FPGA_SIP_PROTO_STRUCT;

/* EXTSCRATCH1_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x20000294)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Extended scratchpad register1
*/


#define EXTSCRATCH1_FPGA_SIP_PROTO_REG 0x1A420294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extscratch : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Extended scratchpad register */

  } Bits;
  UINT32 Data;

} EXTSCRATCH1_FPGA_SIP_PROTO_STRUCT;

/* EXTSCRATCH2_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x20000298)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Extended scratchpad register2
*/


#define EXTSCRATCH2_FPGA_SIP_PROTO_REG 0x1A420298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extscratch : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Extended scratchpad register */

  } Bits;
  UINT32 Data;

} EXTSCRATCH2_FPGA_SIP_PROTO_STRUCT;

/* EXTSCRATCH3_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x2000029c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Extended scratchpad register3
*/


#define EXTSCRATCH3_FPGA_SIP_PROTO_REG 0x1A42029C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extscratch : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Extended scratchpad register */

  } Bits;
  UINT32 Data;

} EXTSCRATCH3_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0300_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000300)                                                        */
/* Register default value on WHR: 0x000FF000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LCP - Link layer capability
*/


#define RESERVED_0300_FPGA_SIP_PROTO_REG 0x1A420300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ktiportver : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Version of KTI port: KTI 1.0 */
    UINT32 reserved4 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 crcmode : 2;

                            /* Bits[9:8], Access Type=RO, default=0x00000000*/

                            /* CRC Mode supported: CRC16 only */
    UINT32 reserved10 : 2;

                            /* Bits[11:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 llretryqwrapval : 8;

                            /* Bits[19:12], Access Type=RO, default=0x000000FF*/

                            /* Link layer retry queue wrap value */
    UINT32 reserved12 : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0300_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0304_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000304)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LS - Link layer status
*/


#define RESERVED_0304_FPGA_SIP_PROTO_REG 0x1A420304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 numvalentiesinretryq : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Number of valid entries in the retry queue */
    UINT32 rxlinkstate : 2;

                            /* Bits[9:8], Access Type=RO, default=0x00000000*/

                            /* RX link state: 0: IDLE 1: NORMAL 2: RETRY_START */
    UINT32 txlinkstate : 3;

                            /* Bits[12:10], Access Type=RO, default=0x00000000*/

                            /*
                               TX link state: 0: IDLE 1: NORMAL 2: RETRY_START
                               3: RETRY_ROLLBACK 4: ROLLBACK_DONE 5:
                               REPLAY_START 6: RETRY_REPLAY 7: RETRY_DONE
                            */
    UINT32 llsentparaminit : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Link layer has received flit and sent out the
                               parameter init flit
                            */
    UINT32 curcrcmode : 2;

                            /* Bits[15:14], Access Type=RO, default=0x00000000*/

                            /* Current CRC mode: CRC16 only */
    UINT32 linkinitdone : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* Link initialization done */
    UINT32 reserved17 : 15;

                            /* Bits[31:17], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0304_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0308_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000308)                                                        */
/* Register default value on WHR: 0x000000F0                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LP0 - Link received parameter0
*/


#define RESERVED_0308_FPGA_SIP_PROTO_REG 0x1A420308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ktiver : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* KTI Version */
    UINT32 skutype : 4;

                            /* Bits[7:4], Access Type=RO, default=0x0000000F*/

                            /* SKU Type */
    UINT32 portnum : 5;

                            /* Bits[12:8], Access Type=RO, default=0x00000000*/

                            /* Port number */
    UINT32 reserved13 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 ltagent : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* LT Agent */
    UINT32 firmwareagent : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Firmware Agent */
    UINT32 basenodeid : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /* Base Node ID */
    UINT32 reserved20 : 4;

                            /* Bits[23:20], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 rxllrwrapval : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* RX LLR Wrap Value */

  } Bits;
  UINT32 Data;

} RESERVED_0308_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_030C_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x2000030c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LP1 - Link received parameter1
*/


#define RESERVED_030C_FPGA_SIP_PROTO_REG 0x1A42030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_030C_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0310_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000310)                                                        */
/* Register default value on WHR: 0x39800000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_TX_CRD - Link layer TX credit status register
*/


#define RESERVED_0310_FPGA_SIP_PROTO_REG 0x1A420310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txvnacrdavail : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               TX VNA credits available (each bit correspond to
                               one of 8 VC)
                            */
    UINT32 txvn0crdavail : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               TX VN0 credits available (each bit correspond to
                               one of 8 VC)
                            */
    UINT32 rxvnacrdavail : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000080*/

                            /*
                               RX VNA credits available (each bit correspond to
                               one of 8 VC)
                            */
    UINT32 rxvn0crdavail : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000039*/

                            /*
                               RX VN0 credits available (each bit correspond to
                               one of 8 VC)
                            */

  } Bits;
  UINT32 Data;

} RESERVED_0310_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0314_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000314)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LES - Link layer error status
*/


#define RESERVED_0314_FPGA_SIP_PROTO_REG 0x1A420314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 numfailedretry : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Number of failed retry */
    UINT32 reserved4 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 numphyinit : 3;

                            /* Bits[10:8], Access Type=RO, default=0x00000000*/

                            /* Number of phy init */
    UINT32 rsvd : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reserved12 : 20;

                            /* Bits[31:12], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0314_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0318_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000318)                                                        */
/* Register default value on WHR: 0x00000300                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LCL - Link Layer Control
*/


#define RESERVED_0318_FPGA_SIP_PROTO_REG 0x1A420318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 linkrst : 1;

                            /* Bits[0:0], Access Type=RW1S, default=0x00000000*/

                            /* Link layer reset */
    UINT32 debugen : 1;

                            /* Bits[1:1], Access Type=RWSL, default=0x00000000*/

                            /* Enable sending of debug flit (Not supported) */
    UINT32 initstall : 1;

                            /* Bits[2:2], Access Type=RWSL, default=0x00000000*/

                            /* Stall credit return after init */
    UINT32 txholdcrd : 1;

                            /* Bits[3:3], Access Type=RWSL, default=0x00000000*/

                            /* Hold credits (Not supported) */
    UINT32 retrytimeoutval : 3;

                            /* Bits[6:4], Access Type=RWSL, default=0x00000000*/

                            /*
                               Retry Timeout Value: 000 - 8K flits 001 - 4K
                               flits 010 - 2K flits 011 - 1K flits 100 - 1/2K
                               flits 101 - 1/4K flits 110 - 1/8K flits 111 -
                               Disabled
                            */
    UINT32 reserved7 : 1;

                            /* Bits[7:7], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 maxretry : 4;

                            /* Bits[11:8], Access Type=RWSL, default=0x00000003*/

                            /* Number of failed retries to trigger Phy Reset */
    UINT32 maxphyrstbefinitabort : 2;

                            /* Bits[13:12], Access Type=RWSL, default=0x00000000*/

                            /*
                               Max phy reset before initialization aborts 00 -
                               disable initialization abort (infinite reinit)
                               01 - 1 10 - 2 11 - 4
                            */
    UINT32 reserved14 : 18;

                            /* Bits[31:14], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0318_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_031C_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x2000031c)                                                        */
/* Register default value on WHR: 0x00904880                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LCCL - Link Layer Credit Control
*/


#define RESERVED_031C_FPGA_SIP_PROTO_REG 0x1A42031C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vnamax : 8;

                            /* Bits[7:0], Access Type=RWSL, default=0x00000080*/

                            /* VNA credits: up to 128 filits */
    UINT32 reserved8 : 3;

                            /* Bits[10:8], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 vn0snp : 1;

                            /* Bits[11:11], Access Type=RWSL, default=0x00000001*/

                            /* VN0 SNP credit: only 1 is supported */
    UINT32 reserved12 : 2;

                            /* Bits[13:12], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 vn0rsp : 1;

                            /* Bits[14:14], Access Type=RWSL, default=0x00000001*/

                            /* VN0 RSP credit: only 1 is supported */
    UINT32 reserved15 : 5;

                            /* Bits[19:15], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 vn0ncb : 1;

                            /* Bits[20:20], Access Type=RWSL, default=0x00000001*/

                            /* VN0 NCB credit: only 1 is supported */
    UINT32 reserved21 : 2;

                            /* Bits[22:21], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 vn0ncs : 1;

                            /* Bits[23:23], Access Type=RWSL, default=0x00000001*/

                            /* VN0 NCS credit: only 1 is supported */
    UINT32 reserved24 : 8;

                            /* Bits[31:24], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_031C_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0320_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000320)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_LL_LDCL - Link Layer DC
*/


#define RESERVED_0320_FPGA_SIP_PROTO_REG 0x1A420320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0320_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0324_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000324)                                                        */
/* Register default value on WHR: 0x000000F0                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_TX_LP0 - Link Transmit Parameter0
*/


#define RESERVED_0324_FPGA_SIP_PROTO_REG 0x1A420324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ktiversion : 4;

                            /* Bits[3:0], Access Type=RW-L, default=0x00000000*/

                            /* KTI Version */
    UINT32 skutype : 4;

                            /* Bits[7:4], Access Type=RW-L, default=0x0000000F*/

                            /* SKU Type */
    UINT32 portnumb : 5;

                            /* Bits[12:8], Access Type=RW-L, default=0x00000000*/

                            /* Port number */
    UINT32 reserved13 : 1;

                            /* Bits[13:13], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 ltagent : 1;

                            /* Bits[14:14], Access Type=RW-L, default=0x00000000*/

                            /* LT Agent */
    UINT32 firmwareagent : 1;

                            /* Bits[15:15], Access Type=RW-L, default=0x00000000*/

                            /* Firmware Agent */
    UINT32 reserved16 : 8;

                            /* Bits[23:16], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 txeseqmax : 8;

                            /* Bits[31:24], Access Type=RW-L, default=0x00000000*/

                            /* TX LLR Wrap Value */

  } Bits;
  UINT32 Data;

} RESERVED_0324_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0328_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000328)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_TX_LP1 - Link Transmit Parameter1
*/


#define RESERVED_0328_FPGA_SIP_PROTO_REG 0x1A420328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 32;

                            /* Bits[31:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0328_FPGA_SIP_PROTO_STRUCT;

/* KTI_TIMER_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x2000032c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Private KTI Timer
*/


#define KTI_TIMER_FPGA_SIP_PROTO_REG 0x1A42032C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clkcnt : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Counts number of clocks */

  } Bits;
  UINT32 Data;

} KTI_TIMER_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0330_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000330)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_STAT0 - KTI TX CRC/Retry count
*/


#define RESERVED_0330_FPGA_SIP_PROTO_REG 0x1A420330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 numtxcrc : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Number of TX CRC (from RX retry request)
                               detected
                            */

  } Bits;
  UINT32 Data;

} RESERVED_0330_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0334_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000334)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_STAT1 - KTI RX CRC/Retry count
*/


#define RESERVED_0334_FPGA_SIP_PROTO_REG 0x1A420334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 numrxcrc : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Number of RX CRC (from TX retry request)
                               detected
                            */

  } Bits;
  UINT32 Data;

} RESERVED_0334_FPGA_SIP_PROTO_STRUCT;

/* KTI_STAT10_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000358)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI error count
*/


#define KTI_STAT10_FPGA_SIP_PROTO_REG 0x1A420358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcount : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Number of KTI errors detected */
    UINT32 parerrcount : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Number of parity errors detected */
    UINT32 pendsnprespcount : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Number of pending snoop responses */
    UINT32 pendccireqcount : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Number of CCI pending requests */

  } Bits;
  UINT32 Data;

} KTI_STAT10_FPGA_SIP_PROTO_STRUCT;

/* KTI_STAT11_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x2000035c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI error indication
*/


#define KTI_STAT11_FPGA_SIP_PROTO_REG 0x1A42035C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 error : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* KTI error detected */
    UINT32 parerror : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /* KTI parity error detected */
    UINT32 snppendor : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Snoop response is pending */
    UINT32 ccipendor : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* CCI completion is pending */
    UINT32 reqpendor : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* UPI request is pending */
    UINT32 reserved5 : 27;

                            /* Bits[31:5], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_STAT11_FPGA_SIP_PROTO_STRUCT;

/* KTI_STAT12_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000360)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI RX cycle count
*/


#define KTI_STAT12_FPGA_SIP_PROTO_REG 0x1A420360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxcyclesel : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Number of RX cycles */

  } Bits;
  UINT32 Data;

} KTI_STAT12_FPGA_SIP_PROTO_STRUCT;

/* KTI_STAT13_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000364)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI TX cycle count
*/


#define KTI_STAT13_FPGA_SIP_PROTO_REG 0x1A420364

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txcyclesel : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Number of TX cycles */

  } Bits;
  UINT32 Data;

} KTI_STAT13_FPGA_SIP_PROTO_STRUCT;

/* KTI_STAT14_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x20000368)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI RX Data count
*/


#define KTI_STAT14_FPGA_SIP_PROTO_REG 0x1A420368

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdatacnt : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Number of RX cache line transferred */

  } Bits;
  UINT32 Data;

} KTI_STAT14_FPGA_SIP_PROTO_STRUCT;

/* KTI_STAT15_FPGA_SIP_PROTO_REG supported on:                                  */
/*      WHR (0x2000036c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI TX Data count
*/


#define KTI_STAT15_FPGA_SIP_PROTO_REG 0x1A42036C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdatacnt : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Number of TX cache line transferred */

  } Bits;
  UINT32 Data;

} KTI_STAT15_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0370_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000370)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_CTRL0 - Private Link Layer Control
*/


#define RESERVED_0370_FPGA_SIP_PROTO_REG 0x1A420370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sampleen : 1;

                            /* Bits[0:0], Access Type=RW-L, default=0x00000000*/

                            /*
                               Sample enable: start counter at rising
                               transition
                            */
    UINT32 rsvd : 23;

                            /* Bits[23:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxlinkthrottle : 2;

                            /* Bits[25:24], Access Type=RW-L, default=0x00000000*/

                            /*
                               RX link throttle 00 - no throttle 01 - throttle
                               to 3/4 10 - throttle to 1/2 11 - throttle to 1/4
                            */
    UINT32 txlinkthrottle : 2;

                            /* Bits[27:26], Access Type=RW-L, default=0x00000000*/

                            /*
                               TX link throttle 00 - no throttle 01 - throttle
                               to 3/4 10 - throttle to 1/2 11 - throttle to 1/4
                            */
    UINT32 rxcrcerren : 2;

                            /* Bits[29:28], Access Type=RW-L, default=0x00000000*/

                            /*
                               RX CRC error injection 00 - disable random RX
                               CRC error injection 01 - inject RX CRC error
                               average every 256 clks 10 - inject RX CRC error
                               average every 64K clks 11 - inject RX CRC error
                               average 16 per second
                            */
    UINT32 txcrcen : 2;

                            /* Bits[31:30], Access Type=RW-L, default=0x00000000*/

                            /*
                               TX CRC error injection 00 - disable random TX
                               crc error injection 01 - inject TX crc error
                               average every 256 clks 10 - inject TX crc error
                               average every 64K clks 11 - inject TX crc error
                               average 16 per second
                            */

  } Bits;
  UINT32 Data;

} RESERVED_0370_FPGA_SIP_PROTO_STRUCT;

/* KTI_CTRL6_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x20000388)                                                        */
/* Register default value on WHR: 0x01000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Node ID
*/


#define KTI_CTRL6_FPGA_SIP_PROTO_REG 0x1A420388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cpunodeid0 : 4;

                            /* Bits[3:0], Access Type=RW-L, default=0x00000000*/

                            /* CPU Node ID0 */
    UINT32 cpunodeid1 : 4;

                            /* Bits[7:4], Access Type=RW-L, default=0x00000000*/

                            /* CPU Node ID1 */
    UINT32 cpunodeid2 : 4;

                            /* Bits[11:8], Access Type=RW-L, default=0x00000000*/

                            /* CPU Node ID2 */
    UINT32 cpunodeid3 : 2;

                            /* Bits[13:12], Access Type=RW-L, default=0x00000000*/

                            /* CPU Node ID3 */
    UINT32 reserved14 : 10;

                            /* Bits[23:14], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 fpganodeid : 4;

                            /* Bits[27:24], Access Type=RO, default=0x00000001*/

                            /* FPGA Node ID */
    UINT32 srcaddrintvmode : 4;

                            /* Bits[31:28], Access Type=RW-L, default=0x00000000*/

                            /* Source address interleave mode */

  } Bits;
  UINT32 Data;

} KTI_CTRL6_FPGA_SIP_PROTO_STRUCT;

/* KTI_CTRL7_FPGA_SIP_PROTO_REG supported on:                                   */
/*      WHR (0x2000038c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Protocol Control
*/


#define KTI_CTRL7_FPGA_SIP_PROTO_REG 0x1A42038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sampleen : 1;

                            /* Bits[0:0], Access Type=RW-L, default=0x00000000*/

                            /*
                               1: clear cycle count and begin sample 0: stop
                               sample
                            */
    UINT32 rstcnt : 1;

                            /* Bits[1:1], Access Type=RW-L, default=0x00000000*/

                            /* Clear protocol timer and cycle count */
    UINT32 reserved2 : 14;

                            /* Bits[15:2], Access Type=RsvdP, default=0x00000000*/

                            /* Reserved */
    UINT32 rxcyclesel : 5;

                            /* Bits[20:16], Access Type=RW-L, default=0x00000000*/

                            /* Select RX cycle type to count */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txcyclesel : 5;

                            /* Bits[28:24], Access Type=RW-L, default=0x00000000*/

                            /* Select TX cycle type to count */
    UINT32 reserved29 : 3;

                            /* Bits[31:29], Access Type=RsvdP, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_CTRL7_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0390_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000390)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_ERR_STAT0 - KTI Link Layer Error 0
*/


#define RESERVED_0390_FPGA_SIP_PROTO_REG 0x1A420390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxcycleerr : 1;

                            /* Bits[0:0], Access Type=RW1C, default=0x00000000*/

                            /* RX received unsupported cycles */
    UINT32 txretryerr : 1;

                            /* Bits[1:1], Access Type=RW1C, default=0x00000000*/

                            /* TX retry is out of range */
    UINT32 txtimeouterr : 1;

                            /* Bits[2:2], Access Type=RW1C, default=0x00000000*/

                            /* TX timeout error */
    UINT32 rxtimeouterr : 1;

                            /* Bits[3:3], Access Type=RW1C, default=0x00000000*/

                            /* RX timeout error */
    UINT32 rxcsrerr : 1;

                            /* Bits[4:4], Access Type=RW1C, default=0x00000000*/

                            /* RX CSR cycle write to msg reserved space */
    UINT32 reiniterr : 1;

                            /* Bits[5:5], Access Type=RW1C, default=0x00000000*/

                            /* TX reinit failed after multiple phy reset */
    UINT32 rxvnaerr : 1;

                            /* Bits[6:6], Access Type=RW1C, default=0x00000000*/

                            /* RX received VNA flits without VNA credits */
    UINT32 txvnaerr : 1;

                            /* Bits[7:7], Access Type=RW1C, default=0x00000000*/

                            /* TX send VNA flits without VNA credits */
    UINT32 rxvnacrderr : 1;

                            /* Bits[8:8], Access Type=RW1C, default=0x00000000*/

                            /*
                               RX receive VNA credits more than TX_VNA_MAX (CPU
                               VNA credits)
                            */
    UINT32 txvnacrderr : 1;

                            /* Bits[9:9], Access Type=RW1C, default=0x00000000*/

                            /*
                               TX send VNA credits greater than RX_VNA_MAX
                               (FPGA VNA credits)
                            */
    UINT32 rxvn0crderr : 1;

                            /* Bits[10:10], Access Type=RW1C, default=0x00000000*/

                            /* RX received more than 1 VN0 credit */
    UINT32 txvn0crderr : 1;

                            /* Bits[11:11], Access Type=RW1C, default=0x00000000*/

                            /* TX sent more than 1 VN0 credit */
    UINT32 rxvn0err : 1;

                            /* Bits[12:12], Access Type=RW1C, default=0x00000000*/

                            /* RX received VN0 flit without VN0 credit */
    UINT32 txvn0err : 1;

                            /* Bits[13:13], Access Type=RW1C, default=0x00000000*/

                            /* TX sent VN0 flit without VN0 credit */
    UINT32 rxvn1crderr : 1;

                            /* Bits[14:14], Access Type=RW1C, default=0x00000000*/

                            /* RX receives VN1 credit */
    UINT32 txvn1crderr : 1;

                            /* Bits[15:15], Access Type=RW1C, default=0x00000000*/

                            /* TX sent VN1 credit */
    UINT32 rxvn1err : 1;

                            /* Bits[16:16], Access Type=RW1C, default=0x00000000*/

                            /* RX sent VN1 flit */
    UINT32 txvn1err : 1;

                            /* Bits[17:17], Access Type=RW1C, default=0x00000000*/

                            /* TX sent VN1 flit */
    UINT32 rxackerr : 1;

                            /* Bits[18:18], Access Type=RW1C, default=0x00000000*/

                            /* RX Eseq value less than 0 */
    UINT32 txackerr : 1;

                            /* Bits[19:19], Access Type=RW1C, default=0x00000000*/

                            /* TX Eseq value less than 0 */
    UINT32 rxeseqerr : 1;

                            /* Bits[20:20], Access Type=RW1C, default=0x00000000*/

                            /*
                               RX Eseq value greater than RX_Eseq_Max (CPU
                               retry buffer depth)
                            */
    UINT32 txeseqerr : 1;

                            /* Bits[21:21], Access Type=RW1C, default=0x00000000*/

                            /*
                               TX Eseq value greater than TX_Eseq_Max (FPGA
                               retry buffer depth)
                            */
    UINT32 rxllctrlerr : 1;

                            /* Bits[22:22], Access Type=RW1C, default=0x00000000*/

                            /* RX received unknown LLCTRL flit */
    UINT32 rxdebugerr : 1;

                            /* Bits[23:23], Access Type=RW1C, default=0x00000000*/

                            /* RX received debug flit */
    UINT32 rxpoisonerr : 1;

                            /* Bits[24:24], Access Type=RW1C, default=0x00000000*/

                            /* RX received poisoned flit */
    UINT32 reserved25 : 7;

                            /* Bits[31:25], Access Type=RW1C, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0390_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0394_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000394)                                                        */
/* Register default value on WHR: 0x01FFFFFF                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_ERR_EN0 - KTI Link Layer Error Enable 0
*/


#define RESERVED_0394_FPGA_SIP_PROTO_REG 0x1A420394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llerren0 : 25;

                            /* Bits[24:0], Access Type=RW, default=0x01FFFFFF*/

                            /*
                               0 - disable corresponding bit in ERR_STAT0 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved25 : 7;

                            /* Bits[31:25], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0394_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_0398_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x20000398)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_ERR_STAT1 - KTI Link Layer Error 1
*/


#define RESERVED_0398_FPGA_SIP_PROTO_REG 0x1A420398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxoddrtidfifoparerr : 1;

                            /* Bits[0:0], Access Type=RW1C, default=0x00000000*/

                            /* RX odd RTID fifo parity error */
    UINT32 rxevenrtidfifoparerr : 1;

                            /* Bits[1:1], Access Type=RW1C, default=0x00000000*/

                            /* RX even RTID fifo parity error */
    UINT32 rxfifoparerr : 1;

                            /* Bits[2:2], Access Type=RW1C, default=0x00000000*/

                            /* RX fifo parity error */
    UINT32 txfifoparerr : 1;

                            /* Bits[3:3], Access Type=RW1C, default=0x00000000*/

                            /* TX fifo parity error */
    UINT32 txoddcycleparerr : 1;

                            /* Bits[4:4], Access Type=RW1C, default=0x00000000*/

                            /* TX odd cycle fifo parity error */
    UINT32 txevencycleparerr : 1;

                            /* Bits[5:5], Access Type=RW1C, default=0x00000000*/

                            /* TX even cycle fifo parity error */
    UINT32 txevendataparerr : 1;

                            /* Bits[6:6], Access Type=RW1C, default=0x00000000*/

                            /* TX even data fifo parity error */
    UINT32 txodddataparerr : 1;

                            /* Bits[7:7], Access Type=RW1C, default=0x00000000*/

                            /* TX odd data fifo parity error */
    UINT32 rxodddataparerr : 1;

                            /* Bits[8:8], Access Type=RW1C, default=0x00000000*/

                            /* RX odd data fifo parity error */
    UINT32 rxoddcycleparerr : 1;

                            /* Bits[9:9], Access Type=RW1C, default=0x00000000*/

                            /* RX odd cycle fifo parity error */
    UINT32 rxevendataparerr : 1;

                            /* Bits[10:10], Access Type=RW1C, default=0x00000000*/

                            /* RX even data fifo parity error */
    UINT32 rxevencycleparerr : 1;

                            /* Bits[11:11], Access Type=RW1C, default=0x00000000*/

                            /* RX even cycle fifo parity error */
    UINT32 rxoddrtiderr : 1;

                            /* Bits[12:12], Access Type=RW1C, default=0x00000000*/

                            /* RX odd RTID fifo overflow/underflow */
    UINT32 rxevenrtiderr : 1;

                            /* Bits[13:13], Access Type=RW1C, default=0x00000000*/

                            /* RX even RTID fifo overflow/underflow */
    UINT32 txoddcycleerr : 1;

                            /* Bits[14:14], Access Type=RW1C, default=0x00000000*/

                            /* TX odd cycle fifo overflow/underflow */
    UINT32 txevencycleerr : 1;

                            /* Bits[15:15], Access Type=RW1C, default=0x00000000*/

                            /* TX even cycle fifo overflow/underflow */
    UINT32 txevendataerr : 1;

                            /* Bits[16:16], Access Type=RW1C, default=0x00000000*/

                            /* TX even data fifo overflow/underflow */
    UINT32 txodddataerr : 1;

                            /* Bits[17:17], Access Type=RW1C, default=0x00000000*/

                            /* TX odd data fifo overflow/underflow */
    UINT32 rxodddataerr : 1;

                            /* Bits[18:18], Access Type=RW1C, default=0x00000000*/

                            /* RX odd data fifo overflow/underflow */
    UINT32 rxoddcycleerr : 1;

                            /* Bits[19:19], Access Type=RW1C, default=0x00000000*/

                            /* RX odd cycle fifo overflow/underflow */
    UINT32 rxevendataerr : 1;

                            /* Bits[20:20], Access Type=RW1C, default=0x00000000*/

                            /* RX even data fifo overflow/underflow */
    UINT32 rxevencycleerr : 1;

                            /* Bits[21:21], Access Type=RW1C, default=0x00000000*/

                            /* RX even cycle fifo overflow/underflow */
    UINT32 rxfifoerr : 1;

                            /* Bits[22:22], Access Type=RW1C, default=0x00000000*/

                            /* RX fifo overflow/underflow */
    UINT32 reserved23 : 9;

                            /* Bits[31:23], Access Type=RW1C, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_0398_FPGA_SIP_PROTO_STRUCT;

/* RESERVED_039C_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x2000039c)                                                        */
/* Register default value on WHR: 0x007FFFFF                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI_ERR_EN1 - KTI Link Layer Error Enable 1
*/


#define RESERVED_039C_FPGA_SIP_PROTO_REG 0x1A42039C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llerren1 : 23;

                            /* Bits[22:0], Access Type=RW, default=0x007FFFFF*/

                            /*
                               0 - disable corresponding bit in ERR_STAT1 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved17 : 9;

                            /* Bits[31:23], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RESERVED_039C_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_STAT2_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x200003a0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Protocol Layer Error (Reserved[31:16] when SINGLE_CHANNEL)
*/


#define KTI_ERR_STAT2_FPGA_SIP_PROTO_REG 0x1A4203A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rspqerr0 : 1;

                            /* Bits[0:0], Access Type=RW1C, default=0x00000000*/

                            /*
                               response queue fifo overflow/underflow error
                               (Ch0)
                            */
    UINT32 reqqerr0 : 1;

                            /* Bits[1:1], Access Type=RW1C, default=0x00000000*/

                            /*
                               request queue fifo overflow/underflow error
                               (Ch0)
                            */
    UINT32 reloopqerr0 : 1;

                            /* Bits[2:2], Access Type=RW1C, default=0x00000000*/

                            /* reloop queue fifo overflow/underflow error (Ch0) */
    UINT32 ccicmperr0 : 1;

                            /* Bits[3:3], Access Type=RW1C, default=0x00000000*/

                            /* sending cci cmp while cci_cmp is 0 (Ch0) */
    UINT32 validclrerr0 : 1;

                            /* Bits[4:4], Access Type=RW1C, default=0x00000000*/

                            /* clearing request_valid when it's 0 (Ch0) */
    UINT32 validseterr0 : 1;

                            /* Bits[5:5], Access Type=RW1C, default=0x00000000*/

                            /* setting request_valid when it's 1 (Ch0) */
    UINT32 rxdniderr0 : 1;

                            /* Bits[6:6], Access Type=RW1C, default=0x00000000*/

                            /* received cycle with DNID!=FPGA_NID (Ch0) */
    UINT32 memrangeerr0 : 1;

                            /* Bits[7:7], Access Type=RW1C, default=0x00000000*/

                            /*
                               received request with address out of memory
                               range (Ch0)
                            */
    UINT32 respqparerr0 : 1;

                            /* Bits[8:8], Access Type=RW1C, default=0x00000000*/

                            /* response queue fifo parity error (Ch0) */
    UINT32 reqqparerr0 : 1;

                            /* Bits[9:9], Access Type=RW1C, default=0x00000000*/

                            /* request queue fifo parity error (Ch0) */
    UINT32 reloopqparerr0 : 1;

                            /* Bits[10:10], Access Type=RW1C, default=0x00000000*/

                            /* reloop queue fifo parity error (Ch0) */
    UINT32 cmpqparerr0 : 1;

                            /* Bits[11:11], Access Type=RW1C, default=0x00000000*/

                            /* completion queue parity error (Ch0) */
    UINT32 intfredirectparerr0 : 2;

                            /* Bits[13:12], Access Type=RW1C, default=0x00000000*/

                            /*
                               Interface redirection patity error (Ch0)
                               (Reserved when SINGLE_CHANNEL)
                            */
    UINT32 reserved14 : 2;

                            /* Bits[15:14], Access Type=RW1C, default=0x00000000*/

                            /* Reserved */
    UINT32 rspqerr1 : 1;

                            /* Bits[16:16], Access Type=RW1C, default=0x00000000*/

                            /*
                               response queue fifo overflow/underflow error
                               (Ch1)
                            */
    UINT32 reqqerr1 : 1;

                            /* Bits[17:17], Access Type=RW1C, default=0x00000000*/

                            /*
                               request queue fifo overflow/underflow error
                               (Ch1)
                            */
    UINT32 reloopqerr1 : 1;

                            /* Bits[18:18], Access Type=RW1C, default=0x00000000*/

                            /* reloop queue fifo overflow/underflow error (Ch1) */
    UINT32 ccicmperr1 : 1;

                            /* Bits[19:19], Access Type=RW1C, default=0x00000000*/

                            /* sending cci cmp while cci_cmp is 0 (Ch1) */
    UINT32 validclrerr1 : 1;

                            /* Bits[20:20], Access Type=RW1C, default=0x00000000*/

                            /* clearing request_valid when it's 0 (Ch1) */
    UINT32 validseterr1 : 1;

                            /* Bits[21:21], Access Type=RW1C, default=0x00000000*/

                            /* setting request_valid when it's 1 (Ch1) */
    UINT32 rxdniderr1 : 1;

                            /* Bits[22:22], Access Type=RW1C, default=0x00000000*/

                            /* received cycle with DNID!=FPGA_NID (Ch1) */
    UINT32 memrangeerr1 : 1;

                            /* Bits[23:23], Access Type=RW1C, default=0x00000000*/

                            /*
                               received request with address out of memory
                               range (Ch1)
                            */
    UINT32 respqparerr1 : 1;

                            /* Bits[24:24], Access Type=RW1C, default=0x00000000*/

                            /* response queue fifo parity error (Ch1) */
    UINT32 reqqparerr1 : 1;

                            /* Bits[25:25], Access Type=RW1C, default=0x00000000*/

                            /* request queue fifo parity error (Ch1) */
    UINT32 reloopqparerr1 : 1;

                            /* Bits[26:26], Access Type=RW1C, default=0x00000000*/

                            /* reloop queue fifo parity error (Ch1) */
    UINT32 cmpqparerr1 : 1;

                            /* Bits[27:27], Access Type=RW1C, default=0x00000000*/

                            /* completion queue parity error (Ch1) */
    UINT32 intfredirectparerr1 : 2;

                            /* Bits[29:28], Access Type=RW1C, default=0x00000000*/

                            /* Interface redirection patity error (Ch1) */
    UINT32 reserved30 : 2;

                            /* Bits[31:30], Access Type=RW1C, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_ERR_STAT2_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_EN2_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x200003a4)                                                        */
/* Register default value on WHR: 0x0FFF0FFF                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI Protocol Layer Error Enable (Reserved[31:16] when SINGLE_CHANNEL)
*/


#define KTI_ERR_EN2_FPGA_SIP_PROTO_REG 0x1A4203A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llerrench0 : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000FFF*/

                            /*
                               0 - disable corresponding bit in ERR_STAT2 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved12 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 llerrench1 : 12;

                            /* Bits[27:16], Access Type=RsvdZ, default=0x00000FFF*/

                            /*
                               0 - disable corresponding bit in ERR_STAT2 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved28 : 4;

                            /* Bits[31:28], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_ERR_EN2_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_STAT3_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x200003a8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Cache Controller Channel 0 Error
*/


#define KTI_ERR_STAT3_FPGA_SIP_PROTO_REG 0x1A4203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ccirdevictparerr : 1;

                            /* Bits[0:0], Access Type=RW1C, default=0x00000000*/

                            /* cci_pu_rd evict fifo parity error */
    UINT32 ccirdevicterr : 1;

                            /* Bits[1:1], Access Type=RW1C, default=0x00000000*/

                            /* cci_pu_rd evict fifo overflow/underflow */
    UINT32 holdramparerr : 1;

                            /* Bits[2:2], Access Type=RW1C, default=0x00000000*/

                            /* hram parity error */
    UINT32 compramparerr : 1;

                            /* Bits[3:3], Access Type=RW1C, default=0x00000000*/

                            /* completion ram parity error */
    UINT32 wrrxfifoparerr : 1;

                            /* Bits[4:4], Access Type=RW1C, default=0x00000000*/

                            /* wrrx fifo parity error */
    UINT32 wrrxfifoerr : 1;

                            /* Bits[5:5], Access Type=RW1C, default=0x00000000*/

                            /* wrrx fifo overflow/underflow */
    UINT32 grantidowriteerr : 1;

                            /* Bits[6:6], Access Type=RW1C, default=0x00000000*/

                            /* Grant ID overwrite error */
    UINT32 grantidconferr : 1;

                            /* Bits[7:7], Access Type=RW1C, default=0x00000000*/

                            /* Grant ID Conflict error */
    UINT32 rxchconferr : 1;

                            /* Bits[8:8], Access Type=RW1C, default=0x00000000*/

                            /* Rx channel conflict error */
    UINT32 ccicomp0inviderr : 1;

                            /* Bits[9:9], Access Type=RW1C, default=0x00000000*/

                            /* cci completion port0 with an invalid TID */
    UINT32 ccicomp1inviderr : 1;

                            /* Bits[10:10], Access Type=RW1C, default=0x00000000*/

                            /* cci completion port1 with an invalid TID */
    UINT32 rdcompinprt1err : 1;

                            /* Bits[11:11], Access Type=RW1C, default=0x00000000*/

                            /* illegal read completion on cci port1 */
    UINT32 wridorwrramparerr : 1;

                            /* Bits[12:12], Access Type=RW1C, default=0x00000000*/

                            /* write ID fifo or write ram parity error */
    UINT32 wrtxhdrfifoparerr : 1;

                            /* Bits[13:13], Access Type=RW1C, default=0x00000000*/

                            /* write Tx header fifo parity error */
    UINT32 rdtxhdrfifoparerr : 1;

                            /* Bits[14:14], Access Type=RW1C, default=0x00000000*/

                            /* read Tx header fifo parity error */
    UINT32 wridfifoerr : 1;

                            /* Bits[15:15], Access Type=RW1C, default=0x00000000*/

                            /* write ID fifo overflow/underflow */
    UINT32 wrtxhdrfifoerr : 1;

                            /* Bits[16:16], Access Type=RW1C, default=0x00000000*/

                            /* write Tx header fifo overflow/underflow */
    UINT32 rdtxhdrfifoerr : 1;

                            /* Bits[17:17], Access Type=RW1C, default=0x00000000*/

                            /* read Tx header fifo overflow/underlow */
    UINT32 ccicnf0overwriteerr : 1;

                            /* Bits[18:18], Access Type=RW1C, default=0x00000000*/

                            /* cci conflict pipe 0 overwrite an entry */
    UINT32 ccicnf0reademptyerr : 1;

                            /* Bits[19:19], Access Type=RW1C, default=0x00000000*/

                            /* cci conflict pipe 0 cleared an empty entry */
    UINT32 ccicnf1overwriteerr : 1;

                            /* Bits[20:20], Access Type=RW1C, default=0x00000000*/

                            /* cci conflict pipe 1 overwrite an entry */
    UINT32 ccicnf1reademptyerr : 1;

                            /* Bits[21:21], Access Type=RW1C, default=0x00000000*/

                            /* cci conflict pipe 1 cleared an empty entry */
    UINT32 cciwrevictparerr : 1;

                            /* Bits[22:22], Access Type=RW1C, default=0x00000000*/

                            /* cci_pu_wr evict fifo parity error */
    UINT32 cciwrevicterr : 1;

                            /* Bits[23:23], Access Type=RW1C, default=0x00000000*/

                            /* cci_pu_wr evict fifo overflow/underflow */
    UINT32 rdreissuefifoparerr : 1;

                            /* Bits[24:24], Access Type=RW1C, default=0x00000000*/

                            /* read reissue fifo parity error */
    UINT32 wrreissuefifoparerr : 1;

                            /* Bits[25:25], Access Type=RW1C, default=0x00000000*/

                            /* write reissue fifo parity error */
    UINT32 rdreissuefifoerr : 1;

                            /* Bits[26:26], Access Type=RW1C, default=0x00000000*/

                            /* read reissue fifo overflow/underflow */
    UINT32 wrreissuefifoerr : 1;

                            /* Bits[27:27], Access Type=RW1C, default=0x00000000*/

                            /* write reissue fifo overflow/underflow */
    UINT32 reqmonfifoparerr : 1;

                            /* Bits[28:28], Access Type=RW1C, default=0x00000000*/

                            /* request monitor fifo parity error */
    UINT32 reqmonrespfifoerr : 1;

                            /* Bits[29:29], Access Type=RW1C, default=0x00000000*/

                            /*
                               request monitor Rx response fifo
                               overlow/underflow
                            */
    UINT32 reqmonrdfifoerr : 1;

                            /* Bits[30:30], Access Type=RW1C, default=0x00000000*/

                            /*
                               request monitor Read request fifo
                               overlow/underflow
                            */
    UINT32 reserved31 : 1;

                            /* Bits[31:31], Access Type=RW1C, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_ERR_STAT3_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_EN3_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x200003ac)                                                        */
/* Register default value on WHR: 0x7FFFFFFF                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Cache Controller Channel 0 Error Enable
*/


#define KTI_ERR_EN3_FPGA_SIP_PROTO_REG 0x1A4203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ccierrench0 : 31;

                            /* Bits[30:0], Access Type=RW, default=0x7FFFFFFF*/

                            /*
                               0 - disable corresponding bit in ERR_STAT3 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_ERR_EN3_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_STAT4_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x200003b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Cache Controller Channel 1 Error (Reserved when SINGLE_CHANNEL)
*/


#define KTI_ERR_STAT4_FPGA_SIP_PROTO_REG 0x1A4203B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ccirdevictparerr : 1;

                            /* Bits[0:0], Access Type=RsvdZ, default=0x00000000*/

                            /* cci_pu_rd evict fifo parity error */
    UINT32 ccirdevicterr : 1;

                            /* Bits[1:1], Access Type=RsvdZ, default=0x00000000*/

                            /* cci_pu_rd evict fifo overflow/underflow */
    UINT32 holdramparerr : 1;

                            /* Bits[2:2], Access Type=RsvdZ, default=0x00000000*/

                            /* hram parity error */
    UINT32 compramparerr : 1;

                            /* Bits[3:3], Access Type=RsvdZ, default=0x00000000*/

                            /* completion ram parity error */
    UINT32 wrrxfifoparerr : 1;

                            /* Bits[4:4], Access Type=RsvdZ, default=0x00000000*/

                            /* wrrx fifo parity error */
    UINT32 wrrxfifoerr : 1;

                            /* Bits[5:5], Access Type=RsvdZ, default=0x00000000*/

                            /* wrrx fifo overflow/underflow */
    UINT32 grantidowriteerr : 1;

                            /* Bits[6:6], Access Type=RsvdZ, default=0x00000000*/

                            /* Grant ID overwrite error */
    UINT32 grantidconferr : 1;

                            /* Bits[7:7], Access Type=RsvdZ, default=0x00000000*/

                            /* Grant ID Conflict error */
    UINT32 rxchconferr : 1;

                            /* Bits[8:8], Access Type=RsvdZ, default=0x00000000*/

                            /* Rx channel conflict error */
    UINT32 ccicomp0inviderr : 1;

                            /* Bits[9:9], Access Type=RsvdZ, default=0x00000000*/

                            /* cci completion port0 with an invalid TID */
    UINT32 ccicomp1inviderr : 1;

                            /* Bits[10:10], Access Type=RsvdZ, default=0x00000000*/

                            /* cci completion port1 with an invalid TID */
    UINT32 rdcompinprt1err : 1;

                            /* Bits[11:11], Access Type=RsvdZ, default=0x00000000*/

                            /* illegal read completion on cci port1 */
    UINT32 wridorwrramparerr : 1;

                            /* Bits[12:12], Access Type=RsvdZ, default=0x00000000*/

                            /* write ID fifo or write ram parity error */
    UINT32 wrtxhdrfifoparerr : 1;

                            /* Bits[13:13], Access Type=RsvdZ, default=0x00000000*/

                            /* write Tx header fifo parity error */
    UINT32 rdtxhdrfifoparerr : 1;

                            /* Bits[14:14], Access Type=RsvdZ, default=0x00000000*/

                            /* read Tx header fifo parity error */
    UINT32 wridfifoerr : 1;

                            /* Bits[15:15], Access Type=RsvdZ, default=0x00000000*/

                            /* write ID fifo overflow/underflow */
    UINT32 wrtxhdrfifoerr : 1;

                            /* Bits[16:16], Access Type=RsvdZ, default=0x00000000*/

                            /* write Tx header fifo overflow/underflow */
    UINT32 rdtxhdrfifoerr : 1;

                            /* Bits[17:17], Access Type=RsvdZ, default=0x00000000*/

                            /* read Tx header fifo overflow/underlow */
    UINT32 ccicnf0overwriteerr : 1;

                            /* Bits[18:18], Access Type=RsvdZ, default=0x00000000*/

                            /* cci conflict pipe 0 overwrite an entry */
    UINT32 ccicnf0reademptyerr : 1;

                            /* Bits[19:19], Access Type=RsvdZ, default=0x00000000*/

                            /* cci conflict pipe 0 cleared an empty entry */
    UINT32 ccicnf1overwriteerr : 1;

                            /* Bits[20:20], Access Type=RsvdZ, default=0x00000000*/

                            /* cci conflict pipe 1 overwrite an entry */
    UINT32 ccicnf1reademptyerr : 1;

                            /* Bits[21:21], Access Type=RsvdZ, default=0x00000000*/

                            /* cci conflict pipe 1 cleared an empty entry */
    UINT32 cciwrevictparerr : 1;

                            /* Bits[22:22], Access Type=RsvdZ, default=0x00000000*/

                            /* cci_pu_wr evict fifo parity error */
    UINT32 cciwrevicterr : 1;

                            /* Bits[23:23], Access Type=RsvdZ, default=0x00000000*/

                            /* cci_pu_wr evict fifo overflow/underflow */
    UINT32 rdreissuefifoparerr : 1;

                            /* Bits[24:24], Access Type=RsvdZ, default=0x00000000*/

                            /* read reissue fifo parity error */
    UINT32 wrreissuefifoparerr : 1;

                            /* Bits[25:25], Access Type=RsvdZ, default=0x00000000*/

                            /* write reissue fifo parity error */
    UINT32 rdreissuefifoerr : 1;

                            /* Bits[26:26], Access Type=RsvdZ, default=0x00000000*/

                            /* read reissue fifo overflow/underflow */
    UINT32 wrreissuefifoerr : 1;

                            /* Bits[27:27], Access Type=RsvdZ, default=0x00000000*/

                            /* write reissue fifo overflow/underflow */
    UINT32 reqmonfifoparerr : 1;

                            /* Bits[28:28], Access Type=RsvdZ, default=0x00000000*/

                            /* request monitor fifo parity error */
    UINT32 reqmonrespfifoerr : 1;

                            /* Bits[29:29], Access Type=RsvdZ, default=0x00000000*/

                            /*
                               request monitor Rx response fifo
                               overlow/underflow
                            */
    UINT32 reqmonrdfifoerr : 1;

                            /* Bits[30:30], Access Type=RsvdZ, default=0x00000000*/

                            /*
                               request monitor Read request fifo
                               overlow/underflow
                            */
    UINT32 reserved31 : 1;

                            /* Bits[31:31], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_ERR_STAT4_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_EN4_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x200003b4)                                                        */
/* Register default value on WHR: 0x7FFFFFFF                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Cache Controller Channel 1 Error Enable (Reserved when SINGLE_CHANNEL)
*/


#define KTI_ERR_EN4_FPGA_SIP_PROTO_REG 0x1A4203B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ccierrench1 : 31;

                            /* Bits[30:0], Access Type=RsvdZ, default=0x7FFFFFFF*/

                            /*
                               0 - disable corresponding bit in ERR_STAT4 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTI_ERR_EN4_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_STAT5_FPGA_SIP_PROTO_REG supported on:                               */
/*      WHR (0x200003b8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Local Tag/Cache Error (Reserved[31:16] when SINGLE_CHANNEL)
*/


#define KTI_ERR_STAT5_FPGA_SIP_PROTO_REG 0x1A4203B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cacheparerr0 : 1;

                            /* Bits[0:0], Access Type=RW1C, default=0x00000000*/

                            /* cache ram parity error (Ch0) */
    UINT32 remoddtagparerr0 : 1;

                            /* Bits[1:1], Access Type=RW1C, default=0x00000000*/

                            /* remote odd tag ram parity error (Ch0) */
    UINT32 remoddtagmulthiterr0 : 1;

                            /* Bits[2:2], Access Type=RW1C, default=0x00000000*/

                            /* remote odd tag hit more than 1-way (Ch0) */
    UINT32 remoddtagwrcnflterr0 : 1;

                            /* Bits[3:3], Access Type=RW1C, default=0x00000000*/

                            /*
                               remote odd cci tag write address conflict with
                               protocol write (Ch0)
                            */
    UINT32 reserved4 : 1;

                            /* Bits[4:4], Access Type=RW1C, default=0x00000000*/

                            /* reserved */
    UINT32 remeventagparerr0 : 1;

                            /* Bits[5:5], Access Type=RW1C, default=0x00000000*/

                            /* remote even tag ram parity error (Ch0) */
    UINT32 remeventagmulthiterr0 : 1;

                            /* Bits[6:6], Access Type=RW1C, default=0x00000000*/

                            /* remote even tag hit more than 1-way (Ch0) */
    UINT32 remeventagwrcnflterr0 : 1;

                            /* Bits[7:7], Access Type=RW1C, default=0x00000000*/

                            /*
                               remote even cci tag write address conflict with
                               protocol write (Ch0)
                            */
    UINT32 reserved8 : 1;

                            /* Bits[8:8], Access Type=RW1C, default=0x00000000*/

                            /* reserved */
    UINT32 locoddtagparerr0 : 1;

                            /* Bits[9:9], Access Type=RW1C, default=0x00000000*/

                            /* local odd tag ram parity error (Ch0) */
    UINT32 locoddtagmulthiterr0 : 1;

                            /* Bits[10:10], Access Type=RW1C, default=0x00000000*/

                            /* local odd tag hit more than 1-way (Ch0) */
    UINT32 locoddtagwrcnflterr0 : 1;

                            /* Bits[11:11], Access Type=RW1C, default=0x00000000*/

                            /*
                               local odd cci tag write address conflict with
                               protocol write (Ch0)
                            */
    UINT32 reserved12 : 1;

                            /* Bits[12:12], Access Type=RW1C, default=0x00000000*/

                            /* reserved */
    UINT32 loceventagparerr0 : 1;

                            /* Bits[13:13], Access Type=RW1C, default=0x00000000*/

                            /* local even tag ram parity error (Ch0) */
    UINT32 loceventagmulthiterr0 : 1;

                            /* Bits[14:14], Access Type=RW1C, default=0x00000000*/

                            /* local even tag hit more than 1-way (Ch0) */
    UINT32 loceventagwrcnflterr0 : 1;

                            /* Bits[15:15], Access Type=RW1C, default=0x00000000*/

                            /*
                               local even cci tag write address conflict with
                               protocol write (Ch0)
                            */
    UINT32 cacheparerr : 1;

                            /* Bits[16:16], Access Type=RW1C, default=0x00000000*/

                            /* cache ram parity error (Ch1) */
    UINT32 remoddtagparerr1 : 1;

                            /* Bits[17:17], Access Type=RW1C, default=0x00000000*/

                            /* remote odd tag ram parity error (Ch1) */
    UINT32 remoddtagmulthiterr1 : 1;

                            /* Bits[18:18], Access Type=RW1C, default=0x00000000*/

                            /* remote odd tag hit more than 1-way (Ch1) */
    UINT32 remoddtagwrcnflterr1 : 1;

                            /* Bits[19:19], Access Type=RW1C, default=0x00000000*/

                            /*
                               remote odd cci tag write address conflict with
                               protocol write (Ch1)
                            */
    UINT32 reserved20 : 1;

                            /* Bits[20:20], Access Type=RW1C, default=0x00000000*/

                            /* reserved */
    UINT32 remeventagparerr1 : 1;

                            /* Bits[21:21], Access Type=RW1C, default=0x00000000*/

                            /* remote even tag ram parity error (Ch1) */
    UINT32 remeventagmulthiterr1 : 1;

                            /* Bits[22:22], Access Type=RW1C, default=0x00000000*/

                            /* remote even tag hit more than 1-way (Ch1) */
    UINT32 remeventagwrcnflterr1 : 1;

                            /* Bits[23:23], Access Type=RW1C, default=0x00000000*/

                            /*
                               remote even cci tag write address conflict with
                               protocol write (Ch1)
                            */
    UINT32 reserved24 : 1;

                            /* Bits[24:24], Access Type=RW1C, default=0x00000000*/

                            /* reserved */
    UINT32 locoddtagparerr1 : 1;

                            /* Bits[25:25], Access Type=RW1C, default=0x00000000*/

                            /* local odd tag ram parity error (Ch1) */
    UINT32 locoddtagmulthiterr1 : 1;

                            /* Bits[26:26], Access Type=RW1C, default=0x00000000*/

                            /* local odd tag hit more than 1-way (Ch1) */
    UINT32 locoddtagwrcnflterr1 : 1;

                            /* Bits[27:27], Access Type=RW1C, default=0x00000000*/

                            /*
                               local odd cci tag write address conflict with
                               protocol write (Ch1)
                            */
    UINT32 reserved28 : 1;

                            /* Bits[28:28], Access Type=RW1C, default=0x00000000*/

                            /* reserved */
    UINT32 loceventagparerr1 : 1;

                            /* Bits[29:29], Access Type=RW1C, default=0x00000000*/

                            /* local even tag ram parity error (Ch1) */
    UINT32 loceventagmulthiterr1 : 1;

                            /* Bits[30:30], Access Type=RW1C, default=0x00000000*/

                            /* local even tag hit more than 1-way (Ch1) */
    UINT32 loceventagwrcnflterr1 : 1;

                            /* Bits[31:31], Access Type=RW1C, default=0x00000000*/

                            /*
                               local even cci tag write address conflict with
                               protocol write (Ch1)
                            */

  } Bits;
  UINT32 Data;

} KTI_ERR_STAT5_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_EN5_FPGA_SIP_PROTO_REG supported on:                                 */
/*      WHR (0x200003bc)                                                        */
/* Register default value on WHR: 0xEEEFEEEF                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* Local Tag/Cache Error Enable (Reserved[31:16] when SINGLE_CHANNEL)
*/


#define KTI_ERR_EN5_FPGA_SIP_PROTO_REG 0x1A4203BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cacheerren0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 remoddtagerren0 : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 remeventagerren0 : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 locoddtagerren0 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 loceventagerren0 : 3;

                            /* Bits[15:13], Access Type=RW, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 cacheerren1 : 1;

                            /* Bits[16:16], Access Type=RsvdZ, default=0x00000001*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 remoddtagerren1 : 3;

                            /* Bits[19:17], Access Type=RsvdZ, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved20 : 1;

                            /* Bits[20:20], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 remeventagerren1 : 3;

                            /* Bits[23:21], Access Type=RsvdZ, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved24 : 1;

                            /* Bits[24:24], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 locoddtagerren1 : 3;

                            /* Bits[27:25], Access Type=RsvdZ, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */
    UINT32 reserved28 : 1;

                            /* Bits[28:28], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 loceventagerren1 : 3;

                            /* Bits[31:29], Access Type=RsvdZ, default=0x00000007*/

                            /*
                               0 - disable corresponding bit in ERR_STAT5 1 -
                               enable corresponding error detection
                            */

  } Bits;
  UINT32 Data;

} KTI_ERR_EN5_FPGA_SIP_PROTO_STRUCT;

/* KTI_FERR_FPGA_SIP_PROTO_REG supported on:                                    */
/*      WHR (0x200003c8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI first error
*/


#define KTI_FERR_FPGA_SIP_PROTO_REG 0x1A4203C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errorstatus : 32;

                            /* Bits[31:0], Access Type=RW1C, default=0x00000000*/

                            /*
                               Holds the Status of the Error Register that was
                               triggered first
                            */

  } Bits;
  UINT32 Data;

} KTI_FERR_FPGA_SIP_PROTO_STRUCT;

/* KTI_NERR_FPGA_SIP_PROTO_REG supported on:                                    */
/*      WHR (0x200003cc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI next error
*/


#define KTI_NERR_FPGA_SIP_PROTO_REG 0x1A4203CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errorstatus : 32;

                            /* Bits[31:0], Access Type=RW1C, default=0x00000000*/

                            /*
                               Holds the status of the Error Register that was
                               triggered second
                            */

  } Bits;
  UINT32 Data;

} KTI_NERR_FPGA_SIP_PROTO_STRUCT;

/* KTI_ERR_INFO_FPGA_SIP_PROTO_REG supported on:                                */
/*      WHR (0x200003d0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_kti_csr                                           */
/* Struct generated from WHR BDF: 0_0_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* KTI error ID and parity counter
*/


#define KTI_ERR_INFO_FPGA_SIP_PROTO_REG 0x1A4203D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ktiperrcnt : 26;

                            /* Bits[25:0], Access Type=RO, default=0x00000000*/

                            /*
                               Number of parity errors detected in KTI
                               controller
                            */
    UINT32 nerrorsourceid : 3;

                            /* Bits[28:26], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the Error Register that was triggered
                               second 0 - KTI Link Layer Error 0 1 - KTI Link
                               Layer Error 1 2 - KTI Protocol Layer Error 3 -
                               Cache Controller Error 4 - Local Tag/Cache Error
                               5 - Snoop Filter Error (only for HA)
                            */
    UINT32 ferrorsourceid : 3;

                            /* Bits[31:29], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the Error Register that was triggered
                               first 0 - KTI Link Layer Error 0 1 - KTI Link
                               Layer Error 1 2 - KTI Protocol Layer Error 3 -
                               Cache Controller Error 4 - Local Tag/Cache Error
                               5 - Snoop Filter Error (only for HA)
                            */

  } Bits;
  UINT32 Data;

} KTI_ERR_INFO_FPGA_SIP_PROTO_STRUCT;
#endif /* _FPGA_SIP_PROTO_h */
