
/** @file
  FPGA_UPILINK_FUNC1.h

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


#ifndef _FPGA_UPILINK_FUNC1_h
#define _FPGA_UPILINK_FUNC1_h
#include <Base.h>

/* PCIE_FUNC1_0_FPGA_UPILINK_FUNC1_REG supported on:                            */
/*      WHR (0x20021000)                                                        */
/* Register default value on WHR: 0xBCC58086                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 0.
*/


#define PCIE_FUNC1_0_FPGA_UPILINK_FUNC1_REG 0x1A120000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vid : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00008086*/

                            /* This field is for vendor ID. */
    UINT32 did : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x0000BCC5*/

                            /* This field is for device ID. */

  } Bits;
  UINT32 Data;

} PCIE_FUNC1_0_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_4_FPGA_UPILINK_FUNC1_REG supported on:                            */
/*      WHR (0x20021004)                                                        */
/* Register default value on WHR: 0x00100000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 4.
*/


#define PCIE_FUNC1_4_FPGA_UPILINK_FUNC1_REG 0x1A120004

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

} PCIE_FUNC1_4_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_8_FPGA_UPILINK_FUNC1_REG supported on:                            */
/*      WHR (0x20021008)                                                        */
/* Register default value on WHR: 0x08800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 8.
*/


#define PCIE_FUNC1_8_FPGA_UPILINK_FUNC1_REG 0x1A120008

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

} PCIE_FUNC1_8_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_C_FPGA_UPILINK_FUNC1_REG supported on:                            */
/*      WHR (0x2002100c)                                                        */
/* Register default value on WHR: 0x00800000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset C.
*/


#define PCIE_FUNC1_C_FPGA_UPILINK_FUNC1_REG 0x1A12000C

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

} PCIE_FUNC1_C_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_34_FPGA_UPILINK_FUNC1_REG supported on:                           */
/*      WHR (0x20021034)                                                        */
/* Register default value on WHR: 0x00000040                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 34.
*/


#define PCIE_FUNC1_34_FPGA_UPILINK_FUNC1_REG 0x1A120034

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

} PCIE_FUNC1_34_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_3C_FPGA_UPILINK_FUNC1_REG supported on:                           */
/*      WHR (0x2002103c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 3C.
*/


#define PCIE_FUNC1_3C_FPGA_UPILINK_FUNC1_REG 0x1A12003C

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

} PCIE_FUNC1_3C_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_40_FPGA_UPILINK_FUNC1_REG supported on:                           */
/*      WHR (0x20021040)                                                        */
/* Register default value on WHR: 0x00920010                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCIe header function 0 offset 40.
*/


#define PCIE_FUNC1_40_FPGA_UPILINK_FUNC1_REG 0x1A120040

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

} PCIE_FUNC1_40_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_UE_STATUS_0_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x200210a8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Uncorrectable Error Status Registe.
*/


#define WHR_UPI_UE_STATUS_0_FPGA_UPILINK_FUNC1_REG 0x1A1200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_rcvd : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Poison flit received (Error
                               can be masked if system can handle poison.)
                            */
    UINT32 rx_rspod_par_0 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspod_par_1 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspod_par_2 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wb_be_par : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_0 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_1 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_wbd_par_2 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncs_be_par : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_0 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_1 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncsd_par_2 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncb_be_par : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_0 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_1 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_ncbd_par_2 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rx_rspcd_par : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for RX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspod_par_0 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspod_par_1 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspod_par_2 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_rspcd_par_0 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspcd_par_1 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_rspcd_par_2 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for TX CFG Data Buffer Parity
                               Error.
                            */
    UINT32 tx_wb_be_par : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_0 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_1 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_wbd_par_2 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncs_be_par : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_0 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_1 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncsd_par_2 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncb_be_par : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_UE_STATUS_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_UE_STATUS_1_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x200210ac)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Uncorrectable Error Status Registe.
*/


#define WHR_UPI_UE_STATUS_1_FPGA_UPILINK_FUNC1_REG 0x1A1200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ncbd_par_0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncbd_par_1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ncbd_par_2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 tx_ecc_p0_data_0 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_1 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_2 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_3 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC double-bit
                               error.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_UE_STATUS_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_CE_STATUS_FPGA_UPILINK_FUNC1_REG supported on:                       */
/*      WHR (0x200210b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Correctable Error Status Registe.
*/


#define WHR_UPI_CE_STATUS_FPGA_UPILINK_FUNC1_REG 0x1A1200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_ecc_p0_header : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p1_header : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 1 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p2_header : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 2 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p3_header : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 3 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p4_header : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 4 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p5_header : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 5 header ECC single-
                               bit error.
                            */
    UINT32 tx_ecc_p0_data_0 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_1 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_2 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 tx_ecc_p0_data_3 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 data ECC single-bit
                               error.
                            */
    UINT32 rx_llctrl_err : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Reserved LLCTRL flit.. Error_id is logged in
                               whr_upi_ce_log_0, and the entire flit is logged
                               in whr_upi_ce_log_1/2/3/4/5/6
                            */
    UINT32 rx_llctrl_dbg_err : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received debug flit..
                               Error_id is logged in whr_upi_ce_log_0, and the
                               entire flit is logged in
                               whr_upi_ce_log_1/2/3/4/5/6
                            */
    UINT32 rx_crc_err : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx detects CRC error. */
    UINT32 rx_viral_rcvd : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Unsupported Viral flit
                               received.. Error_id is logged in
                               whr_upi_ce_log_0, and the entire flit is logged
                               in whr_upi_ce_log_1/2/3/4/5/6
                            */
    UINT32 pld_avmm_snapshot_err : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for PLD AVMM snapshot rule
                               violated.
                            */
    UINT32 rsvd0 : 17;

                            /* Bits[31:15], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CE_STATUS_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_MISC_FATAL_STATUS_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200210b4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Misc Fatal Error Status Register.
*/


#define WHR_UPI_MISC_FATAL_STATUS_FPGA_UPILINK_FUNC1_REG 0x1A1200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_deskew_lock_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx deskew loses lock during
                               operation.. This will result into Tx
                               transmitting unknow flits
                            */
    UINT32 pll_lock_lost : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for UPI Core clock PLL lock is
                               deasserted.
                            */
    UINT32 tx_ecc_p0_header : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 0 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p1_header : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 1 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p2_header : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 2 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p3_header : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 3 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p4_header : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 4 header ECC double-
                               bit error.
                            */
    UINT32 tx_ecc_p5_header : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Port 5 header ECC double-
                               bit error.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_MISC_FATAL_STATUS_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200210b8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Rx Fatal Error Status Register.
*/


