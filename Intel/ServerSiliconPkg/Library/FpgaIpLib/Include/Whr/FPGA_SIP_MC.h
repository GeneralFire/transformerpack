
/** @file
  FPGA_SIP_MC.h

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


#ifndef _FPGA_SIP_MC_h
#define _FPGA_SIP_MC_h
#include <Base.h>

/* PCIE_00_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001000)                                                        */
/* Register default value on WHR: 0xBCC78086                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Device ID and Vendor ID
*/


#define PCIE_00_FPGA_SIP_MC_REG 0x1A520000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vendorid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* Vendor ID */
    UINT32 deviceid : 16;

                            /* Bits[31:16], Access Type=RO, default=0x0000BCC7*/

                            /* Device ID */

  } Bits;
  UINT32 Data;

} PCIE_00_FPGA_SIP_MC_STRUCT;

/* PCIE_04_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001004)                                                        */
/* Register default value on WHR: 0x00100000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Status and Command
*/


#define PCIE_04_FPGA_SIP_MC_REG 0x1A520004

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

} PCIE_04_FPGA_SIP_MC_STRUCT;

/* PCIE_08_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001008)                                                        */
/* Register default value on WHR: 0x08800000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Class code and Revision ID
*/


#define PCIE_08_FPGA_SIP_MC_REG 0x1A520008

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

} PCIE_08_FPGA_SIP_MC_STRUCT;

/* PCIE_0C_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x2000100c)                                                        */
/* Register default value on WHR: 0x00000010                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE Latency Timer and CL size
*/


#define PCIE_0C_FPGA_SIP_MC_REG 0x1A52000C

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

} PCIE_0C_FPGA_SIP_MC_STRUCT;

/* PCIE_10_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001010)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #0
*/


#define PCIE_10_FPGA_SIP_MC_REG 0x1A520010

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

} PCIE_10_FPGA_SIP_MC_STRUCT;

/* PCIE_14_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001014)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #1
*/


#define PCIE_14_FPGA_SIP_MC_REG 0x1A520014

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

} PCIE_14_FPGA_SIP_MC_STRUCT;

/* PCIE_18_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001018)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #2
*/


#define PCIE_18_FPGA_SIP_MC_REG 0x1A520018

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

} PCIE_18_FPGA_SIP_MC_STRUCT;

/* PCIE_1C_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x2000101c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #3
*/


#define PCIE_1C_FPGA_SIP_MC_REG 0x1A52001C

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

} PCIE_1C_FPGA_SIP_MC_STRUCT;

/* PCIE_20_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001020)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #4
*/


#define PCIE_20_FPGA_SIP_MC_REG 0x1A520020

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

} PCIE_20_FPGA_SIP_MC_STRUCT;

/* PCIE_24_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001024)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Base Address Register #5
*/


#define PCIE_24_FPGA_SIP_MC_REG 0x1A520024

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

} PCIE_24_FPGA_SIP_MC_STRUCT;

/* PCIE_28_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001028)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Cardbus CIS Pointer
*/


#define PCIE_28_FPGA_SIP_MC_REG 0x1A520028

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

} PCIE_28_FPGA_SIP_MC_STRUCT;

/* PCIE_2C_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x2000102c)                                                        */
/* Register default value on WHR: 0x00008086                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Subsystem and Subsystem Vendor ID
*/


#define PCIE_2C_FPGA_SIP_MC_REG 0x1A52002C

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

} PCIE_2C_FPGA_SIP_MC_STRUCT;

/* PCIE_30_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001030)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Expansion ROM base address
*/


#define PCIE_30_FPGA_SIP_MC_REG 0x1A520030

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

} PCIE_30_FPGA_SIP_MC_STRUCT;

/* PCIE_34_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001034)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Capabilities pointer
*/


#define PCIE_34_FPGA_SIP_MC_REG 0x1A520034

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

} PCIE_34_FPGA_SIP_MC_STRUCT;

/* PCIE_38_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001038)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* PCIE_Reserved
*/


#define PCIE_38_FPGA_SIP_MC_REG 0x1A520038

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

} PCIE_38_FPGA_SIP_MC_STRUCT;

/* PCIE_3C_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x2000103c)                                                        */
/* Register default value on WHR: 0x00000100                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Latency and Interrupt
*/


#define PCIE_3C_FPGA_SIP_MC_REG 0x1A52003C

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

} PCIE_3C_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_SIZE_HI_FPGA_SIP_MC_REG supported on:                             */
/*      WHR (0x20001100)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Size High
*/


