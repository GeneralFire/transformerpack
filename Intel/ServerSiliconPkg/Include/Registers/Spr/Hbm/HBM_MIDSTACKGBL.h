
/** @file
  HBM_MIDSTACKGBL.h

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

/* SPRA0 Security Policy Groups:                                                */

/* SPRB0 Security Policy Groups:                                                */

/* SPRHBM Security Policy Groups:                                               */
/* HBMIO                                                                        */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    HOSTIA_BOOT_SAI | OOB_MSM_SAI                                             */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* HBMIO_MAILBOX                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    HOSTIA_BOOT_SAI | OOB_MSM_SAI                                             */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI                                                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */


#ifndef _HBM_MIDSTACKGBL_h
#define _HBM_MIDSTACKGBL_h
#include <Base.h>

#ifdef SPRHBM_HOST
/* HBM_GLOBAL_CTL_CREG_HBM_MIDSTACKGBL_REG supported on:                        */
/*      SPRHBM (0x20107800)                                                     */
/* Register default value on SPRHBM: 0x000000A7                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Global Control of HBM. Controls xxRESET_n/xxWRST_n pins. Controls for 1500 FSM controls. RCB and LCB enable and override control bits. Clock gating is disabled by default and will be enabled before entering functional mode. THESE CRs are ignored when safe_mode or FscanClkUngate is asserted. Those global signals take higher precedence and will disable clock gating irrespective of these CRs:  en_* - 0 = Clock is forced OFF (flat/gated irrespective of corresponding ovrd_*), 1 = Clock is gated/ungated based on ovrd_*; ovrd_* - 0 = Clock gating is enabled (can be gated based on clock gating enable), 1 = Clock gating is disabled (clock will be free running)
*/


#define HBM_GLOBAL_CTL_CREG_HBM_MIDSTACKGBL_REG 0x15327800

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cr_hbm_reset : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               HBM Reset Control Register. Controls
                               xxHBM_RESET_n pin. 0 = Normal operation:
                               xxHBM_RESET_n pin is inactive (high). 1 = Reset:
                               xxHBM_RESET_n pin is active (low)
                            */
    UINT32 cr_hbm_1500_reset : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               HBM 1500 Port Reset Control Register. Controls
                               xxHBM_WRST_n pin. 0 = Normal operation:
                               xxHBM_WRST_n pin is inactive (high). 1 = Reset:
                               xxHBM_WRST_n pin is active (low)
                            */
    UINT32 cr_1500_fsm_reset : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* IEEE 1500 Controller FSM Reset */
    UINT32 cr_1500_fsm_wrck_en : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* IEEE 1500 Controller FSM WRCK out enable */
    UINT32 reserved0__nc : 1;

                            /* Bits[4:4], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 cr_resetn_cren : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* xxHBM_RESET_n cren. Enable CR Data */
    UINT32 cr_resetn_rxen : 1;

                            /* Bits[6:6], Access Type=RW, default=None*/

                            /* xxHBM_RESET_n rxen. Enable RX */
    UINT32 cr_wrstn_cren : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* xxHBM_WRST_n cren. Enable CR Data */
    UINT32 cr_wrstn_rxen : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /* xxHBM_WRST_n rxen. Enable RX */
    UINT32 en_cr_ctrl_xxrst_wrst : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set to 1;Enable
                               hbm_reset/hbm_1500_reset/resetn_cren CRs to
                               drive xxreset/xxwrst. When disabled (=0);
                               xxrst/wrst stays latched to prev value
                            */
    UINT32 rst_ntl_ovrd : 1;

                            /* Bits[10:10], Access Type=RW, default=None*/

                            /*
                               Alternate cr based reset for NTL to enable
                               dclk_div4 clk
                            */
    UINT32 reserved1__nc : 4;

                            /* Bits[14:11], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 en_vdm_odi_lcb : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               Enable non-scan LCBs in midstack and
                               panhandle(ODI_VDM) and pulse stretchers in
                               midstack. Override ctrl not required (no
                               functional enable).
                            */
    UINT32 en_aword_creg_lcb : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /* Enable AWORD Creg LCB */
    UINT32 en_dword_creg_lcb : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /* Enable DWORD Creg LCB */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en_div_4_training_lcb : 1;

                            /* Bits[19:19], Access Type=RW, default=None*/

                            /* Enable Midstack Div 4 Training LCB */
    UINT32 en_ntl_extest_lcb : 1;

                            /* Bits[20:20], Access Type=RW, default=None*/

                            /* Enable Midstack NTL LCB */
    UINT32 en_visa_lcb : 1;

                            /* Bits[21:21], Access Type=RW, default=None*/

                            /* Enable Midstack VISA LCB */
    UINT32 en_panhandle_visa_lcb : 1;

                            /* Bits[22:22], Access Type=RW, default=None*/

                            /* Enable Panhandle VISA LCB */
    UINT32 reserved2 : 1;

                            /* Bits[23:23], Access Type=RW, default=None*/

                            /* Reserved */
    UINT32 ovrd_aword_creg_lcb : 1;

                            /* Bits[24:24], Access Type=RW, default=None*/

                            /* Override AWORD Creg LCB */
    UINT32 ovrd_dword_creg_lcb : 1;

                            /* Bits[25:25], Access Type=RW, default=None*/

                            /* Override DWORD Creg LCB */
    UINT32 rsvd_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovrd_div_4_training_lcb : 1;

                            /* Bits[27:27], Access Type=RW, default=None*/

                            /* Override Midstack Div 4 Training LCB */
    UINT32 ovrd_ntl_extest_lcb : 1;

                            /* Bits[28:28], Access Type=RW, default=None*/

                            /* Override Midstack NTL LCB */
    UINT32 ovrd_visa_lcb : 1;

                            /* Bits[29:29], Access Type=RW, default=None*/

                            /* Override Midstack VISA LCB */
    UINT32 ovrd_panhandle_visa_lcb : 1;

                            /* Bits[30:30], Access Type=RW, default=None*/

                            /* Override Panhandle VISA LCB */
    UINT32 reserved3 : 1;

                            /* Bits[31:31], Access Type=RW, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HBM_GLOBAL_CTL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_HW_CONTROL_CREG_HBM_MIDSTACKGBL_REG supported on:                         */
/*      SPRHBM (0x20107804)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC Hardware Control
*/


#define UC_HW_CONTROL_CREG_HBM_MIDSTACKGBL_REG 0x15327804

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uc_rst : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               Active high reset control (BReset of
                               uController)
                            */
    UINT32 uc_en : 1;

                            /* Bits[1:1], Access Type=RW/V, default=None*/

                            /* Active high enable (~RunStall) */
    UINT32 uc_clk_en : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /*
                               Functional clock enable for all microcontroller
                               logic. Set to 1 before microcontroller reset
                               exit flow is executed. Set to 0 during
                               microcontroller reset entry flow for critical
                               power savings.
                            */
    UINT32 uc_iram_busy : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /* IRAM busy */
    UINT32 uc_dram_busy : 1;

                            /* Bits[4:4], Access Type=RW, default=None*/

                            /* DRAM busy */
    UINT32 uc_interrupt : 1;

                            /* Bits[5:5], Access Type=RW, default=None*/

                            /* Interrupt (can be SW) */
    UINT32 uc_debug_en : 1;

                            /* Bits[6:6], Access Type=RW, default=None*/

                            /* Debug trace port enable */
    UINT32 uc_ocd_rst_en : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /* Enable OCD mode on reset */
    UINT32 uc_ecc_en : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /* Enable RAM ECC feature */
    UINT32 dreset : 1;

                            /* Bits[9:9], Access Type=RW, default=None*/

                            /* Active high Microcontroller OCD Reset Control */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_HW_CONTROL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_HW_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                          */
/*      SPRHBM (0x20107808)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC Hardware Status
*/


#define UC_HW_STATUS_CREG_HBM_MIDSTACKGBL_REG 0x15327808

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uc_wait : 1;

                            /* Bits[0:0], Access Type=RO/V, default=None*/

                            /* Wait instruction set */
    UINT32 uc_fatal_err : 1;

                            /* Bits[1:1], Access Type=RO/V, default=None*/

                            /* Fatal error */
    UINT32 uc_dbl_ex_err : 1;

                            /* Bits[2:2], Access Type=RO/V, default=None*/

                            /* Double exception error */
    UINT32 uc_pfinfo_sev : 6;

                            /* Bits[8:3], Access Type=RO/V, default=None*/

                            /* PFaultInfo severity */
    UINT32 uc_pfinfo_user : 4;

                            /* Bits[12:9], Access Type=RO/V, default=None*/

                            /* PFaultInfo user field */
    UINT32 uc_pfinfo_mult : 1;

                            /* Bits[13:13], Access Type=RO/V, default=None*/

                            /* PFaultInfo multiple faults */
    UINT32 uc_pfinfo_val : 1;

                            /* Bits[14:14], Access Type=RO/V, default=None*/

                            /* PFaultInfo valid */
    UINT32 uc_function_id : 8;

                            /* Bits[22:15], Access Type=RW, default=None*/

                            /* Function ID */
    UINT32 uc_interrupt_service_done : 1;

                            /* Bits[23:23], Access Type=RW, default=None*/

                            /* Interrupt service done */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_HW_STATUS_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_RAM_DIRACC_ADDR_CREG_HBM_MIDSTACKGBL_REG supported on:                    */
/*      SPRHBM (0x2010780c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC RAM Direct Access Address Register
*/


#define UC_RAM_DIRACC_ADDR_CREG_HBM_MIDSTACKGBL_REG 0x1532780C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 13;

                            /* Bits[12:0], Access Type=RW/V, default=None*/

                            /* RAM address */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 raminit_en : 1;

                            /* Bits[16:16], Access Type=RW/V, default=None*/

                            /* Enable 0-init FSM */
    UINT32 raminit_cmplt : 1;

                            /* Bits[17:17], Access Type=RO/V, default=None*/

                            /* 0-init complete or skipped */
    UINT32 skip_raminit : 1;

                            /* Bits[18:18], Access Type=RW, default=None*/

                            /* Skip 0-init FSM */
    UINT32 data_ram_sel : 1;

                            /* Bits[19:19], Access Type=RW, default=None*/

                            /* Data ram select */
    UINT32 sram_shutoff : 1;

                            /* Bits[20:20], Access Type=RW, default=None*/

                            /* Place SRAMs in shutoff mode */
    UINT32 uc_sram_mbist_done : 1;

                            /* Bits[21:21], Access Type=RO/V, default=None*/

                            /*
                               mBIST diagnosis done status, when BIST is
                               running with diag_mode enabled, status =0 , when
                               idle, status =1
                            */
    UINT32 sram_load_done : 1;

                            /* Bits[22:22], Access Type=RW, default=None*/

                            /* SRAM image load done indication */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_RAM_DIRACC_ADDR_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_RAM_DIRACC_DATA_CREG_HBM_MIDSTACKGBL_REG supported on:                    */
/*      SPRHBM (0x20107810)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC RAM Direct Access Data Register
*/


#define UC_RAM_DIRACC_DATA_CREG_HBM_MIDSTACKGBL_REG 0x15327810

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 byte0 : 8;

                            /* Bits[7:0], Access Type=RW/V, default=None*/

                            /* RAM byte 0 */
    UINT32 byte1 : 8;

                            /* Bits[15:8], Access Type=RW/V, default=None*/

                            /* RAM byte 1 */
    UINT32 byte2 : 8;

                            /* Bits[23:16], Access Type=RW/V, default=None*/

                            /* RAM byte 2 */
    UINT32 byte3 : 8;

                            /* Bits[31:24], Access Type=RW/V, default=None*/

                            /* RAM byte 3 */

  } Bits;
  UINT32 Data;

} UC_RAM_DIRACC_DATA_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_SCRATCHPAD0_CREG_HBM_MIDSTACKGBL_REG supported on:                        */
/*      SPRHBM (0x20107814)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC scratchpad register
*/