#define WHR_UPI_RX_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_REG 0x1A1200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flit_static_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Received flit Static Bits does
                               not match as per spec; Error_id is logged in
                               whr_upi_rx_fatal_log_0, and the entire flit is
                               logged in whr_upi_rx_fatal_log_1/2/3/4/5/6.
                            */
    UINT32 nonzero_rsvd_field_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for non-zero VNA/VN fields in data
                               flits or headless data flits detected; Error_id
                               is logged in whr_upi_rx_fatal_log_0, and the
                               entire flit is logged in
                               whr_upi_rx_fatal_log_1/2/3/4/5/6.
                            */
    UINT32 illegal_vn0_flit_err : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VN0 flit with slot
                               1 non-NULL or slot 2 non-LLCRD; Error_id is
                               logged in whr_upi_rx_fatal_log_0, and the entire
                               flit is logged in
                               whr_upi_rx_fatal_log_1/2/3/4/5/6.
                            */
    UINT32 illegal_llctrl_slot12_err : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for LLCTRL flit slot 1 is not
                               SLOT_NULL or slot 2 is not LLCRD with all 0's;
                               Error_id is logged in whr_upi_rx_fatal_log_0,
                               and the entire flit is logged in
                               whr_upi_rx_fatal_log_1/2/3/4/5/6.
                            */
    UINT32 rx_vn1_err : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VN1 flits;
                               Error_id is logged in whr_upi_rx_fatal_log_0,
                               and the entire flit is logged in
                               whr_upi_rx_fatal_log_1/2/3/4/5/6.
                            */
    UINT32 rx_vn01_crd_err : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Reserved channel encodings; Error_id is logged
                               in whr_upi_rx_fatal_log_0, and the entire flit
                               is logged in whr_upi_rx_fatal_log_1/2/3/4/5/6.
                            */
    UINT32 rx_vn0_err : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VN0 flit without
                               VN0 credit .. This means VNA buffer does not
                               have space to store for the incoming flit;
                               Error_id is logged in whr_upi_rx_fatal_log_0,
                               and credit counter values are logged in
                               whr_upi_rx_fatal_log_7
                            */
    UINT32 rx_vna_err : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received VNA flits without
                               VNA credits.. This means RX buffer does not have
                               space to store incoming flits; Error_id is
                               logged in whr_upi_rx_fatal_log_0, and credit
                               counter values are logged in
                               whr_upi_rx_fatal_log_7
                            */
    UINT32 rtry_abort_err : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for LRSM reaches RETRY_ABORT
                               state.
                            */
    UINT32 unexpected_rtry_ack_err : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx received unexpected retry
                               ACK.
                            */
    UINT32 unexpected_poison_err : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Received Poison flit recevied
                               independely with out interleved data flits..
                            */
    UINT32 snp_urun : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspoh_urun : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspod_urun : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspu_urun : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wbh_urun : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wb_be_urun : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 wbd_urun : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncsh_urun : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncs_be_urun : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncsd_urun : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncbh_urun : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncb_be_urun : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 ncbd_urun : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 req_urun : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspch_urun : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 rspcd_urun : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Underrun. */
    UINT32 snp_orun : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspoh_orun : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspod_orun : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspu_orun : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 wbh_orun : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200210bc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Rx Fatal Error Status Register.
*/


#define WHR_UPI_RX_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_REG 0x1A1200BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wb_be_orun : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 wbd_orun : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncsh_orun : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncs_be_orun : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncsd_orun : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncbh_orun : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncb_be_orun : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 ncbd_orun : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 req_orun : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspch_orun : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 rspcd_orun : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Rx Buffer Overrun. */
    UINT32 snp_par_0 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 snp_par_1 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par_0 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par_1 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_0 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_1 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_2 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspuh_par_3 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 wbh_par_0 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 wbh_par_1 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par_0 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par_1 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncbh_par : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_0 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_1 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_2 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_3 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Rx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspch_par : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for RX CFG Buffer Parity Error. */
    UINT32 rx_rcvd_rsvd_op : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved opcode error. */
    UINT32 rsvd0 : 2;

                            /* Bits[31:30], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200210c0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Tx Fatal Error Status Register.
*/


#define WHR_UPI_TX_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_REG 0x1A1200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_vn0_crd_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Total VN0 Credits returned
                               from remote entity are more than programed VN0
                               credit (Tx check avail crd counter); Error_id is
                               logged in whr_upi_tx_fatal_log_0, and credit
                               counter values are logged in
                               whr_upi_tx_fatal_log_1.
                            */
    UINT32 tx_vna_crd_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Total VNA Credits returned
                               from remote entity are more than programed VNA
                               credit (Tx check avail crd counter); Error_id is
                               logged in whr_upi_tx_fatal_log_0, and credit
                               counter values are logged in
                               whr_upi_tx_fatal_log_1.
                            */
    UINT32 init_rsvd_after_init_done_err : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Init flit is received after
                               Link Init done.
                            */
    UINT32 vld_flit_rsvd_b4init_done_err : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Valid flit is received before
                               init done (UPI spec 1.0) (Valid Flit means any
                               flit other than LLCTRL-NULL/RETRY/DEBUG.)
                            */
    UINT32 tx_rtry_err : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Eseq in received retry req is
                               out of range defined by retry queue rd_ptr and
                               wr_ptr.
                            */
    UINT32 snp_urun : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspoh_urun : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspod_urun : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspu_urun : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspch_urun : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rspcd_urun : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wbh_urun : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wb_be_urun : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 wbd_urun : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncsh_urun : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncs_be_urun : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncsd_urun : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncbh_urun : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncb_be_urun : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 ncbd_urun : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 req_urun : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Underrun. */
    UINT32 rtry_urun : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Retry Buffer Underrun. */
    UINT32 snp_orun : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspoh_orun : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspod_orun : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspu_orun : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspch_orun : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rspcd_orun : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wbh_orun : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wb_be_orun : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 wbd_orun : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncsh_orun : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200210c4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Tx Fatal Error Status Register.
*/


#define WHR_UPI_TX_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_REG 0x1A1200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ncs_be_orun : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncsd_orun : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncbh_orun : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncb_be_orun : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 ncbd_orun : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 req_orun : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Buffer Overrun. */
    UINT32 rtry_orun : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx Retry Buffer Overrun. */
    UINT32 snp_par_0 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 snp_par_1 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspoh_par : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_0 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_1 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_2 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspu_par_3 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rspch_par : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for TX CFG Buffer Parity Error. */
    UINT32 wbh_par : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncsh_par : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 ncbh_par : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_0 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_1 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_2 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 req_par_3 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rtry_par_0 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rtry_par_1 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx Buffer Parity Error
                               (Suffix: D is data buffer, H is header.)
                            */
    UINT32 rsvd0 : 8;

                            /* Bits[31:24], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_STRUCT;

/* PCIE_FUNC1_100_FPGA_UPILINK_FUNC1_REG supported on:                          */
/*      WHR (0x20021100)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for PCI Express Enhanced Capability.
*/


