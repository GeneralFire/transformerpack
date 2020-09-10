
/** @file
  FPGA_UPIPHY_FUNC2.h

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


#ifndef _FPGA_UPIPHY_FUNC2_h
#define _FPGA_UPIPHY_FUNC2_h
#include <Base.h>

/* WHR_PH_PCIE_FUNC0_0_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x20022000)                                                        */
/* Register default value on WHR: 0xBCC28086                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset 0
*/


#define WHR_PH_PCIE_FUNC0_0_FPGA_UPIPHY_FUNC2_REG 0x1A220000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vid : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00008086*/

                            /* This is the vendor ID */
    UINT32 did : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x0000BCC2*/

                            /* This is the device ID */

  } Bits;
  UINT32 Data;

} WHR_PH_PCIE_FUNC0_0_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_4_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x20022004)                                                        */
/* Register default value on WHR: 0x00100000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset 4
*/


#define WHR_PH_PCIE_FUNC0_4_FPGA_UPIPHY_FUNC2_REG 0x1A220004

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

} WHR_PH_PCIE_FUNC0_4_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_8_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x20022008)                                                        */
/* Register default value on WHR: 0x08800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset 8
*/


#define WHR_PH_PCIE_FUNC0_8_FPGA_UPIPHY_FUNC2_REG 0x1A220008

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

} WHR_PH_PCIE_FUNC0_8_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_C_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x2002200c)                                                        */
/* Register default value on WHR: 0x00800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset C
*/


#define WHR_PH_PCIE_FUNC0_C_FPGA_UPIPHY_FUNC2_REG 0x1A22000C

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

} WHR_PH_PCIE_FUNC0_C_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_34_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x20022034)                                                        */
/* Register default value on WHR: 0x00000040                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset 34
*/


#define WHR_PH_PCIE_FUNC0_34_FPGA_UPIPHY_FUNC2_REG 0x1A220034

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

} WHR_PH_PCIE_FUNC0_34_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_3C_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x2002203c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset 3C
*/


#define WHR_PH_PCIE_FUNC0_3C_FPGA_UPIPHY_FUNC2_REG 0x1A22003C

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

} WHR_PH_PCIE_FUNC0_3C_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_40_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x20022040)                                                        */
/* Register default value on WHR: 0x00920010                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe header function 0 offset 40
*/


#define WHR_PH_PCIE_FUNC0_40_FPGA_UPIPHY_FUNC2_REG 0x1A220040

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

} WHR_PH_PCIE_FUNC0_40_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TOPOLOGY_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x200220d4)                                                        */
/* Register default value on WHR: 0x00000030                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Wharfrock UPI PHY topology and func_mode
*/


#define WHR_PH_TOPOLOGY_FPGA_UPIPHY_FUNC2_REG 0x1A2200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 func_mode : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000003*/

                            /*
                               This is the 0 - UPI_X20_11P2G 1 - UPI_X20_10P4G
                               2 - UPI_X20_9P6G 3 - UPI_X20_11P2G_SLOW 4 -
                               UPI_X20_10P4G_SLOW 5 - UPI_X20_9P6G_SLOW 6 to 15
                               - DISABLE_FUNC_MODE
                            */
    UINT32 rsvd_8 : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hw_mode : 16;

                            /* Bits[31:16], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This is the 16'h8010 - UPI_X20_11P2G 16'h8011 -
                               UPI_X20_10P4G 16'h8012 - UPI_X20_9P6G 16'h8013 -
                               UPI_X20_11P2G_SLOW 16'h8014 - UPI_X20_10P4G_SLOW
                               16'h8015 - UPI_X20_9P6G_SLOW 16'h0000 - UPI
                               Disabled UPI PHY hw_mode decode will only
                               indicate these legal values. If user programmed
                               out-of-range values to func_mode or topology,
                               UPI PHY will default hw_mode to 16'h0000 to
                               indicate UPI Core is disabled.
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_TOPOLOGY_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PCIE_FUNC0_ENH_CAP_FPGA_UPIPHY_FUNC2_REG supported on:                */
/*      WHR (0x20022100)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PCIe Enhanced Capability
*/


#define WHR_PH_PCIE_FUNC0_ENH_CAP_FPGA_UPIPHY_FUNC2_REG 0x1A220100

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

} WHR_PH_PCIE_FUNC0_ENH_CAP_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_ERR_STS_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022104)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PHY agent fatal interrupt for RAS
*/


#define WHR_PH_ERR_STS_FPGA_UPIPHY_FUNC2_REG 0x1A220104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 28;

                            /* Bits[27:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cor_err : 1;

                            /* Bits[28:28], Access Type=RO/V/P, default=0x00000000*/

                            /* This is the Correctable error flag */
    UINT32 uncor_err : 1;

                            /* Bits[29:29], Access Type=RO/V/P, default=0x00000000*/

                            /* This is the Uncorrectable error flag */
    UINT32 fatal_err : 1;

                            /* Bits[30:30], Access Type=RO/V/P, default=0x00000000*/

                            /* This is the Fatal error flag */
    UINT32 error_vld : 1;

                            /* Bits[31:31], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This is the Set by hardware when a
                               fatal/uncorrectable/correctable error is logged.
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_ERR_STS_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_FATAL_INT_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x20022108)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the UPI PHY only has uncorrectable and correctable errors.
      For uniformity of register space, this register is also created
*/


#define WHR_PH_FATAL_INT_FPGA_UPIPHY_FUNC2_REG 0x1A220108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fatal_err : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Reserved space for fatal error
                               logging
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_FATAL_INT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_FATAL_INT_EN_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x2002210c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the UPI PHY only has uncorrectable and correctable errors.
      For uniformity of register space, this register for fatal error enable is also created.
*/


#define WHR_PH_FATAL_INT_EN_FPGA_UPIPHY_FUNC2_REG 0x1A22010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fatal_err_en : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Reserved space for fatal error
                               interrupt enable
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_FATAL_INT_EN_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_UE_INT_FPGA_UPIPHY_FUNC2_REG supported on:                            */
/*      WHR (0x20022110)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PHY agent uncorrectable (UC) error interrupt for RAS
*/


#define WHR_PH_UE_INT_FPGA_UPIPHY_FUNC2_REG 0x1A220110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 initcnt_exceed : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the If the user programs a non-zero
                               InitCount value, the Phy must train to
                               L0/Loopback/Compliance in NumInit attempts. On
                               each entry to ResetC, NumInit is incremented.
                               Should NumInit exceed a non-zero InitCount, an
                               error will be logged and the Phy LTSSM will idle
                               in ResetC.
                            */
    UINT32 driftbuffer_alarm : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Drift detected exceeds driftbuffer
                               lower limit and upper limit. Drift of the RX
                               serial bit stream in CDR clock domain will be
                               check against the UPI clock.
                            */
    UINT32 rxfifo_ovrwr : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Indicates a Latency Buffer Rollover
                               was detected by the PHY. RXFIFO overwritten.
                               This could be caused by deskew error or latency
                               fixing or incorrect FIFO Lower Limit and Upper
                               Limit setting
                            */
    UINT32 txfifo_ovrwr : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the UC Reserved. Indicates a Latency
                               Buffer Rollover was detected by the PHY. TXFIFO
                               overwritten. This could be caused by latency
                               fixing
                            */
    UINT32 rxfifo_ecc_uncor : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the UC Reserved. RXFIFO ECC
                               uncorrectable error.
                            */
    UINT32 txfifo_ecc_uncor : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the UC Reserved. TXFIFO ECC
                               uncorrectable error.
                            */
    UINT32 ibist_err : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the UC Reserved. Indicates that the PHY
                               detected an IBIST error during Loopback.
                            */
    UINT32 skew_exceed : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Interlane skew exceed 64UI 1/2TSL
                               (PH_KES)
                            */
    UINT32 deskew_err : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the TS lock shifted after deskew done */
    UINT32 ppm_exceed : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the PPM detected in CDR during slowmode
                               exceeds design limit
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_UE_INT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_UE_INT_EN_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x20022114)                                                        */
/* Register default value on WHR: 0x00000387                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PHY agent uncorrectable (UC) interrupt enable for RAS
*/


#define WHR_PH_UE_INT_EN_FPGA_UPIPHY_FUNC2_REG 0x1A220114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 initcnt_exceed_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the UC Init Count Exceeded interrupt
                               enable
                            */
    UINT32 driftbuffer_alarm_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the UC Drift buffer alarm interrupt
                               enable
                            */
    UINT32 rxfifo_ovrwr_en : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the UC Phy Detected Latency Buffer
                               Rollover interrupt enable
                            */
    UINT32 txfifo_ovrwr_en : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* This is the UC Reserved */
    UINT32 rxfifo_ecc_uncor_en : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the UC Reserved */
    UINT32 txfifo_ecc_uncor_en : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* This is the UC Reserved */
    UINT32 ibist_err_en : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* This is the UC Reserved */
    UINT32 skew_exceed_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* This is the UC Lane skew exceed interrupt enable */
    UINT32 deskew_err_en : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the UC Lane deskew error interrupt
                               enable
                            */
    UINT32 ppm_exceed_en : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the UC PPM exceed limit error interrupt
                               enable
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_UE_INT_EN_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_CE_INT_FPGA_UPIPHY_FUNC2_REG supported on:                            */
/*      WHR (0x20022118)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PHY agent correctable (COR) interrupt for RAS
*/


#define WHR_PH_CE_INT_FPGA_UPIPHY_FUNC2_REG 0x1A220118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0c_mismatch : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the If the voting on the L0c cannot find
                               four matching Rx L0c sequences among the 8
                               participating lanes, it will log an error. Rx
                               flit traffic will be shut down by deassertiong
                               RxValid. On the next Tx L0c it will initiate an
                               inband reset to recover the link.
                            */
    UINT32 l0c_biterr : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the If the Rx L0c voting detects any L0c
                               sequence miscompares among the 8 participating
                               lanes, it will log an error. Assuming there is
                               no L0c Mismatch error, there are no implications
                               to link operation. This means that L0c error was
                               corrected without Phy Reset
                            */
    UINT32 l0p_exiterr : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the COR Reserved. On L0p exit the KTI
                               Phy needs to rebyte lock, deskew idle lanes, and
                               deskew idle lanes with the active lanes. If any
                               of these functions fails, an error will be
                               logged.
                            */
    UINT32 l0c_unsup : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Must qual with PH_CAP and PH_CTR1 on
                               L0p and L1 support. When RX receives valid
                               L0cwhich is not supported by WHR UPI PHY
                               capability, this error will trigger and the phy
                               will enter reset.
                            */
    UINT32 rst_lw : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Indicates a reset was received by
                               the PHY and the link width did not change after
                               re-init.
                            */
    UINT32 rst_lw_change : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the COR Reserved. Indicates a Reset was
                               received by the PHY and the link width changed
                               after re-init.
                            */
    UINT32 clm_unsup : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Unsupported CLM is decoded in the
                               Training Sequence. (3'b100 or 3'b001)
                            */
    UINT32 phy_err : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Loss of 4k count of WakeDetect/LOS,
                               CDR lose lock and Slow mode eye lose lock
                               LTSSM.CONFIG* to LTSSM.L0/L0c will cause misc
                               phy error to be triggered.
                            */
    UINT32 rxfifo_ecc_cor : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the COR Reserved */
    UINT32 txfifo_ecc_cor : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the COR Reserved */
    UINT32 init_abort : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Init abort occurs when init states
                               fail to make progress in the state after
                               secondary timer expires.
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_CE_INT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_CE_INT_EN_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x2002211c)                                                        */
/* Register default value on WHR: 0x0000009B                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PHY agent correctable (COR) interrupt enable for RAS
*/


#define WHR_PH_CE_INT_EN_FPGA_UPIPHY_FUNC2_REG 0x1A22011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0c_mismatch_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* This is the COR L0c mismatch interrupt enable */
    UINT32 l0c_biterr_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* This is the COR L0c bit error interrupt enable */
    UINT32 l0p_exiterr_en : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* This is the COR Reserved */
    UINT32 l0c_unsup_en : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* This is the COR Unsupported L0c interrupt enable */
    UINT32 rst_lw_en : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the COR Reset receiver with no link
                               width change interrupt enable
                            */
    UINT32 rst_lw_change_en : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* This is the COR Reserved */
    UINT32 clm_unsup_en : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Unsupported CLM is decoded in the
                               Training Sequence. (3'b100 or 3'b001)
                            */
    UINT32 phy_err_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* This is the COR Misc Phy error interrupt enable */
    UINT32 rxfifo_ecc_cor_en : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the COR Reserved */
    UINT32 txfifo_ecc_cor_en : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* This is the COR Reserved */
    UINT32 init_abort_en : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* This is the COR Init Abort interrupt enable */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_CE_INT_EN_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_ERRCNT0_EN_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x20022120)                                                        */
/* Register default value on WHR: 0x0000009B                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This mask is used to select which error are
      counted as part of error 0 counter. When the bit is set, the error type is
      included in the count.
*/


#define WHR_PH_ERRCNT0_EN_FPGA_UPIPHY_FUNC2_REG 0x1A220120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcnt0_en : 21;

                            /* Bits[20:0], Access Type=RW/P, default=0x0000009B*/

                            /*
                               [This is the 20] - UC PPM exceed limit error
                               enable [19] - UC Lane deskew error error count 0
                               enable [18] - UC Lane skew exceed error count 0
                               enable [17] - Reserved. Ibist error error count
                               0 enable [16] - Reserved. TXFIFO ECC
                               uncorrectable error error count 0 enable [15] -
                               Reserved. RXFIFO ECC uncorrectable error error
                               count 0 enable [14] - Reserved. Phy Detected
                               Latency Buffer Rollover on TX FIFO error count 0
                               enable [13] - UC Phy Detected Latency Buffer
                               Rollover error count 0 enable [12] - UC Drift
                               buffer alarm error count 0 enable [11] - UC Init
                               Count Exceeded error count 0 enable [10] - COR
                               Init Abort error count 0 enable [9] - Reserved.
                               TXFIFO ECC correctable error error count 0
                               enable [8] - Reserved. RXFIFO ECC correctable
                               error error count 0 enable [7] - COR Misc Phy
                               error error count 0 enable [6] - COR Unsupported
                               CLM error count 0 enable [5] - Reserved. Reset
                               with link width change error count 0 enable [4]
                               - COR Reset receiver with no link width change
                               error count 0 enable [3] - COR Unsupported L0c
                               error count 0 enable [2] - Reserved. L0p exit
                               error interrupt enable [1] - COR L0c bit error
                               interrupt enable [0] - COR L0c mismatch error
                               count 0 enable
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_ERRCNT0_EN_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_ERRCNT0_STS_FPGA_UPIPHY_FUNC2_REG supported on:                       */
/*      WHR (0x20022124)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This mask is used to select which error are
      counted as part of error 0 counter. When the bit is set, the error type is
      included in the count.
*/


