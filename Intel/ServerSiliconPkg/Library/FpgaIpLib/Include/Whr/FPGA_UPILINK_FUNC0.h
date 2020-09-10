
/** @file
  FPGA_UPILINK_FUNC0.h

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


#ifndef _FPGA_UPILINK_FUNC0_h
#define _FPGA_UPILINK_FUNC0_h
#include <Base.h>

/* PCIE_FUNC0_0_FPGA_UPILINK_FUNC0_REG supported on:                            */
/*      WHR (0x20020000)                                                        */
/* Register default value on WHR: 0xBCC48086                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 0.
*/


#define PCIE_FUNC0_0_FPGA_UPILINK_FUNC0_REG 0x1A020000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vid : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00008086*/

                            /* This field is for vendor ID. */
    UINT32 did : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x0000BCC4*/

                            /* This field is for device ID. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_0_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_4_FPGA_UPILINK_FUNC0_REG supported on:                            */
/*      WHR (0x20020004)                                                        */
/* Register default value on WHR: 0x00100000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 4.
*/


#define PCIE_FUNC0_4_FPGA_UPILINK_FUNC0_REG 0x1A020004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_space_en : 1;

                            /* Bits[0:0], Access Type=RO/P, default=0x00000000*/

                            /* This field is for IO space enable. */
    UINT32 mem_space_en : 1;

                            /* Bits[1:1], Access Type=RO/P, default=0x00000000*/

                            /* This field is for memory space enable. */
    UINT32 bus_master_en : 1;

                            /* Bits[2:2], Access Type=RO/P, default=0x00000000*/

                            /* This field is for bus mater enable. */
    UINT32 spec_cyc_en : 1;

                            /* Bits[3:3], Access Type=RO/P, default=0x00000000*/

                            /* This field is for special cycle enable. */
    UINT32 mem_wr_inv_en : 1;

                            /* Bits[4:4], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for memory write and invalidate
                               enable.
                            */
    UINT32 vga_palette_snp_en : 1;

                            /* Bits[5:5], Access Type=RO/P, default=0x00000000*/

                            /* This field is for VGA palette snoop enable. */
    UINT32 par_err_rsp : 1;

                            /* Bits[6:6], Access Type=RO/P, default=0x00000000*/

                            /* This field is for parity error response. */
    UINT32 idsel_stp_wait_cyc_ctrl : 1;

                            /* Bits[7:7], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for IDSEL stepping wait cycle
                               control.
                            */
    UINT32 serr_en : 1;

                            /* Bits[8:8], Access Type=RO/P, default=0x00000000*/

                            /* This field is for SERR enables. */
    UINT32 fast_b2b_en : 1;

                            /* Bits[9:9], Access Type=RO/P, default=0x00000000*/

                            /* This field is for Fast back to back enable. */
    UINT32 intx_dis : 1;

                            /* Bits[10:10], Access Type=RO/P, default=0x00000000*/

                            /* This field is for INTx disable. */
    UINT32 rsvd0 : 5;

                            /* Bits[15:11], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 rsvd1 : 3;

                            /* Bits[18:16], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 intx_status : 1;

                            /* Bits[19:19], Access Type=RO/P, default=0x00000000*/

                            /* This field is for INTx status. */
    UINT32 cap_lst : 1;

                            /* Bits[20:20], Access Type=RO/P, default=0x00000001*/

                            /* This field is for capability list. */
    UINT32 x66mhz_cap : 1;

                            /* Bits[21:21], Access Type=RO/P, default=0x00000000*/

                            /* This field is for X66MHz capable. */
    UINT32 rsvd2 : 1;

                            /* Bits[22:22], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 fast_b2b : 1;

                            /* Bits[23:23], Access Type=RO/P, default=0x00000000*/

                            /* This field is for Fast back to back. */
    UINT32 mstr_data_par_err : 1;

                            /* Bits[24:24], Access Type=RO/P, default=0x00000000*/

                            /* This field is for master data parity error. */
    UINT32 devsel_timing : 2;

                            /* Bits[26:25], Access Type=RO/P, default=0x00000000*/

                            /* This field is for DEVSEL timing. */
    UINT32 signaled_tar_abort : 1;

                            /* Bits[27:27], Access Type=RO/P, default=0x00000000*/

                            /* This field is for signaled target abort. */
    UINT32 rcvd_tar_abort : 1;

                            /* Bits[28:28], Access Type=RO/P, default=0x00000000*/

                            /* This field is for received target abort. */
    UINT32 rcvd_mstr_abort : 1;

                            /* Bits[29:29], Access Type=RO/P, default=0x00000000*/

                            /* This field is for received master abort. */
    UINT32 signaled_sys_err : 1;

                            /* Bits[30:30], Access Type=RO/P, default=0x00000000*/

                            /* This field is for signaled system error. */
    UINT32 detected_par_err : 1;

                            /* Bits[31:31], Access Type=RO/P, default=0x00000000*/

                            /* This field is for detected parity error. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_4_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_8_FPGA_UPILINK_FUNC0_REG supported on:                            */
/*      WHR (0x20020008)                                                        */
/* Register default value on WHR: 0x08800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 8.
*/


#define PCIE_FUNC0_8_FPGA_UPILINK_FUNC0_REG 0x1A020008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rev_id : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000000*/

                            /* This field is for revision ID. */
    UINT32 reg_lvl_prog_intf : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for register level programming
                               interface.
                            */
    UINT32 sub_class : 8;

                            /* Bits[23:16], Access Type=RO/P, default=0x00000080*/

                            /* This field is for sub class code. */
    UINT32 base_calss : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000008*/

                            /* This field is for base class code. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_8_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_C_FPGA_UPILINK_FUNC0_REG supported on:                            */
/*      WHR (0x2002000c)                                                        */
/* Register default value on WHR: 0x00800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset C.
*/


#define PCIE_FUNC0_C_FPGA_UPILINK_FUNC0_REG 0x1A02000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cacheline_size : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000000*/

                            /* This field is for cache line size. */
    UINT32 primary_latency_timer : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /* This field is for primary latency timer. */
    UINT32 config_layout : 7;

                            /* Bits[22:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for configration layout; type 0
                               header.
                            */
    UINT32 multi_func_dev : 1;

                            /* Bits[23:23], Access Type=RO/P, default=0x00000001*/

                            /* This field is for multi-function device. */
    UINT32 bist_test : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* This field is for BIST test. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_C_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_34_FPGA_UPILINK_FUNC0_REG supported on:                           */
/*      WHR (0x20020034)                                                        */
/* Register default value on WHR: 0x00000040                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 34.
*/


#define PCIE_FUNC0_34_FPGA_UPILINK_FUNC0_REG 0x1A020034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_ptr : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000040*/

                            /* This field is for capability structure pointer. */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_34_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_3C_FPGA_UPILINK_FUNC0_REG supported on:                           */
/*      WHR (0x2002003c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 3C.
*/


#define PCIE_FUNC0_3C_FPGA_UPILINK_FUNC0_REG 0x1A02003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 int_line : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000000*/

                            /* This field is for Interrupt line. */
    UINT32 int_pin : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /* This field is for interrupt pin. */
    UINT32 mgv : 8;

                            /* Bits[23:16], Access Type=RO/P, default=0x00000000*/

                            /* This field is for minimum grant. */
    UINT32 mlv : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* This field is for maximum latency. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_3C_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_40_FPGA_UPILINK_FUNC0_REG supported on:                           */
/*      WHR (0x20020040)                                                        */
/* Register default value on WHR: 0x00920010                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 40.
*/


#define PCIE_FUNC0_40_FPGA_UPILINK_FUNC0_REG 0x1A020040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO/P, default=0x00000010*/

                            /* This field is for capability ID. */
    UINT32 next_ptr : 8;

                            /* Bits[15:8], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for next capability structure
                               pointer; no more capability structures.
                            */
    UINT32 cap_ver : 4;

                            /* Bits[19:16], Access Type=RO/P, default=0x00000002*/

                            /* This field is for capability version. */
    UINT32 dev_port_type : 4;

                            /* Bits[23:20], Access Type=RO/P, default=0x00000009*/

                            /*
                               This field is for device port type; device type
                               is root complex integrated endpoint.
                            */
    UINT32 slot_imp : 1;

                            /* Bits[24:24], Access Type=RO/P, default=0x00000000*/

                            /* This field is for slot implemented. */
    UINT32 int_msg_num : 5;

                            /* Bits[29:25], Access Type=RO/P, default=0x00000000*/

                            /* This field is for interrupt message number. */
    UINT32 rsvd0 : 2;

                            /* Bits[31:30], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_40_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LCP_FPGA_UPILINK_FUNC0_REG supported on:                                  */
/*      WHR (0x20020044)                                                        */
/* Register default value on WHR: 0x0007F000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Capability; UPI specified register.
*/


#define LL_LCP_FPGA_UPILINK_FUNC0_REG 0x1A020044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upi_prot_ver : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* This field is for Version of KTI port: KTI 1.0. */
    UINT32 rsvd0 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT32 crc_mode : 2;

                            /* Bits[9:8], Access Type=RO, default=0x00000000*/

                            /*
                               This field is for CRC Mode supported: CRC16
                               only.
                            */
    UINT32 rsvd1 : 2;

                            /* Bits[11:10], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT32 rtryq_wrapval : 8;

                            /* Bits[19:12], Access Type=RO, default=0x0000007F*/

                            /*
                               This field is for Link layer retry queue wrap
                               value; default 128 entries.
                            */
    UINT32 rsvd2 : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LCP_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LS_FPGA_UPILINK_FUNC0_REG supported on:                                   */
/*      WHR (0x20020048)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Layer Status; UPI specified register.
*/