#define PCIE_FUNC1_100_FPGA_UPILINK_FUNC1_REG 0x1A120100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 16;

                            /* Bits[15:0], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for Capability_ID:There is no
                               capability at this location.
                            */
    UINT32 next_ptr : 4;

                            /* Bits[19:16], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for Capability_Version:There is no
                               capability at this location..
                            */
    UINT32 cap_ver : 12;

                            /* Bits[31:20], Access Type=RO/P, default=0x00000000*/

                            /*
                               This field is for Next_Capability_Offset:Pointer
                               to the next capability in the enhanced
                               configuration space.. Set to 0 to indicate there
                               are nomore capability structures.
                            */

  } Bits;
  UINT32 Data;

} PCIE_FUNC1_100_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_REQ_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021104)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX REQ Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_REQ_HED_FPGA_UPILINK_FUNC1_REG 0x1A120104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of REQ
                               Message received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_REQ_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_RESPU_HED_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021108)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESPU Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_RESPU_HED_FPGA_UPILINK_FUNC1_REG 0x1A120108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RESPU
                               Message received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_RESPU_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_RESPO_HED_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x2002110c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESPO Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_RESPO_HED_FPGA_UPILINK_FUNC1_REG 0x1A12010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RESPO
                               Message received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_RESPO_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_WB_HED_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x20021110)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_WB_HED_FPGA_UPILINK_FUNC1_REG 0x1A120110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of WB Message
                               received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_WB_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_NCS_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021114)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_NCS_HED_FPGA_UPILINK_FUNC1_REG 0x1A120114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCS
                               Message received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_NCS_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_NCB_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021118)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_NCB_HED_FPGA_UPILINK_FUNC1_REG 0x1A120118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCB
                               Message received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_NCB_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_SNP_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x2002111c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX SNP Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_SNP_HED_FPGA_UPILINK_FUNC1_REG 0x1A12011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of SNP
                               Message received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_SNP_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_RESPO_DATA_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x20021120)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESPO Data Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_RESPO_DATA_FPGA_UPILINK_FUNC1_REG 0x1A120120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RESPO Data
                               received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_RESPO_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_WB_DATA_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021124)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB Data Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_WB_DATA_FPGA_UPILINK_FUNC1_REG 0x1A120124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of WB Data
                               received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_WB_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_NCS_DATA_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021128)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS DataReceived; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_NCS_DATA_FPGA_UPILINK_FUNC1_REG 0x1A120128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCS Data
                               received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_NCS_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_NCB_DATA_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x2002112c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB Data Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_NCB_DATA_FPGA_UPILINK_FUNC1_REG 0x1A12012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCB Data
                               received from FPGA protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_NCB_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_TX_POISON_RCVD_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021130)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX Poison Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_TX_POISON_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A120130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of TX poison
                               request received from protocol layer.. This
                               includes the poison sent from FPGA or Tx ECC
                               encoder 2-bit error, but does not include buffer
                               parity errors.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_TX_POISON_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RTRY_REQ_SENT_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021134)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX Retry REQ Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_RTRY_REQ_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of TX Retry
                               Req flits sent.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RTRY_REQ_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RTRY_ACK_SENT_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021138)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX Retry ACK Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_RTRY_ACK_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of TX Retry
                               ACK flits sent.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RTRY_ACK_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RTRY_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x2002113c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX Retryable flits Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_RTRY_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A12013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               retry able flits sent..
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RTRY_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_TOTAL_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021140)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Total Number of flits sent on TX; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_TOTAL_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               flits sent.. This includes both control and
                               protocol flits
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_TOTAL_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_LLCRD_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021144)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX LLCRD Flits Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_LLCRD_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of TX LLCRD
                               flits sent.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_LLCRD_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_POISON_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021148)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX Poison Flits Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_POISON_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of TX Poison
                               flits sent.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_POISON_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_VNA_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x2002114c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX VNA flits Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_VNA_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A12014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VNA credits.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_VNA_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_REQ_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x20021150)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX REQ VN0 Flits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_REQ_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VN0 credits on REQ Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_REQ_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESP_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:           */
/*      WHR (0x20021154)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESP VN0 Flits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_RESP_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VN0 credits on RESP Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESP_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021158)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB VN0 Flits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_WB_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VN0 credits on WB Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x2002115c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS VN0 Flits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_NCS_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A12015C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VN0 credits on NCS Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x20021160)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB VN0 Flits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_NCB_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VN0 credits on NCB Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_SNP_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x20021164)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX SNP VN0 Flits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_SNP_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits sent using VN0 credits on SNP Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_SNP_VN0_FLIT_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_ACK_SENT_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x20021168)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX ACK Sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_ACK_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of ACKs
                               returned from local RX to remote TX..
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_ACK_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_VNA_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x2002116c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX VNA Credits sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_VNA_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A12016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of VNA
                               credits return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_VNA_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_REQ_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021170)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX REQ VN0 CRDs sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_REQ_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of REQ
                               VN0 CRDs return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_REQ_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESP_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x20021174)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESP VN0 CRDs sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_RESP_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RESP
                               VN0 CRDs return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESP_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021178)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB VN0 CRDs sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_WB_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of WB
                               VN0 CRDs return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x2002117c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS VN0 CRDs sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_NCS_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A12017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of NCS
                               VN0 CRDs return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021180)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB VN0 CRDs sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_NCB_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of NCB
                               VN0 CRDs return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_SNP_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021184)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX SNP VN0 CRDs sent; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_TX_SNP_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_REG 0x1A120184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of SNP
                               VN0 CRDs return from local RX to remote TX.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_SNP_VN0_CRD_SENT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_REQ_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021190)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX REQ Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_REQ_HED_FPGA_UPILINK_FUNC1_REG 0x1A120190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of REQ
                               Message received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_REQ_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_RESPU_HED_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021194)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPU Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_RESPU_HED_FPGA_UPILINK_FUNC1_REG 0x1A120194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RESPU
                               Message received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_RESPU_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_RESPO_HED_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021198)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPO Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_RESPO_HED_FPGA_UPILINK_FUNC1_REG 0x1A120198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RESPO
                               Message received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_RESPO_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_WB_HED_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x2002119c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_WB_HED_FPGA_UPILINK_FUNC1_REG 0x1A12019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of WB Message
                               received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_WB_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_NCS_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200211a0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_NCS_HED_FPGA_UPILINK_FUNC1_REG 0x1A1201A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCS
                               Message received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_NCS_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_NCB_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200211a4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_NCB_HED_FPGA_UPILINK_FUNC1_REG 0x1A1201A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCB
                               Message received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_NCB_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_SNP_HED_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200211ac)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX SNP Message Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_SNP_HED_FPGA_UPILINK_FUNC1_REG 0x1A1201AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of SNP
                               Message received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_SNP_HED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_RESPO_DATA_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200211b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPO Data Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_RESPO_DATA_FPGA_UPILINK_FUNC1_REG 0x1A1201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RESPO Data
                               received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_RESPO_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_WB_DATA_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200211b4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB Data Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_WB_DATA_FPGA_UPILINK_FUNC1_REG 0x1A1201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of WB Data
                               received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_WB_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_NCS_DATA_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x200211b8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS DataReceived; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_NCS_DATA_FPGA_UPILINK_FUNC1_REG 0x1A1201B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCS Data
                               received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_NCS_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_RX_NCB_DATA_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x200211bc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB Data Received; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_RX_NCB_DATA_FPGA_UPILINK_FUNC1_REG 0x1A1201BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of NCB Data
                               received from Remote protocol layer.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_RX_NCB_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RTRY_REQ_RCVD_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200211c0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX Retry REQ RCVD ; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_RTRY_REQ_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RX Retry
                               Req flits RCVD.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RTRY_REQ_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RTRY_ACK_RCVD_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200211c4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX Retry ACK RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_RTRY_ACK_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RX Retry
                               ACK flits RCVD.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RTRY_ACK_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RTRY_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200211c8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX Retryable flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_RTRY_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               retry able flits RCVD..
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RTRY_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VAL_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200211cc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Total Number of Valid flits RCVD on RX; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_VAL_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               flits recevied with atlest one valid protocol
                               cycle.. This flit should consume atlest one RX
                               buffer entry
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VAL_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_LLCRD_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x200211d0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX LLCRD Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_LLCRD_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RX LLCRD
                               flits RCVD.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_LLCRD_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_POISON_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x200211d4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX Poison Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_POISON_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of RX Poison
                               flits RCVD.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_POISON_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VNA_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200211d8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX VNA flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_VNA_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VNA credits.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VNA_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_REQ_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x200211dc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX REQ VN0 Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_REQ_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VN0 credits on REQ Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_REQ_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESP_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:           */
