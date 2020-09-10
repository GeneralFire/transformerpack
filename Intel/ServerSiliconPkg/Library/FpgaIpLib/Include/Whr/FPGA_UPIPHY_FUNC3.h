
/** @file
  FPGA_UPIPHY_FUNC3.h

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


#ifndef _FPGA_UPIPHY_FUNC3_h
#define _FPGA_UPIPHY_FUNC3_h
#include <Base.h>

/* WHR_PH_PCIE_FUNC1_0_FPGA_UPIPHY_FUNC3_REG supported on:                      */
/*      WHR (0x20023000)                                                        */
/* Register default value on WHR: 0xBCC38086                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset 0
*/


#define WHR_PH_PCIE_FUNC1_0_FPGA_UPIPHY_FUNC3_REG 0x1A320000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vid : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00008086*/

                            /* This is the vendor ID */
    UINT32 did : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x0000BCC3*/

                            /* This is the device ID */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_0_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_4_FPGA_UPIPHY_FUNC3_REG supported on:                      */
/*      WHR (0x20023004)                                                        */
/* Register default value on WHR: 0x00100000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset 4
*/


#define WHR_PH_PCIE_FUNC1_4_FPGA_UPIPHY_FUNC3_REG 0x1A320004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_space_en : 1;

                            /* Bits[0:0], Access Type=RO/P, default=0x00000000*/

                            /* This is the IO space enable */
    UINT32 mem_space_en : 1;

                            /* Bits[1:1], Access Type=RO/P, default=0x00000000*/

                            /* This is the memory space enable */
    UINT32 bus_master_en : 1;

                            /* Bits[2:2], Access Type=RO/P, default=0x00000000*/

                            /* This is the bus mater enable */
    UINT32 spec_cyc_en : 1;

                            /* Bits[3:3], Access Type=RO/P, default=0x00000000*/

                            /* This is the special cycle enable */
    UINT32 mem_wr_inv_en : 1;

                            /* Bits[4:4], Access Type=RO/P, default=0x00000000*/

                            /* This is the memory write and invalidate enable */
    UINT32 vga_palette_snp_en : 1;

                            /* Bits[5:5], Access Type=RO/P, default=0x00000000*/

                            /* This is the VGA palette snoop enable */
    UINT32 par_err_rsp : 1;

                            /* Bits[6:6], Access Type=RO/P, default=0x00000000*/

                            /* This is the parity error response */
    UINT32 idsel_stp_wait_cyc_ctrl : 1;

                            /* Bits[7:7], Access Type=RO/P, default=0x00000000*/

                            /* This is the IDSEL stepping wait cycle control */
    UINT32 serr_en : 1;

                            /* Bits[8:8], Access Type=RO/P, default=0x00000000*/

                            /* This is the SERR enables */
    UINT32 fast_b2b_en : 1;

                            /* Bits[9:9], Access Type=RO/P, default=0x00000000*/

                            /* This is the Fast back to back enable */
    UINT32 intx_dis : 1;

                            /* Bits[10:10], Access Type=RO/P, default=0x00000000*/

                            /* This is the INTx disable */
    UINT32 rsvd : 8;

                            /* Bits[18:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 intx_status : 1;

                            /* Bits[19:19], Access Type=RO/P, default=0x00000000*/

                            /* This is the INTx status */
    UINT32 cap_lst : 1;

                            /* Bits[20:20], Access Type=RO/P, default=0x00000001*/

                            /* This is the capability list */
    UINT32 x66mhz_cap : 1;

                            /* Bits[21:21], Access Type=RO/P, default=0x00000000*/

                            /* This is the X66MHz capable */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fast_b2b : 1;

                            /* Bits[23:23], Access Type=RO/P, default=0x00000000*/

                            /* This is the Fast back to back */
    UINT32 mstr_data_par_err : 1;

                            /* Bits[24:24], Access Type=RO/P, default=0x00000000*/

                            /* This is the master data parity error */
    UINT32 devsel_timing : 2;

                            /* Bits[26:25], Access Type=RO/P, default=0x00000000*/

                            /* This is the DEVSEL timing */
    UINT32 signaled_tar_abort : 1;

                            /* Bits[27:27], Access Type=RO/P, default=0x00000000*/

                            /* This is the signaled target abort */
    UINT32 rcvd_tar_abort : 1;

                            /* Bits[28:28], Access Type=RO/P, default=0x00000000*/

                            /* This is the received target abort */
    UINT32 rcvd_mstr_abort : 1;

                            /* Bits[29:29], Access Type=RO/P, default=0x00000000*/

                            /* This is the received master abort */
    UINT32 signaled_sys_err : 1;

                            /* Bits[30:30], Access Type=RO/P, default=0x00000000*/

                            /* This is the signaled system error */
    UINT32 detected_par_err : 1;

                            /* Bits[31:31], Access Type=RO/P, default=0x00000000*/

                            /* This is the detected parity error */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_4_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_8_FPGA_UPIPHY_FUNC3_REG supported on:                      */