#define LL_LS_FPGA_UPILINK_FUNC0_REG 0x1A020048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtryq_val_etry : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of valid entries in
                               retry queue.
                            */
    UINT32 init_state : 2;

                            /* Bits[9:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for current initialization status
                               of LL.
                            */
    UINT32 rtry_state : 3;

                            /* Bits[12:10], Access Type=RO/V, default=0x00000000*/

                            /* This field is for LRSM state. */
    UINT32 rx_normal_op : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for This bit is set to 1 upon the
                               first LRSM entry to RETRY_LOCAL_NORMAL after any
                               reset.
                            */
    UINT32 cur_crc_mode : 2;

                            /* Bits[15:14], Access Type=RO/V, default=0x00000000*/

                            /* This field is for Current CRC mode: CRC16 only. */
    UINT32 init_done : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* This field is for Link initialization done. */
    UINT32 rsvd0 : 15;

                            /* Bits[31:17], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LS_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LP0_FPGA_UPILINK_FUNC0_REG supported on:                                  */
/*      WHR (0x2002004c)                                                        */
/* Register default value on WHR: 0x09000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Received Parameter 0 from remote LL partner, updated after init flit exchange; UPI specified register.
*/


#define LL_LP0_FPGA_UPILINK_FUNC0_REG 0x1A02004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upi_ver : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for KTI Version. */
    UINT32 sku_type : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /* This field is for SKU Type. */
    UINT32 port_num : 5;

                            /* Bits[12:8], Access Type=RO/V, default=0x00000000*/

                            /* This field is for Port Number. */
    UINT32 rsvd0 : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* Reserved. */
    UINT32 lt_agent : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* This field is for LT Agent. */
    UINT32 fw_agent : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* This field is for Firmware Agent. */
    UINT32 base_node_id : 4;

                            /* Bits[19:16], Access Type=RO/V, default=0x00000000*/

                            /* This field is for Base Node ID. */
    UINT32 rsvd1 : 4;

                            /* Bits[23:20], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT32 rx_rtry_wrap_val : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000009*/

                            /* This field is for RX LLR Wrap Value. */

  } Bits;
  UINT32 Data;

} LL_LP0_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LP1_FPGA_UPILINK_FUNC0_REG supported on:                                  */
/*      WHR (0x20020050)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Received Parameter 1 from remote LL partner, updated after init flit exchange; UPI specified register.
*/


#define LL_LP1_FPGA_UPILINK_FUNC0_REG 0x1A020050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LP1_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_TX_CRD_FPGA_UPILINK_FUNC0_REG supported on:                               */
/*      WHR (0x20020054)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Transmit Credit Status; UPI specified register.
*/


#define LL_TX_CRD_FPGA_UPILINK_FUNC0_REG 0x1A020054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_vna_crd_avail : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for TX VNA credits available for
                               transmission.
                            */
    UINT32 tx_vn0_crd_avail : 6;

                            /* Bits[13:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for TX VN0 credits available for
                               transmission; each bit corresponds to one of 6
                               VCs in the order of {NCS, NCB, WB, RSP, SNP,
                               REQ.}
                            */
    UINT32 tx_vn1_crd_avail : 6;

                            /* Bits[19:14], Access Type=RO, default=0x00000000*/

                            /*
                               This field is for TX VN1 credits available for
                               transmission; each bit corresponds to one of 6
                               VCs in the order of {NCS, NCB, WB, RSP, SNP,
                               REQ}; due to no VN1 credit support, this field
                               is hardwired to 0.
                            */
    UINT32 rsvd0 : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_TX_CRD_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LES_FPGA_UPILINK_FUNC0_REG supported on:                                  */
/*      WHR (0x20020058)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Layer Error Status; UPI specified register.
*/


#define LL_LES_FPGA_UPILINK_FUNC0_REG 0x1A020058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_failed_retry : 4;

                            /* Bits[3:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for highest NUM_RETRY count
                               achieved in combination with the highest
                               NUM_PHY_REINIT count achieved since the last
                               software write all 1's to clear this field.
                            */
    UINT32 rsvd1 : 4;

                            /* Bits[7:4], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 num_phy_init : 2;

                            /* Bits[9:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for highest NUM_PHY_REINIT count
                               achieved since the last SW write of all 1's to
                               clear this field.
                            */
    UINT32 rsvd0 : 22;

                            /* Bits[31:10], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LES_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LCL_FPGA_UPILINK_FUNC0_REG supported on:                                  */
/*      WHR (0x2002005c)                                                        */
/* Register default value on WHR: 0x00003F01                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Layer Control; UPI specified register.
*/


#define LL_LCL_FPGA_UPILINK_FUNC0_REG 0x1A02005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ll_rst : 1;

                            /* Bits[0:0], Access Type=RW/1S/V, default=0x00000001*/

                            /*
                               This field is for Link layer reset; hardware
                               clears this bit after reset done.
                            */
    UINT32 debug_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Enable sending of debug flit;
                               not supported.
                            */
    UINT32 init_stall : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Stall LLCTRL-INIT.Param flit
                               transmission on next init until this bit is
                               cleared.
                            */
    UINT32 tx_hold_crd : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Stall the refund of initial LL
                               credits on next init until this bit is cleared.
                            */
    UINT32 rtry_to : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Retry Timeout Value; 000: 4095
                               flits; 001: 2047 flits; 010: 1023 flits; 011:
                               511 flits; 100: 255 flits; 101: 127 flits; 110:
                               63 flits; 111: 31 flits.
                            */
    UINT32 rsvd1 : 1;

                            /* Bits[7:7], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 max_retry : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x0000000F*/

                            /*
                               This field is for Number of failed retries to
                               trigger PHY reset.
                            */
    UINT32 max_phy_abort : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000003*/

                            /*
                               This field is for Max PHY reinitialization
                               before retry abort.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[31:14], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LCL_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LCCL_FPGA_UPILINK_FUNC0_REG supported on:                                 */
/*      WHR (0x20020060)                                                        */
/* Register default value on WHR: 0x0092497C                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Layer Credit Control; UPI specified register.
*/


#define LL_LCCL_FPGA_UPILINK_FUNC0_REG 0x1A020060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_max : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x0000007C*/

                            /*
                               This field is for VNA credit up to 128 flits to
                               advertise at next link initialization.
                            */
    UINT32 vn0_req : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for VN0 REQ credit to advertise at
                               next link initialization.
                            */
    UINT32 vn0_snp : 3;

                            /* Bits[13:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for VN0 SNP credit to advertise at
                               next link initialization.
                            */
    UINT32 vn0_rsp : 3;

                            /* Bits[16:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for VN0 RSP credit to advertise at
                               next link initialization.
                            */
    UINT32 vn0_wb : 3;

                            /* Bits[19:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for VN0 WB credit to advertise at
                               next link initialization.
                            */
    UINT32 vn0_ncb : 3;

                            /* Bits[22:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for VN0 NCB credit to advertise at
                               next link initialization.
                            */
    UINT32 vn0_ncs : 3;

                            /* Bits[25:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for VN0 NCS credit to advertise at
                               next link initialization.
                            */
    UINT32 vn1_en : 1;

                            /* Bits[26:26], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for Use VN0 credit for VN1
                               channels.. VN0 value below applies to
                               corresponding channels in VN1 channels. VN1
                               credit is not supported, thus hardwire to 0
                            */
    UINT32 rsvd0 : 5;

                            /* Bits[31:27], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LCCL_FPGA_UPILINK_FUNC0_STRUCT;

/* LL_LDCL_FPGA_UPILINK_FUNC0_REG supported on:                                 */
/*      WHR (0x20020064)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Link Layer DC; UPI specified register.
*/


#define LL_LDCL_FPGA_UPILINK_FUNC0_REG 0x1A020064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot1_dis : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for When set to 1, the slot 1
                               opcode can only be SLOT_NULL; this bit takes
                               effect immediately upon CSR write.
                            */
    UINT32 slot2_dis : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for When set to 1, the slot 2
                               opcode can only be LLCRD.. Non-zero values for
                               LLCRD payload are allowed. This bit takes effect
                               immediately upon CSR write
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} LL_LDCL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_LL_LP0_FPGA_UPILINK_FUNC0_REG supported on:                       */
/*      WHR (0x20020068)                                                        */
/* Register default value on WHR: 0x7F000010                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI TX Link Paramters 0, payload of the init flit sent from local Tx to remote LL parnter.
*/


#define WHR_UPI_TX_LL_LP0_FPGA_UPILINK_FUNC0_REG 0x1A020068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upi_ver : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for KTI Version. */
    UINT32 sku_type : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000001*/

                            /* This field is for SKU Type. */
    UINT32 port_num : 5;

                            /* Bits[12:8], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Port Number. */
    UINT32 rsvd0 : 1;

                            /* Bits[13:13], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 lt_agent : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* This field is for LT Agent. */
    UINT32 fw_agent : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Firmware Agent. */
    UINT32 base_node_id : 4;

                            /* Bits[19:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for this field should be
                               configured by whr_upi_node_id_ctrl.
                            */
    UINT32 rsvd1 : 4;

                            /* Bits[23:20], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 rx_rtry_wrap_val : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x0000007F*/

                            /* This field is for RX LLR Wrap Value. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_LL_LP0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_LL_LP1_FPGA_UPILINK_FUNC0_REG supported on:                       */
/*      WHR (0x2002006c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI TX Link Paramters 1, payload of the init flit sent from local Tx to remote LL parnter.
*/


#define WHR_UPI_TX_LL_LP1_FPGA_UPILINK_FUNC0_REG 0x1A02006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_LL_LP1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_INIT_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                       */
/*      WHR (0x20020070)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Initialization Control.
*/