#define WHR_PH_ERRCNT0_STS_FPGA_UPIPHY_FUNC2_REG 0x1A220124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcnt0 : 15;

                            /* Bits[14:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Counter is cleared by writing all
                               ONES to both the errcnt0 and the 1 bit overflow
                            */
    UINT32 rsvd : 16;

                            /* Bits[30:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errcnt0_ovf : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the Error Counter 0 overflow */

  } Bits;
  UINT32 Data;

} WHR_PH_ERRCNT0_STS_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_ERRCNT1_EN_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x20022128)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This mask is used to select which error are
      counted as part of error 1 counter. When the bit is set, the error type is
      included in the count.
*/


#define WHR_PH_ERRCNT1_EN_FPGA_UPIPHY_FUNC2_REG 0x1A220128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcnt1_en : 21;

                            /* Bits[20:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               [This is the 20] - UC PPM exceed limit error
                               enable [19] - UC Lane deskew error error count 1
                               enable [18] - UC Lane skew exceed error count 1
                               enable [17] - Reserved. Ibist error error count
                               1 enable [16] - Reserved. TXFIFO ECC
                               uncorrectable error error count 1 enable [15] -
                               Reserved. RXFIFO ECC uncorrectable error error
                               count 1 enable [14] - Reserved. Phy Detected
                               Latency Buffer Rollover on TX FIFO error count 1
                               enable [13] - UC Phy Detected Latency Buffer
                               Rollover error count 1 enable [12] - UC Drift
                               buffer alarm error count 1 enable [11] - UC Init
                               Count Exceeded error count 1 enable [10] - COR
                               Init Abort error count 1 enable [9] - Reserved.
                               TXFIFO ECC correctable error error count 1
                               enable [8] - Reserved. RXFIFO ECC correctable
                               error error count 1 enable [7] - COR Misc Phy
                               error error count 1 enable [6] - COR Unsupported
                               CLM error count 1 enable [5] - Reserved. Reset
                               with link width change error count 1 enable [4]
                               - COR Reset receiver with no link width change
                               error count 1 enable [3] - COR Unsupported L0c
                               error count 1 enable [2] - Reserved. L0p exit
                               error interrupt enable [1] - COR L0c bit error
                               interrupt enable [0] - COR L0c mismatch error
                               count 1 enable
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_ERRCNT1_EN_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_ERRCNT1_STS_FPGA_UPIPHY_FUNC2_REG supported on:                       */
/*      WHR (0x2002212c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This mask is used to select which error are
      counted as part of error 1 counter. When the bit is set, the error type is
      included in the count.
*/


#define WHR_PH_ERRCNT1_STS_FPGA_UPIPHY_FUNC2_REG 0x1A22012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcnt1 : 15;

                            /* Bits[14:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Counter is cleared by writing all
                               ONES to both the errcnt1 and the 1 bit overflow
                            */
    UINT32 rsvd : 16;

                            /* Bits[30:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errcnt1_ovf : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the Error Counter 1 overflow */

  } Bits;
  UINT32 Data;

} WHR_PH_ERRCNT1_STS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TPRI_SL_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x20022200)                                                        */
/* Register default value on WHR: 0x00C13344                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tpri Exponential Timer Set For Slow Speed
*/


#define PH_TPRI_SL_FPGA_UPIPHY_FUNC2_REG 0x1A220200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ttc : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000004*/

                            /* This is the For stagger on. Granularity of 128 */
    UINT32 trc : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               This is the For stagger Tx off. Granularity of
                               128
                            */
    UINT32 trs : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000003*/

                            /* This is the Granularity of 128 */
    UINT32 trdb : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000003*/

                            /*
                               This is the Should be programmed to same value
                               on both sides of link. Granularity of 128
                            */
    UINT32 ntdr : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the Number of consecutive 4K UI windows
                               during which wake must be detected for debounce
                               success in Tx Detect.
                            */
    UINT32 ntdt : 8;

                            /* Bits[27:20], Access Type=RW/P, default=0x0000000C*/

                            /*
                               This is the Number of TxDetect wake
                               supersequence (PatDetect) count for handshake
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TPRI_SL_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TSEC_SL_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x20022204)                                                        */
/* Register default value on WHR: 0x33406440                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tsec Exponential Timer Set For Slow Speed
*/


#define PH_TSEC_SL_FPGA_UPIPHY_FUNC2_REG 0x1A220204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlmst : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the For Tx adaptation in loopback
                               master-master mode non-zero value should be used
                               for fast speed (e.g. value used for Tclst).
                               Tlmst is typically 0. For debug, CSingleStep and
                               CFIA (with non-zero Tlmst) can be used.
                               Granularity of 4K
                            */
    UINT32 tcfst : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               This is the If determinism is enabled, Tcfst
                               must be greater than EPhyL0Sync period.
                               Granularity of 4K
                            */
    UINT32 tclst : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000004*/

                            /* This is the Granularity of 4K */
    UINT32 tplst : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 4K */
    UINT32 trdst : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 4K */
    UINT32 ttdst : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000004*/

                            /*
                               This is the This value (in UI) should be larger
                               than TL0c. Granularity of 4K
                            */
    UINT32 ttcst : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000003*/

                            /* This is the Granularity of 4K */
    UINT32 trcst : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000003*/

                            /* This is the Granularity of 4K */

  } Bits;
  UINT32 Data;

} PH_TSEC_SL_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0CENTRY_SL_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x20022208)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0centry Linear Timer Set For Slow Speed
*/


#define PH_TL0CENTRY_SL_FPGA_UPIPHY_FUNC2_REG 0x1A220208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tieo : 12;

                            /* Bits[11:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TL0CENTRY_SL_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0C_SL_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x2002220c)                                                        */
/* Register default value on WHR: 0x00010021                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0c Linear Timer Set For Slow Speed
*/


#define PH_TL0C_SL_FPGA_UPIPHY_FUNC2_REG 0x1A22020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0c : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000021*/

                            /* This is the Granularity of 128 */
    UINT32 tl1reissuedelay : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 512 */
    UINT32 tl0cq : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* This is the Value must be 1. Granularity of 8 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TL0C_SL_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TPRI_OP_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x20022210)                                                        */
/* Register default value on WHR: 0x00C133B4                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tpri Exponential Timer Set For Operational Speed
*/


#define PH_TPRI_OP_FPGA_UPIPHY_FUNC2_REG 0x1A220210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ttc : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000004*/

                            /* This is the For stagger on. Granularity of 128 */
    UINT32 trc : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x0000000B*/

                            /*
                               This is the For stagger Tx off. Granularity of
                               128
                            */
    UINT32 trs : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000003*/

                            /* This is the Granularity of 128 */
    UINT32 trdb : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000003*/

                            /*
                               This is the Should be programmed to same value
                               on both sides of link. Granularity of 128
                            */
    UINT32 ntdr : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the Number of consecutive 4K UI windows
                               during which wake must be detected for debounce
                               success in Tx Detect.
                            */
    UINT32 ntdt : 8;

                            /* Bits[27:20], Access Type=RW/P, default=0x0000000C*/

                            /*
                               This is the Number of TxDetect wake
                               supersequence (PatDetect) count for handshake
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TPRI_OP_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TSEC_OP_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x20022214)                                                        */
/* Register default value on WHR: 0xA940A440                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tsec Exponential Timer Set For Operational Speed
*/


#define PH_TSEC_OP_FPGA_UPIPHY_FUNC2_REG 0x1A220214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlmst : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the For Tx adaptation in loopback
                               master-master mode non-zero value should be used
                               for fast speed (e.g. value used for Tclst).
                               Tlmst is typically 0. For debug, CSingleStep and
                               CFIA (with non-zero Tlmst) can be used.
                               Granularity of 4K
                            */
    UINT32 tcfst : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000004*/

                            /*
                               This is the If determinism is enabled, Tcfst
                               must be greater than EPhyL0Sync period.
                               Granularity of 4K
                            */
    UINT32 tclst : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000004*/

                            /* This is the Granularity of 4K */
    UINT32 tplst : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x0000000A*/

                            /* This is the Granularity of 4K */
    UINT32 trdst : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 4K */
    UINT32 ttdst : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000004*/

                            /*
                               This is the This value (in UI) should be larger
                               than TL0c. Granularity of 4K
                            */
    UINT32 ttcst : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000009*/

                            /* This is the Granularity of 4K */
    UINT32 trcst : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x0000000A*/

                            /* This is the Granularity of 4K */

  } Bits;
  UINT32 Data;

} PH_TSEC_OP_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0CENTRY_OP_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x20022218)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0centry Linear Timer Set For Operational Speed
*/


#define PH_TL0CENTRY_OP_FPGA_UPIPHY_FUNC2_REG 0x1A220218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tieo : 12;

                            /* Bits[11:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TL0CENTRY_OP_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0C_OP_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x2002221c)                                                        */
/* Register default value on WHR: 0x00010021                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0c Linear Timer Set For Operational Speed
*/


#define PH_TL0C_OP_FPGA_UPIPHY_FUNC2_REG 0x1A22021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0c : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000021*/

                            /* This is the Granularity of 128 */
    UINT32 tl1reissuedelay : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 512 */
    UINT32 tl0cq : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000001*/

                            /* This is the Value must be 1. Granularity of 8 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TL0C_OP_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0PENTRYTX_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022220)                                                        */
/* Register default value on WHR: 0x06000664                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0pentrytx Linear Timer Set
*/


#define PH_TL0PENTRYTX_FPGA_UPIPHY_FUNC2_REG 0x1A220220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0ptxtran2done : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000064*/

                            /* This is the Granularity of 8 */
    UINT32 tl0ptxenter2tran : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 8 */
    UINT32 tl0ptxnullstart2nullstop : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 tl0ptxenter2null : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 8 */

  } Bits;
  UINT32 Data;

} PH_TL0PENTRYTX_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0PENTRYRX_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022224)                                                        */
/* Register default value on WHR: 0x06000664                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0pentryrx Linear Timer Set
*/


#define PH_TL0PENTRYRX_FPGA_UPIPHY_FUNC2_REG 0x1A220224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0prxtran2done : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000064*/

                            /* This is the Granularity of 8 */
    UINT32 tl0prxenter2tran : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 8 */
    UINT32 tl0prxtran2unblock : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 tl0prxenter2block : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 8 */

  } Bits;
  UINT32 Data;

} PH_TL0PENTRYRX_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0PEXITTXACT_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x20022228)                                                        */
/* Register default value on WHR: 0x00001E00                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0pexittxact Linear Timer Set
*/


#define PH_TL0PEXITTXACT_FPGA_UPIPHY_FUNC2_REG 0x1A220228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0ptxnullsexittran : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 tl0ptxsqend2sds : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x0000001E*/

                            /* This is the Granularity of 128 */
    UINT32 tl0ptxnullsexitsq : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 tl0ptxexitend2l0c : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */

  } Bits;
  UINT32 Data;

} PH_TL0PEXITTXACT_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0PEXITTXIDL_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x2002222c)                                                        */
/* Register default value on WHR: 0x04990620                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0pexittxidl Linear Timer Set
*/


#define PH_TL0PEXITTXIDL_FPGA_UPIPHY_FUNC2_REG 0x1A22022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0ptxexit2next : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000020*/

                            /* This is the Granularity of 128 */
    UINT32 tl0ptxsds2widthchg : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 8 */
    UINT32 tl0ptxsds : 2;

                            /* Bits[17:16], Access Type=RO/P, default=0x00000001*/

                            /* This is the Granularity of 128 */
    UINT32 tl0ptxpat : 8;

                            /* Bits[25:18], Access Type=RW/P, default=0x00000026*/

                            /* This is the Granularity of 128 */
    UINT32 tl0ptxsbpa : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000001*/

                            /* This is the Granularity of 128 */
    UINT32 tl0ptxnull2firston : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */

  } Bits;
  UINT32 Data;

} PH_TL0PEXITTXIDL_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL0PEXITRX_FPGA_UPIPHY_FUNC2_REG supported on:                            */
/*      WHR (0x20022230)                                                        */
/* Register default value on WHR: 0x00270600                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl0pexitrx Linear Timer Set
*/


#define PH_TL0PEXITRX_FPGA_UPIPHY_FUNC2_REG 0x1A220230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl0prxnullswidthchg : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */
    UINT32 tl0prxsds2widthchg : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000006*/

                            /* This is the Granularity of 8 */
    UINT32 tl0pest : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000027*/

                            /* This is the Granularity of 128 */
    UINT32 tl0prxnullsexitsq : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 8 */

  } Bits;
  UINT32 Data;

} PH_TL0PEXITRX_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TL1ENTRY_FPGA_UPIPHY_FUNC2_REG supported on:                              */
/*      WHR (0x20022234)                                                        */
/* Register default value on WHR: 0x0F00004A                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for ph_tl1entry Linear Timer Set
*/


#define PH_TL1ENTRY_FPGA_UPIPHY_FUNC2_REG 0x1A220234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tl1minres : 12;

                            /* Bits[11:0], Access Type=RW/P, default=0x0000004A*/

                            /* This is the Granularity of 128 */
    UINT32 rsvd : 8;

                            /* Bits[19:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tl1txfirstoff2lastoff : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* This is the Granularity of 128 */
    UINT32 tl1txenter2firstoff : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x0000000F*/

                            /* This is the Granularity of 8 */

  } Bits;
  UINT32 Data;

} PH_TL1ENTRY_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_CPR_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022240)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Capability Register
*/


#define PH_CPR_FPGA_UPIPHY_FUNC2_REG 0x1A220240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 interconnect_version : 4;

                            /* Bits[3:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Version of UPI the port is compliant
                               with. KTI 1.0 = 0b0000; Others Reserved
                            */
    UINT32 ccl0p : 1;

                            /* Bits[4:4], Access Type=RO/P, default=0x00000000*/

                            /* This is the 1:support L0p; enabled by CL0pEn. */
    UINT32 ccl1 : 1;

                            /* Bits[5:5], Access Type=RO/P, default=0x00000000*/

                            /* This is the 1:support L1; enabled by CL1En. */
    UINT32 ccdeterminism : 1;

                            /* Bits[6:6], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the 1:support Latency fixing and
                               alignment of exit from init aligned to external
                               signal; enabled by CDetEn.
                            */
    UINT32 cctxadaptsettings : 8;

                            /* Bits[14:7], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Number of Tx adapt settings that can
                               be tried in one iteration; Tx adaptation enabled
                               by CTxAdaptEn. 0 means Tx adaptation not
                               supported
                            */
    UINT32 cctdeterminism : 1;

                            /* Bits[15:15], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the 1:support tester determinism (ATE);
                               enabled by CATE.
                            */
    UINT32 ccfailover : 1;

                            /* Bits[16:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the 0:not capable of failover on fault;
                               1: capable of failover on fault; enabled by
                               CFailoverEn.
                            */
    UINT32 ccbdeterminism : 1;

                            /* Bits[17:17], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the 1:support basic determinism i.e.
                               latency fixing; enabled by CLatFixEn.
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_CPR_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_CTR1_FPGA_UPIPHY_FUNC2_REG supported on:                                  */
/*      WHR (0x20022244)                                                        */
/* Register default value on WHR: 0x20000020                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Control Register 1
*/