#define UC_SCRATCHPAD0_CREG_HBM_MIDSTACKGBL_REG 0x15327814

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 byte0 : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 0 */
    UINT32 byte1 : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 1 */
    UINT32 byte2 : 8;

                            /* Bits[23:16], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 2 */
    UINT32 byte3 : 8;

                            /* Bits[31:24], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 3 */

  } Bits;
  UINT32 Data;

} UC_SCRATCHPAD0_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_SCRATCHPAD1_CREG_HBM_MIDSTACKGBL_REG supported on:                        */
/*      SPRHBM (0x20107818)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC scratchpad register
*/


#define UC_SCRATCHPAD1_CREG_HBM_MIDSTACKGBL_REG 0x15327818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 byte0 : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 0 */
    UINT32 byte1 : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 1 */
    UINT32 byte2 : 8;

                            /* Bits[23:16], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 2 */
    UINT32 byte3 : 8;

                            /* Bits[31:24], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 3 */

  } Bits;
  UINT32 Data;

} UC_SCRATCHPAD1_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_SCRATCHPAD2_CREG_HBM_MIDSTACKGBL_REG supported on:                        */
/*      SPRHBM (0x2010781c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC scratchpad register
*/


#define UC_SCRATCHPAD2_CREG_HBM_MIDSTACKGBL_REG 0x1532781C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 byte0 : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 0 */
    UINT32 byte1 : 8;

                            /* Bits[15:8], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 1 */
    UINT32 byte2 : 8;

                            /* Bits[23:16], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 2 */
    UINT32 byte3 : 8;

                            /* Bits[31:24], Access Type=RW, default=None*/

                            /* uC scratch pad register for val - Byte 3 */

  } Bits;
  UINT32 Data;

} UC_SCRATCHPAD2_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x20107820)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO_MAILBOX                                   */
/* Midstack uC/BIOS mailbox control
*/


#define UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_REG 0x15327820

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 opcode : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /* GPIO in: BIOS mailbox opcode */
    UINT32 parameters : 16;

                            /* Bits[23:8], Access Type=RW, default=None*/

                            /* GPIO in: BIOS mailbox parameters */
    UINT32 reserved1__nc : 4;

                            /* Bits[27:24], Access Type=RW, default=None*/

                            /* GPIO in: reserved */
    UINT32 return_code : 3;

                            /* Bits[30:28], Access Type=RW, default=None*/

                            /* GPIO in: BIOS mailbox return code */
    UINT32 run_busy : 1;

                            /* Bits[31:31], Access Type=RW, default=None*/

                            /* GPIO in: BIOS mailbox run/busy */

  } Bits;
  UINT32 Data;

} UC_BIOS_MAILBOX_CTL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_BIOS_MAILBOX_DATA0_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x20107824)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO_MAILBOX                                   */
/* Midstack uC/BIOS mailbox data 0
*/


#define UC_BIOS_MAILBOX_DATA0_CREG_HBM_MIDSTACKGBL_REG 0x15327824

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* BIOS mailbox data */

  } Bits;
  UINT32 Data;

} UC_BIOS_MAILBOX_DATA0_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_BIOS_MAILBOX_DATA1_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x20107828)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO_MAILBOX                                   */
/* Midstack uC/BIOS mailbox data 0
*/


#define UC_BIOS_MAILBOX_DATA1_CREG_HBM_MIDSTACKGBL_REG 0x15327828

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW, default=None*/

                            /* BIOS mailbox data */

  } Bits;
  UINT32 Data;

} UC_BIOS_MAILBOX_DATA1_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_FW_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                          */
/*      SPRHBM (0x2010782c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO_MAILBOX                                   */
/* Midstack uC firmware status
*/


#define UC_FW_STATUS_CREG_HBM_MIDSTACKGBL_REG 0x1532782C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 obj_label : 16;

                            /* Bits[15:0], Access Type=RO/V, default=None*/

                            /* GPIO out: Object file label */
    UINT32 debug_obs : 16;

                            /* Bits[31:16], Access Type=RO/V, default=None*/

                            /* GPIO out: Debug observation */

  } Bits;
  UINT32 Data;

} UC_FW_STATUS_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_CRI_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                         */
/*      SPRHBM (0x20107830)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack uC CRI Status
*/


#define UC_CRI_STATUS_CREG_HBM_MIDSTACKGBL_REG 0x15327830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cri_err_addr : 16;

                            /* Bits[15:0], Access Type=RO/V, default=None*/

                            /* Address that caused CRI error */
    UINT32 cri_err_be : 4;

                            /* Bits[19:16], Access Type=RO/V, default=None*/

                            /* Byte enables that caused CRI error */
    UINT32 cri_err_rw : 1;

                            /* Bits[20:20], Access Type=RO/V, default=None*/

                            /* Read/write bit that caused CRI error */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_CRI_STATUS_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_RAM_ECC_INJ_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x20107834)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Used to control ECC Injection FSM for RAMs
*/


#define UC_RAM_ECC_INJ_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x15327834

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 array_sel : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /* Select which SRAM (IRAM=0 or DRAM=1) */
    UINT32 dbl : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /*
                               Control bit for 2-bit error injection (default
                               1-bit)
                            */
    UINT32 cont : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /* Control bit for continuous mode (default 0) */
    UINT32 arm : 1;

                            /* Bits[3:3], Access Type=RW/V, default=None*/

                            /*
                               Control bit for ECC injection FSM to start
                               injection
                            */
    UINT32 log_mode : 2;

                            /* Bits[5:4], Access Type=RW, default=None*/

                            /* Control bit for selecting ECC log mode */
    UINT32 bit_index1 : 6;

                            /* Bits[11:6], Access Type=RW, default=None*/

                            /*
                               ECC bit Control CR1. Default CR for 1-bit inject
                               mode. [5:1] index which bit of data bus needs
                               ECC injection. [3:1] index which bit of ECC bus
                               needs ECC injection. 3'b111 is invalid. [0]
                               selects which of the two values above are valid.
                            */
    UINT32 bit_index2 : 6;

                            /* Bits[17:12], Access Type=RW, default=None*/

                            /*
                               ECC bit Control CR2. Used for 2-bit inject mode
                               only. [5:1] index which bit of data bus needs
                               ECC injection. [3:1] index which bit of ECC bus
                               needs ECC injection. 3'111 is invalid. [0]
                               selects which of the two values above are valid.
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_RAM_ECC_INJ_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_RAM_ECC_LOG0_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x20107838)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Used to log ECC information from the RAMs
*/


#define UC_RAM_ECC_LOG0_CREG_HBM_MIDSTACKGBL_REG 0x15327838

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 array : 1;

                            /* Bits[0:0], Access Type=RW/V, default=None*/

                            /*
                               Log bit for logging which SRAM (IRAM=0 or
                               DRAM=1)
                            */
    UINT32 syndrome : 7;

                            /* Bits[7:1], Access Type=RW/V, default=None*/

                            /* Log bits for logging syndrome vector */
    UINT32 addr : 13;

                            /* Bits[20:8], Access Type=RW/V, default=None*/

                            /*
                               Log the SRAM address which had ECC related
                               errors
                            */
    UINT32 uncor : 1;

                            /* Bits[21:21], Access Type=RW/V, default=None*/

                            /*
                               Log bit for logging uncorrectable errors (>=2bit
                               errors)
                            */
    UINT32 valid : 1;

                            /* Bits[22:22], Access Type=RW/V, default=None*/

                            /*
                               Log bit for logging if this log register is
                               valid
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_RAM_ECC_LOG0_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* UC_RAM_ECC_LOG1_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x2010783c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Used to log ECC information from the RAMs
*/


#define UC_RAM_ECC_LOG1_CREG_HBM_MIDSTACKGBL_REG 0x1532783C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 array : 1;

                            /* Bits[0:0], Access Type=RW/V, default=None*/

                            /*
                               Log bit for logging which SRAM (IRAM=0 or
                               DRAM=1)
                            */
    UINT32 syndrome : 7;

                            /* Bits[7:1], Access Type=RW/V, default=None*/

                            /* Log bits for logging syndrome vector */
    UINT32 addr : 13;

                            /* Bits[20:8], Access Type=RW/V, default=None*/

                            /*
                               Log the SRAM address which had ECC related
                               errors
                            */
    UINT32 uncor : 1;

                            /* Bits[21:21], Access Type=RW/V, default=None*/

                            /*
                               Log bit for logging uncorrectable errors (>=2bit
                               errors)
                            */
    UINT32 valid : 1;

                            /* Bits[22:22], Access Type=RW/V, default=None*/

                            /*
                               Log bit for logging if this log register is
                               valid
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UC_RAM_ECC_LOG1_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* FW_HW_REV_ID_CREG_HBM_MIDSTACKGBL_REG supported on:                          */
/*      SPRHBM (0x20107840)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Firmware revision ID is written by Firmware during boot time based on what is compiled into the image. Hardware is a one-hot encoding of features and options based on straps.
*/


#define FW_HW_REV_ID_CREG_HBM_MIDSTACKGBL_REG 0x15327840

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fw_rev_id : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /*
                               Upper byte is major revision. Lower byte is
                               minor revision.
                            */
    UINT32 hw_rev_code : 16;

                            /* Bits[31:16], Access Type=RO/V, default=None*/

                            /*
                               One-hot encoding of hardware options, settings,
                               versions based on external or internal straps.
                               [0]=strap_is_dfi, [1]=dfi_freq_ratio,
                               [3:2]=Reserved, [7:4] Revision_ID, [8]= (NOT
                               Foveros) derivative, [9]= (7nm) derivative,
                               [15:10]=Reserved
                            */

  } Bits;
  UINT32 Data;

} FW_HW_REV_ID_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CTL_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x20107844)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Used to Start sequence, Poll for sequence done. Check for all [3:0] bits to be 0, before setting start_busy. If thermal_status is 1, these IEEE1500 CRs will be locked out and not used. Plus, refer to HAS and follow the proper steps to arbitrate between GPSB CR based IEEE1500 controller FSM and TAP control of IEEE1500 interface to HBM stack. If WTAP based IEEE1500 access is enabled, it has the highest priority over the physical IEEE1500 interface.

*/


