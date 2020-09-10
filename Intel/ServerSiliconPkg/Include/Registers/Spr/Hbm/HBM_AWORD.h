
/** @file
  HBM_AWORD.h

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

/* SPRC0 Security Policy Groups:                                                */

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */


#ifndef _HBM_AWORD_h
#define _HBM_AWORD_h
#include <Base.h>

#ifdef SPRHBM_HOST
/* AWORD_DLL_STATUS_CREG_HBM_AWORD_REG supported on:                            */
/*      SPRHBM (0x20100400)                                                     */
/* Register default value on SPRHBM: 0x00018000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL status
*/


#define AWORD_DLL_STATUS_CREG_HBM_AWORD_REG 0x15020400

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

} AWORD_DLL_STATUS_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_PIFSMTEST_CREG_HBM_AWORD_REG supported on:                             */
/*      SPRHBM (0x20100404)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DFt - PI fsm test
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_CONTROL_CREG_HBM_AWORD_REG supported on:                           */
/*      SPRHBM (0x20100408)                                                     */
/* Register default value on SPRHBM: 0x00000500                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL control
*/


#define AWORD_DLL_CONTROL_CREG_HBM_AWORD_REG 0x15020408

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

} AWORD_DLL_CONTROL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_ADC_CONTROL_CREG_HBM_AWORD_REG supported on:                           */
/*      SPRHBM (0x2010040c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* ADC control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_TXPI_CTRL_CREG_HBM_AWORD_REG supported on:                         */
/*      SPRHBM (0x20100410)                                                     */
/* Register default value on SPRHBM: 0xC484E0A0                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL TXPIs (DQ and DQS)
*/


#define AWORD_DLL_TXPI_CTRL_CREG_HBM_AWORD_REG 0x15020410

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

} AWORD_DLL_TXPI_CTRL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DLL_REFPI_CTRL_CREG_HBM_AWORD_REG supported on:                        */
/*      SPRHBM (0x20100414)                                                     */
/* Register default value on SPRHBM: 0x00000090                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL REFPI
*/


#define AWORD_DLL_REFPI_CTRL_CREG_HBM_AWORD_REG 0x15020414

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

} AWORD_DLL_REFPI_CTRL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_MISC_CTRL_CREG_HBM_AWORD_REG supported on:                             */
/*      SPRHBM (0x20100418)                                                     */
/* Register default value on SPRHBM: 0x60137A00                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Power Management and misc controls, including Train/FIFO reset.
*/


#define AWORD_MISC_CTRL_CREG_HBM_AWORD_REG 0x15020418

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

} AWORD_MISC_CTRL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_PM_TIMING0_CREG_HBM_AWORD_REG supported on:                            */
/*      SPRHBM (0x2010041c)                                                     */
/* Register default value on SPRHBM: 0x01010101                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* This CR holds PM FSM pre-delay timings that are applied before taking a PM action
*/


#define AWORD_PM_TIMING0_CREG_HBM_AWORD_REG 0x1502041C

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

} AWORD_PM_TIMING0_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_PM_TIMING1_CREG_HBM_AWORD_REG supported on:                            */
/*      SPRHBM (0x20100420)                                                     */
/* Register default value on SPRHBM: 0x01010801                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* This CR holds PM FSM post-delay timings that are applied after taking a PM action (before responding with ACK or taking subsequent action)
*/


#define AWORD_PM_TIMING1_CREG_HBM_AWORD_REG 0x15020420

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

} AWORD_PM_TIMING1_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_REG supported on:                      */
/*      SPRHBM (0x20100424)                                                     */
/* Register default value on SPRHBM: 0x8120090D                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Read Timing CTRL: the read timing parameters in the register are based on spid rdcmd in AWORD and in Dclk/nCK granularity. spid_rdcmd is used to generate read data valid (for MC), RXFIFO read enable, DQ RX sense-amp enable, and RDQS enable. Due to the micro-architechture property, the minimum DRAM read latency (tRL) is limited by the minimum time that the rd_data_valid is generated and sent back from PHY to MC on the spid interface: it's 13nCKs, i.e., the minimum read latency PHY can support is 13nCKs. Round Trip Latency (tRTL) defaults to tRL + 5 to account for worst case tDQSCK (2.8G) analog delay defined by HBM JEDEC.
    
*/