/*      WHR (0x200211e0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESP VN0 Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_RESP_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VN0 credits on RESP Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESP_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_WB_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x200211e4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB VN0 Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_WB_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VN0 credits on WB Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_WB_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCS_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x200211e8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS VN0 Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_NCS_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VN0 credits on NCS Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCS_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCB_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x200211ec)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB VN0 Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_NCB_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VN0 credits on NCB Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCB_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_SNP_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x200211f0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX SNP VN0 Flits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_SNP_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               flits RCVD using VN0 credits on SNP Message
                               class.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_SNP_VN0_FLIT_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_ACK_RCVD_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x200211f4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX ACK RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_ACK_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of ACKs
                               returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_ACK_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VNA_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x200211f8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX VNA Credits RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_VNA_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of VNA
                               credits returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VNA_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_REQ_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x200211fc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX REQ VN0 CRDs RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_REQ_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A1201FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of REQ
                               VN0 CRDs returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_REQ_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESP_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x20021200)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESP VN0 CRDs RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_RESP_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A120200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RESP
                               VN0 CRDs returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESP_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_WB_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021204)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB VN0 CRDs RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_WB_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A120204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of WB
                               VN0 CRDs returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_WB_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCS_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021208)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS VN0 CRDs RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_NCS_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A120208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of NCS
                               VN0 CRDs returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCS_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCB_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x2002120c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB VN0 CRDs RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_NCB_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A12020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of NCB
                               VN0 CRDs returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCB_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_SNP_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021210)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX SNP VN0 CRDs RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_SNP_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_REG 0x1A120210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of SNP
                               VN0 CRDs returned from remote link partner.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_SNP_VN0_CRD_RCVD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CRC_ERR_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x20021214)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX CRC Error RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CRC_ERR_FPGA_UPILINK_FUNC1_REG 0x1A120214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               CRC Error Detected.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CRC_ERR_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_INBAND_ACC_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x20021218)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX Inband Access RCVD; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_RX_INBAND_ACC_FPGA_UPILINK_FUNC1_REG 0x1A120218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               Inband Access RCVD.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_INBAND_ACC_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_0_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x2002121c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_0_FPGA_UPILINK_FUNC1_REG 0x1A12021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on P0.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_1_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021220)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_1_FPGA_UPILINK_FUNC1_REG 0x1A120220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on P1.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_2_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021224)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_2_FPGA_UPILINK_FUNC1_REG 0x1A120224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on P2.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_2_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_3_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021228)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_3_FPGA_UPILINK_FUNC1_REG 0x1A120228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on P3.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_3_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_4_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x2002122c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_4_FPGA_UPILINK_FUNC1_REG 0x1A12022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on P4.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_4_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_5_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021230)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_5_FPGA_UPILINK_FUNC1_REG 0x1A120230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on P5.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_5_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_CE_ERR_DATA_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021234)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of CE Error Detected on Data; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_CE_ERR_DATA_FPGA_UPILINK_FUNC1_REG 0x1A120234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Correctable Error Detected on Data.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_CE_ERR_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_0_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021238)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_0_FPGA_UPILINK_FUNC1_REG 0x1A120238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on P0.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_1_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x2002123c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_1_FPGA_UPILINK_FUNC1_REG 0x1A12023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on P1.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_2_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021240)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_2_FPGA_UPILINK_FUNC1_REG 0x1A120240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on P2.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_2_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_3_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021244)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_3_FPGA_UPILINK_FUNC1_REG 0x1A120244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on P3.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_3_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_4_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021248)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_4_FPGA_UPILINK_FUNC1_REG 0x1A120248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on P4.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_4_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_5_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x2002124c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_5_FPGA_UPILINK_FUNC1_REG 0x1A12024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on P5.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_5_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_NUM_UE_ERR_DATA_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021250)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of UE Error Detected on Data; This counter will saturate once it reaches the max value until SW clears it.
*/


#define WHR_UPI_NUM_UE_ERR_DATA_FPGA_UPILINK_FUNC1_REG 0x1A120250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of
                               Uncorrectable Error Detected on Data.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_NUM_UE_ERR_DATA_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_VNA_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021264)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX VNA Credits AVAL.
*/


#define WHR_UPI_TX_VNA_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               VNA CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_VNA_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_REQ_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021268)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX REQ VN0 CRDs AVAL.
*/


#define WHR_UPI_TX_REQ_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               REQ VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_REQ_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x2002126c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESP VN0 CRDs AVAL.
*/


#define WHR_UPI_TX_RESP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A12026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               RESP VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021270)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB VN0 CRDs AVAL.
*/


#define WHR_UPI_TX_WB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               WB VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021274)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS VN0 CRDs AVAL.
*/


#define WHR_UPI_TX_NCS_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               NCS VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021278)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB VN0 CRDs AVAL.
*/


#define WHR_UPI_TX_NCB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               NCB VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_SNP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x2002127c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX SNP VN0 CRDs AVAL.
*/


#define WHR_UPI_TX_SNP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A12027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               SNP VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_SNP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_ACK_AVAL_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x20021280)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX ACK AVAL.
*/


#define WHR_UPI_RX_ACK_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               ACK available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_ACK_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VNA_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021284)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX VNA Credits AVAL.
*/


#define WHR_UPI_RX_VNA_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               VNA CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VNA_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_REQ_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021288)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX REQ VN0 CRDs AVAL.
*/


#define WHR_UPI_RX_REQ_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               REQ VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_REQ_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x2002128c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESP VN0 CRDs AVAL.
*/


#define WHR_UPI_RX_RESP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A12028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               RESP VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_WB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021290)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB VN0 CRDs AVAL.
*/


#define WHR_UPI_RX_WB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               WB VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_WB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCS_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021294)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS VN0 CRDs AVAL.
*/


#define WHR_UPI_RX_NCS_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               NCS VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCS_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021298)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB VN0 CRDs AVAL.
*/


#define WHR_UPI_RX_NCB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A120298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               NCB VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCB_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_SNP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x2002129c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX SNP VN0 CRDs AVAL.
*/


#define WHR_UPI_RX_SNP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_REG 0x1A12029C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               SNP VN0 CRDs available.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_SNP_VN0_CRD_AVAL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_PHY_REINIT_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x200212a0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX PHY Reinit.
*/


#define WHR_UPI_RX_PHY_REINIT_FPGA_UPILINK_FUNC1_REG 0x1A1202A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of RX
                               PHY Reinit Done.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_PHY_REINIT_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RTRY_FPGA_UPILINK_FUNC1_REG supported on:                         */
/*      WHR (0x200212a4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX Retry.
*/


#define WHR_UPI_RX_RTRY_FPGA_UPILINK_FUNC1_REG 0x1A1202A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of RX Retry
                               Attempts since last normal state.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RTRY_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_ESEQ_FPGA_UPILINK_FUNC1_REG supported on:                         */
/*      WHR (0x200212a8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for RX Eseq Number.
*/