#define HBM_1500_PORT_CTL_CREG_HBM_MIDSTACKGBL_REG 0x15327844

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cr_start_busy : 1;

                            /* Bits[0:0], Access Type=RW/V, default=None*/

                            /* IEEE 1500 Port Set to start. Poll for clear. */
    UINT32 csr_idle_status : 1;

                            /* Bits[1:1], Access Type=RO/V, default=None*/

                            /* IEEE 1500 Port idle status. */
    UINT32 csr_polling_enabled : 1;

                            /* Bits[2:2], Access Type=RO/V, default=None*/

                            /*
                               IEEE 1500 Port Periodic/Thermal polling ENABLED
                               status. Check before executing IEEE1500 cmds via
                               start_busy.
                            */
    UINT32 cr_periodic_poll_en : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable periodic execution of IEEE1500 cmd
                               (defined in IREG CR). Thermal polling is the
                               only known use.
                            */
    UINT32 cr_freq_sel : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               IEEE 1500 Port Frequency Select. Mapping: 2'b00:
                               50 MHz, 2'b01: 100 MHz, 2'b10: 200 MHz, 2'b11:
                               25 MHz
                            */
    UINT32 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cr_periodic_poll_interval : 24;

                            /* Bits[31:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Defines interval between thermal polling reads
                               in 1.28us ticks (50MHz xp50clks X 64).
                            */

  } Bits;
  UINT32 Data;

} HBM_1500_PORT_CTL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_IREG_CREG_HBM_MIDSTACKGBL_REG supported on:                    */
/*      SPRHBM (0x20107848)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Used to setup IEEE1500 instruction per sequence
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_WRDATA0_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010784c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value is shifted out over WSI. Four 32b registers = 128b of write data per sequence.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_WRDATA1_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x20107850)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value is shifted out over WSI. Four 32b registers = 128b of write data per sequence.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_WRDATA2_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x20107854)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value is shifted out over WSI. Four 32b registers = 128b of write data per sequence.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_WRDATA3_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x20107858)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value is shifted out over WSI. Four 32b registers = 128b of write data per sequence.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH0_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x2010785c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH1_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x20107860)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH2_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x20107864)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH3_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x20107868)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH4_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x2010786c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH5_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x20107870)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH6_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x20107874)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBM_1500_PORT_CH7_RDDATA_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x20107878)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Data value read from HBM via 1500 port WSO[a:h] reads is stored into the read data registers. Shift 32b at a time.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* CH_MAP_CREG_HBM_MIDSTACKGBL_REG supported on:                                */
/*      SPRHBM (0x2010787c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Not useful. Only maps channels, not Dwords.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MID_TRAIN_CTL_STS_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x20107880)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Each Aword's fail indication based on cr_aword_train_lfsr_cfg1_creg.fail_src_sel in each Aword. fail_src_sel mapping: 2'b00 = aword_train_compare_status1_creg.error_count_threshold_fail, 2'b01 = aword_train_compare_status1_creg.compare_rise_sticky_or, 2'b10 = aword_train_compare_status1_creg.compare_fall_sticky_or, 2'b11 = aword_dll_status_creg.lock
            
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MID_TRAIN_DWORD_STS_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x20107884)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Each channel's Dwords' fail indication based on cr_dword_train_pattern_cfg1_creg.fail_src_sel in each Dword. fail_src_sel mapping: 2'b00 = dword_train_error_count_status_creg.error_count_threshold_fail, 2'b01 = dword_train_error_count_status_creg.compare_rise_sticky_or, 2'b10 = dword_train_error_count_status_creg.compare_fall_sticky_or, 2'b11 = dword_dll_status_creg.lock
            
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* VREF_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                             */
/*      SPRHBM (0x20107888)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Non-FUSE. Fine tuning is not required at 100MT/s of IEEE1500.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* FUSE_PULL_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010788c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/*  Status of the Fuse Pulls. And fuse pull bypass control for warm-reset. 
*/


#define FUSE_PULL_STATUS_CREG_HBM_MIDSTACKGBL_REG 0x1532788C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_valid : 1;

                            /* Bits[0:0], Access Type=RO/V, default=None*/

                            /* Fuse pull successful */
    UINT32 fuse_pull_done : 1;

                            /* Bits[1:1], Access Type=RO/V, default=None*/

                            /* Fuse pull done */
    UINT32 fuse_pull_fabric_err : 1;

                            /* Bits[2:2], Access Type=RO/V, default=None*/

                            /* Fuse pull fabric error */
    UINT32 fuse_pull_err : 1;

                            /* Bits[3:3], Access Type=RO/V, default=None*/

                            /* Fuse pull error */
    UINT32 fuse_pull_cri_err : 1;

                            /* Bits[4:4], Access Type=RO/V, default=None*/

                            /* Fuse pull cri error */
    UINT32 fuse_pull_error_code : 8;

                            /* Bits[12:5], Access Type=RO/V, default=None*/

                            /* Fuse pull error code */
    UINT32 rsvd : 18;

                            /* Bits[30:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fuse_pull_warm_reset_bypass : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Fuse pull bypass enable for warm-reset. This
                               sticky bit, when set, will bypass the fuse-pull
                               request on warm-reset. Avoiding fuse pull on
                               every warm-reset, allows the training tuned
                               version of the fuse CR values to remain sticky
                               across warm-reset and avoid re-training.
                            */

  } Bits;
  UINT32 Data;

} FUSE_PULL_STATUS_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* ADC_CONTROL_CREG_HBM_MIDSTACKGBL_REG supported on:                           */
/*      SPRHBM (0x20107890)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* midstack adc control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* ADC_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                            */
/*      SPRHBM (0x20107894)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* midstack adc status
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* VIEWDIG_RCOMP_CREG_HBM_MIDSTACKGBL_REG supported on:                         */
/*      SPRHBM (0x20107898)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* midstack viewdig rcomp
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* VIEWDIG_CREG_HBM_MIDSTACKGBL_REG supported on:                               */
/*      SPRHBM (0x2010789c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* For mux selection (upper 8 bits), refer to $MODEL_ROOT/tools/view/hbmio_viewmux_auto_addr_doc.csv (auto-generated). Finally, for individual signal selection within muxes (lower 8 bits), refer to the individual *_viewmux_*_doc.csv files (auto-generated). View0 and View1 have independent and different selection options. Note: Seperate DLL view controls (dll_digviewen/sel) live in dword_pifsmtest_creg. Refer to HAS for details.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* VIEWANA_CREG_HBM_MIDSTACKGBL_REG supported on:                               */
/*      SPRHBM (0x201078a0)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* All the following multi-bit select fields have their own tables. Refer to HAS for details.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* TXRX_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                             */
/*      SPRHBM (0x201078a4)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Static CR control (not FUSE) for midstack IEEE1500 DQ analog settings and TX/RX direction. All midstack pins are unidirectional. At 100MT/s, fine tuning (per part) of analog settings is not required.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* EXTEST_1500_TXRX_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x201078a8)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* CR control for TX settings during extest mode only. extest_en needs to be set from extestCTL TDR for these overrides to take place
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MIDSTACK_STICKY_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x201078ac)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Learnings from previous project emphasized the need for sticky registers across warm resets. BIOS used these a lot in the survivability space. 
*/


#define MIDSTACK_STICKY_CREG_HBM_MIDSTACKGBL_REG 0x153278AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratch_pad : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Scratch Pad Register */

  } Bits;
  UINT32 Data;

} MIDSTACK_STICKY_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MIDSTACK_ACIO_LB_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x201078b0)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Midstack ACIO Loopback Control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MIDSTACK_UC_SRAM_REPAIR_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:        */
/*      SPRHBM (0x201078b4)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug read out of the SRAM repair value from MRA
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MIDSTACK_UC_SRAM_ASSIST_FUSE_CREG_HBM_MIDSTACKGBL_REG supported on:          */
/*      SPRHBM (0x201078e8)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for SRAM Read / Write Assist. Defined using wave3_SDG_SRAM_Integration_Guide_rev0.3.pdf
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MIDSTACK_HBMPHY_CFG0_FUSE_CREG_HBM_MIDSTACKGBL_REG supported on:             */
/*      SPRHBM (0x201078f8)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Refer to integration guide's 'Initalization Sequence: HBM Configuration values' (4.12.2.1) section; Using HEX values to match fuse spreadsheet.
*/


#define MIDSTACK_HBMPHY_CFG0_FUSE_CREG_HBM_MIDSTACKGBL_REG 0x153278F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mr0_dbiac_read : 1;

                            /* Bits[0:0], Access Type=RW/P, default=None*/

                            /* Read DBIac enable */
    UINT32 mr0_dbiac_write : 1;

                            /* Bits[1:1], Access Type=RW/P, default=None*/

                            /* Write DBIac enable */
    UINT32 mr0_tcsr : 1;

                            /* Bits[2:2], Access Type=RW/P, default=None*/

                            /*
                               Temperature Compensated Self-Refresh (TCSR)
                               enable
                            */
    UINT32 mr0_rsvd0 : 1;

                            /* Bits[3:3], Access Type=RW/P, default=None*/

                            /* Reserved */
    UINT32 mr0_dq_read_parity : 1;

                            /* Bits[4:4], Access Type=RW/P, default=None*/

                            /* DQ Read Parity enable */
    UINT32 mr0_dq_write_parity : 1;

                            /* Bits[5:5], Access Type=RW/P, default=None*/

                            /* DQ Write Parity enable */
    UINT32 mr0_add_cmd_parity : 1;

                            /* Bits[6:6], Access Type=RW/P, default=None*/

                            /* ADD/CMD Parity enable */
    UINT32 mr0_rsvd1 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=None*/

                            /* Reserved */
    UINT32 mr1_write_recovery : 5;

                            /* Bits[12:8], Access Type=RW/P, default=None*/

                            /* HBM Write Recovery (WR) */
    UINT32 mr1_driver_strength : 3;

                            /* Bits[15:13], Access Type=RW/P, default=None*/

                            /* HBM Nominal Driver Strength (HBM Stack, NOT PHY) */
    UINT32 mr2_write_latency : 3;

                            /* Bits[18:16], Access Type=RW/P, default=None*/

                            /* Write Latency (WL) */
    UINT32 mr2_read_latency : 5;

                            /* Bits[23:19], Access Type=RW/P, default=None*/

                            /* Read Latency (RL) */
    UINT32 mr3_ras : 6;

                            /* Bits[29:24], Access Type=RW/P, default=None*/

                            /* HBM Active to Precharge (RAS) */
    UINT32 mr3_bank_group : 1;

                            /* Bits[30:30], Access Type=RW/P, default=None*/

                            /* Bank Group enable */
    UINT32 mr3_bl4 : 1;

                            /* Bits[31:31], Access Type=RW/P, default=None*/

                            /* Burst Length 4 (always 1'b1) */

  } Bits;
  UINT32 Data;

} MIDSTACK_HBMPHY_CFG0_FUSE_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* MIDSTACK_HBMPHY_CFG1_FUSE_CREG_HBM_MIDSTACKGBL_REG supported on:             */
/*      SPRHBM (0x201078fc)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/midstack_creg/mem_hbmio_midstack_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Refer to integration guide's 'Initalization Sequence: HBM Configuration values' (4.12.2.1) section; Using HEX values to match fuse spreadsheet.
*/


#define MIDSTACK_HBMPHY_CFG1_FUSE_CREG_HBM_MIDSTACKGBL_REG 0x153278FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mr4_ecc : 1;

                            /* Bits[0:0], Access Type=RW/P, default=None*/

                            /* ECC enable */
    UINT32 mr4_dm : 1;

                            /* Bits[1:1], Access Type=RW/P, default=None*/

                            /* Data Mask disable */
    UINT32 mr4_parity_latency : 2;

                            /* Bits[3:2], Access Type=RW/P, default=None*/

                            /* Parity Latency (PL) */
    UINT32 mr4_extended_wl : 1;

                            /* Bits[4:4], Access Type=RW/P, default=None*/

                            /* Extended Write Latency (EWL) */
    UINT32 mr4_extended_rl : 1;

                            /* Bits[5:5], Access Type=RW/P, default=None*/

                            /* Extended Read Latency (ERL) */
    UINT32 mr4_rsvd : 2;

                            /* Bits[7:6], Access Type=RW/P, default=None*/

                            /* Reserved0 */
    UINT32 mr5_hbm_opt_int_vref : 3;

                            /* Bits[10:8], Access Type=RW/P, default=None*/

                            /*
                               HBM Optional Internal Vref (HBM Stack, NOT PHY)
                               - (Moved from JEDEC MR15)
                            */
    UINT32 die_mirror : 2;

                            /* Bits[12:11], Access Type=RW/P, default=None*/

                            /*
                               The 2 bit -hbmio_midstack_hbmphy_cfg_mr5_rsvd
                               fuse entry is now
                               hbmio_midstack_hbmphy_cfg_die_mirror normal
                               orientation = 00 mirrored orientation = 01 These
                               are the only 2 legal values for this fuse The CR
                               local storage element remains
                               midstack_hbmphy_cfg1_fuse_creg.die_mirror
                            */
    UINT32 mr5_sid0 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=None*/

                            /* SID[0] valid (PHY only, not JEDEC) */
    UINT32 mr5_sid1 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=None*/

                            /* SID[1] valid (PHY only, not JEDEC) */
    UINT32 mr5_ra14 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=None*/

                            /* RA14 valid (PHY only, not JEDEC) */
    UINT32 mr6_rsvd : 3;

                            /* Bits[18:16], Access Type=RW/P, default=None*/

                            /* Reserved2 */
    UINT32 mr6_trp : 5;

                            /* Bits[23:19], Access Type=RW/P, default=None*/

                            /* HBM imPRE tRP Value */
    UINT32 mr7_round_trip_latency : 6;

                            /* Bits[29:24], Access Type=RW/P, default=None*/

                            /*
                               PHY Round Trip Latency (RTL) through RX FIFO
                               (different from Read Latency) - (PHY only, not
                               JEDEC)
                            */
    UINT32 mr7_rsvd : 2;

                            /* Bits[31:30], Access Type=RW/P, default=None*/

                            /* Reserved3 */

  } Bits;
  UINT32 Data;

} MIDSTACK_HBMPHY_CFG1_FUSE_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010fa00)                                                     */
/* Register default value on SPRHBM: 0x00018000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL status
*/