#define PH_CTR1_FPGA_UPIPHY_FUNC2_REG 0x1A220244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cpreset : 1;

                            /* Bits[0:0], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the Resulting hw actions restore all
                               status RO-V bits to default values. Cleared by
                               hardware on entering ResetC.
                            */
    UINT32 csinglestep : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Forward progess on clearing of
                               SPSSHold. Setting this bit also sets SPSSHold.
                            */
    UINT32 cfia : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:enable Freeze on init abort;
                               backward progress on clearing of SPSSHold
                            */
    UINT32 cate : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:enable ATE mode; forward progress
                               on timeout
                            */
    UINT32 copspeed : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1:op speed; 0:slow speed */
    UINT32 cinitbegin : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* This is the 0:blocks exit from ResetC */
    UINT32 cinit : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0:Init to L0; 1:CLocalLBM;
                               2:CLocalComplianceSlave; 3:
                               CLocalComplianceMaster
                            */
    UINT32 cdeten : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0:no determinism; 1:determinism
                               enabled
                            */
    UINT32 cdetslave : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0:Determinism master; 1:Determinism
                               slave
                            */
    UINT32 creinitprbs : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Should be same on both sides of link
                               0:no reinit of PRBS generating LFSRs 1:reinit
                               after every EIEOS 2/3:reserved
                            */
    UINT32 claloaddisable : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:disable loading of effective
                               values from late action registers in reset
                            */
    UINT32 cnuminit : 8;

                            /* Bits[20:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 00:no limit on consecutive init
                               iterations Others:number of init iterations
                               attempted before coming to rest in ResetC
                            */
    UINT32 cenablephyresetd : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:drift alarm initates a PHY reset;
                               late action
                            */
    UINT32 cl0pen : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1:enables L0p */
    UINT32 cl1en : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1:enables L1 */
    UINT32 rsvd : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cenhqualen : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:enables enhanced (matching lane
                               id) lane qualification
                            */
    UINT32 ccompbypen : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Ignore wake detection, adaptation
                               and so on prior to bitlock Used by compliance
                               slave
                            */
    UINT32 cfailoveren : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1:enable failover */
    UINT32 cenablephyresetw : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:loss of acquired wake detect
                               during init initates phy reset
                            */
    UINT32 crctermsoff : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* This is the 1:Rx pulldowns are off in ResetC */
    UINT32 crxresetdis : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:Disables RxReset for Testing
                               purposes
                            */
    UINT32 clatfixen : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1:enables latency fixing */

  } Bits;
  UINT32 Data;

} PH_CTR1_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_CTR2_FPGA_UPIPHY_FUNC2_REG supported on:                                  */
/*      WHR (0x20022248)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Control Register 2
*/


#define PH_CTR2_FPGA_UPIPHY_FUNC2_REG 0x1A220248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxadapten : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1:enable Tx Adaptation */
    UINT32 ctxadaptsettings : 8;

                            /* Bits[8:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Number of Tx adapt settings that are
                               tried in one iteration
                            */
    UINT32 ctxadaptpat : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 1:use loopback master pattern based
                               on pat gen mux; 0:use L0 PRBS
                            */
    UINT32 csbbackchannel : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0:use slow mode backchannel; 1:use
                               sideband backchannel
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_CTR2_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TDC_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x2002224c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Tx Data Lane Control Register
*/


#define PH_TDC_FPGA_UPIPHY_FUNC2_REG 0x1A22024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxdatalanedisable : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Used for debug and validation
                               purposes also. A bit value of 1 indicates the
                               corresponding lane is disabled
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TDC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_RDC_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022250)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Data Lane Control Register
*/


#define PH_RDC_FPGA_UPIPHY_FUNC2_REG 0x1A220250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crxdatalanedisable : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Used for debug and validation
                               purposes also. A bit value of 1 indicates the
                               corresponding lane is disabled
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_RDC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_LDC_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022254)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Link Determinism Control Register
*/


#define PH_LDC_FPGA_UPIPHY_FUNC2_REG 0x1A220254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cdriftalarmthreshold : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the When the difference (both underflow
                               and overflow) between drift buffer read and
                               write pointers(depth) is less than the value in
                               this field, drift buffer alarm status will be
                               set
                            */
    UINT32 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cdriftdepth : 6;

                            /* Bits[13:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the The field indicates the amount in UI
                               by which read pointer is intended to lag the
                               write pointer, and is to be set during Polling
                               in all lanes.
                            */
    UINT32 ctargetlatency : 9;

                            /* Bits[22:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Target latency at Rx is 8 UI
                               granularity
                            */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LDC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TEC_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022258)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Lane Address into Best Rx Metric Memory
*/


#define PH_TEC_FPGA_UPIPHY_FUNC2_REG 0x1A220258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 claneaddress : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               'This is the x00=Lane 0; x01=Lane 1; x02=Lane 2
                               ... etc.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TEC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_LCS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022260)                                                        */
/* Register default value on WHR: 0x00001210                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Late Action Current Status Register
*/


#define PH_LCS_FPGA_UPIPHY_FUNC2_REG 0x1A220260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 senablephyresetd : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the description for Updated from
                               CEnablePhyResetd on exit from ResetC
                            */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sincludedroppedlanes : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the description for Updated from
                               CIncludeDroppedLanes on exit from ResetC
                            */
    UINT32 stl0c : 8;

                            /* Bits[11:4], Access Type=RO/V, default=0x00000021*/

                            /*
                               This is the This value is loaded into the local
                               TL0ct and TL0cr timer under PHY LTSSM control.
                            */
    UINT32 stl0cq : 4;

                            /* Bits[15:12], Access Type=RO/V, default=0x00000001*/

                            /*
                               This is the This value is loaded into the local
                               TL0cqt and TL0cqr timer under PHY LTSSM control.
                            */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LCS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_CIS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022264)                                                        */
/* Register default value on WHR: 0x0000000A                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Current Init Status Register
*/


#define PH_CIS_FPGA_UPIPHY_FUNC2_REG 0x1A220264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sopspeed : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 0:Slow speed, 1:Op speed. Updated at
                               entry and exit from ResetC.
                            */
    UINT32 sprxcalib : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000001*/

                            /*
                               This is the 1:Rx calib done; sw clears by
                               writing 1 prior to speed change to force re-
                               calib.
                            */
    UINT32 spdetect : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the 1:Remote Rx detected by Tx. */
    UINT32 sptxcalib : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000001*/

                            /*
                               This is the 1:Tx calib done; sw clears by
                               writing 1 prior to speed change to force re-
                               calib.
                            */
    UINT32 swakedetected : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 1:Wake detected on one or more
                               lanes.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spusebackchannel : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This is the 1:Backchannel in use after Tx Adapt
                               done; sw clears by writting 1 after backchannel
                               operations completed.
                            */
    UINT32 sptxadapt : 8;

                            /* Bits[14:7], Access Type=RO/V, default=0x00000000*/

                            /* This is the description for Tx Adapt index */
    UINT32 slinkup : 2;

                            /* Bits[16:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 00:STXLinkUp FALSE, SRxLinkUp FALSE
                               01:STXLinkUp FALSE, SRxLinkUp TRUE 10:STXLinkUp
                               TRUE, SRxLinkUp FALSE 11:STXLinkUp TRUE,
                               SRxLinkUp TRUE
                            */
    UINT32 sinitcount : 8;

                            /* Bits[24:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the description for Init iterations
                               count(no rollover)
                            */
    UINT32 rsvd_25 : 2;

                            /* Bits[26:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sremotelbm : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Set if received TS destination state
                               says loopback master
                            */
    UINT32 srxlanerev : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* This is the description for Lane Reversal at Rx */
    UINT32 spsshold : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the 1:stay in state, 0:exit state. */
    UINT32 sinitfail : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 0:init progress, 1:secondary timeout
                               and init fail
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_CIS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_PIS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022268)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Previous (to Init Abort) Init Status Register
*/


#define PH_PIS_FPGA_UPIPHY_FUNC2_REG 0x1A220268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sopspeedp : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 0:Slow speed, 1:Op speed. Updated at
                               entry and exit from ResetC.
                            */
    UINT32 sprxcalibp : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Rx calib done; sw clears by
                               writing 1 prior to speed change to force re-
                               calib.
                            */
    UINT32 spdetectp : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the 1:Remote Rx detected by Tx. */
    UINT32 sptxcalibp : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Tx calib done; sw clears by
                               writing 1 prior to speed change to force re-
                               calib.
                            */
    UINT32 swakedetectedp : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Wake detected on one or more
                               lanes.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spusebackchannelp : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Backchannel in use after Tx Adapt
                               done; sw clears by writting 1 after backchannel
                               operations completed.
                            */
    UINT32 sptxadaptp : 8;

                            /* Bits[14:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the description for Tx Adapt index */
    UINT32 slinkupp : 2;

                            /* Bits[16:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 00:STXLinkUp FALSE, SRxLinkUp FALSE
                               01:STXLinkUp FALSE, SRxLinkUp TRUE 10:STXLinkUp
                               TRUE, SRxLinkUp FALSE 11:STXLinkUp TRUE,
                               SRxLinkUp TRUE
                            */
    UINT32 sinitcountp : 8;

                            /* Bits[24:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the description for Init iterations
                               count(no rollover)
                            */
    UINT32 rsvd_25 : 2;

                            /* Bits[26:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sremotelbmp : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Set if received TS destination state
                               says loopback master
                            */
    UINT32 srxlanerevp : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the description for Lane Reversal at Rx */
    UINT32 spssholdp : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the 1:stay in state, 0:exit state. */
    UINT32 sinitfailp : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 0:init progress, 1:secondary timeout
                               and init fail
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_PIS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_CSS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x2002226c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Currect Link State Status Register
*/


#define PH_CSS_FPGA_UPIPHY_FUNC2_REG 0x1A22026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stxstate : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the description for Tx State */
    UINT32 srxstate : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the description for Rx State */
    UINT32 sclm : 3;

                            /* Bits[18:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Lane map code determined at Rx and
                               sent in Tx. Config for remote Tx; also updated
                               on L0p entry/exit. Encoded as follows: 0b111 -
                               lanes 0..19 0b001 - lanes 0..7 0b100 - lanes
                               12..19
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_CSS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_PSS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022270)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Previous Link State Status Register
*/


#define PH_PSS_FPGA_UPIPHY_FUNC2_REG 0x1A220270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stxstatep : 8;

                            /* Bits[7:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the description for Tx State */
    UINT32 srxstatep : 8;

                            /* Bits[15:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the description for Rx State */
    UINT32 sclmp : 3;

                            /* Bits[18:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Lane map code determined at Rx and
                               sent in Tx. Config for remote Tx; also updated
                               on L0p entry/exit. Encoded as follows: 0b111 -
                               lanes 0..19 0b001 - lanes 0..7 0b100 - lanes
                               12..19
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_PSS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TDS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022274)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Tx Data Lane Dropped Status Register
*/


#define PH_TDS_FPGA_UPIPHY_FUNC2_REG 0x1A220274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stxlanedropped : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the A bit of value of 1 indicates Tx
                               lane is TxLaneDropped: Bit 0:Status of Physical
                               lane 0. Bit 1:Status of Physical lane 1. .. and
                               so on
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TDS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_RDS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022278)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Data Lane Dropped Status Register
*/


#define PH_RDS_FPGA_UPIPHY_FUNC2_REG 0x1A220278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srxlanedropped : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the A bit of value of 1 indicates Rx
                               lane is RxLaneDropped: Bit 0:Status of Physical
                               lane 0. Bit 1:Status of Physical lane 1. .. and
                               so on
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_RDS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_YIS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x2002227c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Data Lane Polarity Inversion Status Register
*/


#define PH_YIS_FPGA_UPIPHY_FUNC2_REG 0x1A22027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srxpolinv : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the A bit of value of 1 indicates Rx
                               lane has detected polarity inversion. Bit
                               0:Status of Physical lane 0. Bit 1:Status of
                               Physical lane 1. .. and so on
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_YIS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_DIS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022280)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Physical Layer Determinism Init Status Register
*/


#define PH_DIS_FPGA_UPIPHY_FUNC2_REG 0x1A220280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slatadded : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 7 - Reserved 6:0 - Latency added
                               after deskew div 8 UI, i.e., 8 UI granularity
                            */
    UINT32 srxobsdlatency : 12;

                            /* Bits[19:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Rx sync counter value when EIEOS of
                               TS.Polling received at latency buffer
                            */
    UINT32 rsvd : 4;

                            /* Bits[23:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 slatbuferror : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 1 - latency no fixable i.e. fix
                               value larger than latency buffer depth; init
                               continues without latency fixing
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_DIS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_CLS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022284)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Current Physical Link Status Register
*/


#define PH_CLS_FPGA_UPIPHY_FUNC2_REG 0x1A220284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sl1rerr : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This is the description for L1r received at
                               wrong time
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spdriftalarm : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This is the 1:Indicates drift buffer underflow
                               or overflow alarm
                            */
    UINT32 sdriftalarmlaneaddress : 5;

                            /* Bits[7:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the The lane ID of first lane that
                               caused Drift Buffer Alarm to set. Valid only
                               when alarm is set
                            */
    UINT32 sl0pefail : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the 1:L0p exit failed at Rx */
    UINT32 sl0pelatsub : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 1:Latency subtracted on at least one
                               idle lane during L0p exit
                            */
    UINT32 sl0pelatadd : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the 1:Latency added on at least one idle
                               lane during L0p exit
                            */
    UINT32 sl1rout : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Set when Ql1r sent; cleared when
                               QL1n received
                            */
    UINT32 sl1npend : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Set when QL1r received; cleared when
                               QL1n sent
                            */
    UINT32 srstrcvd : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* This is the 1:QRst code received */
    UINT32 smml0c : 8;

                            /* Bits[21:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Mismatch pattern by lane when L0c
                               mismatch causes reset; will be b'1 for bad lane,
                               b'0 for good lane
                            */
    UINT32 ssentl0c : 4;

                            /* Bits[25:22], Access Type=RO/V, default=0x00000000*/

                            /* This is the description for Sent L0c Code */
    UINT32 srcvdl0c : 4;

                            /* Bits[29:26], Access Type=RO/V, default=0x00000000*/

                            /* This is the description for Decoded L0c Code */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_CLS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_PLS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022288)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Previous (to Reset) Physical Link Status Register
*/