#define WHR_UPI_INIT_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wait_proc_init : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for wait for protocol
                               initializtion done before sending init flit.
                            */
    UINT32 rsvd0 : 31;

                            /* Bits[31:1], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_INIT_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CSR_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x20020074)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI CSR Control.
*/


#define WHR_UPI_CSR_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 func0_cfg_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field is for CSR lock for 1st 4KB config
                               space allocated to UPI_CORE, i.e., offset 0x0000
                               - 0x0FFF; 0: CSRs are writable; 1: CSRs are read
                               only, and write takes no effect; CSR unlock to
                               lock can be done by any master at anytime, but
                               lock to unlock can only happen when IP_SEC[1:0]
                               cannot equal 2'b00.
                            */
    UINT32 func1_cfg_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This field is for CSR Lock for 2nd 4KB config
                               space allocated to UPI_CORE, i.e., offset 0x1000
                               - 0x1FFF; 0: CSRs are writable; 1: CSRs are read
                               only, and write takes no effect; CSR unlock to
                               lock can be done by any master at anytime, but
                               lock to unlock can only happen when IP_SEC[1:0]
                               cannot equal 2'b00.
                            */
    UINT32 rsvd0 : 30;

                            /* Bits[31:2], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CSR_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CLK_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x20020078)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Clock Control.
*/


#define WHR_UPI_CLK_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clk_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for clock enable. */
    UINT32 rsvd0 : 31;

                            /* Bits[31:1], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CLK_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RST_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x2002007c)                                                        */
/* Register default value on WHR: 0x00000001                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Reset Control.
*/


#define WHR_UPI_RST_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A02007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ll_rst_phy_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for 0: disable phy reset during
                               soft reset 1: enable phy reset during soft
                               reset.
                            */
    UINT32 rsvd0 : 31;

                            /* Bits[31:1], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RST_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                         */
/*      WHR (0x20020080)                                                        */
/* Register default value on WHR: 0x00000021                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI RX Control.
*/


#define WHR_UPI_RX_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p1_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for 0: RX Port 1 Disable 1: RX
                               Port 1 Enable.
                            */
    UINT32 p2_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: RX Port 2 Disable 1: RX
                               Port 2 Enable.
                            */
    UINT32 p3_en : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: RX Port 3 Disable 1: RX
                               Port 3 Enable.
                            */
    UINT32 p4_en : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: RX Port 4 Disable 1: RX
                               Port 4 Enable.
                            */
    UINT32 p5_en : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: RX Port 5 Disable 1: RX
                               Port 5 Enable.
                            */
    UINT32 halt_upon_fatal : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Halt Rx traffic upon detecting
                               fatal error.
                            */
    UINT32 lrsm_wait_tx_normal_op : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 1: LRSM will wait for
                               rx_normal_op or tx_normal_op before transition
                               from LRSM_PHY_REINIT state to LRSM_LLRREQ
                               state0: LRSM will only wait for rx_normal_op
                               before transition from LRSM_PHY_REINIT state to
                               LRSM_LLRREQ state.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                         */
/*      WHR (0x20020084)                                                        */
/* Register default value on WHR: 0x00021141                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI TX Control.
*/


#define WHR_UPI_TX_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for 0: Disable Tx sending poison
                               flits 1: Enable Tx sending poison flits.
                            */
    UINT32 buf_full_thre : 7;

                            /* Bits[7:1], Access Type=RW/P, default=0x00000020*/

                            /* This field is for Tx buffer full threshold. */
    UINT32 halt_upon_fatal : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Halt Tx traffic upon detecting
                               fatal error.
                            */
    UINT32 rtry_buf_ok_thre : 8;

                            /* Bits[16:9], Access Type=RW/P, default=0x00000008*/

                            /*
                               This field is for Threshold of number of retry
                               buffer available when Tx stops sending protocol
                               cycles.
                            */
    UINT32 rtry_buf_ok_llcrd_thre : 8;

                            /* Bits[24:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Threshold of number of retry
                               buffer available when Tx stops sending LLCRD.
                            */
    UINT32 rsvd0 : 7;

                            /* Bits[31:25], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CRD_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x20020088)                                                        */
/* Register default value on WHR: 0x00000480                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI credit Control register.
*/


#define WHR_UPI_CRD_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_stall : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VNA credits as per
                               spec 1: Stall returning all VNA credits; this
                               bit creates credit check window, and it's easier
                               for credit checking compared to using
                               backpressure from protocol layer.
                            */
    UINT32 vn0_req_stall : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VN0 REQ credits as
                               per spec 1: Stall returning all VN0 REQ credits;
                               this bit creates credit check window, and it's
                               easier for credit checking compared to using
                               backpressure from protocol layer.
                            */
    UINT32 vn0_snp_stall : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VN0 Snoop credits as
                               per spec 1: Stall returning all VN0 Snoop
                               credits; this bit creates credit check window,
                               and it's easier for credit checking compared to
                               using backpressure from protocol layer.
                            */
    UINT32 vn0_rsp_stall : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VN0 RESP credits as
                               per spec 1: Stall returning all VN0 RESP
                               credits; this bit creates credit check window,
                               and it's easier for credit checking compared to
                               using backpressure from protocol layer.
                            */
    UINT32 vn0_wb_stall : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VN0 WB credits as
                               per spec 1: Stall returning all VN0 WB credits;
                               this bit creates credit check window, and it's
                               easier for credit checking compared to using
                               backpressure from protocol layer.
                            */
    UINT32 vn0_ncb_stall : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VN0 NCB credits as
                               per spec 1: Stall returning all VN0 NCB credits;
                               this bit creates credit check window, and it's
                               easier for credit checking compared to using
                               backpressure from protocol layer.
                            */
    UINT32 vn0_ncs_stall : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Retun VN0 NCS credits as
                               per spec 1: Stall returning all VN0 NCS credits;
                               this bit creates credit check window, and it's
                               easier for credit checking compared to using
                               backpressure from protocol layer.
                            */
    UINT32 vn0_crd_rtrn_op_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for 0: disable VN0 credit return
                               optimization 1: enable VN0 credit return
                               optimization.
                            */
    UINT32 tx_vna_ok_thre : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000004*/

                            /*
                               This field is for Threshold of number of VNA
                               credits available when Tx stops sending protocol
                               cycles; This field has to be >= 4 in order for
                               logic to function correctly; Severe performance
                               degradation will be seen when this field is
                               closed to 128; When this field >= 128, Tx logic
                               can only use VN0 credits.
                            */
    UINT32 rsvd0 : 16;

                            /* Bits[31:16], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CRD_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CFG_FPGA_UPILINK_FUNC0_REG supported on:                             */
/*      WHR (0x2002008c)                                                        */
/* Register default value on WHR: 0x00000007                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI CFG Control register.
*/


#define WHR_UPI_CFG_FPGA_UPILINK_FUNC0_REG 0x1A02008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snapshot_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for PLD AVMM interface snapshot
                               feature enable.
                            */
    UINT32 inband_en : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* This field is for UPI_CFG inband access enable. */
    UINT32 pld_avmm_en : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* This field is for PLD AVMM access enable. */
    UINT32 rsvd0 : 29;

                            /* Bits[31:3], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CFG_FPGA_UPILINK_FUNC0_STRUCT;





/* WHR_UPI_ECC_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200a0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI ECC control register.
*/


#define WHR_UPI_ECC_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A0200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ecc_dec_en : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: bypass ECC1: ECC with 1
                               cycle latency2: ECC with 2 cycle latency.
                            */
    UINT32 rx_ecc_enc_en : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: bypass ECC1: ECC with 1
                               cycle latency2: ECC with 2 cycle latency.
                            */
    UINT32 rsvd0 : 28;

                            /* Bits[31:4], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_ECC_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CRC_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200a4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI CRC control register.
*/


#define WHR_UPI_CRC_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A0200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_inj_mode : 2;

                            /* Bits[1:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Reserved; User should not write 2'b11 to this
                               field.
                            */
    UINT32 tx_inj_mode : 2;

                            /* Bits[3:2], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Reserved; User should not write 2'b11 to this
                               field.
                            */
    UINT32 rsvd0 : 28;

                            /* Bits[31:4], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CRC_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_UE_EN_0_FPGA_UPILINK_FUNC0_REG supported on:                         */
/*      WHR (0x200200a8)                                                        */
/* Register default value on WHR: 0xFFFFFFFE                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Uncorrectable Error Enable Register.
*/


#define WHR_UPI_UE_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A0200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_rcvd : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Poison flit received (Error
                               can be masked if system can handle poison.)
                            */
    UINT32 rx_rspod_par_0 : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspod_par_1 : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspod_par_2 : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wb_be_par : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_0 : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_1 : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_2 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncs_be_par : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_0 : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_1 : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_2 : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncb_be_par : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_0 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_1 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_2 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspcd_par : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for RX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspod_par_0 : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspod_par_1 : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspod_par_2 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspcd_par_0 : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspcd_par_1 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspcd_par_2 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_wb_be_par : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_0 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_1 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_2 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncs_be_par : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_0 : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_1 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_2 : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncb_be_par : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_UE_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_UE_EN_1_FPGA_UPILINK_FUNC0_REG supported on:                         */
/*      WHR (0x200200ac)                                                        */
/* Register default value on WHR: 0x0000007F                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Uncorrectable Error Enable Register.
*/


#define WHR_UPI_UE_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A0200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ncbd_par_0 : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncbd_par_1 : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncbd_par_2 : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ecc_p0_data_0 : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_1 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_2 : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_3 : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_UE_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_EN_FPGA_UPILINK_FUNC0_REG supported on:                           */
/*      WHR (0x200200b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Correctable Error Enable Register.
*/