#define WHR_UPI_RX_ESEQ_FPGA_UPILINK_FUNC1_REG 0x1A1202A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates current RX Eseq
                               Number.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_ESEQ_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_ESEQ_FPGA_UPILINK_FUNC1_REG supported on:                         */
/*      WHR (0x200212ac)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for TX Eseq Number.
*/


#define WHR_UPI_TX_ESEQ_FPGA_UPILINK_FUNC1_REG 0x1A1202AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates current TX Eseq
                               Number.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_ESEQ_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_AVAL_RTRY_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x200212b0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RTRY Free Entries.
*/


#define WHR_UPI_TX_AVAL_RTRY_FPGA_UPILINK_FUNC1_REG 0x1A1202B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 etry : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of free entry
                               avialbe in Retry Buffer.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_AVAL_RTRY_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_HEAD_FPGA_UPILINK_FUNC1_REG supported on:                         */
/*      WHR (0x200212b4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for TX Head Pointer.
*/


#define WHR_UPI_TX_HEAD_FPGA_UPILINK_FUNC1_REG 0x1A1202B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pointer : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Inidcates total number of
                               retryable Flits sent.. Counter rolls over on
                               RTRY Buffer Queue Size
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_HEAD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_TAIL_FPGA_UPILINK_FUNC1_REG supported on:                         */
/*      WHR (0x200212b8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RTRY Free Entries.
*/


#define WHR_UPI_TX_TAIL_FPGA_UPILINK_FUNC1_REG 0x1A1202B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pointer : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Inidcates total number of
                               acknoledge Flits.. Counter rolls over on RTRY
                               Buffer Queue Size
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_TAIL_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_REQ_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200212bc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX REQ Header Buffer Enties.
*/


#define WHR_UPI_TX_REQ_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_3 : 7;

                            /* Bits[27:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_REQ_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESPU_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200212c0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESPU Header Buffer Enties.
*/


#define WHR_UPI_TX_RESPU_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_3 : 7;

                            /* Bits[27:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates Number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESPU_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESPO_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200212c4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESPO Header Buffer Enties.
*/


#define WHR_UPI_TX_RESPO_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESPO_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x200212c8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB Header Buffer Enties.
*/


#define WHR_UPI_TX_WB_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200212cc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS Header Buffer Enties.
*/


#define WHR_UPI_TX_NCS_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200212d0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB Header Buffer Enties.
*/


#define WHR_UPI_TX_NCB_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 25;

                            /* Bits[31:7], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_SNP_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200212d4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX SNP Header Buffer Enties.
*/


#define WHR_UPI_TX_SNP_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_SNP_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESPO_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x200212d8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX RESPO Data Buffer Enties.
*/


#define WHR_UPI_TX_RESPO_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 11;

                            /* Bits[31:21], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESPO_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200212dc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB Data Buffer Enties.
*/


#define WHR_UPI_TX_WB_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 11;

                            /* Bits[31:21], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x200212e0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS Data Buffer Enties.
*/


#define WHR_UPI_TX_NCS_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 11;

                            /* Bits[31:21], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x200212e4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB Data Buffer Enties.
*/


#define WHR_UPI_TX_NCB_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 11;

                            /* Bits[31:21], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_BE_BUF_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x200212e8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX WB BE Buffer Enties.
*/


#define WHR_UPI_TX_WB_BE_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_BE_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_BE_BUF_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x200212ec)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCS BE Buffer Enties.
*/


#define WHR_UPI_TX_NCS_BE_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_BE_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_BE_BUF_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x200212f0)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of TX NCB BE Buffer Enties.
*/


#define WHR_UPI_TX_NCB_BE_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_BE_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_REQ_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x200212f4)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX REQ Header Buffer Enties.
*/


#define WHR_UPI_RX_REQ_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_3 : 7;

                            /* Bits[27:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_REQ_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESPU_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200212f8)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPU Header Buffer Enties.
*/


#define WHR_UPI_RX_RESPU_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_3 : 7;

                            /* Bits[27:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESPU_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESPO_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x200212fc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPO Header Buffer Enties.
*/


#define WHR_UPI_RX_RESPO_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A1202FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESPO_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_WB_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x20021300)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB Header Buffer Enties.
*/


#define WHR_UPI_RX_WB_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_WB_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCS_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021304)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS Header Buffer Enties.
*/


#define WHR_UPI_RX_NCS_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCS_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCB_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021308)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB Header Buffer Enties.
*/


#define WHR_UPI_RX_NCB_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 5;

                            /* Bits[4:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 5;

                            /* Bits[9:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCB_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_SNP_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x2002130c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX SNP Header Buffer Enties.
*/


#define WHR_UPI_RX_SNP_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A12030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 18;

                            /* Bits[31:14], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_SNP_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESPO_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x20021310)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPO Data Buffer Enties.
*/


#define WHR_UPI_RX_RESPO_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 14;

                            /* Bits[31:18], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESPO_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_WB_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021314)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB Data Buffer Enties.
*/


#define WHR_UPI_RX_WB_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 14;

                            /* Bits[31:18], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_WB_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCS_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021318)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS Data Buffer Enties.
*/


#define WHR_UPI_RX_NCS_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 14;

                            /* Bits[31:18], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCS_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCB_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x2002131c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB Data Buffer Enties.
*/


#define WHR_UPI_RX_NCB_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A12031C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_1 : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 fifo_cntr_2 : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 14;

                            /* Bits[31:18], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCB_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_WB_BE_BUF_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021320)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX WB BE Buffer Enties.
*/


#define WHR_UPI_RX_WB_BE_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 26;

                            /* Bits[31:6], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_WB_BE_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCS_BE_BUF_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x20021324)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCS BE Buffer Enties.
*/


#define WHR_UPI_RX_NCS_BE_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 26;

                            /* Bits[31:6], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCS_BE_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_NCB_BE_BUF_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x20021328)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX NCB BE Buffer Enties.
*/


#define WHR_UPI_RX_NCB_BE_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 26;

                            /* Bits[31:6], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_NCB_BE_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESPC_HED_BUF_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x2002132c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPC Header Buffer Enties.
*/


#define WHR_UPI_RX_RESPC_HED_BUF_FPGA_UPILINK_FUNC1_REG 0x1A12032C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESPC_HED_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_RESPC_DATA_BUF_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x20021330)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RX RESPC Data Buffer Enties.
*/


#define WHR_UPI_RX_RESPC_DATA_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates number of occupied
                               entries.
                            */
    UINT32 rsvd0 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_RESPC_DATA_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_DSK_STATUS_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x20021334)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Deskew Staus.
*/


#define WHR_UPI_TX_DSK_STATUS_FPGA_UPILINK_FUNC1_REG 0x1A120334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hed_0 : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates the stauts of
                               deskewd channels for header port 0.
                            */
    UINT32 hed_1 : 3;

                            /* Bits[5:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates the stauts of
                               deskewd channels for header port 1 and SA AD
                               CMD3.
                            */
    UINT32 data : 3;

                            /* Bits[8:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates the stauts of
                               deskewd channels for data port 0, or header port
                               2 - 5, or SA AD CMD0 - 1.
                            */
    UINT32 sa_ad : 3;

                            /* Bits[11:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates the status of
                               deskewed channels for SA AD CMD2.
                            */
    UINT32 rsvd0 : 20;

                            /* Bits[31:12], Access Type=RO, default=0x00000000*/

                            /* This field is for RSVD. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_DSK_STATUS_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_DSK_ERR_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x20021338)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Deskew Error Status.
*/