#define PH_PLS_FPGA_UPIPHY_FUNC2_REG 0x1A220288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sl1rerrp : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the description for L1r received at
                               wrong time
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spdriftalarmp : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Indicates drift buffer underflow
                               or overflow alarm
                            */
    UINT32 sdriftalarmlaneaddressp : 5;

                            /* Bits[7:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the The lane ID of first lane that
                               caused Drift Buffer Alarm to set. Valid only
                               when alarm is set
                            */
    UINT32 sl0pefailp : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the 1:L0p exit failed at Rx */
    UINT32 sl0pelatsubp : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Latency subtracted on at least one
                               idle lane during L0p exit
                            */
    UINT32 sl0pelataddp : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the 1:Latency added on at least one idle
                               lane during L0p exit
                            */
    UINT32 sl1routp : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Set when Ql1r sent; cleared when
                               QL1n received
                            */
    UINT32 sl1npendp : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Set when QL1r received; cleared when
                               QL1n sent
                            */
    UINT32 srstrcvdp : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the 1:QRst code received */
    UINT32 smml0cp : 8;

                            /* Bits[21:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the Mismatch pattern by lane when L0c
                               mismatch causes reset; will be b'1 for bad lane,
                               b'0 for good lane
                            */
    UINT32 ssentl0cp : 4;

                            /* Bits[25:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the description for Sent L0c Code */
    UINT32 srcvdl0cp : 4;

                            /* Bits[29:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the description for Decoded L0c Code */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_PLS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_WES_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x2002228c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Data Lane Wake Error Status Register
*/


#define PH_WES_FPGA_UPIPHY_FUNC2_REG 0x1A22028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srxlanewakeerr : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the A bit value of 1 indicates Rx lane
                               wake detect failed. Bit 0:Status of Physical
                               Lane 0 Bit 1:Status of Physical Lane 1 .. and so
                               on
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_WES_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_KES_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022290)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Data Lane Deskew Error Status Register
*/


#define PH_KES_FPGA_UPIPHY_FUNC2_REG 0x1A220290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srxlanedeskewerr : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the A bit value of 1 indicates Rx lane
                               deskew failed. Bit 0:Status of Physical Lane 0
                               Bit 1:Status of Physical Lane 1 .. and so on
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_KES_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_DAS_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022294)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Data Lane Drift Alarm Status Register
*/


#define PH_DAS_FPGA_UPIPHY_FUNC2_REG 0x1A220294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srxlanedriftalarm : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the A bit value of 1 indicates Rx lane
                               drift alarm. Bit 0:Status of Physical Lane 0 Bit
                               1:Status of Physical Lane 1 .. and so on
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_DAS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TES_FPGA_UPIPHY_FUNC2_REG supported on:                                   */
/*      WHR (0x20022298)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Index of Best Rx Metric Memory Status Register
*/


#define PH_TES_FPGA_UPIPHY_FUNC2_REG 0x1A220298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 soptindex : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Optimal Tx adapt index for lane
                               addressed in ph_tec
                            */
    UINT32 salmetric : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the description for Rx Metric for
                               Addressed Lane
                            */
    UINT32 salmetricmax : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the 0 means metric not implemented */

  } Bits;
  UINT32 Data;

} PH_TES_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTCAP_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222a0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Test Pattern Capability Register
*/


#define PH_TTCAP_FPGA_UPIPHY_FUNC2_REG 0x1A2202A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 testexectrcap : 1;

                            /* Bits[0:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If 0, this field indicates that the
                               ph_ttpgc register is used for the Start Test and
                               Stop Test control bits. If 1, this field
                               inidcates that the ph_ttec register is used for
                               the Start Test and Stop Test control bits
                            */
    UINT32 testexedelaycap : 1;

                            /* Bits[1:1], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicates that
                               the register is availabel to set the delay
                               between setting Start Test and the actual start
                               of the test. This bit MUST be a 1 when the Start
                               Coordination Capability is 1
                            */
    UINT32 testexestatus : 1;

                            /* Bits[2:2], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicates the
                               status register is implemented
                            */
    UINT32 selerrmasktype : 1;

                            /* Bits[3:3], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the description for Selective Error Mask
                               Type
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sinbandparamext : 1;

                            /* Bits[5:5], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, slave is capable of
                               extracting electrical parameter from master's
                               TS.Loopback and applying during the test
                            */
    UINT32 startcoordcap : 1;

                            /* Bits[6:6], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicates that
                               the Start Test field has the ability to be
                               further qualified by an implementation specific
                               control signal from a component wide Global
                               Event
                            */
    UINT32 txerrinjcap : 1;

                            /* Bits[7:7], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the Only valid if Selective Error Mask
                               Type is 1. If set, this field indicates that the
                               transmit side of the Loopback Master can inject
                               bit errors.
                            */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flatpatbufcap : 1;

                            /* Bits[9:9], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicates that
                               the Pattern Buffer Access registers are not
                               indexed, but are flattened.
                            */
    UINT32 rsvd_10 : 6;

                            /* Bits[15:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lpcntrinhibitcap : 1;

                            /* Bits[16:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicates that
                               the Loop Counter Inhibit is implemented and the
                               user can stall the Loop Counter
                            */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lfsrstagger : 1;

                            /* Bits[18:18], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicated that
                               the LFSR1 Stagger Enabled and LFSR2 Stagger
                               Enable fields are implemented and the user can
                               choose between staggering of each LFSR or not
                            */
    UINT32 dcpolaritysel : 1;

                            /* Bits[19:19], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicated that
                               the Drive DC Zero or DC One field is implemented
                               and the user can choose between driving a 0 or a
                               1 on those lanes set to drive a DC value in
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errcntrinhibit : 1;

                            /* Bits[21:21], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, the design is capable of
                               inhibiting error counters
                            */
    UINT32 polysel : 1;

                            /* Bits[22:22], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicated that
                               the LFSR Polynomial Select field is implemented
                               and the user can choose between the two LFSR
                               polynomials
                            */
    UINT32 patbufselrotate : 1;

                            /* Bits[23:23], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, user can enable rotating of
                               the ph_ttpg2s register select during testing
                            */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 exterrcntrreasign : 1;

                            /* Bits[26:26], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicated that
                               the field is implemented and the user can update
                               the assigned lane for the Extended Error
                               Counter(s)
                            */
    UINT32 extcntr : 1;

                            /* Bits[27:27], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the If set, this field indicated that
                               the Extended Error Count All Lanes field is
                               implemented and the user can enable counting of
                               all bit errors for the complete set of lanes for
                               the tested lanes, starting from the lane
                               selected in Extended Error Counter Lane Select
                               and including any consecutive higher lanes
                               included in the testing
                            */
    UINT32 exterrcntrnum : 4;

                            /* Bits[31:28], Access Type=RO/P, default=0x00000000*/

                            /*
                               This is the This field is used to enumerate the
                               total number of Extended Error Counters.
                            */

  } Bits;
  UINT32 Data;

} PH_TTCAP_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTPGC_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222a4)                                                        */
/* Register default value on WHR: 0x06000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Pattern Generator Control Register
*/


#define PH_TTPGC_FPGA_UPIPHY_FUNC2_REG 0x1A2202A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cpstarttest : 1;

                            /* Bits[0:0], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the If ph_ttcap(0) is 0, test starts if
                               this bit is set. 1-Start the test - Initiates
                               transition from Loopback.Marker to
                               Loopback.Pattern The bit is cleared when
                               Loopback.Pattern is exited
                            */
    UINT32 cstoptestonerror : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Exit Loopback.Pattern to
                               Loopback.Marker on error flag 0-Do not stop test
                               on error 1-Stop test and latch loop status on
                               first error in pattern checker
                            */
    UINT32 autoinvrotateen : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the If set, this bit will enable logical
                               rotation of the ph_ttpis register (see ph_ttpis)
                               during the test. Each lane begins by selecting
                               the appropriate bit in the Pattern Invert Select
                               field, and sequentially the next higher bit,
                               wrapping back to bit 0, following each loop. 1 -
                               Logical rotation of Buffer Invert Select 0 - No
                               logical rotation of Buffer Invert Select
                            */
    UINT32 pseudorandomen1 : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Enable Pseudo random pattern
                               generation in pattern Generation Buffer 1 0 -
                               Use Pattern Buffer 1 as 128 bit pattern 1 - Use
                               Pattern Buffer 1 XORed with the LFSR
                            */
    UINT32 pseudorandomen2 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Enable Pseudo random pattern
                               generation in pattern Generation Buffer 2 0 -
                               Use Pattern Buffer 2 as 128 bit pattern 1 - Use
                               Pattern Buffer 2 XORed with the LFSR
                            */
    UINT32 ignoreremoteparam : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the If set, EPayload and Remote
                               parameter received in TS.Loopback with
                               electrical parameter during loop back slave is
                               ignored. Instead, local parameters are applied
                               in Loopback.Pattern.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cincludedroppedlanes : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the If set, include Tx lanes marked
                               TxLaneDropped and Rx lanes marked RxLaneDropped
                               in Loopback.Pattern with deskew setting of 0.
                               Late action.
                            */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 patbufselection : 5;

                            /* Bits[13:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Only used when device is Flat
                               Pattern Buffer capable (see ph_ttcap). Pattern
                               buffer selection used to index pattern buffers
                               through the ph_ttpgb register (see ph_ttpgb).
                               Since ph_ttpgb is a through register, target
                               registers must be provided for each address
                               below. x00-x03 - Access pattern buffer 1 LS-
                               Dword (lsdw) to MS-Dword (msfw) x04-x07 - Access
                               pattern buffer 2 LD-Dword to MS-Dword. x08 -
                               Access Loopback LFSR1 seed x09 - Access Loopback
                               LFSR2 seed x0a - x1f: reserved
                            */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lpcntrinhibit : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Valid only if the Loop Counter
                               Inhibit Capability is 1. This field may be used
                               to inhibit the counting in the Loop Counter when
                               it is set. Normally this would be used when
                               setting Error Counter Inhibit, so there is an
                               accurate reading of the Loop Counter Status. 0 -
                               Normal counting of pattern check loops 1 -
                               Inhibit counting of pattern check loops
                            */
    UINT32 rsvd_17 : 2;

                            /* Bits[18:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 drdc0insteadof1 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Select the DC Logic Value to use
                               when ph_ttddc selects a DC pattern override. 0 -
                               drive a logic 1 for all lanes selected in
                               ph_ttddc 1 - drive a logic 0 for all lanes
                               selected in ph_ttddc
                            */
    UINT32 cpstoptest : 1;

                            /* Bits[20:20], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the If ph_ttcap(0) is 0, setting this
                               bit stops the test. 1 - Stops the test - forces
                               an exit from Loopback.Pattern. The bit is
                               cleared when Loopback.Pattern is exited.
                            */
    UINT32 cerrorcountinhibit : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Inhibits the counting of errors when
                               in Loopback.Pattern. 0 - Normal Error counting 1
                               - Inhibit Error counting
                            */
    UINT32 polyselect : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* This is the 0 - Use the spec polynomial */
    UINT32 rotatepatbufsel : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the If set, this bit will enable
                               rotation of the PH-TTPG2S register during the
                               test. The actual register is not rotated, but an
                               operation copy is made at the beginning of test,
                               and then the copy is rotated across all lanes of
                               the interface. 0 - Rotation is disabled 1 -
                               Rotation is enabled
                            */
    UINT32 cloopcount : 5;

                            /* Bits[28:24], Access Type=RW/P, default=0x00000006*/

                            /*
                               This is the Exponential Test Loop Count. Loop
                               count is 2^(count in this field). Each increment
                               in loop counter indicates 128 UI of Pattern
                               Buffer Register content. 0 - Infinte count. Use
                               Soft Reset to exit test.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTPGC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTTEC_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222a8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Test Execution Control Regsiter1
*/


#define PH_TTTEC_FPGA_UPIPHY_FUNC2_REG 0x1A2202A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cpstarttest : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Test started on change from 0 to 1
                               (edge detect) if in Compliance or
                               Loopback.Marker. Note: Writing Start Test from a
                               0 to 1 while also writing Stop Test to a 1 will
                               result in no test being executed. Clearing Start
                               Test while a test is in process will corrupt
                               (clear) the test process status bits in the
                               ph_tttes register. Start Test should only be
                               cleared after all test results have been
                               examined.
                            */
    UINT32 cpstoptest : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Test stopped on change from 0 to 1
                               (edge detect) if in Loopback.Pattern.
                            */
    UINT32 startdelayselindex_ph_ttted : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Selects the execution delay in
                               ph_ttted
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTTEC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTTED_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222ac)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Test Execution Delay Register1
*/


#define PH_TTTED_FPGA_UPIPHY_FUNC2_REG 0x1A2202AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ttestdelayctrl0 : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Test execution delay Value number 0.
                               Following the setting of the Start Test bit, the
                               selected delay value as indexed in Start Delay
                               Select is used to delay the actual start of the
                               test by the programmed number of TSL. If Start
                               Coordination is set then the timer will not
                               begin until the input trigger has happened
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTTED_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTSNL_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Slave Loopback Path Select Register
*/


#define PH_TTSNL_FPGA_UPIPHY_FUNC2_REG 0x1A2202B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 loopback_path : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Lane selects for loopback from Rx
                               Bit 0 - Controls Lane 0 Bit 1 - Controls Lane 1
                               ...., and so on. 0 - Corresponding lane will use
                               RX loopback path. 1 - Corresponding lane will
                               use path from pattern generator.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTSNL_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTPCC_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222b4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Pattern Checker Control Register
*/


#define PH_TTPCC_FPGA_UPIPHY_FUNC2_REG 0x1A2202B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tinitialcheckmask : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the The Initial Check Mask is used for a
                               settling time for any Electrical Parameter
                               offsets that are applied upon entry to
                               Loopback.Pattern and for settling time for any
                               Electrical Parameter Global values applied upon
                               re-entry to Loopback.Marker. Upon entry to
                               Loopback.Pattern, no RX lane errors will be
                               logged during the Initial Check Mask period.
                               Initial check mask period is 2^(Count in this
                               field)*1TSL. 0 - Indicates no masking.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 selerrchckinterval : 3;

                            /* Bits[7:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Selective Error Check Interval =
                               2^(Count in thisField). Checks error for 1 UI at
                               every Error Check Interval starting from Error
                               Check Start. For example, (Error Check Start,
                               Error Check Interval) - Expected behavior. (0,0)
                               - Checks errors on every UI. (0,1) - Checks
                               errors on even UI. (1,1) - Checks errors on ood
                               UI. (0,2) - Checks errors starting with bit 0,
                               repeat every 4 UI. (1,2) - Checks errors
                               starting with bit 1, repeat every 4 UI. ...
                               (34,7) - Checks errors starting with bit 34,
                               repeat every 128 UI. ... and so on. Values 2-6
                               are optional, but encouraged.
                            */
    UINT32 selerrchckstart : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the The initial UI in the pattern buffer
                               to start checking errors in the arriving pattern
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxerrcntrlanesel : 5;

                            /* Bits[20:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Rx Lane selection for the ph_ttlecr
                               register (see ph_ttlecr). 0 - 19 - Rx Lane
                               Selected. 20 - 31 - Reserved.
                            */
    UINT32 rsvd_21 : 5;

                            /* Bits[25:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 exterrcntrlaneassign : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the This field is valid only if Extended
                               Count Lane Assign Capability is set. This bit is
                               used to force a re-assignment of the 31-bit
                               error counter. Entry into Loopback.Pattern
                               continues to also assign the long error counter
                               lane. 1 - Use the current RxErrCntLaneSel value
                               for assigning the lane that will use the 31-bit
                               error counter. 0 - Only normal entry to
                               Loopback.Pattern will cause the lane assignment
                               to use the 31-bit error counter.
                            */
    UINT32 exterrcntalllanes : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the This field is only valid if Extended
                               Count All Lanes Capability is 1. The user can
                               enable counting of all bit errors for the
                               complete set of lanes for the tested lanes,
                               starting from the lane selected in Extended
                               Error Counter Lane Select and including any
                               consecutive higher lanes included in the
                               testing. These errors are accumulated using the
                               Extended Error Counter. Any per-lane error
                               counters are invalid for the tested lanes. 1 -
                               Accumulate all errors for all lanes 0 - Per-lane
                               error counters are used
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTPCC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTPGB_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222b8)                                                        */
/* Register default value on WHR: 0xAAAAAAAA                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Pattern Generator Buffer Access Register (virtual, writethru or readthru)
*/