#define AWORD_DLL_STATUS_CREG_HBM_MIDSTACKGBL_REG 0x1532FA00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pilags : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* PI Lags from pifsm test */
    UINT32 adc_done : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* ADC Done */
    UINT32 weak_lock : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* Weak Lock */
    UINT32 early_lock : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* earlylock from dlldetlock CBB */
    UINT32 long_lock : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* 1 if detlock asserts befor rawlock;else 0 */
    UINT32 adcout : 10;

                            /* Bits[14:5], Access Type=RO/V, default=0x00000000*/

                            /* ADC Out */
    UINT32 dll_reset : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000001*/

                            /* DLL Reset , function of PLL lock */
    UINT32 digtimeout : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000001*/

                            /*
                               PreCharge Counter complete indication status=0
                               when timer reaches timeout
                            */
    UINT32 lock : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               Final Lock to mdllpi enable PIclk. Either
                               Detlock/Rawlock/Debug override
                            */
    UINT32 raw_lock : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* Raw Lock from detlock CBB */
    UINT32 detrm_lock : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Deterministic Lock from Counter */
    UINT32 stkyrawlock : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Sticky Raw Lock */
    UINT32 readcrdlldfxcount : 10;

                            /* Bits[30:21], Access Type=RO/V, default=0x00000000*/

                            /* PI FSM DFx Count */
    UINT32 lockrst_b : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* Lockrst_b from dlldetlock CBB */

  } Bits;
  UINT32 Data;

} AWORD_DLL_STATUS_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_PIFSMTEST_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x2010fa04)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DFt - PI fsm test
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_CONTROL_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fa08)                                                     */
/* Register default value on SPRHBM: 0x00000500                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL control
*/


#define AWORD_DLL_CONTROL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lockthresh : 3;

                            /* Bits[2:0], Access Type=RW, default=None*/

                            /* LockThresh */
    UINT32 lockctr : 2;

                            /* Bits[4:3], Access Type=RW, default=None*/

                            /* DllDetlock CBB LockCtr */
    UINT32 locksel : 2;

                            /* Bits[6:5], Access Type=RW, default=None*/

                            /*
                               Sel between detlock/rawlock/LockOvrd to be sent
                               to mdllpi to enable PI 10/11 = LockOvrd (debug
                               ovrd) 01 = Detlock 00 = Rawlock
                            */
    UINT32 lockovrd : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /* LockOvrd */
    UINT32 pulsgenctrl : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000001*/

                            /* pulsegenctrl */
    UINT32 ensdlbias : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* EnSdlBias */
    UINT32 selvctrlhi : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* SelVctrlHi */
    UINT32 vctrldischrate : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000000*/

                            /* VctrlDischRate */
    UINT32 rstovrd : 1;

                            /* Bits[14:14], Access Type=RW, default=None*/

                            /* RstOvrd */
    UINT32 selrstovrd : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /* SelRstOvrd */
    UINT32 timerovrd : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /* TimerOvrd */
    UINT32 seltimerovrd : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /* SelTimerOvrd */
    UINT32 openloopovrd : 1;

                            /* Bits[18:18], Access Type=RW, default=None*/

                            /* OpenLoopOvrd */
    UINT32 selopenloopovrd : 1;

                            /* Bits[19:19], Access Type=RW, default=None*/

                            /* SelOpenLoopOvrd */
    UINT32 dllenable : 1;

                            /* Bits[20:20], Access Type=RW, default=None*/

                            /* DllEnable */
    UINT32 dllprechargecount : 5;

                            /* Bits[25:21], Access Type=RW, default=None*/

                            /*
                               Dll Precharge Counter. Each count is 4 cycles of
                               dclk. Overall Dll Lock time after lock request =
                               Precharge Delay + Lock Delay
                            */
    UINT32 dlldetlockcount : 5;

                            /* Bits[30:26], Access Type=RW, default=None*/

                            /*
                               Dll Deterministic Lock Counter after Precharge
                               counter is complete. Each count is 8 cycles of
                               dclk. Overall Dll Lock time after lock request =
                               Precharge Delay + Lock Delay
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AWORD_DLL_CONTROL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_ADC_CONTROL_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fa0c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* ADC control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_TXPI_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x2010fa10)                                                     */
/* Register default value on SPRHBM: 0xC484E0A0                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL TXPIs (DQ and DQS)
*/


#define AWORD_DLL_TXPI_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dqs_pi0_code : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000020*/

                            /* Picode for CK_t/WDQS_t */
    UINT32 dqs_pi0_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for CK_t/WDQS_t */
    UINT32 dqs_pi1_code : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x00000060*/

                            /* Picode for CK_c/WDQS_c */
    UINT32 dqs_pi1_en : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for CK_c/WDQS_c */
    UINT32 txdq_pi0_code : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x00000004*/

                            /* Picode for clktxdqpi0 */
    UINT32 txdq_pi0_en : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for clktxdqpi0 */
    UINT32 txdq_pi1_code : 7;

                            /* Bits[30:24], Access Type=RW/P, default=0x00000044*/

                            /* Picode for clktxdqpi1 */
    UINT32 txdq_pi1_en : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for clktxdqpi1 */

  } Bits;
  UINT32 Data;

} AWORD_DLL_TXPI_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_REFPI_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fa14)                                                     */
/* Register default value on SPRHBM: 0x00000090                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL REFPI
*/


#define AWORD_DLL_REFPI_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 refpi_code : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000010*/

                            /* Picode for RefPi */
    UINT32 refpi_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for RefPi */
    UINT32 do_not_add_refpi_offset : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Don't add RefPi offset to ALL other DLL Pi
                               codes. The raw CR Pi codes are send to the DLL.
                            */
    UINT32 ovrd_xoversel : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set the xover selects from the manual CR
                               controls of this CR.
                            */
    UINT32 xover_train_enable : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Xover Training enable */
    UINT32 rsvd : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqs_pi0_xoversel : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000000*/

                            /* XoverSel for CK_t/WDQS_t */
    UINT32 txdq_pi0_xoversel : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000000*/

                            /* XoverSel for DQ_t */
    UINT32 xover_train_muxsel_pdout : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               MuxSel to choose phse_detect out from which
                               xover instance is used during xover training. DW
                               ={xover_pd_dqdrven,xover_pd_txfifo_rden,xover_pd
                               _pardrven,xover_pd_partxfifo_rden,xover_pd_wdqse
                               n} AW ={xover_pd_c,xover_pd_r,xover_pd_rr,xover_
                               pd_rc,xover_pd_aerr,xover_pd_cke,xover_pd_rcoute
                               n,xover_pd_clken}
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AWORD_DLL_REFPI_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_MISC_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x2010fa18)                                                     */
/* Register default value on SPRHBM: 0x60137A00                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Power Management and misc controls, including Train/FIFO reset.
*/


#define AWORD_MISC_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disablechinckepwrdwn : 1;

                            /* Bits[0:0], Access Type=RW, default=None*/

                            /*
                               PWR_SAVING:Ignore lpmode = 10 (PD) coming from
                               MC. This does not disable PD in memory; only
                               disables PHY actions during lpmode = 10
                            */
    UINT32 forcechinckepwrdwn : 1;

                            /* Bits[1:1], Access Type=RW, default=None*/

                            /*
                               Force ch into PD irrespective of lpmode value.
                               This does not put memory into PD; only mimics
                               PHY actions during lpmode = 10
                            */
    UINT32 disablechinselfrefresh : 1;

                            /* Bits[2:2], Access Type=RW, default=None*/

                            /*
                               PWR_SAVING:Ignore lpmode = 11 (SR) coming from
                               MC. This does not disable SR in memory; only
                               disables PHY actions during lpmode = 11
                            */
    UINT32 forcechinselfrefresh : 1;

                            /* Bits[3:3], Access Type=RW, default=None*/

                            /*
                               Force ch into SR irrespective of lpmode value.
                               This does not put memory into SR; only mimics
                               PHY actions during lpmode = 11
                            */
    UINT32 disableckpark0 : 1;

                            /* Bits[4:4], Access Type=RW, default=None*/

                            /*
                               PWR_SAVING:Disable actions taken in PM1 state.
                               still sends ack to PM1 message
                            */
    UINT32 forceckpark0 : 1;

                            /* Bits[5:5], Access Type=RW, default=None*/

                            /* Force PM1 actions irrespective of PM state */
    UINT32 disablechdlloff : 1;

                            /* Bits[6:6], Access Type=RW, default=None*/

                            /*
                               PWR_SAVING:Disable actions taken in PM2 state.
                               still sends ack to PM2 message
                            */
    UINT32 forcechdlloff : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /* Force PM2 actions irrespective of PM state */
    UINT32 ignoredlllock : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /*
                               Don't wait for Dll Lock to send ack on PM2 -->
                               PM0/PM1 transition. This is used in case there
                               is hang becuase HBMIO does not send pm0 ack as
                               it keeps waiting for dlllock.
                            */
    UINT32 gate_rcpi_lpmode : 2;

                            /* Bits[10:9], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING:Gate AW RCPI in lpmode 00 = Only
                               gated when DLL is off 01 = gate in lpmode 01 (MC
                               IDLE) & Deeper (PD/SR/PM1/PM2) 10 = gate in
                               lpmode 10 (HBM PD) & Deeper (SR/PM1/PM2) 11 =
                               gate in lpmode 11 (HBM SR) & Deeper (PM1/PM2)
                            */
    UINT32 gate_ckpi_lpmode : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000003*/

                            /*
                               PWR_SAVING:Gate AW CKPI in lpmode 00 = Only
                               gated when DLL is off 01 = gate in lpmode 01 (MC
                               IDLE) & Deeper (PD/SR/PM1/PM2) 10 = gate in
                               lpmode 10 (HBM PD) & Deeper (SR/PM1/PM2) 11 =
                               gate in lpmode 11 (HBM SR) & Deeper (PM1/PM2)
                            */
    UINT32 gate_rcpi_pm2 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING:Gate AW RCPI in PM2.Set to 0 only if
                               DLL is not turned off in PM2
                            */
    UINT32 gate_ckpi_pm2 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING:Gate AW CK PI in PM2.Set to 0 only if
                               DLL is not turned off in PM2
                            */
    UINT32 turn_off_xxck : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* when set to 1; xxCK turns off */
    UINT32 static_dll_off_lpmode : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000003*/

                            /*
                               PWR_SAVING:Static control to turn off DLL in low
                               power modes (en_dynamic_dll_off_lpmode must be
                               set to 0) 00 = Don't turn off DLL in low power
                               modes. IF CHOSEN; PLL MUST STAY LOCKED AND DOPS
                               MUST STAY ENABLED IN ALL LOW POWER MODES. 01 =
                               turn off DLL in lpmode 10 (HBM PD) & Deeper
                               (PD/SR/PM1/PM2) 10 = turn off DLL in lpmode 11
                               (HBM SR) & Deeper (SR/PM1/PM2) 11 = turn off DLL
                               in PM2 & Deeper
                            */
    UINT32 en_dynamic_dll_off_lpmode : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               PWR_SAVING:Control whether to use CR or MC
                               driven lpmode[2] to turn off dll. 0 = dll_off
                               controlled by 'static_turn_off_dll_lpmode' CR
                               bit above (static control). 1 = dll_off
                               controlled by lpmode[2] driven by MC (dynamic
                               control).
                            */
    UINT32 swap_xclk_polarity : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               differential clk pair polarity swapping if
                               needed in debug
                            */
    UINT32 enable_ck_sac_drv : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               AGING:Control xxCK anti aging sac drivers. When
                               1; it enables sacrifical driver dynamically one
                               cycle after CK toggle stops and removes it 3
                               cycle before the toggle begins. When set to 0;
                               it will keep main TX driver of CK always enabled
                               insetad of sacrifical one. Bad for aging but
                               does not need 3 cycle early indication before
                               driving CK.
                            */
    UINT32 cr_cmd_pseudo_ch_remap : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               NOTE: Bit 0 is being used for ECO to swap
                               rd/wrcmd's pseudo channel bits to support SoC
                               die mirroring
                            */
    UINT32 reserved : 6;

                            /* Bits[27:22], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 hbm2e_enable : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select 2E Mode. When 1; it enables R6 and C8
                               pins and training LFSR width is 34b. When 0; R6
                               and C8 pins are HiZ and lfsr cmd bus width is
                               30b
                            */
    UINT32 cr_train_reset : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Train reset for Dword TX/RX FIFOs, Aword ACIOLB
                               RX FIFO, AERR/DERR sticky logic, and Rd/Wr Gen
                               logic.
                            */
    UINT32 cr_train_reset_enable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Enable CR override of Train Reset
                               (cr_train_reset bit overrides MC input from IP
                               interface)
                            */
    UINT32 phy_init_complete : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Once BIOS training is done, set this bit to 1,
                               the bit drives ispid_init_complete pin and
                               informs MC channel training is done
                            */

  } Bits;
  UINT32 Data;

} AWORD_MISC_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_PM_TIMING0_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010fa1c)                                                     */
/* Register default value on SPRHBM: 0x01010101                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* This CR holds PM FSM pre-delay timings that are applied before taking a PM action
*/