#define WHR_UPI_CE_EN_FPGA_UPILINK_FUNC0_REG 0x1A0200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ecc_p0_header : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p1_header : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 1 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p2_header : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 2 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p3_header : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 3 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p4_header : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 4 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p5_header : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 5 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p0_data_0 : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_1 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_2 : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_3 : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 rx_llctrl_err : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Reserved LLCTRL flit. */
    UINT32 rx_llctrl_dbg_err : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Rx received debug flit. */
    UINT32 rx_crc_err : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Rx detects CRC error. */
    UINT32 rx_viral_rcvd : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Unsupported Viral flit
                               received.
                            */
    UINT32 pld_avmm_snapshot_err : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for PLD AVMM snapshot rule
                               violated.
                            */
    UINT32 rsvd0 : 17;

                            /* Bits[31:15], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_EN_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_MISC_FATAL_EN_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x200200b4)                                                        */
/* Register default value on WHR: 0x000000FF                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Misc Fatal Error Enable Register.
*/


#define WHR_UPI_MISC_FATAL_EN_FPGA_UPILINK_FUNC0_REG 0x1A0200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_deskew_lock_err : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx deskew loses lock during
                               operation.. This will result into Tx
                               transmitting unknow flits
                            */
    UINT32 pll_lock_lost : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for UPI Core clock PLL lock is
                               deasserted.
                            */
    UINT32 tx_ecc_p0_header : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 0 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p1_header : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 1 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p2_header : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 2 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p3_header : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 3 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p4_header : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 4 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p5_header : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Port 5 header ECC double-
                               bit error.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_MISC_FATAL_EN_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_EN_0_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x200200b8)                                                        */
/* Register default value on WHR: 0xFFFFFFFF                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Rx Fatal Error Enable Register.
*/


#define WHR_UPI_RX_FATAL_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A0200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_static_err : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Received flit Static Bits does
                               not match as per spec.
                            */
    UINT32 nonzero_rsvd_field_err : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for non-zero VNA/VN fields in data
                               flits or headless data flits detected.
                            */
    UINT32 illegal_vn0_flit_err : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx received VN0 flit with slot
                               1 non-NULL or slot 2 non-LLCRD.
                            */
    UINT32 illegal_llctrl_slot12_err : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for LLCTRL flit slot 1 is not
                               SLOT_NULL or slot 2 is not LLCRD with all 0's.
                            */
    UINT32 rx_vn1_err : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx received VN1 flits. */
    UINT32 rx_vn01_crd_err : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* Reserved channel encodings. */
    UINT32 rx_vn0_err : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx received VN0 flit without
                               VN0 credit .. This means VNA buffer does not
                               have space to store for the incoming flit
                            */
    UINT32 rx_vna_err : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx received VNA flits without
                               VNA credits.. This means RX buffer does not have
                               space to store incoming flits
                            */
    UINT32 rtry_abort_err : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for LRSM reaches RETRY_ABORT
                               state.
                            */
    UINT32 unexpected_rtry_ack_err : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx received unexpected retry
                               ACK.
                            */
    UINT32 unexpected_poison_err : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Received Poison flit recevied
                               independely with out interleved data flits..
                            */
    UINT32 snp_urun : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspoh_urun : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspod_urun : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspu_urun : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wbh_urun : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wb_be_urun : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wbd_urun : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncsh_urun : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncs_be_urun : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncsd_urun : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncbh_urun : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncb_be_urun : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncbd_urun : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 req_urun : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspch_urun : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspcd_urun : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 snp_orun : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspoh_orun : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspod_orun : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspu_orun : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 wbh_orun : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_EN_1_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x200200bc)                                                        */
/* Register default value on WHR: 0x3FFFFFFF                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Rx Fatal Error Enable Register.
*/


#define WHR_UPI_RX_FATAL_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A0200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wb_be_orun : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 wbd_orun : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncsh_orun : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncs_be_orun : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncsd_orun : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncbh_orun : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncb_be_orun : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncbd_orun : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 req_orun : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspch_orun : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspcd_orun : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 snp_par_0 : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 snp_par_1 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par_0 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par_1 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_0 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_1 : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_2 : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_3 : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 wbh_par_0 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 wbh_par_1 : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par_0 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par_1 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncbh_par : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_0 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_1 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_2 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_3 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspch_par : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000001*/

                            /* This field is for RX CFG Buffer Parity Error. */
    UINT32 rx_rcvd_rsvd_op : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* Reserved opcode error. */
    UINT32 rsvd0 : 2;

                            /* Bits[31:30], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_FATAL_EN_0_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x200200c0)                                                        */
/* Register default value on WHR: 0xFFFFFFFC                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Tx Fatal Error Enable Register.
*/


#define WHR_UPI_TX_FATAL_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A0200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_vn0_crd_err : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Total VN0 Credits returned
                               from remote entity are more than programed VN0
                               credit (Tx check avail crd counter.)
                            */
    UINT32 tx_vna_crd_err : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Total VNA Credits returned
                               from remote entity are more than programed VNA
                               credit (Tx check avail crd counter.)
                            */
    UINT32 init_rsvd_after_init_done_err : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Init flit is received after
                               Link Init done.
                            */
    UINT32 vld_flit_rsvd_b4init_done_err : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Valid flit is received before
                               init done (UPI spec 1.0) (Valid Flit means any
                               flit other than LLCTRL-NULL/RETRY/DEBUG.)
                            */
    UINT32 tx_rtry_err : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Eseq in received retry req is
                               out of range defined by retry queue rd_ptr and
                               wr_ptr.
                            */
    UINT32 snp_urun : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspoh_urun : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspod_urun : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspu_urun : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspch_urun : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspcd_urun : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wbh_urun : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wb_be_urun : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wbd_urun : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncsh_urun : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncs_be_urun : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncsd_urun : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncbh_urun : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncb_be_urun : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncbd_urun : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 req_urun : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rtry_urun : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Retry Buffer Underrun. */
    UINT32 snp_orun : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspoh_orun : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspod_orun : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspu_orun : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspch_orun : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspcd_orun : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wbh_orun : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wb_be_orun : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wbd_orun : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncsh_orun : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_FATAL_EN_1_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x200200c4)                                                        */
/* Register default value on WHR: 0x00FFFFFF                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Tx Fatal Error Enable Register.
*/


#define WHR_UPI_TX_FATAL_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A0200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ncs_be_orun : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncsd_orun : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncbh_orun : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncb_be_orun : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncbd_orun : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 req_orun : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rtry_orun : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx Retry Buffer Overrun. */
    UINT32 snp_par_0 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 snp_par_1 : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_0 : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_1 : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_2 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_3 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspch_par : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* This field is for TX CFG Buffer Parity Error. */
    UINT32 wbh_par : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncbh_par : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_0 : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_1 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_2 : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_3 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rtry_par_0 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rtry_par_1 : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rsvd0 : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_0_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200c8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_0_FPGA_UPILINK_FUNC0_REG 0x1A0200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 first_ce_vld : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field.
                            */
    UINT32 err_id : 5;

                            /* Bits[5:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_ce_vld is set by
                               HW, the entire CE log cannot be changed by HW
                               until SW writes 1 to clear this field.
                            */
    UINT32 rsvd0 : 26;

                            /* Bits[31:6], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_1_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200cc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_1_FPGA_UPILINK_FUNC0_REG 0x1A0200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field; this log stores flit UI 0-3.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_2_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200d0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_2_FPGA_UPILINK_FUNC0_REG 0x1A0200D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field; this log stores flit UI 4-7.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_2_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_3_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200d4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_3_FPGA_UPILINK_FUNC0_REG 0x1A0200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field; this log stores flit UI 8-11.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_3_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_4_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200d8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_4_FPGA_UPILINK_FUNC0_REG 0x1A0200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field; this log stores flit UI 12-15.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_4_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_5_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200dc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_5_FPGA_UPILINK_FUNC0_REG 0x1A0200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field; this log stores flit UI 16-19.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_5_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_LOG_6_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x200200e0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Correctable Error Log Register.
*/


#define WHR_UPI_CE_LOG_6_FPGA_UPILINK_FUNC0_REG 0x1A0200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for first correctable error valid;
                               once set by HW, the entire CE log cannot be
                               changed by HW until SW writes 1 to clear this
                               field; this log stores flit UI 20-23.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_LOG_6_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_0_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200e4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_0_FPGA_UPILINK_FUNC0_REG 0x1A0200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 first_rx_fatal_vld : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for first rx fatal error valid;
                               once set by HW, the entire rx fatal log cannot
                               be changed by HW until SW writes 1 to clear this
                               field.
                            */
    UINT32 err_id : 5;

                            /* Bits[5:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field.
                            */
    UINT32 rsvd0 : 26;

                            /* Bits[31:6], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_1_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200e8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_1_FPGA_UPILINK_FUNC0_REG 0x1A0200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this log stores flit UI 0-3 for Error ID 0-6, or
                               a snapshot of whr_upi_rx_vna_crd_trk_0 for debug
                               for Error ID 7-8.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_2_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200ec)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_2_FPGA_UPILINK_FUNC0_REG 0x1A0200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this log stores flit UI 4-7 for Error ID 0-6, or
                               a snapshot of whr_upi_rx_vna_crd_trk_1 for debug
                               for Error ID 7-8.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_2_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_3_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200f0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_3_FPGA_UPILINK_FUNC0_REG 0x1A0200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this log stores flit UI 8-11 for Error ID 0-6,
                               or a snapshot of whr_upi_rx_vna_crd_trk_2 for
                               debug for Error ID 7-8.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_3_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_4_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200f4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_4_FPGA_UPILINK_FUNC0_REG 0x1A0200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this log stores flit UI 12-15 for Error ID 0-6,
                               or a snapshot of whr_upi_rx_vn0_crd_trk_0 for
                               debug for Error ID 7-8.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_4_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_5_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200f8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_5_FPGA_UPILINK_FUNC0_REG 0x1A0200F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this log stores flit UI 16-19 for Error ID 0-6,
                               or a snapshot of whr_upi_rx_vn0_crd_trk_1 for
                               debug for Error ID 7-8.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_5_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_6_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x200200fc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_6_FPGA_UPILINK_FUNC0_REG 0x1A0200FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this log stores flit UI 20-23.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_6_FPGA_UPILINK_FUNC0_STRUCT;

/* PCIE_FUNC0_100_FPGA_UPILINK_FUNC0_REG supported on:                          */
/*      WHR (0x20020100)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 100.
*/