#define WHR_UPI_TX_DSK_ERR_FPGA_UPILINK_FUNC1_REG 0x1A120338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hed_0 : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates channels which
                               failed deskew for Header 0.
                            */
    UINT32 hed_1 : 5;

                            /* Bits[8:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates channels which
                               failed deskew for Header 1 and SA AD CMD3.
                            */
    UINT32 data : 18;

                            /* Bits[26:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates channels which
                               failed deskew for Data, or header port 2 - 5, or
                               SA AD CMD0 - 1.
                            */
    UINT32 sa_ad : 4;

                            /* Bits[30:27], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates chanels which failed
                               deskew for SA AD CMD2.
                            */
    UINT32 rsvd0 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /* This field is for RSVD. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_DSK_ERR_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_DSK_ACT_CH_FPGA_UPILINK_FUNC1_REG supported on:                   */
/*      WHR (0x2002133c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Deskew Active Channels.
*/


#define WHR_UPI_TX_DSK_ACT_CH_FPGA_UPILINK_FUNC1_REG 0x1A12033C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hed_0 : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates active deskew
                               channels for Header 0.
                            */
    UINT32 hed_1 : 4;

                            /* Bits[6:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates active deskew
                               channels for Header 1 and SA AD CMD3.
                            */
    UINT32 data : 17;

                            /* Bits[23:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates active deskew
                               channels for Data, or header port 2 - 5, or SA
                               AD CMD0 - 1.
                            */
    UINT32 sa_ad : 3;

                            /* Bits[26:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates active deskew
                               channels for SA AD CMD2.
                            */
    UINT32 rsvd0 : 5;

                            /* Bits[31:27], Access Type=RO, default=0x00000000*/

                            /* This field is for RSVD. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_DSK_ACT_CH_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_INIT_CRD_FPGA_UPILINK_FUNC1_REG supported on:                     */
/*      WHR (0x20021340)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of Credits Advertised from Remote Link Partner Register.
*/


#define WHR_UPI_TX_INIT_CRD_FPGA_UPILINK_FUNC1_REG 0x1A120340

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vna : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VNA Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 vn0_req : 3;

                            /* Bits[10:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VN0 REQ Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 vn0_snp : 3;

                            /* Bits[13:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VN0 SNP Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 vn0_rsp : 3;

                            /* Bits[16:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VN0 RSP Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 vn0_wb : 3;

                            /* Bits[19:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VN0 WB Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 vn0_ncs : 3;

                            /* Bits[22:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VN0 NCS Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 vn0_ncb : 3;

                            /* Bits[25:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Number of VN0 NCB Credits
                               Advertised from Remote Link Partner Register.
                            */
    UINT32 rsvd0 : 6;

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_INIT_CRD_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_CFG_STATUS_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x20021344)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI CFG Block Status Register.
*/


#define WHR_UPI_CFG_STATUS_FPGA_UPILINK_FUNC1_REG 0x1A120344

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upi_inband_access : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for valid UPI CFG inband access;
                               1: valid inband access, 0: no inband access.
                            */
    UINT32 upi_inband_intf_state : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for UPI CFG cycle interface state;
                               1: no valid access or pending valid access to be
                               granted, 0: wait for granted access to finish.
                            */
    UINT32 pld_avmm_access : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for valid pld_avmm access; 1:
                               valid pld_avmm access, 0: no pld_avmm access.
                            */
    UINT32 pld_avmm_snapshot_en : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for pld_avmm snapshot feature
                               enable.
                            */
    UINT32 pld_avmm_intf_state : 8;

                            /* Bits[11:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for pld_avmm interface state, one-
                               hot encoding; 8'h01: WAIT_NEW_ACCESS, 8'h02:
                               WAIT_NEW_RD_DATA, 8'h04: RETURN_RD_DATA, 8'h08:
                               WAIT_NEXT_BURST_RD, 8'h10: WR_SNAPSHOT_DATA,
                               8'h20: WAIT_NEXT_BURST_WR, 8'h40: REQ_CSR_WR,
                               8'h80: WAIT_CSR_WR_COMPLETE.
                            */
    UINT32 cfg_avmm_access : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for valid config_avmm access; 1:
                               valid config_avmm access, 0: no config_avmm
                               access.
                            */
    UINT32 cfg_avmm_intf_state : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for config_avmm interface state;
                               1: no valid access or pending valid access to be
                               granted, 0: wait for granted access to finish.
                            */
    UINT32 upi_cfg_grant : 3;

                            /* Bits[16:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for UPI_CFG is grant which
                               interface; 3'b001:cfg_avmm / 3'b010:pld_avmm /
                               3'b100:upi_cfg_cycle.
                            */
    UINT32 upi_cfg_rw : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for current CSR access is 0:R /
                               1:W.
                            */
    UINT32 upi_cfg_ll_phy : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for current CSR access is for 0:LL
                               / 1:PHY.
                            */
    UINT32 rsvd0 : 13;

                            /* Bits[31:19], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_CFG_STATUS_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_VNA_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021348)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of VNA credits that Tx used.
*/


#define WHR_UPI_TX_VNA_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A120348

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               VNA CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_VNA_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_SNP_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x2002134c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of SNP VN0 credits that Tx used.
*/


#define WHR_UPI_TX_SNP_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A12034C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               SNP VN0 CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_SNP_VN0_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_REQ_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021350)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of REQ VN0 credits that Tx used.
*/


#define WHR_UPI_TX_REQ_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A120350

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               REQ VN0 CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_REQ_VN0_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCS_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021354)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of NCS VN0 credits that Tx used.
*/


#define WHR_UPI_TX_NCS_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A120354

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               NCS VN0 CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCS_VN0_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_NCB_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021358)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of NCB VN0 credits that Tx used.
*/


#define WHR_UPI_TX_NCB_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A120358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               NCB VN0 CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_NCB_VN0_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_WB_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x2002135c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of WB VN0 credits that Tx used.
*/


#define WHR_UPI_TX_WB_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A12035C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               WB VN0 CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_WB_VN0_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_TX_RESP_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG supported on:            */
/*      WHR (0x20021360)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for Number of RESP VN0 credits that Tx used.
*/


#define WHR_UPI_TX_RESP_VN0_CRD_USED_FPGA_UPILINK_FUNC1_REG 0x1A120360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cntr : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Indicates total number of TX
                               RESP VN0 CRDs used.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_TX_RESP_VN0_CRD_USED_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VNA_CRD_TRK_0_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021364)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for VNA counters in Rx credit tracker 0.
*/


#define WHR_UPI_RX_VNA_CRD_TRK_0_FPGA_UPILINK_FUNC1_REG 0x1A120364

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 req_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_req_cnt. */
    UINT32 snp_cnt : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_snp_cnt. */
    UINT32 rspu_cnt : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_rspu_cnt. */
    UINT32 wb_cnt : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_wb_cnt. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VNA_CRD_TRK_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VNA_CRD_TRK_1_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021368)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for VNA counters in Rx credit tracker 1.
*/