#define PH_TTPGB_FPGA_UPIPHY_FUNC2_REG 0x1A2202B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 patgenbuffer : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0xAAAAAAAA*/

                            /*
                               This is the All words of Pattern generator
                               buffers are accessed through this register. A
                               particular pattern buffer is indexed using
                               Pattern buffer selection bits in the ph_ttpgc
                               register (see Table ph_ttpgc). Patters larger
                               than 32 bits are indexed using pattern buffer
                               word selection field in the ph_ttpgc register
                               (see Table ph_ttpgc). Each pattern buffer is 128
                               bit, accessed 32 bit at a time. Least
                               significant bit is sent out first but pattern is
                               not shifted. PRBS pattern is generated by the 23
                               bit LFSR using spec polynomial and the initial
                               seed is programmed using bits 0:23. PatBuf
                               Default (msdw = most significant Dword msdw
                               (D3): ACE3_C3E0h msdw-1 (D2): 100F_F7FFh msdw-2
                               (D1): ACBC_B211h lsdw (D0): 4DAE_1577h
                            */

  } Bits;
  UINT32 Data;

} PH_TTPGB_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTPPG_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222bc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for PRBS Pattern Generator Selection Register1
*/


#define PH_TTPPG_FPGA_UPIPHY_FUNC2_REG 0x1A2202BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 creinitlfsr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Reinit LFSR with seed after
                               CReinitLFSR TSL. (This implies that LFSR seed
                               registers are distinct from the LFSR). 0: do not
                               re-init LFSR
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTPPG_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTPG2S_FPGA_UPIPHY_FUNC2_REG supported on:                                */
/*      WHR (0x200222c0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Pattern Generator Buffer 2 Selection Register
*/


#define PH_TTPG2S_FPGA_UPIPHY_FUNC2_REG 0x1A2202C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 laneselpatbuffer2 : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Lane select Pattern buffer 2 for its
                               testing. One bit per each line Bit 0 - Control
                               lane 0 Bit 1 - Control lane 1 ..., and so on. 1
                               - Corresponding lane will use Pattern Buffer 2
                               for test
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTPG2S_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTPIS_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222c4)                                                        */
/* Register default value on WHR: 0x00000001                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Pattern Invert Selection Register
*/


#define PH_TTPIS_FPGA_UPIPHY_FUNC2_REG 0x1A2202C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 patterninvert : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the One bit per each Tx lane. Bit 0 - Tx
                               lane 0 Bit 1 - Tx lane 1 ..., and so on. 1 -
                               Invert the pattern in corresponding Tx lane.
                               This register sets the initial inversion phase
                               and is not dynamically updated as the pattern
                               progresses.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTPIS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTDDC_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222c8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Tx Testability DC Data Lane Control Register
*/


#define PH_TTDDC_FPGA_UPIPHY_FUNC2_REG 0x1A2202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdcdatalanecontrol : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the A bit mask used for selectively
                               overriding the data TX to output a DC logical 1
                               condition during Loopback.Pattern. Used for
                               debug and validation purposes to quiet
                               neighboring crosstalk lines, and is only applied
                               during Loopback.Pattern. only the ph_tdc
                               register (see Table ph_tdc) will override the
                               behavior of this register. A bit value of 1
                               indicates the corresponding lane is transmitting
                               a logical 1. Bit 0 - Control lane 0 Bit 1 -
                               Control lane 1 ..., and so on.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTDDC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTREC_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222cc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Lane Error Check Disable Register
*/


#define PH_TTREC_FPGA_UPIPHY_FUNC2_REG 0x1A2202CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxlaneerrcheckdisable : 20;

                            /* Bits[19:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the One bit for each Rx lane, if set,
                               will disable error check and accumulation during
                               loop back test. Bit 0 - Rx lane 0 Bit 1 - Rx
                               lane 1 ... Bit 19 - Rx lane 19
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTREC_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTLECR_FPGA_UPIPHY_FUNC2_REG supported on:                                */
/*      WHR (0x200222d0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Lane Error Counter Register
*/


#define PH_TTLECR_FPGA_UPIPHY_FUNC2_REG 0x1A2202D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 laneerrcounter : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the This field will contain the lower
                               bits of the Extended Lane Error Counter, or the
                               error count bits of the short error counter for
                               the lane selected by the current RX Error
                               Counter Lane Select. To properly read all 32
                               bits of the Extended lane Error Counter, the
                               current value in the RX Error Counter Lane
                               Select field must match the assigned lane
                               designated on enrty to Pattern. If Extended
                               Error Count All Lanes is set, then the value of
                               Rx Error Counter Lane Select should not be
                               changed from the assigned lane designated upon
                               entry to Pattern
                            */
    UINT32 extendedlaneerrcounter : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Field is assigned using the RxError
                               Counter Lane Select on entry to Loopback.
                               Pattern. It may also be reassigned at any time
                               by setting the Extended Error Counter Lane
                               Assign Note: Reading this register while the Rx
                               Error Counter Lane Select is not set to the
                               assigned Lane will continue to return the upper
                               bits from the Extended Lane Error Counter. This
                               may be done to read the short Lane Error
                               Counters. Software must be aware of this and
                               clear this field before reporting to the user
                            */
    UINT32 erroverflow : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Error Overflow If set, indicates
                               that the Extended Lane Error Counter has
                               Overflowed. Upon the Extended Lane Error Counter
                               overflowing, the Error Overflow bit is set and
                               the Extended Lane Error Counter rolls over back
                               through 0 and continues counting errors. See
                               Extended Lane Error Counter Description for more
                               details. The Error Overflow will be cleared
                               through software by writing 1 to this field, or
                               a Cold Reset. Writing 0 to this field will have
                               no effect
                            */

  } Bits;
  UINT32 Data;

} PH_TTLECR_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTRES_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222d4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Error Status Register
*/


#define PH_TTRES_FPGA_UPIPHY_FUNC2_REG 0x1A2202D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxlaneerrstatus : 20;

                            /* Bits[19:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the One bit per each Rx lane Bit 0 -
                               Status of lane 0 Bit 1 - Status of lane 1 ...,
                               and so on. Cleared with writing 1. Clearing a
                               lane status also clears the lane error counter
                               for that lane (but not the extended error lane
                               counter/overflow). 1 - Corresponding lane has
                               received error
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTRES_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTLEO_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222d8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Lane Error Overflow Status Register
*/


#define PH_TTLEO_FPGA_UPIPHY_FUNC2_REG 0x1A2202D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxlaneerroverflowstatus : 20;

                            /* Bits[19:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the One bit per lane. Rx Lane Error
                               Overflow indicates that a particular lane
                               encountered a sufficient number of errors during
                               Loopback.Pattern to overflow any per-lane error
                               counter. The counter may be as simple as 1 bit,
                               in which case this can be interpreted as
                               multiple errors. A value of 1 indicates that the
                               particular lane has received an overflow of
                               errors during Loopback.Pattern. One bit per Rx
                               lane. Bit 0 - Status of lane 0 Bit 1 - Status of
                               lane 1 ..., and so on. Cleared with writing 1.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTLEO_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTTES_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222dc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Test Execution Status Register
*/


#define PH_TTTES_FPGA_UPIPHY_FUNC2_REG 0x1A2202DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 testinitiated : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Master Tx SDS sent */
    UINT32 errorcheckstart : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Master Rx SDS received, error
                               checking has begun. Bit is automatically reset
                               to default state,'0' upon Start Test being set
                               to '0'
                            */
    UINT32 stestcomplete : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Full test has been transmitted and
                               checked. Bit is automatically reset to default
                               state,'0' upon Start Test being set to '0'
                            */
    UINT32 markerrelockarchieved : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Post-test execution transition back
                               to Marker and bit/byte lock achieved. Remains
                               persistent until the Tx SDS is sent at which
                               time it is reset to its default state of 0.
                            */
    UINT32 anylanesfail : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Any lanes fails. Bit is
                               automatically reset to default stats, '0', upon
                               Start Test being set to '0'.
                            */
    UINT32 alllanesfail : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the All lanes fail. Bit is automatically
                               reset to default stats, '0', upon Start Test
                               being set to '0'.
                            */
    UINT32 anylanesovrflow : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Any lanes error counter overflow.
                               Bit is automatically reset to default stats,
                               '0', upon Start Test being set to '0'.
                            */
    UINT32 alllanesovrflow : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the All lanes overflow. Bit is
                               automatically reset to default stats, '0', upon
                               Start Test being set to '0'.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_TTTES_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTTLS_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222e0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Tx Loopcount Status Register
*/


#define PH_TTTLS_FPGA_UPIPHY_FUNC2_REG 0x1A2202E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stxloopcount : 32;

                            /* Bits[31:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the The Loop Count Status register
                               represents the current received Loop Count while
                               in Loopback.Pattern or the final transmitted
                               Loop Count after a test. The Loop Count Status
                               is continuously increment by 1 on the
                               transmission of a complete pattern buffer
                               boundary while in Loopback.Pattern and rolls
                               over to all Os upon hittng the maximum value.
                            */

  } Bits;
  UINT32 Data;

} PH_TTTLS_FPGA_UPIPHY_FUNC2_STRUCT;

/* PH_TTLCS_FPGA_UPIPHY_FUNC2_REG supported on:                                 */
/*      WHR (0x200222e4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the description for Rx Loopcount Status Register
*/


#define PH_TTLCS_FPGA_UPIPHY_FUNC2_REG 0x1A2202E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 srxloopcount : 32;

                            /* Bits[31:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This is the The Loop Count Status register
                               represents the current received Loop Count while
                               in Loopback.Pattern or the final checked Loop
                               Count after a test. The Loop Count Status is
                               continuously increment by 1 on the reception of
                               a complete pattern buffer boundary while in
                               Loopback.Pattern and rolls over to all Os upon
                               hittng the maximum value When Stop on Error is
                               set to 1 and a specific LFSR is used to scramble
                               either Pattern buffer 1 or 2, the Loop Count
                               Status can be used to recreate the region of the
                               failing pattern by software replicating the LFSR
                               using the resultant Loop Count Status. The Loop
                               Count Status can be cleared through software
                               writing 1s to this register, or a Cold Reset
                            */

  } Bits;
  UINT32 Data;

} PH_TTLCS_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_CTR_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x20022400)                                                        */
/* Register default value on WHR: 0x2D08FCC0                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Receiver equalization, adaptation of receiver parameters and commands for incrementing or decrementing  
      the transmitter coefficients of the link partner
*/


#define WHR_PH_CTR_FPGA_UPIPHY_FUNC2_REG 0x1A220400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intreset_los : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 00/11: Issue IntReset when LOS is
                               more than 4kUI or CDR lose lock in opmode or Eye
                               center is lost in slow mode 01: Issue IntReset
                               when LOS is more than 4kUI 10: Issue IntReset
                               when CDR lose lock in opmode or Eye center is
                               lost in slow mode
                            */
    UINT32 intreset_drift : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0: Use Deskew FIFO drift detection
                               1: Use Synopsys Drift detection
                               (upipcs_rxX_status[2:0])
                            */
    UINT32 rxdetect_sel : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the LTSSM.RXDETECT passing exit criteria
                               during op speed (SPdetect) 0: Use sigdetect
                               (Invert of LOS) 1: Use CDR lock
                            */
    UINT32 txdetect_sel : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the LTSSM.TXDetect passing exit criteria
                               during op speed (Wakedetect) 0: Use CDR lock 1:
                               Use sigdetect (invert of LOS)
                            */
    UINT32 txdetect_sel_sl : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the LTSSM.TXDetect passing exit criteria
                               during slow mode 0: Use eyelock 1: Use sigdetect
                               (invert of LOS)
                            */
    UINT32 sigdet_b2b_cnt : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000003*/

                            /*
                               This is the Should qual when there's EIEOS
                               (0-based value). 0x0: 16UI 0x1: 32UI 0x3: 64UI
                               (per spec) 0x7: 128UI
                            */
    UINT32 smuitoggle : 2;

                            /* Bits[10:9], Access Type=RW/P, default=0x00000002*/

                            /*
                               This is the Detect number of 1UI prior to
                               smrxlockuicnt expired. Used to establish eye
                               lock. `b00: 2x1UI `b01: 4x1UI `b10: 8x1UI `b11:
                               32x1UI
                            */
    UINT32 smhysteresis : 5;

                            /* Bits[15:11], Access Type=RW/P, default=0x0000001F*/

                            /*
                               This is the UI eye hysteresis during slow mode
                               `b0: +/-9UI `b1: +/-10UI ... `b1F: +/-40UI
                            */
    UINT32 smrxlockuicnt : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the During slow mode, eye lock is
                               established by checking for smuitoggle count
                               within smrxlockuicnt before it expires. 00:
                               Qualify 1kUI before asserting rx_valid (default)
                               01: Qualify 2kUI before asserting rx_valid (back
                               up option) 10: Qualify 4kUI before asserting
                               rx_valid (back up option) 11: Reserved
                            */
    UINT32 wordlock_pass : 2;

                            /* Bits[19:18], Access Type=RW/P, default=0x00000002*/

                            /*
                               This is the Number of consequtive wordlock
                               required to qualify as valid word lock (TS lock)
                               'b00 - Detect TSlock 'b01 - Detect 2 consecutive
                               TSlock 'b10 - Detect 3 consecutive TSlock 'b11 -
                               Detect 4 consecutive TSlock
                            */
    UINT32 rxfifo_latency : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the wr_ptr_sync - rd_ptr 0 - 1 offset 1
                               - 2 offset
                            */
    UINT32 driftbuffer_ll : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x0000000D*/

                            /* This is the Drift buffer lower limit */
    UINT32 driftbuffer_ul : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000002*/

                            /* This is the Drift buffer upper limit */

  } Bits;
  UINT32 Data;

} WHR_PH_CTR_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_CTROVR_FPGA_UPIPHY_FUNC2_REG supported on:                            */
/*      WHR (0x20022404)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Will define bypass scenario upon verification failures found.
*/