#define AWORD_PM_TIMING0_CREG_HBM_MIDSTACKGBL_REG 0x1532FA1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 parkentrypredelay : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000001*/

                            /*
                               Pre-delay before parking DIMM clock. This delay
                               controls how long to wait between receiving PM1
                               request and actually moving PM FSM from PM0 to
                               PM1. The actual delay is equivalent to
                               ParkEntryPreDelay * 8 DCLKs, with a maximum
                               delay of ~2.56uS @ 1600MHz
                            */
    UINT32 dlloffpredelay : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000001*/

                            /*
                               Pre-delay before turning DLL OFF. This delay
                               controls how long to wait between receiving PM2
                               request and actually moving PM FSM from PM0 to
                               PM2. The actual delay is equivalent to
                               DllOffPreDelay * 16 DCLKs, with a maximum delay
                               of ~2.56uS @ 1600MHz, 8'h03 implies 60ns for DLL
                               off
                            */
    UINT32 parkexitpredelay : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000001*/

                            /*
                               Pre-delay before un-parking DRAM Device clock.
                               This delay controls how long to wait between
                               receiving PM0 request and actually moving PM FSM
                               from PM1 to PM0. The actual delay is equivalent
                               to ParkExitPreDelay * 16 DCLKs, with a maximum
                               delay of ~2.56uS @ 1600MHz
                            */
    UINT32 dllonpredelay : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000001*/

                            /*
                               Pre-delay before turning DLL ON. This delay
                               controls how long to wait between receiving PM1
                               request and actually moving PM FSM from PM2 to
                               PM0. The actual delay is equivalent to DllOnPre
                               * 16 DCLKs, with a maximum delay of ~2.56uS @
                               1600MHz, 8'h0A implies 200ns for DLL on and lock
                            */

  } Bits;
  UINT32 Data;

} AWORD_PM_TIMING0_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_PM_TIMING1_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010fa20)                                                     */
/* Register default value on SPRHBM: 0x01010801                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* This CR holds PM FSM post-delay timings that are applied after taking a PM action (before responding with ACK or taking subsequent action)
*/


#define AWORD_PM_TIMING1_CREG_HBM_MIDSTACKGBL_REG 0x1532FA20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 parkentrypostdelay : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000001*/

                            /*
                               Post-delay after parking DRAM device clock. This
                               delay controls how long to wait after parking
                               the clock before sending a PM ACK (if request
                               was for PM1) or moving to DllOffPreDelay timer
                               (if request was for PM2). The actual delay is
                               equivalent to ParkEntryPostDelay * 16 DCLKs,
                               with a maximum delay of ~2.56uS @ 1600MHz
                            */
    UINT32 dlloffpostdelay : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000008*/

                            /*
                               Post-delay after turning DLL OFF. This delay
                               controls how long to wait between turning DLL
                               OFF and sending PM ACK. The actual delay is
                               equivalent to DllOffPostDelay * 16 DCLKs, with a
                               maximum delay of ~2.56uS @ 1600MHz, 8'd8 implies
                               80ns for DLL off
                            */
    UINT32 parkexitpostdelay : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000001*/

                            /*
                               Post-delay after un-parking DRAM device clocks.
                               This delay controls how long to wait between un-
                               parking the DRAM device clocks and responding
                               with PM ACK. The actual delay is equivalent to
                               ParkExitPostDelay * 16 DCLKs, with a maximum
                               delay of ~2.56uS @ 1600MHz
                            */
    UINT32 dllonpostdelay : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000001*/

                            /*
                               Post-delay after turning DLL ON. This delay
                               controls how long to wait between turning DLL ON
                               and responding with PM ACK (if request was for
                               PM1) or moving to ParkExitPreDelay timer (if
                               request was for PM0). The actual delay is
                               equivalent to DllOnPostDelay * 16 DCLKs, with a
                               maximum delay of ~2.56uS @ 1600MHz, 8'd10
                               implies 100ns for DLL on and lock
                            */

  } Bits;
  UINT32 Data;

} AWORD_PM_TIMING1_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_READ_TIMING_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                */
/*      SPRHBM (0x2010fa24)                                                     */
/* Register default value on SPRHBM: 0x8120090D                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Read Timing CTRL: the read timing parameters in the register are based on spid rdcmd in AWORD and in Dclk/nCK granularity. spid_rdcmd is used to generate read data valid (for MC), RXFIFO read enable, DQ RX sense-amp enable, and RDQS enable. Due to the micro-architechture property, the minimum DRAM read latency (tRL) is limited by the minimum time that the rd_data_valid is generated and sent back from PHY to MC on the spid interface: it's 13nCKs, i.e., the minimum read latency PHY can support is 13nCKs. Round Trip Latency (tRTL) defaults to tRL + 5 to account for worst case tDQSCK (2.8G) analog delay defined by HBM JEDEC.
    
*/


#define AWORD_READ_TIMING_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rd_data_valid_dly : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x0000000D*/

                            /*
                               Read data valid logic delay to MC. Typically,
                               (Round Trip Latency (RTL) - 5).
                            */
    UINT32 rx_fifo_rd_en_dly : 7;

                            /* Bits[13:7], Access Type=RW/P, default=0x00000012*/

                            /*
                               RX FIFO read enable logic delay. Typically,
                               Round Trip Latency (RTL)
                            */
    UINT32 rx_sa_en_dly : 7;

                            /* Bits[20:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               DQ RX Sense Amp enable logic delay. Typically,
                               (Read Latency (tRL) - 13)
                            */
    UINT32 rdqs_en_dly : 7;

                            /* Bits[27:21], Access Type=RW/P, default=0x00000009*/

                            /*
                               RDQS enable logic delay. Typically, (Read
                               Latency (tRL) - 4)
                            */
    UINT32 osread_width_ext : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x00000008*/

                            /*
                               Extend the outstanding_rd pulse. Extra delay
                               ensures that read datapath is not gated before
                               rx_sa_en de-asserts. Delays may be optimized to
                               save power. PulseWidth = rx_fifo_rd_en_dly + 2 +
                               ParLat + This Field.
                            */

  } Bits;
  UINT32 Data;

} AWORD_READ_TIMING_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_WRITE_TIMING_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:               */
/*      SPRHBM (0x2010fa28)                                                     */
/* Register default value on SPRHBM: 0x19340063                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Write Timing CTRL: the write timing parameters in the register are based on spid wrcmd in AWORD and in Dclk/nCK granularity. spid_wrcmd is used to generate TXFIFO write/read enable, WDQS and DQ output enable. The delay between spid_wrcmd and the enables are controlled by these fields. Due to the micro-architechture property, there is a hidden latency of 7 dclk cycles (7nCKs), i.e., the minimum DRAM write latency PHY can support is 7nCKs. Note: There are some fields in this CR that are overflow from READ_TIMING_CTRL. The field and original CR description contain all the details.
    
*/


#define AWORD_WRITE_TIMING_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wr_dq_out_en_dly : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000003*/

                            /*
                               TX FIFO read (DQ output) enable logic delay.
                               Typically, (tWL - 5).
                            */
    UINT32 tx_fifo_wr_en_dly : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x00000003*/

                            /*
                               TX FIFO write enable logic delay. Typically,
                               (tWL - 5).
                            */
    UINT32 dq_parity_latency : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000000*/

                            /* DQ Parity Latency (match with DWORD_MISC1_CTRL) */
    UINT32 dq_parity_enable : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* DQ Parity Enable (match with DWORD_MISC1_CTRL) */
    UINT32 wrdata_lb_en : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               DFx Digital/FIFO loopback enable. More details
                               in the HAS.
                            */
    UINT32 reserved0 : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 sa_on_duration : 5;

                            /* Bits[20:16], Access Type=RW/P, default=0x00000014*/

                            /*
                               DQ RX Sense Amp ON duration time. Typically,
                               (Round Trip Latency - tRL + tPL) + 15. Moved
                               from READ_TIMING_CTRL.
                            */
    UINT32 rdqs_en_duration : 4;

                            /* Bits[24:21], Access Type=RW/P, default=0x00000009*/

                            /*
                               RDQS enable duration time. Typically, (Round
                               Trip Latency - tRL) + 4. Moved READ_TIMING_CTRL.
                            */
    UINT32 txbiasen_width : 5;

                            /* Bits[29:25], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Controls how long biasen stays high. Always
                               asserts on wrcmd. Typically, (tWL+4).
                            */
    UINT32 reserved1 : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AWORD_WRITE_TIMING_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_LANE_REPAIR_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fa2c)                                                     */
/* Register default value on SPRHBM: 0x000000FF                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Lane Repair mux control. Follows the HBM2e JEDEC mapping.
*/


#define AWORD_LANE_REPAIR_CREG_HBM_MIDSTACKGBL_REG 0x1532FA2C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 row : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000F*/

                            /* Row Lane Repair */
    UINT32 column : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x0000000F*/

                            /* Column Lane Repair */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AWORD_LANE_REPAIR_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY0_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa30)                                                     */
