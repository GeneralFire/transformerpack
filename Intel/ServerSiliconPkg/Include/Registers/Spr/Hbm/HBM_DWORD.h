
/** @file
  HBM_DWORD.h

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


#ifndef _HBM_DWORD_h
#define _HBM_DWORD_h
#include <Base.h>

#ifdef SPRHBM_HOST
/* DWORD_DLL_STATUS_CREG_HBM_DWORD_REG supported on:                            */
/*      SPRHBM (0x20100000)                                                     */
/* Register default value on SPRHBM: 0x00018000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL status
*/


#define DWORD_DLL_STATUS_CREG_HBM_DWORD_REG 0x15120000

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

} DWORD_DLL_STATUS_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_PIFSMTEST_CREG_HBM_DWORD_REG supported on:                             */
/*      SPRHBM (0x20100004)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DFt - PI fsm test
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_CONTROL_CREG_HBM_DWORD_REG supported on:                           */
/*      SPRHBM (0x20100008)                                                     */
/* Register default value on SPRHBM: 0x00000500                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DLL control
*/


#define DWORD_DLL_CONTROL_CREG_HBM_DWORD_REG 0x15120008

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

} DWORD_DLL_CONTROL_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_ADC_CONTROL_CREG_HBM_DWORD_REG supported on:                           */
/*      SPRHBM (0x2010000c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* ADC control
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_TXPI_CTRL_CREG_HBM_DWORD_REG supported on:                         */
/*      SPRHBM (0x20100010)                                                     */
/* Register default value on SPRHBM: 0xC484E0A0                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL TXPIs (DQ and DQS)
*/


#define DWORD_DLL_TXPI_CTRL_CREG_HBM_DWORD_REG 0x15120010

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

} DWORD_DLL_TXPI_CTRL_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DLL_REFPI_CTRL_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x20100014)                                                     */
/* Register default value on SPRHBM: 0x00000090                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Picodes and enables for all DLL REFPI
*/


#define DWORD_DLL_REFPI_CTRL_CREG_HBM_DWORD_REG 0x15120014

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

} DWORD_DLL_REFPI_CTRL_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_SDLL_VREF_CTRL_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x20100018)                                                     */
/* Register default value on SPRHBM: 0x57081F1F                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Includes SDLL/RDQS picode values and Mini SDL deskew value. Vref setting resides in the fuse CR. Only Vref enable controls are here.
*/


#define DWORD_SDLL_VREF_CTRL_CREG_HBM_DWORD_REG 0x15120018

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

} DWORD_SDLL_VREF_CTRL_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_MISC1_CTRL_CREG_HBM_DWORD_REG supported on:                            */
/*      SPRHBM (0x2010001c)                                                     */
/* Register default value on SPRHBM: 0x00076800                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD misc1 control
*/


#define DWORD_MISC1_CTRL_CREG_HBM_DWORD_REG 0x1512001C

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

} DWORD_MISC1_CTRL_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_LANE_REPAIR_CREG_HBM_DWORD_REG supported on:                           */
/*      SPRHBM (0x20100020)                                                     */
/* Register default value on SPRHBM: 0x0002FFFF                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD lane repair mux control. Follows the HBM2e JEDEC mapping. Since DBI is essential, only Mode2 is supported.
*/