#define WHR_PH_CTROVR_FPGA_UPIPHY_FUNC2_REG 0x1A220404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 deskew_limit : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Allow deskew limit to increase more
                               than 64UI 2'b00 = 64UI 2'b01 = 80UI 2'b10 = 96UI
                               2'b11 = 112UI
                            */
    UINT32 ctovr2 : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0 = Ewakedetect drop below 4kUI in
                               L0/L0c and Loopback patern will not trigger
                               inband reset 1 = Ewakedetect drop below 4kUI in
                               L0/L0c and Loopback patern will trigger inband
                               reset
                            */
    UINT32 ctovr3 : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Chicken bit for RX Slow Mode data
                               comparison error that causes eye lock
                               deassertion. Connects to csr_smignore_err 0 = RX
                               slow mode data comparison error will cause eye
                               lock to deassert 1 = Ignore RX slow mode data
                               comparison error that causes eye lock
                               deassertion
                            */
    UINT32 ctovr4 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr5 : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr6 : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr7 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr8 : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr9 : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr10 : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr11 : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr12 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr13 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr14 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr15 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr16 : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr17 : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr18 : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr19 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr20 : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr21 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr22 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr23 : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr24 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr25 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr26 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr27 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr28 : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr29 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr30 : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ctovr31 : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_CTROVR_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_SMCTR_FPGA_UPIPHY_FUNC2_REG supported on:                             */
/*      WHR (0x20022408)                                                        */
/* Register default value on WHR: 0x00000994                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Slow mode with PPM adaptor control
*/


#define WHR_PH_SMCTR_FPGA_UPIPHY_FUNC2_REG 0x1A220408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 smhysteresis_ppm : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000014*/

                            /*
                               `This is the h0= ideal `h1= +/-1 UI .. `h14=
                               +/-20 UI `h1F= +/-31 UI
                            */
    UINT32 smovr5 : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smrxlockuicnt_ppm : 3;

                            /* Bits[8:6], Access Type=RW/P, default=0x00000006*/

                            /*
                               This is the Density of slowmodeUI should be less
                               than 5%. For example, density for 8
                               slowmodeUI/1024slowmodeUI is 0.8%. `b000=
                               Qualify 16 slowmowdeUI before asserting rx_valid
                               `b001= Qualify 32 slowmodeUI before asserting
                               rx_valid `b010= Qualify 64 slowmodeUI before
                               asserting rx_valid `b011= Qualify 128 slowmodeUI
                               before asserting rx_valid `b100= Qualify 256
                               slowmowdeUI before asserting rx_valid `b101=
                               Qualify 512 slowmodeUI before asserting rx_valid
                               `b110= Qualify 1k slowmodeUI before asserting
                               rx_valid `b111= Qualify 4k slowmodeUI before
                               asserting rx_valid
                            */
    UINT32 smovr9 : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smuitoggle_ppm : 3;

                            /* Bits[12:10], Access Type=RW/P, default=0x00000002*/

                            /*
                               This is the Used to establish eye lock. `b000=
                               2x1 slowmodeUI `b001= 4x1 slowmodeUI `b010= 8x1
                               slowmodeUI `b011= 12x1 slowmodeUI `b100= 16x1
                               slowmodeUI `b101= 20x1 slowmodeUI `b110= 24x1
                               slowmodeUI `b111= 32x1 slowmodeUI
                            */
    UINT32 smovr13 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smedgedet_mode : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Leading or lagging edge selection. 0
                               = 00-> 1 OR 11->0 detected (leading edge) 1 =
                               0->11 OR 1 -> 00 detected (lagging edge)
                            */
    UINT32 smzero_ppm : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0 = Enable slowmode with PPM adaptor
                               1 = Disable slowmode with PPM adaptor and Enable
                               slowmode with 0ppm
                            */
    UINT32 block_newoffset : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               `This is the b0000 = Always update the eye
                               offset every edge transition `b0001 = Don't
                               update the offset for 1st slowmodeUI after edge
                               transition `b0010 = Don't update the offset for
                               2nd slowmodeUI after edge transition .. `b1111 =
                               Don't update the offset for 15th slowmodeUI
                               after edge transtion
                            */
    UINT32 smignore_err_ppm : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0 = RX slow mode data comparison
                               error will cause eye lock to deassert 1 = Ignore
                               RX slow mode data comparison error that causes
                               eye lock deassertion
                            */
    UINT32 smovr21 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr22 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr23 : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr24 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr25 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr26 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr27 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr28 : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr29 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr30 : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 smovr31 : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0 = Slow mode (Main Option) 1 = Slow
                               mode (Backup Option)
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_SMCTR_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PPMCTR_FPGA_UPIPHY_FUNC2_REG supported on:                            */
/*      WHR (0x2002240c)                                                        */
/* Register default value on WHR: 0x00000ED8                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Slow mode with PPM detector and tracker control
*/