#define PCIE_FUNC0_100_FPGA_UPILINK_FUNC0_REG 0x1A020100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00000000*/

                            /* This field is for capability ID. */
    UINT32 cap_ver : 4;

                            /* Bits[19:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for next capability structure
                               pointer; no more capability structures.
                            */
    UINT32 next_cap_offset : 12;

                            /* Bits[31:20], Access Type=RO/P, default=0x00000000*/

                            /* This field is for capability version. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC0_100_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_REG supported on:                        */
/*      WHR (0x20020104)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for FPGA Bus Number Register.
*/


#define WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_REG 0x1A020104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num : 13;

                            /* Bits[12:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for WHR UPI Bus Number for inband
                               CFG cycle addr[27:15] decode.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[30:13], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 num_vld : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0 : Programed bus number is
                               Invalid1 : Programed bus number is Valid.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_FPGA_BUS_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_LOG_7_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x20020108)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Rx Fatal Error Log Register.
*/


#define WHR_UPI_RX_FATAL_LOG_7_FPGA_UPILINK_FUNC0_REG 0x1A020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VNA credits to be
                               returned in Rx.
                            */
    UINT32 vn0_req_cnt : 3;

                            /* Bits[10:8], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VN0 REQ credits to
                               be returned in Rx.
                            */
    UINT32 vn0_snp_cnt : 3;

                            /* Bits[13:11], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VN0 SNP credits to
                               be returned in Rx.
                            */
    UINT32 vn0_rsp_cnt : 3;

                            /* Bits[16:14], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VN0 SNP credits to
                               be returned in Rx.
                            */
    UINT32 vn0_wb_cnt : 3;

                            /* Bits[19:17], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VN0 WB credits to be
                               returned in Rx.
                            */
    UINT32 vn0_ncs_cnt : 3;

                            /* Bits[22:20], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VN0 NCS credits to
                               be returned in Rx.
                            */
    UINT32 vn0_ncb_cnt : 3;

                            /* Bits[25:23], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_rx_fatal_vld is set
                               by HW, the entire rx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this field stores available VN0 NCB credits to
                               be returned in Rx.
                            */
    UINT32 rsvd0 : 6;

                            /* Bits[31:26], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_LOG_7_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_FATAL_LOG_0_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x2002010c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Tx Fatal Error Log Register.
*/


#define WHR_UPI_TX_FATAL_LOG_0_FPGA_UPILINK_FUNC0_REG 0x1A02010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 first_tx_fatal_vld : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for first tx fatal error valid;
                               once set by HW, the entire tx fatal log cannot
                               be changed by HW until SW writes 1 to clear this
                               field.
                            */
    UINT32 err_id : 5;

                            /* Bits[5:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field.
                            */
    UINT32 rsvd0 : 26;

                            /* Bits[31:6], Access Type=RO/V/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_LOG_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_FATAL_LOG_1_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x20020110)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Tx Fatal Error Log Register.
*/


#define WHR_UPI_TX_FATAL_LOG_1_FPGA_UPILINK_FUNC0_REG 0x1A020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VNA credits for
                               Tx transmission.
                            */
    UINT32 vn0_req_cnt : 3;

                            /* Bits[10:8], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VN0 REQ credits
                               for Tx transmission.
                            */
    UINT32 vn0_snp_cnt : 3;

                            /* Bits[13:11], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VN0 SNP credits
                               for Tx transmission.
                            */
    UINT32 vn0_rsp_cnt : 3;

                            /* Bits[16:14], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VN0 RSP credits
                               for Tx transmission.
                            */
    UINT32 vn0_wb_cnt : 3;

                            /* Bits[19:17], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VN0 WB credits
                               for Tx transmission.
                            */
    UINT32 vn0_ncs_cnt : 3;

                            /* Bits[22:20], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VN0 NCS credits
                               for Tx transmission.
                            */
    UINT32 vn0_ncb_cnt : 3;

                            /* Bits[25:23], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for once first_tx_fatal_vld is set
                               by HW, the entire tx fatal log cannot be changed
                               by HW until SW writes 1 to clear this field;
                               this error log stores available VN0 NCB credits
                               for Tx transmission.
                            */
    UINT32 rsvd0 : 6;

                            /* Bits[31:26], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_LOG_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_UE_INJ_EN_0_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x20020114)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Uncorrectable Error Injection Enable Register.. 
                       A write to this location will inject the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_UE_INJ_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_rcvd : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Poison flit received (Error
                               can be masked if system can handle poison.)
                            */
    UINT32 rx_rspod_par_0 : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspod_par_1 : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspod_par_2 : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wb_be_par : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_0 : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_1 : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_2 : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncs_be_par : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_0 : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_1 : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_2 : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncb_be_par : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_0 : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_1 : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_2 : 1;

                            /* Bits[15:15], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspcd_par : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for RX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspod_par_0 : 1;

                            /* Bits[17:17], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspod_par_1 : 1;

                            /* Bits[18:18], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspod_par_2 : 1;

                            /* Bits[19:19], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspcd_par_0 : 1;

                            /* Bits[20:20], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspcd_par_1 : 1;

                            /* Bits[21:21], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspcd_par_2 : 1;

                            /* Bits[22:22], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_wb_be_par : 1;

                            /* Bits[23:23], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_0 : 1;

                            /* Bits[24:24], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_1 : 1;

                            /* Bits[25:25], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_2 : 1;

                            /* Bits[26:26], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncs_be_par : 1;

                            /* Bits[27:27], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_0 : 1;

                            /* Bits[28:28], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_1 : 1;

                            /* Bits[29:29], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_2 : 1;

                            /* Bits[30:30], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncb_be_par : 1;

                            /* Bits[31:31], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_UE_INJ_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_UE_INJ_EN_1_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x20020118)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Uncorrectable Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_UE_INJ_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ncbd_par_0 : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncbd_par_1 : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncbd_par_2 : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ecc_p0_data_0 : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_1 : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_2 : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_3 : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_UE_INJ_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_CE_INJ_EN_FPGA_UPILINK_FUNC0_REG supported on:                       */