#define WHR_UPI_RX_VNA_CRD_TRK_1_FPGA_UPILINK_FUNC1_REG 0x1A120368

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ncs_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_ncs_cnt. */
    UINT32 rspo_cnt : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_rspo_cnt. */
    UINT32 ncb_cnt : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_ncb_cnt. */
    UINT32 pend_cnt : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_pend_cnt. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VNA_CRD_TRK_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VNA_CRD_TRK_2_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x2002136c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for VNA counters in Rx credit tracker 2.
*/


#define WHR_UPI_RX_VNA_CRD_TRK_2_FPGA_UPILINK_FUNC1_REG 0x1A12036C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rspo_data_cnt : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_rspo_data_cnt. */
    UINT32 ncs_data_cnt : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_ncs_data_cnt. */
    UINT32 ncb_data_cnt : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_ncb_data_cnt. */
    UINT32 wb_data_cnt : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vna_wb_data_cnt. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VNA_CRD_TRK_2_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VN0_CRD_TRK_0_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021370)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for VN0 counters in Rx credit tracker 0.
*/


#define WHR_UPI_RX_VN0_CRD_TRK_0_FPGA_UPILINK_FUNC1_REG 0x1A120370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 req_cnt : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_req_cnt. */
    UINT32 req_pend_cnt : 3;

                            /* Bits[5:3], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_req_pend_cnt. */
    UINT32 rspu_cnt : 3;

                            /* Bits[8:6], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_rspu_cnt. */
    UINT32 rspu_pend_cnt : 3;

                            /* Bits[11:9], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_rspu_pend_cnt. */
    UINT32 snp_cnt : 3;

                            /* Bits[14:12], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_snp_cnt. */
    UINT32 snp_pend_cnt : 3;

                            /* Bits[17:15], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_snp_pend_cnt. */
    UINT32 rspo_cnt : 3;

                            /* Bits[20:18], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_rspo_cnt. */
    UINT32 rspo_pend_cnt : 3;

                            /* Bits[23:21], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_rspo_pend_cnt. */
    UINT32 wb_cnt : 3;

                            /* Bits[26:24], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_wb_cnt. */
    UINT32 wb_pend_cnt : 3;

                            /* Bits[29:27], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_wb_pend_cnt. */
    UINT32 rsvd0 : 2;

                            /* Bits[31:30], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VN0_CRD_TRK_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_RX_VN0_CRD_TRK_1_FPGA_UPILINK_FUNC1_REG supported on:                */
/*      WHR (0x20021374)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for VN0 counters in Rx credit tracker 1.
*/


#define WHR_UPI_RX_VN0_CRD_TRK_1_FPGA_UPILINK_FUNC1_REG 0x1A120374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ncs_cnt : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_ncs_cnt. */
    UINT32 ncs_pend_cnt : 3;

                            /* Bits[5:3], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_ncs_pend_cnt. */
    UINT32 ncb_cnt : 3;

                            /* Bits[8:6], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_ncb_cnt. */
    UINT32 ncb_pend_cnt : 3;

                            /* Bits[11:9], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_ncb_pend_cnt. */
    UINT32 rspo_data_cnt : 3;

                            /* Bits[14:12], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_rspo_data_cnt. */
    UINT32 ncs_data_cnt : 3;

                            /* Bits[17:15], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_ncs_data_cnt. */
    UINT32 ncb_data_cnt : 3;

                            /* Bits[20:18], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_ncb_data_cnt. */
    UINT32 wb_data_cnt : 3;

                            /* Bits[23:21], Access Type=RO/V, default=0x00000000*/

                            /* This field is for vn0_wb_data_cnt. */
    UINT32 rsvd0 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_RX_VN0_CRD_TRK_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_LL_PHY_INTF_STATUS_FPGA_UPILINK_FUNC1_REG supported on:              */
/*      WHR (0x20021378)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for LL / PHY interface status register.
*/


#define WHR_UPI_LL_PHY_INTF_STATUS_FPGA_UPILINK_FUNC1_REG 0x1A120378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ph2ln_rxrdy : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* This field is for ph2ln_RxRdy. */
    UINT32 ph2ln_txrdy : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* This field is for ph2ln_TxRdy. */
    UINT32 ln2ph_reset : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* This field is for ln2ph_Reset. */
    UINT32 ph2ln_reset : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* This field is for ph2ln_Reset. */
    UINT32 ph2ln_flitnull : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* This field is for ph2ln_FlitNull. */
    UINT32 rsvd0 : 27;

                            /* Bits[31:5], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_LL_PHY_INTF_STATUS_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_AIB_FREEZE_STATUS_0_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x2002137c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for AIB Unfreeze Status Register 0.
*/


#define WHR_UPI_AIB_FREEZE_STATUS_0_FPGA_UPILINK_FUNC1_REG 0x1A12037C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_rx_sync : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for Tx/Rx sync paths are unfreezed
                               once it's asserted.
                            */
    UINT32 sr_avmm : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for FSR/SSR/User AVMM are
                               unfreezed once it's asserted.
                            */
    UINT32 pld_core_ready : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for sent by user indicating pld
                               core is ready.
                            */
    UINT32 user_mode : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for sent by WRSSM indicating main
                               die is up and running.
                            */
    UINT32 fsr_ssr_status : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for AIB init status for all 24
                               channels.
                            */
    UINT32 ch_a2u_osc_transfer_alive : 24;

                            /* Bits[28:5], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is for AIB init status for {CH0, CH1,
                               , CH22, CH23.}
                            */
    UINT32 rsvd0 : 3;

                            /* Bits[31:29], Access Type=RO/P, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_AIB_FREEZE_STATUS_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_AIB_FREEZE_STATUS_1_FPGA_UPILINK_FUNC1_REG supported on:             */
/*      WHR (0x20021380)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for AIB Unfreeze Status Register 1.
*/


#define WHR_UPI_AIB_FREEZE_STATUS_1_FPGA_UPILINK_FUNC1_REG 0x1A120380

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

} WHR_UPI_AIB_FREEZE_STATUS_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x20021458)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Status Register 0.
*/


#define WHR_UPI_SA_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_REG 0x1A120458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pt0_cnt_orun : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 0 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt1_cnt_orun : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 1 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt2_cnt_orun : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 2 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt3_cnt_orun : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 3 counter overrun
                               (4KB page: cnt > 64; 16KB page: cnt > 256; 64KB
                               page: cnt > 1K; 256KB page: cnt > 4K); this
                               error is logged in whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt0_cnt_urun : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 0 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt1_cnt_urun : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 1 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt2_cnt_urun : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 2 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt3_cnt_urun : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 3 counter underrun;
                               this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 pt0_par : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Page table 0 parity error. */
    UINT32 pt1_par : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Page table 1 parity error. */
    UINT32 pt2_par : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Page tabe 2 parity error. */
    UINT32 pt3_par : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Page table 3 parity error. */
    UINT32 tx_rsp_par_0 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer 0 parity error. */
    UINT32 tx_rsp_par_1 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer 1 parity error. */
    UINT32 tx_snp_par_0 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer 0
                               parity error.
                            */
    UINT32 tx_snp_par_1 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer 1
                               parity error.
                            */
    UINT32 tx_rsp_orun : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer overrun. */
    UINT32 tx_snp_orun : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer
                               overrun.
                            */
    UINT32 tx_rsp_urun : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* This field is for Tx rsp buffer underrun. */
    UINT32 tx_snp_urun : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Tx filtered snp buffer
                               underrun.
                            */
    UINT32 rsvd_fpga_ad_op : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved FPGA allocation/deallocation opcode. */
    UINT32 rsvd_sw_ad_op : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved SW allocation/deallocation opcode. */
    UINT32 pt0_orun : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 0 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt1_orun : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 1 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt2_orun : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 2 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 pt3_orun : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for Page table 3 receives
                               allocation when there's no more entries
                               available.
                            */
    UINT32 snp0_sad : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This field is for SNP from LL Rx SNP buffer 0
                               fails the SAD; this error is logged in
                               whr_upi_sa_fatal_log_0/1.
                            */
    UINT32 snp1_sad : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

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

} WHR_UPI_SA_FATAL_STATUS_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_REG supported on:               */
/*      WHR (0x2002145c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Fatal Error Status Register 1.
*/