/* Register default value on SPRHBM: 0x01010100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY1_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa34)                                                     */
/* Register default value on SPRHBM: 0x01011100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY2_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa38)                                                     */
/* Register default value on SPRHBM: 0x01012100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY3_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa3c)                                                     */
/* Register default value on SPRHBM: 0x01013100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY4_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa40)                                                     */
/* Register default value on SPRHBM: 0x01014100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY5_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa44)                                                     */
/* Register default value on SPRHBM: 0x01015100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY6_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa48)                                                     */
/* Register default value on SPRHBM: 0x01016100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY7_CREG_HBM_MIDSTACKGBL_REG supported on:            */
/*      SPRHBM (0x2010fa4c)                                                     */
/* Register default value on SPRHBM: 0x01017100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_CFG0_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x2010fa50)                                                     */
/* Register default value on SPRHBM: 0x17C043C0                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Cmd Sequencer Bank/Col Address (BA/CA) counter setup. After hitting count max; Both Bank and Column address counters will continue wrapping around the maximum value by performing (modulo count_max) + count_min. Bank counter 'increment by value' is used to jump bank groups to achieve tCCDS, instead of tCCDL delays between back-to-back Write or Read cmds. This is necessary to support high quality Burst-Idle-Burst sequences to stress power delivery during link training. Defaults support basic training to generate toggles on the cmd bus.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_CFG1_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x2010fa54)                                                     */
/* Register default value on SPRHBM: 0x40040000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Cmd Sequencer Row Address counter config plus reset/start_busy controls
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_LFSR_CFG0_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fa58)                                                     */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train LFSR seed lower bits
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_LFSR_CFG1_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fa5c)                                                     */
/* Register default value on SPRHBM: 0x63010402                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train LFSR seed higher bits, 34b mode, ACIO loopback, error count threshold, and other settings.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_COMPARE_STATUS0_CREG_HBM_MIDSTACKGBL_REG supported on:           */
/*      SPRHBM (0x2010fa60)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train comparator row/col error sticky out
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_COMPARE_STATUS1_CREG_HBM_MIDSTACKGBL_REG supported on:           */
/*      SPRHBM (0x2010fa64)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train comparator cke/aerr sticky out, error count, and threshold fail.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_EXTEST_TXCTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x2010fa68)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* CR control for TX settings during extest mode only. extest_en needs to be set from EXTESTCTL TDR for these overrides to take place
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DFX_TXHIZCTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x2010fa6c)                                                     */
/* Register default value on SPRHBM: 0x01400000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Override TxHiz for 24 drivers while not in extest mode
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DFX_RXCTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010fa70)                                                     */
/* Register default value on SPRHBM: 0x34000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD DFX RX overwrite control registers for 24 RX amp
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_CLK_GATE_CREG_HBM_MIDSTACKGBL_REG supported on:                        */
/*      SPRHBM (0x2010fa74)                                                     */
/* Register default value on SPRHBM: 0x043E0C3E                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Adding RCB and LCB enable and override control bits. Clock gating is disabled by default and will be enabled before entering functional mode. THESE CRs are ignored when safe_mode or FscanClkUngate is asserted. Those global signals take higher precedence and will disable clock gating irrespective of these CRs: en_* - 0 = Clock is forced OFF (flat/gated irrespective of corresponding ovrd_*), 1 = Clock is gated/ungated based on ovrd_*; ovrd_* - 0 = Clock gating is enabled (can be gated based on clock gating enable), 1 = Clock gating is disabled (Clock will be free running)
*/


#define AWORD_CLK_GATE_CREG_HBM_MIDSTACKGBL_REG 0x1532FA74

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en_digs_lpmode_lcb : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* en CMD datapath clk */
    UINT32 en_dlldft_lcb : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /* en dclk going to pifsm */
    UINT32 en_training_lcb : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /* en training logic */
    UINT32 en_clkrd_lcb : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               en BOTH WR/RD shift registers and rdwrengen
                               counters
                            */
    UINT32 en_visa_lcb : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /* en visa clk */
    UINT32 rsvd_6 : 4;

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en_ntl_extest_lcb : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /* en dclk_div4 for NTL/EXTEST */
    UINT32 en_vdm_odi_lcb : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* en vdm/odi dclk */
    UINT32 reserved3__nc : 5;

                            /* Bits[16:12], Access Type=RW, default=0x00000000*/

                            /* reserved */
    UINT32 ovrd_digs_lpmode_lcb : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /* ovrd CMD datapath clk */
    UINT32 ovrd_dlldft_lcb : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /* ovrd dclk going to pifsm */
    UINT32 ovrd_training_lcb : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* ovrd training logic */
    UINT32 ovrd_clkrd_lcb : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               ovrd BOTH WR/RD shift registers and rdwrengen
                               counters
                            */
    UINT32 ovrd_visa_lcb : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* ovrd visa clk */
    UINT32 rsvd_22 : 4;

                            /* Bits[25:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovrd_ntl_extest_lcb : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /* ovrd dclk_div4 for NTL/EXTES */
    UINT32 reserved5__nc : 5;

                            /* Bits[31:27], Access Type=RW, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} AWORD_CLK_GATE_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_VREF_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x2010fa78)                                                     */
/* Register default value on SPRHBM: 0x00730001                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Vref setting and VSSHI bias resides in the fuse CR. Only Vref enable controls reside here. VSSHI debug and VSSHI enables also reside here.
*/


#define AWORD_VREF_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FA78

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 aword_vref_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* Vref Enable */
    UINT32 reserved0__nc : 8;

                            /* Bits[8:1], Access Type=RW/P, default=0x00000000*/

                            /* reserved0 */
    UINT32 aword_vref_dfx_en : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* Vref DFx Enable */
    UINT32 vsshi_dac_settle_cnt : 7;

                            /* Bits[16:10], Access Type=RW/P, default=0x00000040*/

                            /*
                               number of D/4 cycles to wait for vsshi DAC to
                               settle before enabling close loop on
                               vsshiana.Typical delay ~165ns. This starts on
                               pwrgood_rst; so not really useful! Ignore this
                               for HBM/P2E
                            */
    UINT32 aword_vsshi_bwctl : 4;

                            /* Bits[20:17], Access Type=RW/P, default=0x00000009*/

                            /* VssHi OpAmp BW control. */
    UINT32 aword_vsshi_tristate : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000001*/

                            /*
                               VssHi OpAmp Tristate.Active low so tristate when
                               0
                            */
    UINT32 aword_vsshi_dacenable : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* VssHi DAC Enable */
    UINT32 swvsxhi_capen : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* vsxhi charge cancelation circuit is enabled */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AWORD_VREF_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DFX_RXFIFO_READ_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fa7c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug Aword ACIO Loopback RXFIFO READ and control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING0_CREG_HBM_MIDSTACKGBL_REG supported on:                    */
/*      SPRHBM (0x2010faf8)                                                     */
/* Register default value on SPRHBM: 0xA1CAB580                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING0_CREG_HBM_MIDSTACKGBL_REG 0x1532FAF8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vref_ctrl : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000080*/

                            /* Vref Setting Control */
    UINT32 dll_bwctrl : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000005*/

                            /* DLL Bandwidth Tuning Control */
    UINT32 dll_cben : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000003*/

                            /* DLL CB Tuning */
    UINT32 vsshi_bias : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000002*/

                            /* VSSHI Bias Control */
    UINT32 vsshi_target : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x0000004A*/

                            /* VSSHI Target */
    UINT32 dll_picb : 5;

                            /* Bits[27:23], Access Type=RW/P, default=0x00000003*/

                            /* DLL PI CB Tuning */
    UINT32 dq_rxbias : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000002*/

                            /* DQ RX Bias Control */
    UINT32 dqs_rxbias : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000002*/

                            /* DQS RX Bias Control */

  } Bits;
  UINT32 Data;

} HBMIO_FUSE_TUNING0_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING1_CREG_HBM_MIDSTACKGBL_REG supported on:                    */
/*      SPRHBM (0x2010fafc)                                                     */
/* Register default value on SPRHBM: 0x64992082                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_aword_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING1_CREG_HBM_MIDSTACKGBL_REG 0x1532FAFC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_tx_dcc_sel : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000002*/

                            /* DQ TX Duty Cycle Control */
    UINT32 dqs_tx_dcc_sel : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x00000002*/

                            /* DQS TX Duty Cycle Control */
    UINT32 dq_txoutpu_sel : 5;

                            /* Bits[16:12], Access Type=RW/P, default=0x00000012*/

                            /* DQ TX Driver Pull Up Control */
    UINT32 dq_txoutpd_sel : 5;

                            /* Bits[21:17], Access Type=RW/P, default=0x0000000C*/

                            /* DQ TX Driver Pull Down Control */
    UINT32 dqs_txoutpu_sel : 5;

                            /* Bits[26:22], Access Type=RW/P, default=0x00000012*/

                            /* DQS TX Driver Pull Up Control */
    UINT32 dqs_txoutpd_sel : 5;

                            /* Bits[31:27], Access Type=RW/P, default=0x0000000C*/

                            /* DQS TX Driver Pull Down Control */

  } Bits;
  UINT32 Data;

} HBMIO_FUSE_TUNING1_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010fd00)                                                     */
/* Register default value on SPRHBM: 0x00018000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL status
*/


#define DWORD_DLL_STATUS_CREG_HBM_MIDSTACKGBL_REG 0x1532FD00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pilags : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* PI Lags from pifsm test */
    UINT32 adc_done : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* ADC Done */
    UINT32 weak_lock : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* Weak Lock */
    UINT32 early_lock : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* earlylock from dlldetlock CBB */
    UINT32 long_lock : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* 1 if detlock asserts befor rawlock;else 0 */
    UINT32 adcout : 10;

                            /* Bits[14:5], Access Type=RO/V, default=0x00000000*/

                            /* ADC Out */
    UINT32 dll_reset : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000001*/

                            /* DLL Reset , function of PLL lock */
    UINT32 digtimeout : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000001*/

                            /*
                               PreCharge Counter complete indication status=0
                               when timer reaches timeout
                            */
    UINT32 lock : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               Final Lock to mdllpi enable PIclk. Either
                               Detlock/Rawlock/Debug override
                            */
    UINT32 raw_lock : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* Raw Lock from detlock CBB */
    UINT32 detrm_lock : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Deterministic Lock from Counter */
    UINT32 stkyrawlock : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Sticky Raw Lock */
    UINT32 readcrdlldfxcount : 10;

                            /* Bits[30:21], Access Type=RO/V, default=0x00000000*/

                            /* PI FSM DFx Count */
    UINT32 lockrst_b : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* Lockrst_b from dlldetlock CBB */

  } Bits;
  UINT32 Data;

} DWORD_DLL_STATUS_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_PIFSMTEST_CREG_HBM_MIDSTACKGBL_REG supported on:                       */
/*      SPRHBM (0x2010fd04)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DFt - PI fsm test
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_CONTROL_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fd08)                                                     */
/* Register default value on SPRHBM: 0x00000500                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL control
*/