#define WHR_PH_PPMCTR_FPGA_UPIPHY_FUNC2_REG 0x1A22040C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppm_threshold : 15;

                            /* Bits[14:0], Access Type=RW/P, default=0x00000ED8*/

                            /*
                               This is the 0x0 = Flag error if PPM detected is
                               > +/-0ppm (Don't use) 0x1 to 0xf = Don't use ..
                               0xbb8 = Flag error if PPM detected is >
                               +/-3000ppm .. 0xed8 = Flag error if PPM detected
                               is > +/-3800ppm .. 0x7fff = Flag error if PPM
                               detected is > +/-33767ppm
                            */
    UINT32 ppmovr15 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmdetect_sel : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0: Eyelock asserted 1: LOS
                               deasserted (use this to measure PPM if cannot
                               eyelock)
                            */
    UINT32 ppmdetect_dis : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0 - Enable PPM detection 1 - Disable
                               PPM detection
                            */
    UINT32 ppmovr18 : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr19 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr20 : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr21 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr22 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr23 : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr24 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmovr25 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmtrack_dis : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0 - Enable PPM UI tracker 1 -
                               Disable PPM UI tracker
                            */
    UINT32 ppmovr27 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */
    UINT32 ppmtrack_limit : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               `This is the b0000 = PPM slowmodeUI tracks with
                               upi_clk_ug `b0001 = PPM slowmodeUI > +/1
                               slowmodeUI `b0010 = PPM slowmodeUI > +/2
                               slowmodeUI .. `b1110 = PPM slowmodeUI > +/14
                               slowmodeUI `b1111 = Invalid setting. Setting
                               this will disable PPM slowmode UI tracker limit
                               detection
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_PPMCTR_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PPMLANE_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022410)                                                        */
/* Register default value on WHR: 0x00000020                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the PPM detector status lane selector
*/


#define WHR_PH_PPMLANE_FPGA_UPIPHY_FUNC2_REG 0x1A220410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppmlane_sel0 : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the 0x0 = Lane 0 0x1 = Lane 1 .. 0x13 =
                               Lane 19
                            */
    UINT32 ppmlane_sel1 : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the 0x0 = Lane 0 0x1 = Lane 1 .. 0x13 =
                               Lane 19
                            */
    UINT32 rsvd : 18;

                            /* Bits[27:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ppmlane_spare : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_PPMLANE_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PHOVR_FPGA_UPIPHY_FUNC2_REG supported on:                             */
/*      WHR (0x20022430)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the SERDES overrides
*/


#define WHR_PH_PHOVR_FPGA_UPIPHY_FUNC2_REG 0x1A220430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 powerdown_ovr : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Sets the power state of the
                               transmitter to P0 or P1.
                            */
    UINT32 powerdown_ovren : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1 = override functional phy controls */
    UINT32 tx_elecidle_ovr : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the If deasserted or the TX lane is not
                               in P0, then the TX output driver is disabled
                            */
    UINT32 tx_elecidle_ovren : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1 = override functional phy controls */
    UINT32 tx_lpd_ovr : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Powers down the transmitter into a
                               power state equivalent to P1. The signal is
                               useful for selectively powering down lanes in a
                               link while maintaining the power state of the
                               link as whole. (for example, link in P0)
                            */
    UINT32 tx_lpd_ovren : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1 = override functional phy controls */
    UINT32 rx_termination_ovr : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the When asserted high, the RX is
                               terminated with nominal 50Ohm resistance. When
                               asserted low, the termination is high impedance.
                            */
    UINT32 rx_termination_ovren : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1 = override functional phy controls */
    UINT32 rx_standby_ovr : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the This signal is only valid when the
                               RX is in P0 power state and should be set to 1
                               in other power states. Note that when the
                               asserted high in P0, then the CDR tracks the
                               reference clock in order to stay close to the
                               desired frequency of operation.
                            */
    UINT32 rx_standby_ovren : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1 = override functional phy controls */
    UINT32 rx_lpd_ovr : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Powers down the RX into a power
                               state equivalent to P1. This signal is useful
                               for selectively powering down lanes in a link
                               while maintaining the power state of the link as
                               a whole (for example, link in P0).
                            */
    UINT32 rx_lpd_ovren : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* This is the 1 = override functional phy controls */
    UINT32 rsvd : 11;

                            /* Bits[25:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 phylane_sel : 5;

                            /* Bits[30:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the b0_0000: No operation. Do not change
                               any phy controls. Can read access. b1_1111 :
                               Broadcast same tx_deemph to all lanes bx_xxxx :
                               Decode the lower bits for the lane number. (0x1
                               - Lane 0, 0x2 - Lane 1, # 0x14 - Lane 19)
                            */
    UINT32 phyovr_wr : 1;

                            /* Bits[31:31], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the This bit will automatically return
                               to zero after write
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_PHOVR_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_ADAPT_FPGA_UPIPHY_FUNC2_REG supported on:                             */
/*      WHR (0x20022434)                                                        */
/* Register default value on WHR: 0x0000085C                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the SERDES adaptation
*/


#define WHR_PH_ADAPT_FPGA_UPIPHY_FUNC2_REG 0x1A220434

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_deemph : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x0000085C*/

                            /*
                               (This is the must support at least -3dB deemph
                               or +3dB preshoot) upipcs_txX_deemph[5:0] ->
                               txX_eq_pre[5:0] upipcs_txX_deemph[11:6] ->
                               txX_eq_main[5:0] upipcs_txX_deemph[17:12] ->
                               txX_eq_post[5:0]
                            */
    UINT32 rsvd : 7;

                            /* Bits[24:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxeqeval : 1;

                            /* Bits[25:25], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the This bit will automatically return
                               to zero after write
                            */
    UINT32 deemph_sel : 5;

                            /* Bits[30:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the b0_0000: No operation. Do not change
                               deemph value b1_1111 : Broadcast same tx_deemph
                               to all lanes bx_xxxx : Decode the lower bits for
                               the lane number. (0x1 - Lane 0, 0x2 - Lane 1,
                               ... 0x14 - Lane 19)
                            */
    UINT32 deemph_wr : 1;

                            /* Bits[31:31], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the This bit will automatically return
                               to zero after write
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_ADAPT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PPMTRACK_STS_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x20022450)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the 0 = slowmode UI is within the limit.
      1 = slowmode UI exceeds the limit for the corresponding lane. Write to clear
      this register. If PPM which exceeds limit is detected again, this bit will be set.
*/


#define WHR_PH_PPMTRACK_STS_FPGA_UPIPHY_FUNC2_REG 0x1A220450

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx19_ppmtrack : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 19 */
    UINT32 rx18_ppmtrack : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 18 */
    UINT32 rx17_ppmtrack : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 17 */
    UINT32 rx16_ppmtrack : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 16 */
    UINT32 rx15_ppmtrack : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 15 */
    UINT32 rx14_ppmtrack : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 14 */
    UINT32 rx13_ppmtrack : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 13 */
    UINT32 rx12_ppmtrack : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 12 */
    UINT32 rx11_ppmtrack : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 11 */
    UINT32 rx10_ppmtrack : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 10 */
    UINT32 rx9_ppmtrack : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 9 */
    UINT32 rx8_ppmtrack : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 8 */
    UINT32 rx7_ppmtrack : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 7 */
    UINT32 rx6_ppmtrack : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 6 */
    UINT32 rx5_ppmtrack : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 5 */
    UINT32 rx4_ppmtrack : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 4 */
    UINT32 rx3_ppmtrack : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 3 */
    UINT32 rx2_ppmtrack : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 2 */
    UINT32 rx1_ppmtrack : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 1 */
    UINT32 rx0_ppmtrack : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM slowmode UI tracker for Lane 0 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_PPMTRACK_STS_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PPMCNT_STS_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x20022454)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the 0 = PPM is within the limit.
      1 = PPM exceeds the limit for the corresponding lane.
      Write to clear this register. If PPM which exceeds limit
      is detected again, this bit will be set.
*/


#define WHR_PH_PPMCNT_STS_FPGA_UPIPHY_FUNC2_REG 0x1A220454

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx19_ppmcnt : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 19 */
    UINT32 rx18_ppmcnt : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 18 */
    UINT32 rx17_ppmcnt : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 17 */
    UINT32 rx16_ppmcnt : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 16 */
    UINT32 rx15_ppmcnt : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 15 */
    UINT32 rx14_ppmcnt : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 14 */
    UINT32 rx13_ppmcnt : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 13 */
    UINT32 rx12_ppmcnt : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 12 */
    UINT32 rx11_ppmcnt : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 11 */
    UINT32 rx10_ppmcnt : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 10 */
    UINT32 rx9_ppmcnt : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 9 */
    UINT32 rx8_ppmcnt : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 8 */
    UINT32 rx7_ppmcnt : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 7 */
    UINT32 rx6_ppmcnt : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 6 */
    UINT32 rx5_ppmcnt : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 5 */
    UINT32 rx4_ppmcnt : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 4 */
    UINT32 rx3_ppmcnt : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 3 */
    UINT32 rx2_ppmcnt : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 2 */
    UINT32 rx1_ppmcnt : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 1 */
    UINT32 rx0_ppmcnt : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the PPM counter status for Lane 0 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_PPMCNT_STS_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PPMCNT_FPGA_UPIPHY_FUNC2_REG supported on:                            */
/*      WHR (0x20022458)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This value is updated every 1,000,000 counts of upi_clk_ug
*/


#define WHR_PH_PPMCNT_FPGA_UPIPHY_FUNC2_REG 0x1A220458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppmcnt_0 : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the PPM value is a two's complement
                               number. [15] is the signed bit.
                            */
    UINT32 ppmcnt_1 : 16;

                            /* Bits[31:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the PPM value is a two's complement
                               number. [31] is the signed bit.
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_PPMCNT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PPM_SPARE_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x2002245c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Spare status register
*/


#define WHR_PH_PPM_SPARE_FPGA_UPIPHY_FUNC2_REG 0x1A22045C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppmsts_spare : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Spare read status */

  } Bits;
  UINT32 Data;

} WHR_PH_PPM_SPARE_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_PHYSTAT_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022460)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This is Phy status done indication from the SERDES. This register is in cold
      reset domain. Phystat from initial PHY poweron reset will be captured in this
      register. Phystat will also be asserted for one clock cycle from P1 to P0 power
      state when upi_rst_n is deasserted. Apart from this, phystat is asserted for one
      clock cycle during RX equalization request is done. User should clear this flag
      before requesting for RX evaluation and the monitor phystat assertion before
      retrieving the FOM value.
*/


#define WHR_PH_PHYSTAT_FPGA_UPIPHY_FUNC2_REG 0x1A220460

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_phystat : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 0 */
    UINT32 rx1_phystat : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 1 */
    UINT32 rx2_phystat : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 2 */
    UINT32 rx3_phystat : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 3 */
    UINT32 rx4_phystat : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 4 */
    UINT32 rx5_phystat : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 5 */
    UINT32 rx6_phystat : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 6 */
    UINT32 rx7_phystat : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 7 */
    UINT32 rx8_phystat : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 8 */
    UINT32 rx9_phystat : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 9 */
    UINT32 rx10_phystat : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 10 */
    UINT32 rx11_phystat : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 11 */
    UINT32 rx12_phystat : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 12 */
    UINT32 rx13_phystat : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 13 */
    UINT32 rx14_phystat : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 14 */
    UINT32 rx15_phystat : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 15 */
    UINT32 rx16_phystat : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 16 */
    UINT32 rx17_phystat : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 17 */
    UINT32 rx18_phystat : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 18 */
    UINT32 rx19_phystat : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This is the PHY Status signal for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_PHYSTAT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_CDRSTAT_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022464)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the CDR lock status for each lane. For operational mode, CDR must lock to data.
*/


#define WHR_PH_CDRSTAT_FPGA_UPIPHY_FUNC2_REG 0x1A220464

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_cdrlock : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 0 */
    UINT32 rx1_cdrlock : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 1 */
    UINT32 rx2_cdrlock : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 2 */
    UINT32 rx3_cdrlock : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 3 */
    UINT32 rx4_cdrlock : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 4 */
    UINT32 rx5_cdrlock : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 5 */
    UINT32 rx6_cdrlock : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 6 */
    UINT32 rx7_cdrlock : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 7 */
    UINT32 rx8_cdrlock : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 8 */
    UINT32 rx9_cdrlock : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 9 */
    UINT32 rx10_cdrlock : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 10 */
    UINT32 rx11_cdrlock : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 11 */
    UINT32 rx12_cdrlock : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 12 */
    UINT32 rx13_cdrlock : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 13 */
    UINT32 rx14_cdrlock : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 14 */
    UINT32 rx15_cdrlock : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 15 */
    UINT32 rx16_cdrlock : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 16 */
    UINT32 rx17_cdrlock : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 17 */
    UINT32 rx18_cdrlock : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 18 */
    UINT32 rx19_cdrlock : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* This is the CDR Lock signal for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_CDRSTAT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_EYESTAT_FPGA_UPIPHY_FUNC2_REG supported on:                           */
/*      WHR (0x20022468)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Slow mode eye lock status for each lane. In slow mode, eye lock must be
      achieved when running at decimated rate of 100Mbps.
*/


#define WHR_PH_EYESTAT_FPGA_UPIPHY_FUNC2_REG 0x1A220468

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_eyelock : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 0 */
    UINT32 rx1_eyelock : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 1 */
    UINT32 rx2_eyelock : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 2 */
    UINT32 rx3_eyelock : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 3 */
    UINT32 rx4_eyelock : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 4 */
    UINT32 rx5_eyelock : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 5 */
    UINT32 rx6_eyelock : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 6 */
    UINT32 rx7_eyelock : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 7 */
    UINT32 rx8_eyelock : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 8 */
    UINT32 rx9_eyelock : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 9 */
    UINT32 rx10_eyelock : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 10 */
    UINT32 rx11_eyelock : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 11 */
    UINT32 rx12_eyelock : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 12 */
    UINT32 rx13_eyelock : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 13 */
    UINT32 rx14_eyelock : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 14 */
    UINT32 rx15_eyelock : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 15 */
    UINT32 rx16_eyelock : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 16 */
    UINT32 rx17_eyelock : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 17 */
    UINT32 rx18_eyelock : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 18 */
    UINT32 rx19_eyelock : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* This is the EYE Lock signal for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_EYESTAT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXFOM_Q0_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x2002246c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the RX evaluation figure of merit for lane 0 to lane 3
*/


#define WHR_PH_RXFOM_Q0_FPGA_UPIPHY_FUNC2_REG 0x1A22046C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_eq_fom : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 0 */
    UINT32 rx1_eq_fom : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 1 */
    UINT32 rx2_eq_fom : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 2 */
    UINT32 rx3_eq_fom : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 3 */

  } Bits;
  UINT32 Data;

} WHR_PH_RXFOM_Q0_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXFOM_Q1_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x20022470)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the RX evaluation figure of merit for lane 4 to lane 7
*/


#define WHR_PH_RXFOM_Q1_FPGA_UPIPHY_FUNC2_REG 0x1A220470

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx4_eq_fom : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 4 */
    UINT32 rx5_eq_fom : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 5 */
    UINT32 rx6_eq_fom : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 6 */
    UINT32 rx7_eq_fom : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 7 */

  } Bits;
  UINT32 Data;

} WHR_PH_RXFOM_Q1_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXFOM_Q2_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x20022474)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the RX evaluation figure of merit for lane 8 to lane 11
*/


#define WHR_PH_RXFOM_Q2_FPGA_UPIPHY_FUNC2_REG 0x1A220474

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx8_eq_fom : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 8 */
    UINT32 rx9_eq_fom : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 9 */
    UINT32 rx10_eq_fom : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 10 */
    UINT32 rx11_eq_fom : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 11 */

  } Bits;
  UINT32 Data;

} WHR_PH_RXFOM_Q2_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXFOM_Q3_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x20022478)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the RX evaluation figure of merit for lane 12 to lane 15
*/


#define WHR_PH_RXFOM_Q3_FPGA_UPIPHY_FUNC2_REG 0x1A220478

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx12_eq_fom : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 12 */
    UINT32 rx13_eq_fom : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 13 */
    UINT32 rx14_eq_fom : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 14 */
    UINT32 rx15_eq_fom : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 15 */

  } Bits;
  UINT32 Data;

} WHR_PH_RXFOM_Q3_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXFOM_Q4_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x2002247c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the RX evaluation figure of merit for lane 16 to lane 19
*/


#define WHR_PH_RXFOM_Q4_FPGA_UPIPHY_FUNC2_REG 0x1A22047C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx16_eq_fom : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 16 */
    UINT32 rx17_eq_fom : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 17 */
    UINT32 rx18_eq_fom : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 18 */
    UINT32 rx19_eq_fom : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the RX figure of merit for Lane 19 */

  } Bits;
  UINT32 Data;

} WHR_PH_RXFOM_Q4_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXLANEREV_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x20022480)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This is used for debug when some lanes indicates lane reversal while
      others does not. Lane reversal will be extracted and reflected at every
      super sequence TS*. The final TS* lane reversal extracted prior to SDS will be
      the lane reversal indicator used during L0.
*/


#define WHR_PH_RXLANEREV_FPGA_UPIPHY_FUNC2_REG 0x1A220480

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_lanerev : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 0 */
    UINT32 rx1_lanerev : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 1 */
    UINT32 rx2_lanerev : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 2 */
    UINT32 rx3_lanerev : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 3 */
    UINT32 rx4_lanerev : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 4 */
    UINT32 rx5_lanerev : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 5 */
    UINT32 rx6_lanerev : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 6 */
    UINT32 rx7_lanerev : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 7 */
    UINT32 rx8_lanerev : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 8 */
    UINT32 rx9_lanerev : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 9 */
    UINT32 rx10_lanerev : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 10 */
    UINT32 rx11_lanerev : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 11 */
    UINT32 rx12_lanerev : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 12 */
    UINT32 rx13_lanerev : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 13 */
    UINT32 rx14_lanerev : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 14 */
    UINT32 rx15_lanerev : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 15 */
    UINT32 rx16_lanerev : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 16 */
    UINT32 rx17_lanerev : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 17 */
    UINT32 rx18_lanerev : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 18 */
    UINT32 rx19_lanerev : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* This is the Lane Reversal received for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_RXLANEREV_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXSDSRCVD_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x20022484)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This is used for debug.
*/


#define WHR_PH_RXSDSRCVD_FPGA_UPIPHY_FUNC2_REG 0x1A220484

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_sdsrcvd : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 0 */
    UINT32 rx1_sdsrcvd : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 1 */
    UINT32 rx2_sdsrcvd : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 2 */
    UINT32 rx3_sdsrcvd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 3 */
    UINT32 rx4_sdsrcvd : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 4 */
    UINT32 rx5_sdsrcvd : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 5 */
    UINT32 rx6_sdsrcvd : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 6 */
    UINT32 rx7_sdsrcvd : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 7 */
    UINT32 rx8_sdsrcvd : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 8 */
    UINT32 rx9_sdsrcvd : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 9 */
    UINT32 rx10_sdsrcvd : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 10 */
    UINT32 rx11_sdsrcvd : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 11 */
    UINT32 rx12_sdsrcvd : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 12 */
    UINT32 rx13_sdsrcvd : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 13 */
    UINT32 rx14_sdsrcvd : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 14 */
    UINT32 rx15_sdsrcvd : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 15 */
    UINT32 rx16_sdsrcvd : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 16 */
    UINT32 rx17_sdsrcvd : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 17 */
    UINT32 rx18_sdsrcvd : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 18 */
    UINT32 rx19_sdsrcvd : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the SDS received for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_RXSDSRCVD_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXEIOSRCVD_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x20022488)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This is used for debug.
*/


#define WHR_PH_RXEIOSRCVD_FPGA_UPIPHY_FUNC2_REG 0x1A220488

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_eiosrcvd : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 0 */
    UINT32 rx1_eiosrcvd : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 1 */
    UINT32 rx2_eiosrcvd : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 2 */
    UINT32 rx3_eiosrcvd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 3 */
    UINT32 rx4_eiosrcvd : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 4 */
    UINT32 rx5_eiosrcvd : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 5 */
    UINT32 rx6_eiosrcvd : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 6 */
    UINT32 rx7_eiosrcvd : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 7 */
    UINT32 rx8_eiosrcvd : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 8 */
    UINT32 rx9_eiosrcvd : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 9 */
    UINT32 rx10_eiosrcvd : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 10 */
    UINT32 rx11_eiosrcvd : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 11 */
    UINT32 rx12_eiosrcvd : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 12 */
    UINT32 rx13_eiosrcvd : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 13 */
    UINT32 rx14_eiosrcvd : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 14 */
    UINT32 rx15_eiosrcvd : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 15 */
    UINT32 rx16_eiosrcvd : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 16 */
    UINT32 rx17_eiosrcvd : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 17 */
    UINT32 rx18_eiosrcvd : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 18 */
    UINT32 rx19_eiosrcvd : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the EIOS received for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_RXEIOSRCVD_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXTS1_ACKRCVD_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x2002248c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This is used for debug.
*/


#define WHR_PH_RXTS1_ACKRCVD_FPGA_UPIPHY_FUNC2_REG 0x1A22048C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_ts1ackrcvd : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 0 */
    UINT32 rx1_ts1ackrcvd : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 1 */
    UINT32 rx2_ts1ackrcvd : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 2 */
    UINT32 rx3_ts1ackrcvd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 3 */
    UINT32 rx4_ts1ackrcvd : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 4 */
    UINT32 rx5_ts1ackrcvd : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 5 */
    UINT32 rx6_ts1ackrcvd : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 6 */
    UINT32 rx7_ts1ackrcvd : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 7 */
    UINT32 rx8_ts1ackrcvd : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 8 */
    UINT32 rx9_ts1ackrcvd : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 9 */
    UINT32 rx10_ts1ackrcvd : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 10 */
    UINT32 rx11_ts1ackrcvd : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 11 */
    UINT32 rx12_ts1ackrcvd : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 12 */
    UINT32 rx13_ts1ackrcvd : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 13 */
    UINT32 rx14_ts1ackrcvd : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 14 */
    UINT32 rx15_ts1ackrcvd : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 15 */
    UINT32 rx16_ts1ackrcvd : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 16 */
    UINT32 rx17_ts1ackrcvd : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 17 */
    UINT32 rx18_ts1ackrcvd : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 18 */
    UINT32 rx19_ts1ackrcvd : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK received for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_RXTS1_ACKRCVD_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_RXTS2_ACKRCVD_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x20022490)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the This is used for debug.
*/


#define WHR_PH_RXTS2_ACKRCVD_FPGA_UPIPHY_FUNC2_REG 0x1A220490

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx0_ts2ackrcvd : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 0 */
    UINT32 rx1_ts2ackrcvd : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 1 */
    UINT32 rx2_ts2ackrcvd : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 2 */
    UINT32 rx3_ts2ackrcvd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 3 */
    UINT32 rx4_ts2ackrcvd : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 4 */
    UINT32 rx5_ts2ackrcvd : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 5 */
    UINT32 rx6_ts2ackrcvd : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 6 */
    UINT32 rx7_ts2ackrcvd : 1;

                            /* Bits[7:7], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 7 */
    UINT32 rx8_ts2ackrcvd : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 8 */
    UINT32 rx9_ts2ackrcvd : 1;

                            /* Bits[9:9], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 9 */
    UINT32 rx10_ts2ackrcvd : 1;

                            /* Bits[10:10], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 10 */
    UINT32 rx11_ts2ackrcvd : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 11 */
    UINT32 rx12_ts2ackrcvd : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 12 */
    UINT32 rx13_ts2ackrcvd : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 13 */
    UINT32 rx14_ts2ackrcvd : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 14 */
    UINT32 rx15_ts2ackrcvd : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 15 */
    UINT32 rx16_ts2ackrcvd : 1;

                            /* Bits[16:16], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 16 */
    UINT32 rx17_ts2ackrcvd : 1;

                            /* Bits[17:17], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 17 */
    UINT32 rx18_ts2ackrcvd : 1;

                            /* Bits[18:18], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 18 */
    UINT32 rx19_ts2ackrcvd : 1;

                            /* Bits[19:19], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK received for Lane 19 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_RXTS2_ACKRCVD_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TXTS_SENT_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x20022498)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the The control for all lanes comes from LTSSM. This is used for debug.
*/


#define WHR_PH_TXTS_SENT_FPGA_UPIPHY_FUNC2_REG 0x1A220498

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx0_ts1naksent : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 NAK send */
    UINT32 tx1_ts1acksent : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS1 ACK send */
    UINT32 tx2_ts2naksent : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 NAK send */
    UINT32 tx3_ts2acksent : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* This is the TS2 ACK send */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_TXTS_SENT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLB_0_3_FPGA_UPIPHY_FUNC2_REG supported on:                          */
/*      WHR (0x200224a0)                                                        */
/* Register default value on WHR: 0x02000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Digital loopback skew injection. 'b00 = 0UI. 'b11 = 3UI
*/


#define WHR_PH_TTLB_0_3_FPGA_UPIPHY_FUNC2_REG 0x1A2204A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dlbskew_l0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 0
                            */
    UINT32 dlbskew_l1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 1
                            */
    UINT32 dlbskew_l2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 2
                            */
    UINT32 dlbskew_l3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 3
                            */
    UINT32 rsvd : 9;

                            /* Bits[24:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lb_rdlatency : 2;

                            /* Bits[26:25], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the The start of read can be programmed
                               to increase the latency and reducing the risk of
                               mestable PC FIFO empty. 0x0 - Read when PCFIFO
                               is not empty 0x1 - Read one cycle after PCFIFO
                               is not empty 0x2 - Read 2 cycles after PCFIFO is
                               not empty 0x3 - Read 3 cycles after PCFIFO is
                               not empty (Don't use this. PCFIFO will hit full)
                            */
    UINT32 f1_sigdet : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the This is used to force LTSSM to move
                               state (DLB-PHY Loopback mode and Serial Loopback
                               should automatically force wake detect without
                               setting bit).This bit also forces
                               upipcsr_rxX_standby to deassert forces CDR to
                               track to data)
                            */
    UINT32 f1_cdrlock : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the This is used to force LTSSM to move
                               state (DLB-PHY Loopback mode should
                               automatically force cdr lock without setting
                               bit)
                            */
    UINT32 lbmode : 3;

                            /* Bits[31:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the b000: Loopback Disabled b001:
                               Digital Loopback Link (dlb_link) b010: Digital
                               Loopback Phy (dlb_phy) b011: TX to RX Serial
                               Loopback (serlb) b100: RX to TX Parallel
                               Loopback (parlb) (OR this with Compliance.Slave)
                               b101: External Serial Loopback (elb) Others:
                               Reserved == Loopback Disabled
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLB_0_3_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLB_4_11_FPGA_UPIPHY_FUNC2_REG supported on:                         */
/*      WHR (0x200224a4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Digital loopback skew injection. 'b00 = 0UI. 'b11 = 3UI
*/


#define WHR_PH_TTLB_4_11_FPGA_UPIPHY_FUNC2_REG 0x1A2204A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dlbskew_l4 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 4
                            */
    UINT32 dlbskew_l5 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 5
                            */
    UINT32 dlbskew_l6 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 6
                            */
    UINT32 dlbskew_l7 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 7
                            */
    UINT32 dlbskew_l8 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 8
                            */
    UINT32 dlbskew_l9 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 9
                            */
    UINT32 dlbskew_l10 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 10
                            */
    UINT32 dlbskew_l11 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 11
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLB_4_11_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLB_12_19_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x200224a8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Digital loopback skew injection. 'b00 = 0UI. 'b11 = 3UI
*/


#define WHR_PH_TTLB_12_19_FPGA_UPIPHY_FUNC2_REG 0x1A2204A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dlbskew_l12 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 12
                            */
    UINT32 dlbskew_l13 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 13
                            */
    UINT32 dlbskew_l14 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 14
                            */
    UINT32 dlbskew_l15 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 15
                            */
    UINT32 dlbskew_l16 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 16
                            */
    UINT32 dlbskew_l17 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 17
                            */
    UINT32 dlbskew_l18 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 18
                            */
    UINT32 dlbskew_l19 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Digital loopback UI skew injection
                               for Lane 19
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLB_12_19_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_CTRL_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x200224ac)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD or Signal Tap Control
*/


#define WHR_PH_TTDFD_CTRL_FPGA_UPIPHY_FUNC2_REG 0x1A2204AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfd_csr_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the When unset, control test_out with
                               PLD fabric test in, when set use Config_AVMM to
                               control out_data pins for both DFD and Signal
                               Tap modes
                            */
    UINT32 debug_en : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the Use this signal to create
                               phy_debug_en as well as a mux selector to
                               upi_debug block in upi_core. 00 = output 0 01 =
                               DFD 10 = Signal Tap L0 output (phy_debug_en = 1)
                               11 = Signal Tap L1 output (phy_debug_en = 1)
                            */
    UINT32 l0xbar_sel : 4;

                            /* Bits[6:3], Access Type=RW/P, default=0x00000000*/

                            /* This is the Legal selection is 0x0-0xe */
    UINT32 l1xbar_sel : 5;

                            /* Bits[11:7], Access Type=RW/P, default=0x00000000*/

                            /* This is the Legal value: 0x0-0x17 */
    UINT32 rsvd : 6;

                            /* Bits[17:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 trig0_sel : 7;

                            /* Bits[24:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the UPI PHY External Trigger select for
                               bit 0
                            */
    UINT32 trig1_sel : 7;

                            /* Bits[31:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               This is the UPI PHY External Trigger select for
                               bit 1
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_CTRL_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_STATUS_FPGA_UPIPHY_FUNC2_REG supported on:                      */
/*      WHR (0x200224b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD Status
*/


#define WHR_PH_TTDFD_STATUS_FPGA_UPIPHY_FUNC2_REG 0x1A2204B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfd_swtrigger : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Can be used by DFD software as a way
                               to carry info such as trigger or lock status
                               (e.g, L0, LinkUp#). In order to share muxes with
                               Signal Tap, dfd_swtrigger = {L1Mux3[15:0],
                               L1Mux2[15:0]}
                            */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_STATUS_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_L0MUX_0_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x200224b4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD/Signal Tap level 0 mux select
*/


#define WHR_PH_TTDFD_L0MUX_0_FPGA_UPIPHY_FUNC2_REG 0x1A2204B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0dw0_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 0 selector */
    UINT32 l0dw1_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 1 selector */
    UINT32 l0dw2_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 2 selector */
    UINT32 l0dw3_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 3 selector */
    UINT32 l0dw4_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 4 selector */
    UINT32 l0dw5_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 5 selector */
    UINT32 l0dw6_sel : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 6 selector */
    UINT32 l0dw7_sel : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 7 selector */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_L0MUX_0_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_L0MUX_1_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x200224b8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD/Signal Tap level 0 mux select
*/


#define WHR_PH_TTDFD_L0MUX_1_FPGA_UPIPHY_FUNC2_REG 0x1A2204B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0dw8_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 8 selector */
    UINT32 l0dw9_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 9 selector */
    UINT32 l0dw10_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 10 selector */
    UINT32 l0dw11_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 11 selector */
    UINT32 l0dw12_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 12 selector */
    UINT32 l0dw13_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 13 selector */
    UINT32 l0dw14_sel : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 14 selector */
    UINT32 l0dw15_sel : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 15 selector */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_L0MUX_1_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_L0MUX_2_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x200224bc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD/Signal Tap level 0 mux select
*/


#define WHR_PH_TTDFD_L0MUX_2_FPGA_UPIPHY_FUNC2_REG 0x1A2204BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0dw16_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 16 selector */
    UINT32 l0dw17_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 17 selector */
    UINT32 l0dw18_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 18 selector */
    UINT32 l0dw19_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 19 selector */
    UINT32 l0dw20_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 20 selector */
    UINT32 l0dw21_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 21 selector */
    UINT32 l0dw22_sel : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 22 selector */
    UINT32 l0dw23_sel : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 23 selector */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_L0MUX_2_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_L0MUX_3_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x200224c0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD/Signal Tap level 0 mux select
*/


#define WHR_PH_TTDFD_L0MUX_3_FPGA_UPIPHY_FUNC2_REG 0x1A2204C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0dw24_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 24 selector */
    UINT32 l0dw25_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 25 selector */
    UINT32 l0dw26_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 26 selector */
    UINT32 l0dw27_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 0: DWord 27 selector */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_L0MUX_3_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_L1MUX_0_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x200224c4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD/Signal Tap level 1 mux select
*/


#define WHR_PH_TTDFD_L1MUX_0_FPGA_UPIPHY_FUNC2_REG 0x1A2204C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l1w0_sel : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 0 selector */
    UINT32 l1w1_sel : 7;

                            /* Bits[13:7], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 1 selector */
    UINT32 l1w2_sel : 7;

                            /* Bits[20:14], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 2 selector */
    UINT32 l1w3_sel : 7;

                            /* Bits[27:21], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 3 selector */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_L1MUX_0_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTDFD_L1MUX_1_FPGA_UPIPHY_FUNC2_REG supported on:                     */
/*      WHR (0x200224c8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the DFD/Signal Tap level 1 mux select
*/


#define WHR_PH_TTDFD_L1MUX_1_FPGA_UPIPHY_FUNC2_REG 0x1A2204C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l1w4_sel : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 4 selector */
    UINT32 l1w5_sel : 7;

                            /* Bits[13:7], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 5 selector */
    UINT32 l1w6_sel : 7;

                            /* Bits[20:14], Access Type=RW/P, default=0x00000000*/

                            /* This is the Level 1: Word 6 selector */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_TTDFD_L1MUX_1_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_CTR_FPGA_UPIPHY_FUNC2_REG supported on:                        */
/*      WHR (0x200224cc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor control
*/


#define WHR_PH_TTLMON_CTR_FPGA_UPIPHY_FUNC2_REG 0x1A2204CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lmon_start : 1;

                            /* Bits[0:0], Access Type=RW/1S/P, default=0x00000000*/

                            /*
                               This is the This bit will be automatically
                               cleared when latency monitor stop criteria is
                               met
                            */
    UINT32 lmon_stop : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               `This is the b00: Stop when bin full or any
                               errors `b01: Stop when bin full and ignore all
                               errors `b10: Stop when bin full or lmon_err or
                               patchk_err. Ignore bin_err `b11: Reserved. Same
                               as `b00
                            */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 maxbin_limit : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               `This is the b000: FFFF_xxxx `b001: 0FFF_Fxxx
                               `b010: 00FF_FFxx `b011: 000F_FFFx `b100:
                               0000_FFFF `b101-`b111: Reserved. Same as `b000
                            */
    UINT32 rsvd_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_CTR_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_STAT_FPGA_UPIPHY_FUNC2_REG supported on:                       */