#define AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_REG 0x15020424

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

} AWORD_READ_TIMING_CTRL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_REG supported on:                     */
/*      SPRHBM (0x20100428)                                                     */
/* Register default value on SPRHBM: 0x19340063                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Write Timing CTRL: the write timing parameters in the register are based on spid wrcmd in AWORD and in Dclk/nCK granularity. spid_wrcmd is used to generate TXFIFO write/read enable, WDQS and DQ output enable. The delay between spid_wrcmd and the enables are controlled by these fields. Due to the micro-architechture property, there is a hidden latency of 7 dclk cycles (7nCKs), i.e., the minimum DRAM write latency PHY can support is 7nCKs. Note: There are some fields in this CR that are overflow from READ_TIMING_CTRL. The field and original CR description contain all the details.
    
*/


#define AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_REG 0x15020428

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

} AWORD_WRITE_TIMING_CTRL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_LANE_REPAIR_CREG_HBM_AWORD_REG supported on:                           */
/*      SPRHBM (0x2010042c)                                                     */
/* Register default value on SPRHBM: 0x000000FF                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Lane Repair mux control. Follows the HBM2e JEDEC mapping.
*/


#define AWORD_LANE_REPAIR_CREG_HBM_AWORD_REG 0x1502042C

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

} AWORD_LANE_REPAIR_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY0_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x20100430)                                                     */
/* Register default value on SPRHBM: 0x01010100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY1_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x20100434)                                                     */
/* Register default value on SPRHBM: 0x01011100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY2_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x20100438)                                                     */
/* Register default value on SPRHBM: 0x01012100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY3_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x2010043c)                                                     */
/* Register default value on SPRHBM: 0x01013100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY4_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x20100440)                                                     */
/* Register default value on SPRHBM: 0x01014100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY5_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x20100444)                                                     */
/* Register default value on SPRHBM: 0x01015100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY6_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x20100448)                                                     */
/* Register default value on SPRHBM: 0x01016100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_ENTRY7_CREG_HBM_AWORD_REG supported on:                  */
/*      SPRHBM (0x2010044c)                                                     */
/* Register default value on SPRHBM: 0x01017100                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Eight (8) entries per Aword (one CMD sequencer per Aword). Per entry, only a row or col cmd may be selected (one hot), but not both. Else, NOP gets executed. Setting both row and col to 0 is a NOP. jump2entry and count are supported with NOPs. Within Row cmd, Activate/Precharge/RefSB should be one hot. Else, NOP gets executed. Within Col cmd, Write or Read should be one hot. Else, NOP gets executed. Activate/Write or PreCharge/Read or RefSB cmds need one or both PC0/PC1 set.
          
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_CFG0_CREG_HBM_AWORD_REG supported on:                    */
/*      SPRHBM (0x20100450)                                                     */
/* Register default value on SPRHBM: 0x17C043C0                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Cmd Sequencer Bank/Col Address (BA/CA) counter setup. After hitting count max; Both Bank and Column address counters will continue wrapping around the maximum value by performing (modulo count_max) + count_min. Bank counter 'increment by value' is used to jump bank groups to achieve tCCDS, instead of tCCDL delays between back-to-back Write or Read cmds. This is necessary to support high quality Burst-Idle-Burst sequences to stress power delivery during link training. Defaults support basic training to generate toggles on the cmd bus.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_CMD_SEQ_CFG1_CREG_HBM_AWORD_REG supported on:                    */
/*      SPRHBM (0x20100454)                                                     */
/* Register default value on SPRHBM: 0x40040000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Cmd Sequencer Row Address counter config plus reset/start_busy controls
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_LFSR_CFG0_CREG_HBM_AWORD_REG supported on:                       */
/*      SPRHBM (0x20100458)                                                     */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train LFSR seed lower bits
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_LFSR_CFG1_CREG_HBM_AWORD_REG supported on:                       */
/*      SPRHBM (0x2010045c)                                                     */
/* Register default value on SPRHBM: 0x63010402                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train LFSR seed higher bits, 34b mode, ACIO loopback, error count threshold, and other settings.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_COMPARE_STATUS0_CREG_HBM_AWORD_REG supported on:                 */
/*      SPRHBM (0x20100460)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train comparator row/col error sticky out
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_TRAIN_COMPARE_STATUS1_CREG_HBM_AWORD_REG supported on:                 */
/*      SPRHBM (0x20100464)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD Train comparator cke/aerr sticky out, error count, and threshold fail.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_EXTEST_TXCTRL_CREG_HBM_AWORD_REG supported on:                         */
/*      SPRHBM (0x20100468)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* CR control for TX settings during extest mode only. extest_en needs to be set from EXTESTCTL TDR for these overrides to take place
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DFX_TXHIZCTRL_CREG_HBM_AWORD_REG supported on:                         */
/*      SPRHBM (0x2010046c)                                                     */
/* Register default value on SPRHBM: 0x01400000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Override TxHiz for 24 drivers while not in extest mode
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DFX_RXCTRL_CREG_HBM_AWORD_REG supported on:                            */
/*      SPRHBM (0x20100470)                                                     */
/* Register default value on SPRHBM: 0x34000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* AWORD DFX RX overwrite control registers for 24 RX amp
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_CLK_GATE_CREG_HBM_AWORD_REG supported on:                              */
/*      SPRHBM (0x20100474)                                                     */
/* Register default value on SPRHBM: 0x043E0C3E                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Adding RCB and LCB enable and override control bits. Clock gating is disabled by default and will be enabled before entering functional mode. THESE CRs are ignored when safe_mode or FscanClkUngate is asserted. Those global signals take higher precedence and will disable clock gating irrespective of these CRs: en_* - 0 = Clock is forced OFF (flat/gated irrespective of corresponding ovrd_*), 1 = Clock is gated/ungated based on ovrd_*; ovrd_* - 0 = Clock gating is enabled (can be gated based on clock gating enable), 1 = Clock gating is disabled (Clock will be free running)
*/