#define MC_IAL_MEM_SIZE_HI_FPGA_SIP_MC_REG 0x1A520100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memsizehigh : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory_Size_High: Corresponds to bits 63:32 of
                               type 1 memory size.
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_SIZE_HI_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_SIZE_LO_FPGA_SIP_MC_REG supported on:                             */
/*      WHR (0x20001104)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Size Low
*/


#define MC_IAL_MEM_SIZE_LO_FPGA_SIP_MC_REG 0x1A520104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meminfovalid : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory_Info_Valid: When set, indicates that the
                               memory size, memory type, Base Unit and
                               LatencyBW_Valid registers are valid.
                            */
    UINT32 memactive : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Memory_Active: When set, indicates that the
                               memory is fully initialized and available for
                               software use.
                            */
    UINT32 reserved2 : 18;

                            /* Bits[19:2], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 memsizelow : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*
                               Memory_Size_Low: Corresponds to bits 31:20 of
                               type 1 memory size.
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_SIZE_LO_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_TYPE_FPGA_SIP_MC_REG supported on:                                */
/*      WHR (0x20001108)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Type
*/


#define MC_IAL_MEM_TYPE_FPGA_SIP_MC_REG 0x1A520108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memtype : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               ACPI_Memory_Type: Returns address range type.
                               Address Range type is enumerated in ACPI 6.1
                               specification. The following values are
                               considered legal 1 - AddressRangeMemory 7 -
                               AddressRangePersistentMemory
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_TYPE_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_BASE_HI_FPGA_SIP_MC_REG supported on:                             */
/*      WHR (0x2000110c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Base High
*/


#define MC_IAL_MEM_BASE_HI_FPGA_SIP_MC_REG 0x1A52010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 membasehigh : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Memory_Base_High: Corresponds to bits 63:32 of
                               type 1 memory base in the host address space.
                               Configured by system BIOS.
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_BASE_HI_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_BASE_LO_FPGA_SIP_MC_REG supported on:                             */
/*      WHR (0x20001110)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Base Low
*/


#define MC_IAL_MEM_BASE_LO_FPGA_SIP_MC_REG 0x1A520110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved0 : 20;

                            /* Bits[19:0], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 membaselow : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /*
                               Memory_Base_Low:Corresponds to bits 31:20 of
                               type 1 memory base in the host address space
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_BASE_LO_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_LAT_BW_VALID_FPGA_SIP_MC_REG supported on:                        */
/*      WHR (0x20001114)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Latency Bandwidth Valid
*/


#define MC_IAL_MEM_LAT_BW_VALID_FPGA_SIP_MC_REG 0x1A520114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memrdlatvalid : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Mem_RD_Lat_Valid: Read Latency valid */
    UINT32 memwrlatvalid : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /* Mem_WR_Lat_Valid: Write Latency valid */
    UINT32 memrdbwvalid : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Mem_RD_BW_Valid: Read Bandwidth valid */
    UINT32 memwrbwvalid : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Mem_WR_BW_Valid: Write Bandwidth valid */
    UINT32 reserved4 : 28;

                            /* Bits[31:4], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_LAT_BW_VALID_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_BW_FPGA_SIP_MC_REG supported on:                                  */
/*      WHR (0x20001118)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Bandwidth
*/


#define MC_IAL_MEM_BW_FPGA_SIP_MC_REG 0x1A520118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memrdbw : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Mem_RD_BW: If Read Bandwidth valid = 1,
                               indicates Read Bandwidth to Type 1 memory
                               expressed in multiple of GB/s
                            */
    UINT32 memwrbw : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*
                               Mem_WR_BW: If Write Bandwidth valid = 1,
                               indicates Write Bandwidth to Type 1 memory
                               expressed in multiple of GB/s
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_BW_FPGA_SIP_MC_STRUCT;

/* MC_IAL_MEM_LATENCY_FPGA_SIP_MC_REG supported on:                             */
/*      WHR (0x2000111c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Latency
*/


#define MC_IAL_MEM_LATENCY_FPGA_SIP_MC_REG 0x1A52011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memrdlat : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Mem_RD_Lat: If Read Latency valid = 1, indicates
                               Read Latency to Type 1 memory expressed in
                               multiple of ns, measured from the Intel AL
                               ingress.
                            */
    UINT32 memwrlat : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*
                               Mem_WR_Lat: If Write Latency valid = 1,
                               indicates Write Latency to Type 1 memory
                               expressed in multiple of ns, measured from the
                               Intel AL ingress.
                            */

  } Bits;
  UINT32 Data;

} MC_IAL_MEM_LATENCY_FPGA_SIP_MC_STRUCT;

/* MC_PTRL_CTRL_FPGA_SIP_MC_REG supported on:                                   */
/*      WHR (0x20001120)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Patrol Scrubber Control
*/