#define DWORD_DLL_CONTROL_CREG_HBM_MIDSTACKGBL_REG 0x1532FD08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lockthresh : 3;

                            /* Bits[2:0], Access Type=RW, default=None*/

                            /* LockThresh */
    UINT32 lockctr : 2;

                            /* Bits[4:3], Access Type=RW, default=None*/

                            /* DllDetlock CBB LockCtr */
    UINT32 locksel : 2;

                            /* Bits[6:5], Access Type=RW, default=None*/

                            /*
                               Sel between detlock/rawlock/LockOvrd to be sent
                               to mdllpi to enable PI 10/11 = LockOvrd (debug
                               ovrd) 01 = Detlock 00 = Rawlock
                            */
    UINT32 lockovrd : 1;

                            /* Bits[7:7], Access Type=RW, default=None*/

                            /* LockOvrd */
    UINT32 pulsgenctrl : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000001*/

                            /* pulsegenctrl */
    UINT32 ensdlbias : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* EnSdlBias */
    UINT32 selvctrlhi : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* SelVctrlHi */
    UINT32 vctrldischrate : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000000*/

                            /* VctrlDischRate */
    UINT32 rstovrd : 1;

                            /* Bits[14:14], Access Type=RW, default=None*/

                            /* RstOvrd */
    UINT32 selrstovrd : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /* SelRstOvrd */
    UINT32 timerovrd : 1;

                            /* Bits[16:16], Access Type=RW, default=None*/

                            /* TimerOvrd */
    UINT32 seltimerovrd : 1;

                            /* Bits[17:17], Access Type=RW, default=None*/

                            /* SelTimerOvrd */
    UINT32 openloopovrd : 1;

                            /* Bits[18:18], Access Type=RW, default=None*/

                            /* OpenLoopOvrd */
    UINT32 selopenloopovrd : 1;

                            /* Bits[19:19], Access Type=RW, default=None*/

                            /* SelOpenLoopOvrd */
    UINT32 dllenable : 1;

                            /* Bits[20:20], Access Type=RW, default=None*/

                            /* DllEnable */
    UINT32 dllprechargecount : 5;

                            /* Bits[25:21], Access Type=RW, default=None*/

                            /*
                               Dll Precharge Counter. Each count is 4 cycles of
                               dclk. Overall Dll Lock time after lock request =
                               Precharge Delay + Lock Delay
                            */
    UINT32 dlldetlockcount : 5;

                            /* Bits[30:26], Access Type=RW, default=None*/

                            /*
                               Dll Deterministic Lock Counter after Precharge
                               counter is complete. Each count is 8 cycles of
                               dclk. Overall Dll Lock time after lock request =
                               Precharge Delay + Lock Delay
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DWORD_DLL_CONTROL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_ADC_CONTROL_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fd0c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* ADC control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_TXPI_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                   */
/*      SPRHBM (0x2010fd10)                                                     */
/* Register default value on SPRHBM: 0xC484E0A0                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL TXPIs (DQ and DQS)
*/


#define DWORD_DLL_TXPI_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FD10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dqs_pi0_code : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000020*/

                            /* Picode for CK_t/WDQS_t */
    UINT32 dqs_pi0_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for CK_t/WDQS_t */
    UINT32 dqs_pi1_code : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x00000060*/

                            /* Picode for CK_c/WDQS_c */
    UINT32 dqs_pi1_en : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for CK_c/WDQS_c */
    UINT32 txdq_pi0_code : 7;

                            /* Bits[22:16], Access Type=RW/P, default=0x00000004*/

                            /* Picode for clktxdqpi0 */
    UINT32 txdq_pi0_en : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for clktxdqpi0 */
    UINT32 txdq_pi1_code : 7;

                            /* Bits[30:24], Access Type=RW/P, default=0x00000044*/

                            /* Picode for clktxdqpi1 */
    UINT32 txdq_pi1_en : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for clktxdqpi1 */

  } Bits;
  UINT32 Data;

} DWORD_DLL_TXPI_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_REFPI_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd14)                                                     */
/* Register default value on SPRHBM: 0x00000090                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL REFPI
*/


#define DWORD_DLL_REFPI_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FD14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 refpi_code : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000010*/

                            /* Picode for RefPi */
    UINT32 refpi_en : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* Pi_en for RefPi */
    UINT32 do_not_add_refpi_offset : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Don't add RefPi offset to ALL other DLL Pi
                               codes. The raw CR Pi codes are send to the DLL.
                            */
    UINT32 ovrd_xoversel : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set the xover selects from the manual CR
                               controls of this CR.
                            */
    UINT32 xover_train_enable : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Xover Training enable */
    UINT32 rsvd : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqs_pi0_xoversel : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000000*/

                            /* XoverSel for CK_t/WDQS_t */
    UINT32 txdq_pi0_xoversel : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000000*/

                            /* XoverSel for DQ_t */
    UINT32 xover_train_muxsel_pdout : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               MuxSel to choose phse_detect out from which
                               xover instance is used during xover training. DW
                               ={xover_pd_dqdrven,xover_pd_txfifo_rden,xover_pd
                               _pardrven,xover_pd_partxfifo_rden,xover_pd_wdqse
                               n} AW ={xover_pd_c,xover_pd_r,xover_pd_rr,xover_
                               pd_rc,xover_pd_aerr,xover_pd_cke,xover_pd_rcoute
                               n,xover_pd_clken}
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DWORD_DLL_REFPI_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_SDLL_VREF_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd18)                                                     */
/* Register default value on SPRHBM: 0x57081F1F                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Includes SDLL/RDQS picode values and Mini SDL deskew value. Vref setting resides in the fuse CR. Only Vref enable controls are here.
*/


#define DWORD_SDLL_VREF_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FD18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rdqs_p_pi_dly : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x0000001F*/

                            /* SDLL/PI pi code to delay RDQSP */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rdqs_n_pi_dly : 7;

                            /* Bits[14:8], Access Type=RW/P, default=0x0000001F*/

                            /* SDLL/PI pi code to delay RDQSN */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_dq_sdl_deskew_ctrl : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               RX Mini SDL stage delay control.settings 5,6,7
                               are illegal.
                            */
    UINT32 rx_dq_sdl_bypass_enb : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000001*/

                            /* Rx Mini SDL bypass control (Active Low) */
    UINT32 rsvd_20 : 4;

                            /* Bits[23:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sdllen : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /* SDLL Enable */
    UINT32 rdqspien : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /* RDQS PI Enable */
    UINT32 en_dynamic_rxminisdl_rst : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               PWR_SAVING: AGING: Enable Dynamic control of
                               rxminisdl reset. 1 = rxmini would be shut off
                               whenever there is no outstanding RD CMD. 0 =
                               rxmini stays on as long bias as MDLL is out of
                               reset
                            */
    UINT32 swvsxhi_capen : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* vsxhi charge cancelation circuit is enabled */
    UINT32 vref_en : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000001*/

                            /* Vref Enable */
    UINT32 vref_dfx_en : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Vref DFx Enable */
    UINT32 en_dynamic_dqsdeskew_rst : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000001*/

                            /*
                               PWR_SAVING: AGING: Enable Dynamic control of
                               dqsdeskew reset. 1 = dqsdekew would be shut off
                               whenever there is no outstanding RD CMD. 0 =
                               dqsdeskew stays on as long bias as MDLL is out
                               of reset
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DWORD_SDLL_VREF_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_MISC1_CTRL_CREG_HBM_MIDSTACKGBL_REG supported on:                      */
/*      SPRHBM (0x2010fd1c)                                                     */
/* Register default value on SPRHBM: 0x00076800                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD misc1 control
*/


#define DWORD_MISC1_CTRL_CREG_HBM_MIDSTACKGBL_REG 0x1532FD1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_parity_latency : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               DQ Parity Latency (match with
                               AWORD_WRITE_TIMING_CTRL)
                            */
    UINT32 reserved : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Reserved0 */
    UINT32 txoutstg_comp_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Tx Output Stage Compensation enable */
    UINT32 txoutstg_comp_derr_rxout : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* Tx Output Stage Compensation DERR rxout value */
    UINT32 txfifo_rst : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* DFX Tx - Resets Tx FIFO */
    UINT32 wrdata_lb_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* write data loop back enable */
    UINT32 dq_parity_enable : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               DQ Parity Enable (match with
                               AWORD_WRITE_TIMING_CTRL)
                            */
    UINT32 rxfifo_ptr_rst_val : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /* RX FIFO Read Pointer (dclk) reset value */
    UINT32 en_wr_par_delay : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Delay WR parity wrt data inside PHY. Set to 1
                               when MC sends parity aligned to data.Used only
                               in DFI mode.Ignored in SPID mode
                            */
    UINT32 disable_rdqs_gating : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable dynamic clk gating of
                               RXPIclk going to rxfifo. This is not power
                               saving feature by itself. It is needed since
                               rxsenseamp of RDQS can glitch when enabled. So
                               it needs to be 0 unless RDQS rx is always
                               enabled
                            */
    UINT32 gate_piclk_lpmode : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /* PWR_SAVING:Gate DW PI in lpmode */
    UINT32 gate_piclk_pm2 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING:Gate DW PI in PM2. Set to 0 only if
                               DLL stays on through PM2
                            */
    UINT32 wdqs_viewsel : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* View WDQS or RefPI */
    UINT32 wdqs_drv_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /* WDQS driver enable */
    UINT32 gate_piclk_dyn : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING: Enable dynamic PIclk gating when no
                               WR traffic. When set to 0; PICLK will be always
                               running until lpmode.
                            */
    UINT32 enable_wdqs_sac_drv : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               AGING:Control WDQS anti aging sac drivers. When
                               1; it enables sacrifical driver dynamically one
                               cycle after WDQS toggle ends and removes it 3
                               cycle before the toggle begins. When set to 0;
                               it will keep main TX driver of WDQS always
                               enabled insetad of sacrifical one. Bad for aging
                               but does not need 3 cycle early indication
                               before driving WDQS.
                            */
    UINT32 reserved1 : 11;

                            /* Bits[29:19], Access Type=RW, default=0x00000000*/

                            /* Reserved1 */
    UINT32 dword_acio_lb_enable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Dword ACIO loopback enable. Enables TX on
                               DERR,RDQS and disables lane repair.
                            */
    UINT32 dword_acio_lb_rdqs_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to have WDQS drive RXPI instead of RDQS
                               in ACIOLB
                            */

  } Bits;
  UINT32 Data;

} DWORD_MISC1_CTRL_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_LANE_REPAIR_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fd20)                                                     */
/* Register default value on SPRHBM: 0x0002FFFF                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD lane repair mux control. Follows the HBM2e JEDEC mapping. Since DBI is essential, only Mode2 is supported.
*/


#define DWORD_LANE_REPAIR_CREG_HBM_MIDSTACKGBL_REG 0x1532FD20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cr_byte0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000F*/

                            /* DWORD Repair Byte 0 */
    UINT32 cr_byte1 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x0000000F*/

                            /* DWORD Repair Byte 1 */
    UINT32 cr_byte2 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x0000000F*/

                            /* DWORD Repair Byte 2 */
    UINT32 cr_byte3 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x0000000F*/

                            /* DWORD Repair Byte 3 */
    UINT32 reserved__nc : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Reserved */
    UINT32 cr_mode : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /* 0 = Mode 1, 1 = Mode 2 */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DWORD_LANE_REPAIR_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_PATTERN_CFG0_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x2010fd24)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Byte pattern mux selects per data byte's 8 DQ bits. Per DQ, select between LFSR0-3 with 2 bit select each. DBI/ECC bit controls are in pattern_cfg1_creg.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_PATTERN_CFG1_CREG_HBM_MIDSTACKGBL_REG supported on:              */