/*      WHR (0x20023008)                                                        */
/* Register default value on WHR: 0x08800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset 8
*/


#define WHR_PH_PCIE_FUNC1_8_FPGA_UPIPHY_FUNC3_REG 0x1A320008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rev_id : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000000*/

                            /* This is the revision ID */
    UINT32 reg_lvl_prog_intf : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /* This is the register level programming interface */
    UINT32 sub_class : 8;

                            /* Bits[23:16], Access Type=RO/P, default=0x00000080*/

                            /* This is the sub class code */
    UINT32 base_class : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000008*/

                            /* This is the base class code */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_8_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_C_FPGA_UPIPHY_FUNC3_REG supported on:                      */
/*      WHR (0x2002300c)                                                        */
/* Register default value on WHR: 0x00800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset C
*/


#define WHR_PH_PCIE_FUNC1_C_FPGA_UPIPHY_FUNC3_REG 0x1A32000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cacheline_size : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000000*/

                            /* This is the cache line size */
    UINT32 primary_latency_timer : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /* This is the primary latency timer */
    UINT32 config_layout : 7;

                            /* Bits[22:16], Access Type=RO/P, default=0x00000000*/

                            /* This is the type 0 header */
    UINT32 multi_func_dev : 1;

                            /* Bits[23:23], Access Type=RO/P, default=0x00000001*/

                            /* This is the multi-function device */
    UINT32 bist_test : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* This is the BIST test */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_C_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_34_FPGA_UPIPHY_FUNC3_REG supported on:                     */
/*      WHR (0x20023034)                                                        */
/* Register default value on WHR: 0x00000040                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset 34
*/


#define WHR_PH_PCIE_FUNC1_34_FPGA_UPIPHY_FUNC3_REG 0x1A320034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_ptr : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000040*/

                            /* This is the capability structure pointer */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_34_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_3C_FPGA_UPIPHY_FUNC3_REG supported on:                     */
/*      WHR (0x2002303c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset 3C
*/


#define WHR_PH_PCIE_FUNC1_3C_FPGA_UPIPHY_FUNC3_REG 0x1A32003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 int_line : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000000*/

                            /* This is the Interrupt line */
    UINT32 int_pin : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /* This is the interrupt pin */
    UINT32 mgv : 8;

                            /* Bits[23:16], Access Type=RO/P, default=0x00000000*/

                            /* This is the minimum grant */
    UINT32 mlv : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* This is the maximum latency */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_3C_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_40_FPGA_UPIPHY_FUNC3_REG supported on:                     */
/*      WHR (0x20023040)                                                        */
/* Register default value on WHR: 0x00920010                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 1 offset 40
*/


#define WHR_PH_PCIE_FUNC1_40_FPGA_UPIPHY_FUNC3_REG 0x1A320040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000010*/

                            /* This is the capability ID */
    UINT32 next_ptr : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /* This is the no more capability structures */
    UINT32 cap_ver : 4;

                            /* Bits[19:16], Access Type=RO/P, default=0x00000002*/

                            /* This is the capability version */
    UINT32 dev_port_type : 4;

                            /* Bits[23:20], Access Type=RO/P, default=0x00000009*/

                            /*
                               This is the device type is root complex
                               integrated endpoint
                            */
    UINT32 slot_imp : 1;

                            /* Bits[24:24], Access Type=RO/P, default=0x00000000*/

                            /* This is the slot implemented */
    UINT32 int_msg_num : 5;

                            /* Bits[29:25], Access Type=RO/P, default=0x00000000*/

                            /* This is the interrupt message number */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_40_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_FUNC1_ENH_CAP_FPGA_UPIPHY_FUNC3_REG supported on:                */
/*      WHR (0x20023100)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe Enhanced Capability
*/


#define WHR_PH_PCIE_FUNC1_ENH_CAP_FPGA_UPIPHY_FUNC3_REG 0x1A320100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the There is no capability at this
                               location
                            */
    UINT32 cap_ver : 4;

                            /* Bits[19:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the There is no capability at this
                               location
                            */
    UINT32 next_cap_off : 12;

                            /* Bits[31:20], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Pointer to the next capability in
                               the enhanced configuration space. Set to 0 to
                               indicate there are no more capability
                               structures.
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC1_ENH_CAP_FPGA_UPIPHY_FUNC3_STRUCT;

/* WHR_PH_PCIE_UNUSED_FPGA_UPIPHY_FUNC3_REG supported on:                       */
/*      WHR (0x20023ffc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func1                */
/* Struct generated from WHR BDF: 0_4_3                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe Unused register
*/


#define WHR_PH_PCIE_UNUSED_FPGA_UPIPHY_FUNC3_REG 0x1A320FFC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pcie_unused : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Dummy entry to claim 8kB register
                               space
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_UNUSED_FPGA_UPIPHY_FUNC3_STRUCT;
#endif /* _FPGA_UPIPHY_FUNC3_h */