#define AWORD_CLK_GATE_CREG_HBM_AWORD_REG 0x15020474

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

} AWORD_CLK_GATE_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_VREF_CTRL_CREG_HBM_AWORD_REG supported on:                             */
/*      SPRHBM (0x20100478)                                                     */
/* Register default value on SPRHBM: 0x00730001                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Vref setting and VSSHI bias resides in the fuse CR. Only Vref enable controls reside here. VSSHI debug and VSSHI enables also reside here.
*/


#define AWORD_VREF_CTRL_CREG_HBM_AWORD_REG 0x15020478

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

} AWORD_VREF_CTRL_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* AWORD_DFX_RXFIFO_READ_CREG_HBM_AWORD_REG supported on:                       */
/*      SPRHBM (0x2010047c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_aword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug Aword ACIO Loopback RXFIFO READ and control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING0_CREG_HBM_AWORD_REG supported on:                          */
/*      SPRHBM (0x201004f8)                                                     */
/* Register default value on SPRHBM: 0xA1CAB580                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING0_CREG_HBM_AWORD_REG 0x150204F8

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

} HBMIO_FUSE_TUNING0_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING1_CREG_HBM_AWORD_REG supported on:                          */
/*      SPRHBM (0x201004fc)                                                     */
/* Register default value on SPRHBM: 0x64992082                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_aword_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING1_CREG_HBM_AWORD_REG 0x150204FC

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

} HBMIO_FUSE_TUNING1_CREG_HBM_AWORD_STRUCT;
#endif /* (SPRHBM_HOST) */
#endif /* _HBM_AWORD_h */