/*      WHR (0x2002011c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Correctable Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_CE_INJ_EN_FPGA_UPILINK_FUNC0_REG 0x1A02011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ecc_p0_header : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p1_header : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 1 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p2_header : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 2 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p3_header : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 3 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p4_header : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 4 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p5_header : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 5 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p0_data_0 : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_1 : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_2 : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_3 : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 rx_llctrl_err : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* Reserved LLCTRL flit. */
    UINT32 rx_llctrl_dbg_err : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx received debug flit. */
    UINT32 rx_crc_err : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx detects CRC error. */
    UINT32 rx_viral_rcvd : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Unsupported Viral flit
                               received.
                            */
    UINT32 pld_avmm_snapshot_err : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for PLD AVMM snapshot rule
                               violated.
                            */
    UINT32 rsvd0 : 17;

                            /* Bits[31:15], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_INJ_EN_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_MISC_FATAL_INJ_EN_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x20020120)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Misc Fatal Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_MISC_FATAL_INJ_EN_FPGA_UPILINK_FUNC0_REG 0x1A020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_deskew_lock_err : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx deskew loses lock during
                               operation.. This will result into Tx
                               transmitting unknow flits
                            */
    UINT32 pll_lock_lost : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for UPI Core clock PLL lock is
                               deasserted.
                            */
    UINT32 tx_ecc_p0_header : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p1_header : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 1 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p2_header : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 2 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p3_header : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 3 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p4_header : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 4 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p5_header : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 5 header ECC double-
                               bit error.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_MISC_FATAL_INJ_EN_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x20020124)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Rx Fatal Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_RX_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_static_err : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Received flit Static Bits does
                               not match as per spec.
                            */
    UINT32 nonzero_rsvd_field_err : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for non-zero VNA/VN fields in data
                               flits or headless data flits detected.
                            */
    UINT32 illegal_vn0_flit_err : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VN0 flit with slot
                               1 non-NULL or slot 2 non-LLCRD.
                            */
    UINT32 illegal_llctrl_slot12_err : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for LLCTRL flit slot 1 is not
                               SLOT_NULL or slot 2 is not LLCRD with all 0's.
                            */
    UINT32 rx_vn1_err : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx received VN1 flits. */
    UINT32 rx_vn01_crd_err : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* Reserved channel encodings. */
    UINT32 rx_vn0_err : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VN0 flit without
                               VN0 credit .. This means VNA buffer does not
                               have space to store for the incoming flit
                            */
    UINT32 rx_vna_err : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VNA flits without
                               VNA credits.. This means RX buffer does not have
                               space to store incoming flits
                            */
    UINT32 rtry_abort_err : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for LRSM reaches RETRY_ABORT
                               state.
                            */
    UINT32 unexpected_rtry_ack_err : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received unexpected retry
                               ACK.
                            */
    UINT32 unexpected_poison_err : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Received Poison flit recevied
                               independely with out interleved data flits..
                            */
    UINT32 snp_urun : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspoh_urun : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspod_urun : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspu_urun : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wbh_urun : 1;

                            /* Bits[15:15], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wb_be_urun : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wbd_urun : 1;

                            /* Bits[17:17], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncsh_urun : 1;

                            /* Bits[18:18], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncs_be_urun : 1;

                            /* Bits[19:19], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncsd_urun : 1;

                            /* Bits[20:20], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncbh_urun : 1;

                            /* Bits[21:21], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncb_be_urun : 1;

                            /* Bits[22:22], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncbd_urun : 1;

                            /* Bits[23:23], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 req_urun : 1;

                            /* Bits[24:24], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspch_urun : 1;

                            /* Bits[25:25], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspcd_urun : 1;

                            /* Bits[26:26], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 snp_orun : 1;

                            /* Bits[27:27], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspoh_orun : 1;

                            /* Bits[28:28], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspod_orun : 1;

                            /* Bits[29:29], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspu_orun : 1;

                            /* Bits[30:30], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 wbh_orun : 1;

                            /* Bits[31:31], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_RX_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x20020128)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Rx Fatal Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_RX_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wb_be_orun : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 wbd_orun : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncsh_orun : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncs_be_orun : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncsd_orun : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncbh_orun : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncb_be_orun : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncbd_orun : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 req_orun : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspch_orun : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspcd_orun : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 snp_par_0 : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 snp_par_1 : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par_0 : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par_1 : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_0 : 1;

                            /* Bits[15:15], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_1 : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_2 : 1;

                            /* Bits[17:17], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_3 : 1;

                            /* Bits[18:18], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 wbh_par_0 : 1;

                            /* Bits[19:19], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 wbh_par_1 : 1;

                            /* Bits[20:20], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par_0 : 1;

                            /* Bits[21:21], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par_1 : 1;

                            /* Bits[22:22], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncbh_par : 1;

                            /* Bits[23:23], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_0 : 1;

                            /* Bits[24:24], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_1 : 1;

                            /* Bits[25:25], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_2 : 1;

                            /* Bits[26:26], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_3 : 1;

                            /* Bits[27:27], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspch_par : 1;

                            /* Bits[28:28], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for RX CFG Buffer Parity Error. */
    UINT32 rx_rcvd_rsvd_op : 1;

                            /* Bits[29:29], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* Reserved opcode error. */
    UINT32 rsvd0 : 2;

                            /* Bits[31:30], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x2002012c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Tx Fatal Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_TX_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A02012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_vn0_crd_err : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Total VN0 Credits returned
                               from remote entity are more than programed VN0
                               credit (Tx check avail crd counter.)
                            */
    UINT32 tx_vna_crd_err : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Total VNA Credits returned
                               from remote entity are more than programed VNA
                               credit (Tx check avail crd counter.)
                            */
    UINT32 init_rsvd_after_init_done_err : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Init flit is received after
                               Link Init done.
                            */
    UINT32 vld_flit_rsvd_b4init_done_err : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Valid flit is received before
                               init done (UPI spec 1.0) (Valid Flit means any
                               flit other than LLCTRL-NULL/RETRY/DEBUG.)
                            */
    UINT32 tx_rtry_err : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Eseq in received retry req is
                               out of range defined by retry queue rd_ptr and
                               wr_ptr.
                            */
    UINT32 snp_urun : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspoh_urun : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspod_urun : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspu_urun : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspch_urun : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspcd_urun : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wbh_urun : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wb_be_urun : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wbd_urun : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncsh_urun : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncs_be_urun : 1;

                            /* Bits[15:15], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncsd_urun : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncbh_urun : 1;

                            /* Bits[17:17], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncb_be_urun : 1;

                            /* Bits[18:18], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncbd_urun : 1;

                            /* Bits[19:19], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 req_urun : 1;

                            /* Bits[20:20], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rtry_urun : 1;

                            /* Bits[21:21], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Retry Buffer Underrun. */
    UINT32 snp_orun : 1;

                            /* Bits[22:22], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspoh_orun : 1;

                            /* Bits[23:23], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspod_orun : 1;

                            /* Bits[24:24], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspu_orun : 1;

                            /* Bits[25:25], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspch_orun : 1;

                            /* Bits[26:26], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspcd_orun : 1;

                            /* Bits[27:27], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wbh_orun : 1;

                            /* Bits[28:28], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wb_be_orun : 1;

                            /* Bits[29:29], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wbd_orun : 1;

                            /* Bits[30:30], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncsh_orun : 1;

                            /* Bits[31:31], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_TX_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x20020130)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Tx Fatal Error Injection Enable Register.. 
                       A write to this location will enable the interrupt, and HW will self clear this bit
*/


#define WHR_UPI_TX_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A020130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ncs_be_orun : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncsd_orun : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncbh_orun : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncb_be_orun : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncbd_orun : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 req_orun : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rtry_orun : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx Retry Buffer Overrun. */
    UINT32 snp_par_0 : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 snp_par_1 : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_0 : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_1 : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_2 : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_3 : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspch_par : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for TX CFG Buffer Parity Error. */
    UINT32 wbh_par : 1;

                            /* Bits[15:15], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncbh_par : 1;

                            /* Bits[17:17], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_0 : 1;

                            /* Bits[18:18], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_1 : 1;

                            /* Bits[19:19], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_2 : 1;

                            /* Bits[20:20], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_3 : 1;

                            /* Bits[21:21], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rtry_par_0 : 1;

                            /* Bits[22:22], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rtry_par_1 : 1;

                            /* Bits[23:23], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rsvd0 : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_DESKEW_CTRL_0_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020134)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Deskew Control Register.
*/


#define WHR_UPI_DESKEW_CTRL_0_FPGA_UPILINK_FUNC0_REG 0x1A020134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_dsk_clr : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for deskew clear common for header
                               0, header 1 & data.
                            */
    UINT32 rx_vld : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Valid signal to start deskew
                               pulse generation.
                            */
    UINT32 force_dsk_vld : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for For troubleshooting (default
                               to 0)1: When in ERROR state, force datapath to
                               be enabled regardless of error.
                            */
    UINT32 wait_count_2x : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 1: double the 'retry wait
                               count' timer from 16 to 32 (for UPI)0: use
                               default 'retry wait count' timer value of 16.
                            */
    UINT32 rx_dsk_pulse_per_2x : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 1: double the 'period' of rx
                               deskew pulse from 64 to 1280: use default
                               'period' = 64.
                            */
    UINT32 rsvd0 : 27;

                            /* Bits[31:5], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_DESKEW_CTRL_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_DESKEW_CTRL_1_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020138)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Deskew Control Register.
*/


#define WHR_UPI_DESKEW_CTRL_1_FPGA_UPILINK_FUNC0_REG 0x1A020138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_DESKEW_CTRL_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_NODE_ID_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                    */
/*      WHR (0x20020140)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Node ID Control Register.
*/


#define WHR_UPI_NODE_ID_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A020140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 node_id_sel : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: Node ID is from strap pins
                               (Node ID = strap pins + 2); 1: Node ID is from
                               CSR register.
                            */
    UINT32 node_id_csr : 4;

                            /* Bits[4:1], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Node ID from CSR. */
    UINT32 rsvd0 : 27;

                            /* Bits[31:5], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_NODE_ID_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_DBG_CTRL_0_FPGA_UPILINK_FUNC0_REG supported on:                      */
/*      WHR (0x20020144)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Debug Control Register.
*/


#define WHR_UPI_DBG_CTRL_0_FPGA_UPILINK_FUNC0_REG 0x1A020144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 send_flit_over_aib : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for send rx flit received / tx
                               flit transmitted over AIB using data port when
                               there is no data transfer during normal
                               operation.
                            */
    UINT32 send_tx_rx_flit : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: send tx flit; 1: send rx
                               flit.
                            */
    UINT32 aib_dbg_ll_phy_sel : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for send LL/PHY DBG signals over
                               AIB during normal operation (0: LL DBG; 1: PHY
                               DBG); note during PHY initialization, PHY needs
                               all AIB BW, and RX arb will only send PHY DBG
                               signals.. The mux selection from PHY controls
                               this behavior
                            */
    UINT32 aib_dbg_sync_sel : 5;

                            /* Bits[7:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for LL DBG signal over AIB mux
                               selection during normal operation.
                            */
    UINT32 clr_tx_dbg_cnt : 1;

                            /* Bits[8:8], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This field is for clear all Tx debug counters;
                               HW self clear this bit.
                            */
    UINT32 clr_rx_dbg_cnt : 1;

                            /* Bits[9:9], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This field is for clear all Rx debug counters;
                               HW self clear this bit.
                            */
    UINT32 dis_phy_dbg_b4_ll_init : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Disable phy_dbg over AIB,
                               during LL initialization.
                            */
    UINT32 rsvd0 : 21;

                            /* Bits[31:11], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_DBG_CTRL_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_DBG_CTRL_1_FPGA_UPILINK_FUNC0_REG supported on:                      */
/*      WHR (0x20020148)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Debug Control Register.
*/


#define WHR_UPI_DBG_CTRL_1_FPGA_UPILINK_FUNC0_REG 0x1A020148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfd_sel : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: mux_sel is from DFD ctrl
                               CSR; 1; mux_sel is from TEST_IN (PLD fabric.)
                            */
    UINT32 dfd_ll_sel : 7;

                            /* Bits[7:1], Access Type=RW/P, default=0x00000000*/

                            /* This field is for LL DFD signal mux selection. */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_DBG_CTRL_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_AIB_FREEZE_CTRL_FPGA_UPILINK_FUNC0_REG supported on:                 */