/*      SPRHBM (0x2010fd28)                                                     */
/* Register default value on SPRHBM: 0xC300FF00                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Per byte pattern select options: 0: Byte Pattern0, 1: Byte Pattern1, 2: ~Byte Pattern1, 3: HBM JEDEC LFSR. Defaults set for Basic/LFSR training. A byte includes 1b DBI and 1b ECC, along with 8b DQ.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_CFG0_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fd2c)                                                     */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* LFSR 0-1 Seeds low range [15:0]. Default value set to match JEDEC reset value.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_CFG1_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fd30)                                                     */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* LFSR 2-3 Seeds low range [15:0]. Default value set to match JEDEC reset value.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_CFG2_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fd34)                                                     */
/* Register default value on SPRHBM: 0x0001AAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* LFSR 0-3 Seeds Hi range [19:16]. Plus, error count threshold.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_POLY_CREG_HBM_MIDSTACKGBL_REG supported on:                 */
/*      SPRHBM (0x2010fd38)                                                     */
/* Register default value on SPRHBM: 0x14090000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Full programmable LFSR polynomial. Same value shared across all 4 LFSRs. Polynomial select enables XOR on the input of each LFSR bit. MSB is unused, since actual LFSR always has LSB to MSB feedback tap. Selecting all 0s for lfsr_poly will turn the LFSR into a 20b shift register. Defaults set to HBM JEDEC LFSR configuration.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_READ_MASK_DQ_RISE_CREG_HBM_MIDSTACKGBL_REG supported on:         */
/*      SPRHBM (0x2010fd3c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Mask individual rise/fall bits of each DQ on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_READ_MASK_DQ_FALL_CREG_HBM_MIDSTACKGBL_REG supported on:         */
/*      SPRHBM (0x2010fd40)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Mask individual rise/fall bits of each DQ on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_READ_MASK_ECC_DBI_PAR_CREG_HBM_MIDSTACKGBL_REG supported on:     */
/*      SPRHBM (0x2010fd44)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Mask individual rise/fall bits of each ECC/DBI/PAR/DERR on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_COMPARE_STATUS_DQ_RISE_CREG_HBM_MIDSTACKGBL_REG supported on:    */
/*      SPRHBM (0x2010fd48)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Show Compare sticky results for individual rise/fall bits of each DQ on the comparator
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_COMPARE_STATUS_DQ_FALL_CREG_HBM_MIDSTACKGBL_REG supported on:    */
/*      SPRHBM (0x2010fd4c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Show Compare sticky results for individual rise/fall bits of each DQ on the comparator
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_COMPARE_STATUS_ECC_DBI_PAR_CREG_HBM_MIDSTACKGBL_REG supported on:*/
/*      SPRHBM (0x2010fd50)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Show compare sticky results of individual rise/fall bits of each ECC/DBI/PAR/DERR on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_ERROR_COUNT_STATUS_CREG_HBM_MIDSTACKGBL_REG supported on:        */
/*      SPRHBM (0x2010fd54)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Comparator error counter value and threshold fail signal.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_EXTEST_TXCTRL0_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd58)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* TX extest bypass data for DQ during extest mode. extest_en needs to be set from BSCN CTL DR for this override to be used 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_EXTEST_TXCTRL1_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd5c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* CR control for TX settings during extest mode 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_TXHIZCTRL0_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd60)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX TX control registers for 48 TX drivers while not in extest mode 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_TXHIZCTRL1_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd64)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX TX control registers for 48 TX drivers while not in extest mode 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_RXCTRL0_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fd68)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX RX overwrite control registers for 48 RX amp 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_RXCTRL1_CREG_HBM_MIDSTACKGBL_REG supported on:                     */
/*      SPRHBM (0x2010fd6c)                                                     */
/* Register default value on SPRHBM: 0x30000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX RX overwrite control registers for 48 RX amp 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_SQW_CREG_HBM_MIDSTACKGBL_REG supported on:                             */
/*      SPRHBM (0x2010fd70)                                                     */
/* Register default value on SPRHBM: 0x00000800                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD SQW controls
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_CLK_GATE_CREG_HBM_MIDSTACKGBL_REG supported on:                        */
/*      SPRHBM (0x2010fd74)                                                     */
/* Register default value on SPRHBM: 0x01BE01FF                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Adding RCB and LCB enable and override control bits. Clock gating is disabled by default and will be enabled before entering functional mode. THESE CRs are ignored when safe_mode or FscanClkUngate is asserted. Those global signals take higher precedence and will disable clock gating irrespective of these CRs: en_* - 0 = Clock is forced OFF (flat/gated irrespective of corresponding ovrd_*), 1 = Clock is gated/ungated based on ovrd_*; ovrd_* - 0 = Clock gating is enabled (can be gated based on clock gating enable), 1 = Clock gating is disabled (Clock will be free running)
*/


#define DWORD_CLK_GATE_CREG_HBM_MIDSTACKGBL_REG 0x1532FD74

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en_phy_lpmode_pd_actions : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING:Enable lpmode PD actions.If set to 0,
                               DW ignores lpmode_pd
                            */
    UINT32 en_digs_lpmode_lcb : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* NOT USED!! */
    UINT32 en_dlldft_lcb : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /* en dclk going to pifsm */
    UINT32 en_training_lcb : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /* en dclk for training */
    UINT32 en_digs_wrdata_lcb : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /* en WR datapath logic dclk */
    UINT32 en_digs_rddata_lcb : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /* en RD datapath logic dclk */
    UINT32 en_phy_lpmode_sr_actions : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               PWR_SAVING:Enable lpmode SR actions.If set to 0,
                               DW ignores lpmode_sr
                            */
    UINT32 en_visa_lcb : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* en visa dclk */
    UINT32 en_ntl_extest_lcb : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /* en dclk_div4 for NTL/EXTEST */
    UINT32 reserved1__nc : 7;

                            /* Bits[15:9], Access Type=RW, default=0x00000000*/

                            /* Reserved */
    UINT32 force_phy_lpmode_pd_actions : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Force lpmode PD actions.If set to 1, DW will act
                               as if in lpmode_pd
                            */
    UINT32 ovrd_digs_lpmode_lcb : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /* NOT USED!! */
    UINT32 ovrd_dlldft_lcb : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /* ovrd dclk going to pifsm */
    UINT32 ovrd_training_lcb : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* ovrd dclk for training */
    UINT32 ovrd_digs_wrdata_lcb : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* ovrd WR datapath logic dclk */
    UINT32 ovrd_digs_rddata_lcb : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* ovrd RD datapath logic dclk */
    UINT32 force_phy_lpmode_sr_actions : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Force lpmode SR actions.If set to 1, DW will act
                               as if in lpmode_sr
                            */
    UINT32 ovrd_visa_lcb : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* ovrd visa dclk */
    UINT32 ovrd_ntl_extest_lcb : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /* ovrd dclk_div4 for NTL/EXTEST */
    UINT32 reserved2__nc : 7;

                            /* Bits[31:25], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DWORD_CLK_GATE_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_FIFO_READ_DATA_CREG_HBM_MIDSTACKGBL_REG supported on:                  */
/*      SPRHBM (0x2010fd78)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug access to DWORD fifo entry readout via CR.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_FIFO_CTRL_READ_DATA_CREG_HBM_MIDSTACKGBL_REG supported on:             */
/*      SPRHBM (0x2010fd7c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug access to DWORD fifo entry readout via CR.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_STICKY_CREG_HBM_MIDSTACKGBL_REG supported on:                          */
/*      SPRHBM (0x2010fd80)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/all_mcast_dword_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Learnings from previous project emphasized the need for sticky registers across warm resets. BIOS used these a lot in the survivability space. 
*/


#define DWORD_STICKY_CREG_HBM_MIDSTACKGBL_REG 0x1532FD80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratch_pad : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* reserved */

  } Bits;
  UINT32 Data;

} DWORD_STICKY_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_CP_LO_CREG_HBM_MIDSTACKGBL_REG supported on:                */
/*      SPRHBM (0x2010fe00)                                                     */
/* Register default value on SPRHBM: 0x01000208                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* HBMIO Control Policy (CP) LO
*/


#define HBMIO_SAI_POLICY_CP_LO_CREG_HBM_MIDSTACKGBL_REG 0x1532FE00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x01000208*/

                            /* HBMIO Control Policy (CP) LO */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_CP_LO_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_CP_HI_CREG_HBM_MIDSTACKGBL_REG supported on:                */
/*      SPRHBM (0x2010fe04)                                                     */
/* Register default value on SPRHBM: 0x00000400                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* HBMIO Control Policy (CP) HI
*/


#define HBMIO_SAI_POLICY_CP_HI_CREG_HBM_MIDSTACKGBL_REG 0x1532FE04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* HBMIO Control Policy (CP) HI */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_CP_HI_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_WAC_LO_CREG_HBM_MIDSTACKGBL_REG supported on:               */
/*      SPRHBM (0x2010fe08)                                                     */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* HBMIO Write Access Control (WAC) LO
*/


#define HBMIO_SAI_POLICY_WAC_LO_CREG_HBM_MIDSTACKGBL_REG 0x1532FE08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100020A*/

                            /* HBMIO Write Access Control (WAC) LO */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_WAC_LO_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_WAC_HI_CREG_HBM_MIDSTACKGBL_REG supported on:               */
/*      SPRHBM (0x2010fe0c)                                                     */
/* Register default value on SPRHBM: 0x00000400                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* HBMIO Write Access Control (WAC) HI
*/


#define HBMIO_SAI_POLICY_WAC_HI_CREG_HBM_MIDSTACKGBL_REG 0x1532FE0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* HBMIO Write Access Control (WAC) HI */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_WAC_HI_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_RAC_LO_CREG_HBM_MIDSTACKGBL_REG supported on:               */
/*      SPRHBM (0x2010fe10)                                                     */
/* Register default value on SPRHBM: 0x0100021A                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* HBMIO Read Access Control (RAC) LO
*/


#define HBMIO_SAI_POLICY_RAC_LO_CREG_HBM_MIDSTACKGBL_REG 0x1532FE10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100021A*/

                            /* HBMIO Read Access Control (RAC) LO */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_RAC_LO_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_RAC_HI_CREG_HBM_MIDSTACKGBL_REG supported on:               */
/*      SPRHBM (0x2010fe14)                                                     */
/* Register default value on SPRHBM: 0x20000500                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* HBMIO Read Access Control (RAC) HI
*/


#define HBMIO_SAI_POLICY_RAC_HI_CREG_HBM_MIDSTACKGBL_REG 0x1532FE14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000500*/

                            /* HBMIO Read Access Control (RAC) HI */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_RAC_HI_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_MAILBOX_WAC_LO_CREG_HBM_MIDSTACKGBL_REG supported on:       */
/*      SPRHBM (0x2010fe18)                                                     */
/* Register default value on SPRHBM: 0x0100021A                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO_MAILBOX                                   */
/* HBMIO Write Access Control (WAC) LO
*/


#define HBMIO_SAI_POLICY_MAILBOX_WAC_LO_CREG_HBM_MIDSTACKGBL_REG 0x1532FE18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100021A*/

                            /* HBMIO Write Access Control (WAC) LO */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_MAILBOX_WAC_LO_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_SAI_POLICY_MAILBOX_WAC_HI_CREG_HBM_MIDSTACKGBL_REG supported on:       */
/*      SPRHBM (0x2010fe1c)                                                     */
/* Register default value on SPRHBM: 0x20000400                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/hbmio_sai_creg/mem_hbmio_sai_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO_MAILBOX                                   */
/* HBMIO Write Access Control (WAC) HI
*/


#define HBMIO_SAI_POLICY_MAILBOX_WAC_HI_CREG_HBM_MIDSTACKGBL_REG 0x1532FE1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_policy : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000400*/

                            /* HBMIO Write Access Control (WAC) HI */

  } Bits;
  UINT32 Data;

} HBMIO_SAI_POLICY_MAILBOX_WAC_HI_CREG_HBM_MIDSTACKGBL_STRUCT;
#endif /* (SPRHBM_HOST) */
#endif /* _HBM_MIDSTACKGBL_h */