#define DWORD_LANE_REPAIR_CREG_HBM_DWORD_REG 0x15120020

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

} DWORD_LANE_REPAIR_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_PATTERN_CFG0_CREG_HBM_DWORD_REG supported on:                    */
/*      SPRHBM (0x20100024)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Byte pattern mux selects per data byte's 8 DQ bits. Per DQ, select between LFSR0-3 with 2 bit select each. DBI/ECC bit controls are in pattern_cfg1_creg.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_PATTERN_CFG1_CREG_HBM_DWORD_REG supported on:                    */
/*      SPRHBM (0x20100028)                                                     */
/* Register default value on SPRHBM: 0xC300FF00                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Per byte pattern select options: 0: Byte Pattern0, 1: Byte Pattern1, 2: ~Byte Pattern1, 3: HBM JEDEC LFSR. Defaults set for Basic/LFSR training. A byte includes 1b DBI and 1b ECC, along with 8b DQ.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_CFG0_CREG_HBM_DWORD_REG supported on:                       */
/*      SPRHBM (0x2010002c)                                                     */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* LFSR 0-1 Seeds low range [15:0]. Default value set to match JEDEC reset value.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_CFG1_CREG_HBM_DWORD_REG supported on:                       */
/*      SPRHBM (0x20100030)                                                     */
/* Register default value on SPRHBM: 0xAAAAAAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* LFSR 2-3 Seeds low range [15:0]. Default value set to match JEDEC reset value.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_CFG2_CREG_HBM_DWORD_REG supported on:                       */
/*      SPRHBM (0x20100034)                                                     */
/* Register default value on SPRHBM: 0x0001AAAA                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* LFSR 0-3 Seeds Hi range [19:16]. Plus, error count threshold.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_LFSR_POLY_CREG_HBM_DWORD_REG supported on:                       */
/*      SPRHBM (0x20100038)                                                     */
/* Register default value on SPRHBM: 0x14090000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Full programmable LFSR polynomial. Same value shared across all 4 LFSRs. Polynomial select enables XOR on the input of each LFSR bit. MSB is unused, since actual LFSR always has LSB to MSB feedback tap. Selecting all 0s for lfsr_poly will turn the LFSR into a 20b shift register. Defaults set to HBM JEDEC LFSR configuration.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_READ_MASK_DQ_RISE_CREG_HBM_DWORD_REG supported on:               */
/*      SPRHBM (0x2010003c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Mask individual rise/fall bits of each DQ on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_READ_MASK_DQ_FALL_CREG_HBM_DWORD_REG supported on:               */
/*      SPRHBM (0x20100040)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Mask individual rise/fall bits of each DQ on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_READ_MASK_ECC_DBI_PAR_CREG_HBM_DWORD_REG supported on:           */
/*      SPRHBM (0x20100044)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Mask individual rise/fall bits of each ECC/DBI/PAR/DERR on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_COMPARE_STATUS_DQ_RISE_CREG_HBM_DWORD_REG supported on:          */
/*      SPRHBM (0x20100048)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Show Compare sticky results for individual rise/fall bits of each DQ on the comparator
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_COMPARE_STATUS_DQ_FALL_CREG_HBM_DWORD_REG supported on:          */
/*      SPRHBM (0x2010004c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Show Compare sticky results for individual rise/fall bits of each DQ on the comparator
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_COMPARE_STATUS_ECC_DBI_PAR_CREG_HBM_DWORD_REG supported on:      */
/*      SPRHBM (0x20100050)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Show compare sticky results of individual rise/fall bits of each ECC/DBI/PAR/DERR on the comparator.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_TRAIN_ERROR_COUNT_STATUS_CREG_HBM_DWORD_REG supported on:              */
/*      SPRHBM (0x20100054)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Comparator error counter value and threshold fail signal.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_EXTEST_TXCTRL0_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x20100058)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* TX extest bypass data for DQ during extest mode. extest_en needs to be set from BSCN CTL DR for this override to be used 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_EXTEST_TXCTRL1_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x2010005c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* CR control for TX settings during extest mode 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_TXHIZCTRL0_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x20100060)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX TX control registers for 48 TX drivers while not in extest mode 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_TXHIZCTRL1_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x20100064)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX TX control registers for 48 TX drivers while not in extest mode 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_RXCTRL0_CREG_HBM_DWORD_REG supported on:                           */
/*      SPRHBM (0x20100068)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX RX overwrite control registers for 48 RX amp 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_DFX_RXCTRL1_CREG_HBM_DWORD_REG supported on:                           */
/*      SPRHBM (0x2010006c)                                                     */
/* Register default value on SPRHBM: 0x30000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD DFX RX overwrite control registers for 48 RX amp 
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_SQW_CREG_HBM_DWORD_REG supported on:                                   */
/*      SPRHBM (0x20100070)                                                     */
/* Register default value on SPRHBM: 0x00000800                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* DWORD SQW controls
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_CLK_GATE_CREG_HBM_DWORD_REG supported on:                              */
/*      SPRHBM (0x20100074)                                                     */
/* Register default value on SPRHBM: 0x01BE01FF                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Adding RCB and LCB enable and override control bits. Clock gating is disabled by default and will be enabled before entering functional mode. THESE CRs are ignored when safe_mode or FscanClkUngate is asserted. Those global signals take higher precedence and will disable clock gating irrespective of these CRs: en_* - 0 = Clock is forced OFF (flat/gated irrespective of corresponding ovrd_*), 1 = Clock is gated/ungated based on ovrd_*; ovrd_* - 0 = Clock gating is enabled (can be gated based on clock gating enable), 1 = Clock gating is disabled (Clock will be free running)
*/


#define DWORD_CLK_GATE_CREG_HBM_DWORD_REG 0x15120074

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

} DWORD_CLK_GATE_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_FIFO_READ_DATA_CREG_HBM_DWORD_REG supported on:                        */
/*      SPRHBM (0x20100078)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug access to DWORD fifo entry readout via CR.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_FIFO_CTRL_READ_DATA_CREG_HBM_DWORD_REG supported on:                   */
/*      SPRHBM (0x2010007c)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Debug access to DWORD fifo entry readout via CR.
*/

#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* DWORD_STICKY_CREG_HBM_DWORD_REG supported on:                                */
/*      SPRHBM (0x20100080)                                                     */
/* Register default value on SPRHBM: 0x00000000                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_dword_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Learnings from previous project emphasized the need for sticky registers across warm resets. BIOS used these a lot in the survivability space. 
*/


#define DWORD_STICKY_CREG_HBM_DWORD_REG 0x15120080

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

} DWORD_STICKY_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING0_CREG_HBM_DWORD_REG supported on:                          */
/*      SPRHBM (0x201000f8)                                                     */
/* Register default value on SPRHBM: 0xA1CAB580                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING0_CREG_HBM_DWORD_REG 0x151200F8

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

} HBMIO_FUSE_TUNING0_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */

#ifdef SPRHBM_HOST
/* HBMIO_FUSE_TUNING1_CREG_HBM_DWORD_REG supported on:                          */
/*      SPRHBM (0x201000fc)                                                     */
/* Register default value on SPRHBM: 0x64992082                                 */
/* SPRHBM Register File:    sprsp_top/hbmio[0]/ch0_dword0_creg/mem_hbmio_fuse_registers*/
/* Struct generated from SPRHBM BDF: 0_0_0                                      */
/* SPRHBM Security PolicyGroup: HBMIO                                           */
/* Fuse Controls for tuning HBMIO Analog circuits. Refer Fuse RDL/XLS for more details
*/


#define HBMIO_FUSE_TUNING1_CREG_HBM_DWORD_REG 0x151200FC

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

} HBMIO_FUSE_TUNING1_CREG_HBM_DWORD_STRUCT;
#endif /* (SPRHBM_HOST) */
#endif /* _HBM_DWORD_h */