/*      WHR (0x2002014c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI AIB Freeze Control Register.
*/


#define WHR_UPI_AIB_FREEZE_CTRL_FPGA_UPILINK_FUNC0_REG 0x1A02014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 force_unfreeze_sr_avmm : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for force unfreezing FSR/SSR/User
                               AVMM path.
                            */
    UINT32 force_unfreeze_tx_rx_sync : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for force unfreezing Tx/Rx sync
                               path.
                            */
    UINT32 rsvd0 : 30;

                            /* Bits[31:2], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_AIB_FREEZE_CTRL_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_CTRL_0_FPGA_UPILINK_FUNC0_REG supported on:                       */
/*      WHR (0x20020400)                                                        */
/* Register default value on WHR: 0x40401C00                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Control Register 0.
*/


#define WHR_UPI_SA_CTRL_0_FPGA_UPILINK_FUNC0_REG 0x1A020400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Snoop assist enable. */
    UINT32 sa_mode : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 1'b0: snoop assist uses page
                               table/CAM to perform address check for incoming
                               snoop; FPGA performs allocation/deallocation to
                               update page talbe/CAM1'b1: snoop assist uses
                               static address range CSRs to perform address
                               check for incoming snoops; This static address
                               range is programmed by whr_upi_sa_sw_alloc_ctrl,
                               and SA will not expect allocation/dealocation
                               from FPGA.
                            */
    UINT32 page_size : 3;

                            /* Bits[4:2], Access Type=RW/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 full_threshold : 9;

                            /* Bits[13:5], Access Type=RW/P, default=0x000000E0*/

                            /*
                               This field is for Full threshold for each page
                               table/CAM.
                            */
    UINT32 sad_src : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for 0: SAD is based on CSR; 1: SAD
                               is based on FSR/SSR signals from FPGA.
                            */
    UINT32 dbg_cnt_clr : 1;

                            /* Bits[15:15], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This field is for Setting this bit will clear
                               all snoop assist debug counters; and HW self
                               clears this bit.
                            */
    UINT32 cache_ch_intrv_bit_l : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for lower index to cache line
                               address that is used for address interleaving in
                               FPGA cache channel, i.e.,
                               {$_line_addr[cache_ch_intrv_bit_u],
                               $_line_addr[cache_ch_intrv_bit_l]} is used to
                               index CAM for incoming SNP or SW
                               allocation/deallocation.. This must be smaller
                               than cache_ch_intrv_bit_u
                            */
    UINT32 cache_ch_intrv_bit_u : 6;

                            /* Bits[27:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for upper index to cache line
                               address that is used for address interleaving in
                               FPGA cache channel, i.e.,
                               {$_line_addr[cache_ch_intrv_bit_u],
                               $_line_addr[cache_ch_intrv_bit_l]} is used to
                               index CAM for incoming SNP or SW
                               allocation/deallocation.. This must be larger
                               than cache_ch_intrv_bit_l and must be smaller
                               than 46
                            */
    UINT32 tx_buf_full_thre : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000004*/

                            /*
                               This field is for SA Tx SNP/RSP buffer full
                               threshold.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_CTRL_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_CTRL_1_FPGA_UPILINK_FUNC0_REG supported on:                       */
/*      WHR (0x20020404)                                                        */
/* Register default value on WHR: 0x00000001                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Control Register 1.
*/


#define WHR_UPI_SA_CTRL_1_FPGA_UPILINK_FUNC0_REG 0x1A020404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 halt_upon_fatal : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Halt SA traffic from/to
                               LL/FPGA upon detecting fatal error.
                            */
    UINT32 rsvd0 : 31;

                            /* Bits[31:1], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_CTRL_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_SW_ALLOC_CTRL_0_FPGA_UPILINK_FUNC0_REG supported on:              */
/*      WHR (0x20020408)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Software Allocation Register 0.
*/


#define WHR_UPI_SA_SW_ALLOC_CTRL_0_FPGA_UPILINK_FUNC0_REG 0x1A020408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This register allows SW to
                               allocate/deallocate a page to the CAM..
                               Allocate/Deallocate a page for addr[37:6]
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_SW_ALLOC_CTRL_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_SW_ALLOC_CTRL_1_FPGA_UPILINK_FUNC0_REG supported on:              */
/*      WHR (0x2002040c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Software Allocation Register 1.
*/


#define WHR_UPI_SA_SW_ALLOC_CTRL_1_FPGA_UPILINK_FUNC0_REG 0x1A02040C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 14;

                            /* Bits[13:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This register allows SW to
                               allocate/deallocate a page to the CAM..
                               Allocate/Deallocate a page for addr[51:38]
                            */
    UINT32 rsvd0 : 1;

                            /* Bits[14:14], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 cnt : 13;

                            /* Bits[27:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for The initial value of the
                               counter in the CAM when SW performs allocation.
                            */
    UINT32 cmd : 3;

                            /* Bits[30:28], Access Type=RW/P, default=0x00000000*/

                            /* Reserved. */
    UINT32 vld : 1;

                            /* Bits[31:31], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This field is for When this bit is set, the
                               matching page will be allocated to/deallocated
                               from the CAM; hardware clears this bit after
                               allocation/deallocation completes.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_SW_ALLOC_CTRL_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE0_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020410)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 0.
*/


#define WHR_UPI_SA_DRAM_RULE0_FPGA_UPILINK_FUNC0_REG 0x1A020410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address;
                               lower limit is 0 for DRAM_RULE_0.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L0_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x20020414)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 0.
*/


#define WHR_UPI_SA_INTRV_L0_FPGA_UPILINK_FUNC0_REG 0x1A020414

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE1_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020418)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 1.
*/


#define WHR_UPI_SA_DRAM_RULE1_FPGA_UPILINK_FUNC0_REG 0x1A020418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L1_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x2002041c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 1.
*/


#define WHR_UPI_SA_INTRV_L1_FPGA_UPILINK_FUNC0_REG 0x1A02041C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE2_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020420)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 2.
*/


#define WHR_UPI_SA_DRAM_RULE2_FPGA_UPILINK_FUNC0_REG 0x1A020420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE2_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L2_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x20020424)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 2.
*/


#define WHR_UPI_SA_INTRV_L2_FPGA_UPILINK_FUNC0_REG 0x1A020424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L2_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE3_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020428)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 3.
*/


#define WHR_UPI_SA_DRAM_RULE3_FPGA_UPILINK_FUNC0_REG 0x1A020428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE3_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L3_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x2002042c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 3.
*/


#define WHR_UPI_SA_INTRV_L3_FPGA_UPILINK_FUNC0_REG 0x1A02042C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L3_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE4_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020430)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 4.
*/


#define WHR_UPI_SA_DRAM_RULE4_FPGA_UPILINK_FUNC0_REG 0x1A020430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE4_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L4_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x20020434)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 4.
*/


#define WHR_UPI_SA_INTRV_L4_FPGA_UPILINK_FUNC0_REG 0x1A020434

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L4_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE5_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020438)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 5.
*/


#define WHR_UPI_SA_DRAM_RULE5_FPGA_UPILINK_FUNC0_REG 0x1A020438

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE5_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L5_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x2002043c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 5.
*/


#define WHR_UPI_SA_INTRV_L5_FPGA_UPILINK_FUNC0_REG 0x1A02043C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L5_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE6_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020440)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 6.
*/


#define WHR_UPI_SA_DRAM_RULE6_FPGA_UPILINK_FUNC0_REG 0x1A020440

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE6_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L6_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x20020444)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 6.
*/


#define WHR_UPI_SA_INTRV_L6_FPGA_UPILINK_FUNC0_REG 0x1A020444

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L6_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_DRAM_RULE7_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020448)                                                        */
/* Register default value on WHR: 0x00000002                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist DRAM Rule 7.
*/


#define WHR_UPI_SA_DRAM_RULE7_FPGA_UPILINK_FUNC0_REG 0x1A020448

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rule_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* This field is for Enable this rule. */
    UINT32 interleave_mode : 2;

                            /* Bits[2:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for DRAM rule interleave mode.. If
                               a dram_rule hits a 3 bit number is used to index
                               into the corresponding interleave_list to
                               determain which package the DRAM belongs to.
                               This mode selects how that number is computed.
                               00: Address bits {8,7,6}. 01: Address bits
                               {10,9,8}. 10: Address bits {14,13,12}. 11:
                               Address bits {15,14,13}.
                            */
    UINT32 attr : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Sets the memory type for the
                               remote DRAM rule according to the following
                               encoding; 00: coherent DRAM; 01: memory mapped
                               CFG; 10: low bandwidth coherent DRAM; 11: high
                               bandwidth coherent DRAM (Xeon attr; unsupported
                               in FPGA.. FPGA only generates transactions to
                               DRAM, not MMCFG space)
                            */
    UINT32 nm_cacheable : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Specifies whether or not this
                               address range is cacheable in near memory; this
                               field has no effect for SA SAD.
                            */
    UINT32 limit : 26;

                            /* Bits[31:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for This corresponds to
                               addr[51:26] of DRAM rule top limit address; must
                               be strictly greater than previous rule, even if
                               this rule is disabled, unless this is and all
                               subsequent rules are disabled.. lower limit is 0
                               if this is the first rule
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DRAM_RULE7_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_INTRV_L7_FPGA_UPILINK_FUNC0_REG supported on:                     */
/*      WHR (0x2002044c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Interleave List 7.
*/