#define WHR_UPI_SA_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_REG 0x1A12045C

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

} WHR_UPI_SA_FATAL_STATUS_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_IN_SNP_FPGA_UPILINK_FUNC1_REG supported on:                       */
/*      WHR (0x20021500)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Incoming Snoop Count Register.
*/


#define WHR_UPI_SA_IN_SNP_FPGA_UPILINK_FUNC1_REG 0x1A120500

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count incoming snoop from LL
                               to SA.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_IN_SNP_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_FILTERED_SNP_FPGA_UPILINK_FUNC1_REG supported on:                 */
/*      WHR (0x20021504)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Filtered Snoop Count Register.
*/


#define WHR_UPI_SA_FILTERED_SNP_FPGA_UPILINK_FUNC1_REG 0x1A120504

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count outgoing filtered snoop
                               from SA to FPGA.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_FILTERED_SNP_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_SNP_RSP_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x20021508)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Response Count Register.
*/


#define WHR_UPI_SA_SNP_RSP_FPGA_UPILINK_FUNC1_REG 0x1A120508

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count snoop response from SA
                               to LL.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_SNP_RSP_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ALLOC_0_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x2002150c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Allocation Count Register 0.
*/


#define WHR_UPI_SA_ALLOC_0_FPGA_UPILINK_FUNC1_REG 0x1A12050C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count allocation from FPGA to
                               SA for page table 0.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ALLOC_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_DEALLOC_0_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021510)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop De-Allocation Count Register 0.
*/


#define WHR_UPI_SA_DEALLOC_0_FPGA_UPILINK_FUNC1_REG 0x1A120510

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count deallocation from FPGA
                               to SA for page table 0.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DEALLOC_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ENTRY_CNT_0_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021514)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Valid Entry Count Register 0.
*/


#define WHR_UPI_SA_ENTRY_CNT_0_FPGA_UPILINK_FUNC1_REG 0x1A120514

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in page table 0.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ENTRY_CNT_0_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ALLOC_1_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x20021518)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Allocation Count Register 1.
*/


#define WHR_UPI_SA_ALLOC_1_FPGA_UPILINK_FUNC1_REG 0x1A120518

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count allocation from FPGA to
                               SA for page table 1.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ALLOC_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_DEALLOC_1_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x2002151c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop De-Allocation Count Register 1.
*/


#define WHR_UPI_SA_DEALLOC_1_FPGA_UPILINK_FUNC1_REG 0x1A12051C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count deallocation from FPGA
                               to SA for page table 1.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DEALLOC_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ENTRY_CNT_1_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021520)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Valid Entry Count Register 1.
*/


#define WHR_UPI_SA_ENTRY_CNT_1_FPGA_UPILINK_FUNC1_REG 0x1A120520

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in page table 1.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ENTRY_CNT_1_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ALLOC_2_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x20021524)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Allocation Count Register 2.
*/


#define WHR_UPI_SA_ALLOC_2_FPGA_UPILINK_FUNC1_REG 0x1A120524

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count allocation from FPGA to
                               SA for page table 2.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ALLOC_2_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_DEALLOC_2_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021528)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop De-Allocation Count Register 2.
*/


#define WHR_UPI_SA_DEALLOC_2_FPGA_UPILINK_FUNC1_REG 0x1A120528

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count deallocation from FPGA
                               to SA for page table 2.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DEALLOC_2_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ENTRY_CNT_2_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x2002152c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Valid Entry Count Register 2.
*/


#define WHR_UPI_SA_ENTRY_CNT_2_FPGA_UPILINK_FUNC1_REG 0x1A12052C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in page table 2.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ENTRY_CNT_2_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ALLOC_3_FPGA_UPILINK_FUNC1_REG supported on:                      */
/*      WHR (0x20021530)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Allocation Count Register 3.
*/


#define WHR_UPI_SA_ALLOC_3_FPGA_UPILINK_FUNC1_REG 0x1A120530

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count allocation from FPGA to
                               SA for page table 3.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ALLOC_3_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_DEALLOC_3_FPGA_UPILINK_FUNC1_REG supported on:                    */
/*      WHR (0x20021534)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop De-Allocation Count Register 3.
*/


#define WHR_UPI_SA_DEALLOC_3_FPGA_UPILINK_FUNC1_REG 0x1A120534

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count deallocation from FPGA
                               to SA for page table 3.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_DEALLOC_3_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SA_ENTRY_CNT_3_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021538)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Valid Entry Count Register 3.
*/


#define WHR_UPI_SA_ENTRY_CNT_3_FPGA_UPILINK_FUNC1_REG 0x1A120538

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in page table 3.
                            */

  } Bits;
  UINT32 Data;

} WHR_UPI_SA_ENTRY_CNT_3_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SAT_TX_SNP_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x2002153c)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Tx Snp FIFO Count Register.
*/


#define WHR_UPI_SAT_TX_SNP_BUF_FPGA_UPILINK_FUNC1_REG 0x1A12053C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in Tx SNP buffer 0.
                            */
    UINT32 fifo_cntr_1 : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in Tx SNP buffer 1.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SAT_TX_SNP_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_SAT_TX_RSP_BUF_FPGA_UPILINK_FUNC1_REG supported on:                  */
/*      WHR (0x20021540)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI Snoop Assist Snoop Tx Rsp FIFO Count Register.
*/


#define WHR_UPI_SAT_TX_RSP_BUF_FPGA_UPILINK_FUNC1_REG 0x1A120540

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fifo_cntr_0 : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in Tx RSP buffer 0.
                            */
    UINT32 fifo_cntr_1 : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field is for Count number of occupied
                               entries in Tx RSP buffer 1.
                            */
    UINT32 rsvd0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} WHR_UPI_SAT_TX_RSP_BUF_FPGA_UPILINK_FUNC1_STRUCT;

/* WHR_UPI_LL_LW_REG_FPGA_UPILINK_FUNC1_REG supported on:                       */
/*      WHR (0x20021ffc)                                                        */
/* Register default value on WHR: 0x00000000                                    */
/* WHR Register File:    wrupi_top_csr_regmap/upilink_agent_func1               */
/* Struct generated from WHR BDF: 0_4_1                                         */
/* WHR Security PolicyGroup:                                                    */
/* This field is for WHR UPI LL Last Word Register.
*/


#define WHR_UPI_LL_LW_REG_FPGA_UPILINK_FUNC1_REG 0x1A120FFC

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

} WHR_UPI_LL_LW_REG_FPGA_UPILINK_FUNC1_STRUCT;
#endif /* _FPGA_UPILINK_FUNC1_h */