#define MC_PTRL_CTRL_FPGA_SIP_MC_REG 0x1A520120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 patroltimeout : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Patrol Timeout interval. Number of 1 MHz clock
                               cycles before the next address location is
                               scrubbed.
                            */
    UINT32 reserved16 : 15;

                            /* Bits[30:16], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 patrolenable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               PatrolEnable: 1 - Enabled patrol scrub
                               operation. 0 - Disable patrol scrub operation.
                            */

  } Bits;
  UINT32 Data;

} MC_PTRL_CTRL_FPGA_SIP_MC_STRUCT;

/* MC_STAT_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001124)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Controller Status
*/


#define MC_STAT_FPGA_SIP_MC_REG 0x1A520124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 emifinitdone : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /*
                               EMIF Phy and Controller initialization complete
                               (1-bit per channel)
                            */
    UINT32 reserved4 : 4;

                            /* Bits[7:4], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 emifcalfail : 4;

                            /* Bits[11:8], Access Type=RO, default=0x00000000*/

                            /* EMIF Phy Calibration Failure (1-bit per channel) */
    UINT32 reserved12 : 4;

                            /* Bits[15:12], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */
    UINT32 emifclearbusy : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /*
                               EMIF Clear State Machine Active (1-bit per
                               channel)
                            */
    UINT32 reserved20 : 12;

                            /* Bits[31:20], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MC_STAT_FPGA_SIP_MC_STRUCT;

/* MC_CTRL_FPGA_SIP_MC_REG supported on:                                        */
/*      WHR (0x20001128)                                                        */
/* Register default value on WHR: 0x0000000F                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Controller Control
*/


#define MC_CTRL_FPGA_SIP_MC_REG 0x1A520128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 emifclearen : 4;

                            /* Bits[3:0], Access Type=RW1C, default=0x0000000F*/

                            /*
                               EMIF Clear State Machine Enable (1-bit per
                               channel)
                            */
    UINT32 reserved4 : 28;

                            /* Bits[31:4], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MC_CTRL_FPGA_SIP_MC_STRUCT;

/* MC_CSR_CTRL_FPGA_SIP_MC_REG supported on:                                    */
/*      WHR (0x2000112c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Controller CSR Status/Control/Address
*/


#define MC_CSR_CTRL_FPGA_SIP_MC_REG 0x1A52012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 address : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /* EMIF CSR address for read/write operation. */
    UINT32 mcchan : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates which memory channel is the target for
                               the CSR operation.
                            */
    UINT32 read : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Indicates a read operation. */
    UINT32 write : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Indicates a write operation. */
    UINT32 rdwrack : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates read data is valid or that a write
                               operation has reached the EMIF IP.
                            */
    UINT32 reserved15 : 17;

                            /* Bits[31:15], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MC_CSR_CTRL_FPGA_SIP_MC_STRUCT;

/* MC_CSR_WRDATA_FPGA_SIP_MC_REG supported on:                                  */
/*      WHR (0x20001130)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Controller CSR Write Data
*/


#define MC_CSR_WRDATA_FPGA_SIP_MC_REG 0x1A520130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrdata : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* EMIF CSR write data. */

  } Bits;
  UINT32 Data;

} MC_CSR_WRDATA_FPGA_SIP_MC_STRUCT;

/* MC_CSR_RDDATA_FPGA_SIP_MC_REG supported on:                                  */
/*      WHR (0x20001134)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Controller CSR Read Data
*/


#define MC_CSR_RDDATA_FPGA_SIP_MC_REG 0x1A520134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rddata : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               EMIF CSR read data. Valid only after
                               MC_CSR_CTRL.RdWrAck is set to 1 by HW
                            */

  } Bits;
  UINT32 Data;

} MC_CSR_RDDATA_FPGA_SIP_MC_STRUCT;

/* MC_DEBUG_FPGA_SIP_MC_REG supported on:                                       */
/*      WHR (0x20001138)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    cpxp_mc_csr                                            */
/* Struct generated from WHR BDF: 0_0_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* Memory Controller Debug 
*/


#define MC_DEBUG_FPGA_SIP_MC_REG 0x1A520138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rdpoisoninject : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               11 - Undefined 10 - A '1' in readdata[511] will
                               generate a poison for the read data. 01 -
                               Enables random injection of poison on read data
                               from memory controller. On average 1 out of 128
                               read completions will be injected with poison.
                               00 - No poison injected. Normal operation.
                            */
    UINT32 reserved4 : 30;

                            /* Bits[31:2], Access Type=RsvdZ, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MC_DEBUG_FPGA_SIP_MC_STRUCT;
#endif /* _FPGA_SIP_MC_h */