#define WHR_UPI_SA_INTRV_L7_FPGA_UPILINK_FUNC0_REG 0x1A02044C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkg0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 0 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 1 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 2 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 3 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg4 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 4 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg5 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 5 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg6 : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 6 - NodeID of
                               interleave list target.
                            */
    UINT32 pkg7 : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is for Package 7 - NodeID of
                               interleave list target.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_INTRV_L7_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x20020450)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Enable Register 0.
*/


#define WHR_UPI_SA_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A020450

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pt0_cnt_orun : 1;

                            /* Bits[0:0], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 0 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt1_cnt_orun : 1;

                            /* Bits[1:1], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 1 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt2_cnt_orun : 1;

                            /* Bits[2:2], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 2 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt3_cnt_orun : 1;

                            /* Bits[3:3], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 3 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt0_cnt_urun : 1;

                            /* Bits[4:4], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 0 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt1_cnt_urun : 1;

                            /* Bits[5:5], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 1 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt2_cnt_urun : 1;

                            /* Bits[6:6], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 2 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt3_cnt_urun : 1;

                            /* Bits[7:7], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 3 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt0_par : 1;

                            /* Bits[8:8], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Page table 0 parity error. */
    UINT32 pt1_par : 1;

                            /* Bits[9:9], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Page table 1 parity error. */
    UINT32 pt2_par : 1;

                            /* Bits[10:10], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Page tabe 2 parity error. */
    UINT32 pt3_par : 1;

                            /* Bits[11:11], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Page table 3 parity error. */
    UINT32 tx_rsp_par_0 : 1;

                            /* Bits[12:12], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer 0 parity error. */
    UINT32 tx_rsp_par_1 : 1;

                            /* Bits[13:13], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer 1 parity error. */
    UINT32 tx_snp_par_0 : 1;

                            /* Bits[14:14], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer 0
                               parity error.
                            */
    UINT32 tx_snp_par_1 : 1;

                            /* Bits[15:15], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer 1
                               parity error.
                            */
    UINT32 tx_rsp_orun : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer overrun. */
    UINT32 tx_snp_orun : 1;

                            /* Bits[17:17], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer
                               overrun.
                            */
    UINT32 tx_rsp_urun : 1;

                            /* Bits[18:18], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer underrun. */
    UINT32 tx_snp_urun : 1;

                            /* Bits[19:19], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer
                               underrun.
                            */
    UINT32 rsvd_fpga_ad_op : 1;

                            /* Bits[20:20], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* Reserved FPGA allocation/deallocation opcode. */
    UINT32 rsvd_sw_ad_op : 1;

                            /* Bits[21:21], Access Type=RW/1S/V/P, default=0x00000000*/

                            /* Reserved SW allocation/deallocation opcode. */
    UINT32 pt0_orun : 1;

                            /* Bits[22:22], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 0 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt1_orun : 1;

                            /* Bits[23:23], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 1 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt2_orun : 1;

                            /* Bits[24:24], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 2 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt3_orun : 1;

                            /* Bits[25:25], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 3 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 snp0_sad : 1;

                            /* Bits[26:26], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for SNP from LL Rx SNP buffer 0
                               fails the SAD; this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 snp1_sad : 1;

                            /* Bits[27:27], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               This field is for SNP from LL Rx SNP buffer 1
                               fails the SAD; this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FATAL_INJ_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_REG supported on:               */
/*      WHR (0x20020454)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Enable Register 1.
*/


#define WHR_UPI_SA_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A020454

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FATAL_INJ_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_FATAL_EN_0_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x20020458)                                                        */
/* Register default value on WHR: 0x0FFFFFFF                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Enable Register 0.
*/


#define WHR_UPI_SA_FATAL_EN_0_FPGA_UPILINK_FUNC0_REG 0x1A020458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pt0_cnt_orun : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 0 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt1_cnt_orun : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 1 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt2_cnt_orun : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 2 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt3_cnt_orun : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 3 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt0_cnt_urun : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 0 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt1_cnt_urun : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 1 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt2_cnt_urun : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 2 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt3_cnt_urun : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 3 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt0_par : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Page table 0 parity error. */
    UINT32 pt1_par : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Page table 1 parity error. */
    UINT32 pt2_par : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Page tabe 2 parity error. */
    UINT32 pt3_par : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Page table 3 parity error. */
    UINT32 tx_rsp_par_0 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx rsp buffer 0 parity error. */
    UINT32 tx_rsp_par_1 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx rsp buffer 1 parity error. */
    UINT32 tx_snp_par_0 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx filtered snp buffer 0
                               parity error.
                            */
    UINT32 tx_snp_par_1 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx filtered snp buffer 1
                               parity error.
                            */
    UINT32 tx_rsp_orun : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx rsp buffer overrun. */
    UINT32 tx_snp_orun : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx filtered snp buffer
                               overrun.
                            */
    UINT32 tx_rsp_urun : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* This field is for Tx rsp buffer underrun. */
    UINT32 tx_snp_urun : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Tx filtered snp buffer
                               underrun.
                            */
    UINT32 rsvd_fpga_ad_op : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000001*/

                            /* Reserved FPGA allocation/deallocation opcode. */
    UINT32 rsvd_sw_ad_op : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /* Reserved SW allocation/deallocation opcode. */
    UINT32 pt0_orun : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 0 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt1_orun : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 1 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt2_orun : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 2 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt3_orun : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for Page table 3 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 snp0_sad : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for SNP from LL Rx SNP buffer 0
                               fails the SAD; this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 snp1_sad : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is for SNP from LL Rx SNP buffer 1
                               fails the SAD; this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FATAL_EN_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_FATAL_EN_1_FPGA_UPILINK_FUNC0_REG supported on:                   */
/*      WHR (0x2002045c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Enable Register 1.
*/


#define WHR_UPI_SA_FATAL_EN_1_FPGA_UPILINK_FUNC0_REG 0x1A02045C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd0 : 32;

                            /* Bits[31:0], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FATAL_EN_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_FATAL_LOG_0_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x20020460)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Log Register 0.
*/


#define WHR_UPI_SA_FATAL_LOG_0_FPGA_UPILINK_FUNC0_REG 0x1A020460

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vld : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Error log valid; Error log
                               only logs the first error that happens, and
                               keeps its value until SW clears this bit.
                            */
    UINT32 err_id : 6;

                            /* Bits[6:1], Access Type=RO/V/P, default=0x00000000*/

                            /* This field is for Loggable Error ID. */
    UINT32 field0 : 6;

                            /* Bits[12:7], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for For pt[0-3]_cnt_[urun, orun]
                               errors, this field logs the counter value [5:0]
                               that is associated with the page number where
                               the error happens.
                            */
    UINT32 field1 : 7;

                            /* Bits[19:13], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for For pt[0-3]_cnt_[urun, orun]
                               errors, this field logs the counter value [12:6]
                               that is associated with the page number where
                               the error happens; for snp[0-1]_sad errors, this
                               field logs the address [11:5] of the SNP which
                               fails the SAD.
                            */
    UINT32 field2 : 12;

                            /* Bits[31:20], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for For pt[0-3]_cnt_[urun, orun]
                               errors, this field logs the page number [11:0]
                               where the error happens; for snp[0-1]_sad
                               errors, this field logs the address [23:12] of
                               the SNP which fails the SAD.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FATAL_LOG_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_FATAL_LOG_1_FPGA_UPILINK_FUNC0_REG supported on:                  */
/*      WHR (0x20020464)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Log Register 1.
*/


#define WHR_UPI_SA_FATAL_LOG_1_FPGA_UPILINK_FUNC0_REG 0x1A020464

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 field0 : 28;

                            /* Bits[27:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for For pt[0-3]_cnt_[urun, orun]
                               errors, this field logs the page number [39:12]
                               where the error happens; for snp[0-1]_sad
                               errors, this field logs the address [51:24] of
                               the SNP which fails the SAD.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FATAL_LOG_1_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_SW_LOOKUP_LOG_0_FPGA_UPILINK_FUNC0_REG supported on:              */
/*      WHR (0x20020468)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist SW Lookup Log Register 0.
*/


#define WHR_UPI_SA_SW_LOOKUP_LOG_0_FPGA_UPILINK_FUNC0_REG 0x1A020468

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vld : 1;

                            /* Bits[0:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This field is for Lookup log valid; When SW
                               lookup completes, this valid bit will complete.
                            */
    UINT32 addr : 31;

                            /* Bits[31:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for page number from SW lookup;
                               this field logs addr[42:12.]
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_SW_LOOKUP_LOG_0_FPGA_UPILINK_FUNC0_STRUCT;

/* WHR_UPI_SA_SW_LOOKUP_LOG_1_FPGA_UPILINK_FUNC0_REG supported on:              */
/*      WHR (0x2002046c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func0               */
/* Struct generated from WHR BDF: 0_4_0                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist SW Lookup Log Register 1.
*/


#define WHR_UPI_SA_SW_LOOKUP_LOG_1_FPGA_UPILINK_FUNC0_REG 0x1A02046C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 9;

                            /* Bits[8:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for page number from SW lookup;
                               this field logs addr[51:43.]
                            */
    UINT32 cnt : 13;

                            /* Bits[21:9], Access Type=RO/V/P, default=0x00000000*/

                            /* This field is for counter value from SW lookup. */
    UINT32 hm : 1;

                            /* Bits[22:22], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for SW lookup results; 1: hit, 0:
                               miss.
                            */
    UINT32 rsvd0 : 9;

                            /* Bits[31:23], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_SW_LOOKUP_LOG_1_FPGA_UPILINK_FUNC0_STRUCT;
#endif /* _FPGA_UPILINK_FUNC0_h */