/*      WHR (0x200224d0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Statistic
*/


#define WHR_PH_TTLMON_STAT_FPGA_UPIPHY_FUNC2_REG 0x1A2204D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lmon_err : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Roundtrip latency is more than 63
                               clock cycles
                            */
    UINT32 patchk_err : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* This is the Pattern Checker error detected */
    UINT32 bin_exceed_err : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Latency distribution exceeds the
                               available bins
                            */
    UINT32 lat_min : 6;

                            /* Bits[8:3], Access Type=RO/V, default=0x00000000*/

                            /* This is the Min latency in clock cycles */
    UINT32 lat_mode : 6;

                            /* Bits[14:9], Access Type=RO/V, default=0x00000000*/

                            /* This is the Mode latency in clock cycles */
    UINT32 lat_max : 6;

                            /* Bits[20:15], Access Type=RO/V, default=0x00000000*/

                            /* This is the Max latency in clock cycles */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_STAT_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_BIN_0_3_FPGA_UPIPHY_FUNC2_REG supported on:                    */
/*      WHR (0x200224d4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Histogram Bin 0, 1, 2, 3
*/


#define WHR_PH_TTLMON_BIN_0_3_FPGA_UPIPHY_FUNC2_REG 0x1A2204D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lmon_bin0 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Bin 0 is always the lowest latency
                               bin
                            */
    UINT32 lmon_bin1 : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the Subsequent bin is always an addition
                               of one upi_clk
                            */
    UINT32 lmon_bin2 : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 2 */
    UINT32 lmon_bin3 : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 3 */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_BIN_0_3_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_BIN_4_7_FPGA_UPIPHY_FUNC2_REG supported on:                    */
/*      WHR (0x200224d8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Histogram Bin 4, 5, 6, 7
*/


#define WHR_PH_TTLMON_BIN_4_7_FPGA_UPIPHY_FUNC2_REG 0x1A2204D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lmon_bin4 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 4 */
    UINT32 lmon_bin5 : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 5 */
    UINT32 lmon_bin6 : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 6 */
    UINT32 lmon_bin7 : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 7 */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_BIN_4_7_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_BIN_8_11_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224dc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Histogram Bin 8, 9, 10, 11
*/


#define WHR_PH_TTLMON_BIN_8_11_FPGA_UPIPHY_FUNC2_REG 0x1A2204DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lmon_bin8 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 8 */
    UINT32 lmon_bin9 : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 9 */
    UINT32 lmon_bin10 : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 10 */
    UINT32 lmon_bin11 : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This is the Latency Monitor Histogram Bin 11 */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_BIN_8_11_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_FLIT_DW0_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224e0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Flit Error
*/


#define WHR_PH_TTLMON_FLIT_DW0_FPGA_UPIPHY_FUNC2_REG 0x1A2204E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_dw0 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Flit[31:0] */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_FLIT_DW0_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_FLIT_DW1_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224e4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Flit Error
*/


#define WHR_PH_TTLMON_FLIT_DW1_FPGA_UPIPHY_FUNC2_REG 0x1A2204E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_dw1 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Flit[63:32] */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_FLIT_DW1_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_FLIT_DW2_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224e8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Flit Error
*/


#define WHR_PH_TTLMON_FLIT_DW2_FPGA_UPIPHY_FUNC2_REG 0x1A2204E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_dw2 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Flit[95:64] */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_FLIT_DW2_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_FLIT_DW3_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224ec)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Flit Error
*/


#define WHR_PH_TTLMON_FLIT_DW3_FPGA_UPIPHY_FUNC2_REG 0x1A2204EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_dw3 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Flit[127:96] */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_FLIT_DW3_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_FLIT_DW4_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224f0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Flit Error
*/


#define WHR_PH_TTLMON_FLIT_DW4_FPGA_UPIPHY_FUNC2_REG 0x1A2204F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_dw4 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Flit[159:128] */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_FLIT_DW4_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_TTLMON_FLIT_DW5_FPGA_UPIPHY_FUNC2_REG supported on:                   */
/*      WHR (0x200224f4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Latency Monitor Flit Error
*/


#define WHR_PH_TTLMON_FLIT_DW5_FPGA_UPIPHY_FUNC2_REG 0x1A2204F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_dw5 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* This is the Flit[191:160] */

  } Bits;
  UINT32 Data;

} WHR_PH_TTLMON_FLIT_DW5_FPGA_UPIPHY_FUNC2_STRUCT;

/* WHR_PH_SEC_FPGA_UPIPHY_FUNC2_REG supported on:                               */
/*      WHR (0x20022ffc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upiphy_agent_func0                */
/* Struct generated from WHR BDF: 0_4_2                                         */
/* WHR Security PolicyGroup:                                                    */
/* This is the Wharfrock UPI PHY region lock
*/


#define WHR_PH_SEC_FPGA_UPIPHY_FUNC2_REG 0x1A220FFC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 region_lock : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is the 0 - Unlock 1 - Lock [0] - PCIE FUNC0
                               Header lock (0x0000-0x0103) [1] - PCIE FUNC1
                               Header lock (0x1000- 0x1FFF) [2] - RAS lock
                               (0x0104-0x01ff) [3] - PHY Timer lock
                               (0x0200-0x023f) [4] - Control and Status lock
                               (0x0240-0x029f) [5] - Testability Toolbox
                               lock0x02a0-0x03ff) [6] - WHR PHY Control and
                               Status lock(0x0400-0x049f) [7] - WHR Testability
                               lock(0x04a0-0x0ffb) 0xffc-0xfff is this
                               register. This should only be guarded by ip_sec.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} WHR_PH_SEC_FPGA_UPIPHY_FUNC2_STRUCT;
#endif /* _FPGA_UPIPHY_FUNC2_h */
