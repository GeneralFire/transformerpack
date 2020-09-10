
/** @file
  CWV_DDRIO_DAT0_CH0.h

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
/* DDRPHY_GATED_P0                                                              */
/*  Security_WriteAccess_Str:                                                   */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI | SAI_HW_SAI |     */
/*    TGL_NCORE_SAI | UNASSIGNED_4_SAI | TGL_EXT_AGENT_SAI | UNASSIGNED_1_SAI   */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _CWV_DDRIO_DAT0_CH0_h
#define _CWV_DDRIO_DAT0_CH0_h
#include <DataTypes.h>













































/* DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG supported on: */
/*      CWV (0x408070bc)                                                        */
/* Register default value on CWV: 0x03FFFFFF                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrdata/data0ch0/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* Timing and Voltage control for writing one data byte on RankX
*/


#define DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG 0x1BA400BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 11;

                            /* Bits[10:0], Access Type=RW, default=0x000007FF*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 15 tQCK with step size of UI/64
                            */
    UINT32 txdqsdelay : 11;

                            /* Bits[21:11], Access Type=RW, default=0x000007FF*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 15 tQCK with step size of UI/64
                            */
    UINT32 txequalization : 4;

                            /* Bits[25:22], Access Type=RW, default=0x0000000F*/

                            /*
                               # of static legs for de-emphasis: 10 levels. (0:
                               0 Static legs/10 DeEmph legs, 1: 1 Static/9
                               DeEmph leg, 2: 2 Static/8 DeEmph Leg, ... 10: 10
                               Static legs, 11-15: rsvd)
                            */
    UINT32 spare : 6;

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /* spares */

  } Bits;
  UINT32 Data;

} DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT;











/* DDRDATA_CR_DDRCRDATACONTROL0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG supported on:*/
/*      CWV (0x408070e8)                                                        */
/* Register default value on CWV: 0x00000002                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrdata/data0ch0/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* Data offsets to training values.
*/


#define DDRDATA_CR_DDRCRDATACONTROL0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG 0x1BA400E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_clock_on_with_txanalogen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               if set, enables TX DLL clocks with TxAnalogEn
                               (as opposed to DqsOutputEnable, which has
                               tighter timing)
                            */
    UINT32 local_gate_d0tx : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* if set, will force on clocks to transmit path. */
    UINT32 dqsodtvss : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Force Dqs to use Vss ODT despite (OdtMode =
                               Vtt).
                            */
    UINT32 internalclockson : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Forces on all PI enables and the LCB enables in
                               the Data AFE/DigS fubs. Does not force on D0 PI
                               enable or override weaklock mode.
                            */
    UINT32 repeaterclockson : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Forces on the LCB enable to the sequential
                               repeaters. Does not force on D0 PI enable or
                               override weaklock mode.
                            */
    UINT32 wllongdelen : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Forces on all PI enables and the LCB enables in
                               the Data Afe/Digs fubs. Does not force on D0 pi
                               enable or override WeakLock mode.
                            */
    UINT32 txdeskewforceon : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Disable all power down modes in Tx Per Bit
                               Deskew
                            */
    UINT32 txpion : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down Disable: Forces TxDq and TxDqs PI
                               clocks on.
                            */
    UINT32 txon : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               When set, forces DQ/DQS drive enable to active.
                               Used in power measurements and io loopback mode
                            */
    UINT32 txdisable : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down: All write commands are ignored for
                               this data-byte. Used in single-channel MCI mode
                               as well as certain training mode.
                            */
    UINT32 endqodtparkmode : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Terminate data when (LPM0 and !ODT and !DrvEn) */
    UINT32 endqsodtparkmode : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /*
                               Park strobe during idle periods (0: Park Off, 1:
                               Park Diff Low when (LPM0 and !ODT and !DrvEn),
                               2: Park Diff low when (LPM0 and !ODT and !DrvEn
                               and TxAnalogEn) 3: Park both low when (LPM0 and
                               !ODT and !DrvEn)
                            */
    UINT32 biaspmctrl : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /*
                               PM control for Bias {0: Bias always on, 1: Bias
                               on with RxAnalogEn, 2: Bias on when not in
                               LPMode3, 3: Bias on when not in lpmode2}
                            */
    UINT32 vrefpmctrl : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /*
                               PM control for Vref {0: Vref always strong, 1:
                               Vref weak in LPMode3 (SR), 2: Vref weak in
                               LPMode2/3, 3: Vref weak during !RxAnalogEn}
                            */
    UINT32 rxdisable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down: All read commands are ignored for
                               this data-byte. Used in single-channel MCI mode
                               as well as certain training modes.
                            */
    UINT32 forcerxon : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Force on the RxAmp only (as opposed to
                               OdtSampOn, which turns on both ODT and the
                               amplifier). Use for overclocking support where
                               we may not be able to hit the fast exit latency
                            */
    UINT32 rxmode : 2;

                            /* Bits[20:19], Access Type=RW, default=0x00000000*/

                            /*
                               Force on the RxAmp only. Use for overclocking
                               support where we may not be able to hit the fast
                               exit latency
                            */
    UINT32 dqsnparklowvoh : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               If EnDQSODTParkMode, DqsN turns on ODT Pullup
                               during idle. If (EnDQSODTParkMode &
                               DqsNParkLowVoh), DqsN also enables the ODT
                               Pulldown during idle.
                            */
    UINT32 odtforceqdrven : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* ODT is forced-on. Qualified with DrvEn. */
    UINT32 odtsampoff : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               ODT and Senseamp are forced-off. Higher priority
                               than ForceRxOn, OdtForceQDrvEn or Delay/Duration
                               values
                            */
    UINT32 disableodtstatic : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* Disable ODT static legs for DQ pins (not DQS) */
    UINT32 ddrcrforceodton : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* Unused */
    UINT32 odtmode : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               0: ODT to Vss 1: ODT to Vddq 2: ODT to VTT 3:
                               not used
                            */
    UINT32 txrankmuxdelay : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               Tx DQ rank settings switch delay to compensate
                               flyby delays.
                               Min(Rank[PopRanks].TxDqPiDelay/(64<<G2)). As TAT
                               is in Dclk, this field must be even.
                            */

  } Bits;
  UINT32 Data;

} DDRDATA_CR_DDRCRDATACONTROL0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT;






/* DDRDATA_CR_DDRCRDATACONTROL3_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG supported on:*/
/*      CWV (0x408070f4)                                                        */
/* Register default value on CWV: 0x00471C01                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrdata/data0ch0/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* Data Control CRs
*/


#define DDRDATA_CR_DDRCRDATACONTROL3_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG 0x1BA400F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gear1 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* 1'b1: gear 1 mode, 1'b0: gear 2 mode. */
    UINT32 vrefbypassvsshi : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Controls VssHi in Internal Vref. 0: use global
                               VssHi 1: bypass vsshi to vss
                            */
    UINT32 rxpwrmuxselvdd2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               control power mux in SAL Rx 0: select VccIOG 1:
                               Select Vdd2G
                            */
    UINT32 vrefpwrmuxselvdd2 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               control power mux in VrefGen 0: select VccDDQ 1:
                               Select Vdd2G
                            */
    UINT32 rxsaltailctrl : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               SAL tail current where larger values yields
                               higher currents (I/Iref = 4 + SalTailCtrl). This
                               impacts delay and thermal noise (due to time CS
                               is in saturation)
                            */
    UINT32 wrpreamble : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               Write Preamble. Number of Preamble = WrPreamble
                               + 1
                            */
    UINT32 rxsaldfeovrdsel : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Not used */
    UINT32 dbienable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Enable DRAM DataBusInversion (Not that DBI isn't
                               support in all versions of the PHY)
                            */
    UINT32 dbimode : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* DBI Mode : 0: DC Mode 1 : AC Mode */
    UINT32 qualifysdlwithrcven : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Controls if SDL input clock is qualified with
                               RcvEn or is free running. FIFO clock is always
                               qualified by RcvEnPost. QualifySdlWithRcvEn =
                               Matched path.
                            */
    UINT32 rxfoldcs : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /*
                               Adjusts the strengths of the npath/nbias folding
                               offset legs (0: 0. 1: 20. 2: 42. 3: 64) and
                               ppath/nbias diode load legs (0: 16. 1: 15. 2:
                               14. 3: 12). higher values provide less current
                               to the load and higher(npath)/lower(ppath)
                               output common mode.
                            */
    UINT32 dqspadmuxselvdd2 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Control PowerMux for DqsPadMux. 1: select Vdd2g
                               0: select VccIOG
                            */
    UINT32 txeq_rankmuxdelay_offset : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000003*/

                            /*
                               delays EQ setting rank mux with respect to the
                               base rank mux delay, as dictated by
                               DqTxRankMuxDelay which controls the PI code
                               switch time. 0:0cycles, 1:1cycles, etc
                            */
    UINT32 txperbit_rankmuxdelay_offset : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /*
                               delays Dq and Dqs pbd setting rank mux with
                               respect to the base rank mux delay, as dictated
                               by DqTxRankMuxDelay which controls the PI code
                               switch time. 0:0cycles, 1:1cycles, etc
                            */
    UINT32 blocksdlwithrcven : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Block SDL with early version of RcvEn for
                               unmatched path when QualifySdlWithRcvEn is low.
                               Mutex1(BlockSdlWithRcvEn, QualifySdlWithRcvEn)
                            */
    UINT32 rcvenextff : 2;

                            /* Bits[22:21], Access Type=RW, default=0x00000002*/

                            /*
                               Drive RcvEnPiFF N QClks early for the VccDLL FF
                               legs. {0: 0, 1: 1, 2: 2, 3: 3} QClk Early
                            */
    UINT32 disabletxdqs : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Disable transmitter PI and DrvEn for DQS_P/N.
                               Used in LPDDR5 mode where DQS is only used in
                               reads (except during LP5 IOLB)
                            */
    UINT32 cwvx4txcontrol : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disable Upper strobe in TX path in CWV X4
                               mode. SXP PHY and DRAM PHY will always disable.
                               1 - Enable both Upper and Lower Strobes in TX
                               path in CWV X4 mode. DDRT PHY with standard
                               buffer will enable this mode
                            */
    UINT32 cwvx4rxstrobesel : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Select Lower strobe in RX path in CWV X4
                               mode. 1 - Select Upper strobe in RX path in CWV
                               X4 mode.
                            */
    UINT32 rxpbdcalibration : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               Calibration value for Rx instaces of rx
                               dqdeskew: (0: 1x step, 1: 2x step, 2-3: rsvd)
                            */
    UINT32 txdqsrankmuxdelay : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /*
                               Tx DQS rank settings switch delay to compensate
                               flyby delays.
                               Min(Rank[PopRanks].TxDqsPiDelay/(64<<G2)). As
                               TAT is in Dclk, this field must be even.
                            */

  } Bits;
  UINT32 Data;

} DDRDATA_CR_DDRCRDATACONTROL3_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT;








/* DDRDATA_CR_DDRCRCMDBUSTRAIN_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG supported on:*/
/*      CWV (0x40807104)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrdata/data0ch0/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* ddrdata misc control
*/


#define DDRDATA_CR_DDRCRCMDBUSTRAIN_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG 0x1BA40104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrdqovrdmodeen : 11;

                            /* Bits[10:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used in LP5 CBT mode to enable DQ lanes to drive
                               a constant value, each bit correspond to a
                               specific DQ lane. Mapping = (0:7 => DQ[0:7], 8:9
                               => DQS_P/N, 10 => DMI) but remap using
                               DdrCrWrReTrainSwizzleControl.DataRetrain_DQ7
                            */
    UINT32 ddrdqovrddata : 11;

                            /* Bits[21:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used in LP5 CBT mode to set value of '0' or '1'
                               to each of the DQ7 lanes when DdrDqOvrdModeEn is
                               set in corresponding DQ lane. Mapping = (0:7 =>
                               DQ[0:7], 8:9 => DQS_P/N, 10 => DMI) but remap
                               using
                               DdrCrWrReTrainSwizzleControl.DataRetrain_DQ7
                            */
    UINT32 ddrcrdqsmaskvalue : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Preamble Mask value is '1 (DDR4 long preamble)
                               or '0 (all other usage)
                            */
    UINT32 ddrcrnumofpulses : 2;

                            /* Bits[24:23], Access Type=RW, default=0x00000000*/

                            /*
                               Toggle DQS N times after the static low preamble
                               where N = (0: 1, 1: 2, 2: 3, 3: BL+LongPreamble-
                               PulseNumStart)
                            */
    UINT32 ddrcrmaskcntpulsenumstart : 4;

                            /* Bits[28:25], Access Type=RW, default=0x00000000*/

                            /*
                               Mask out the first N DQS pulses to create a
                               static low preamble. Can also be used during
                               training modes to shift location of the DQS
                               pulses.
                            */
    UINT32 ddrcrwrpostamble : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Extends period when DQS is driving after the
                               burst by DdrCRWrPostamble tCK cycles. Postamble
                               drives the DqsMaskValue. (B0 Step)
                            */
    UINT32 cwvwrpostambletoggle : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               New bitfield for CWV. Will enable postamble
                               toggle for writes. Need to be set for DDRT phy.
                               SXP and DRAM phys should not set this bit.
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRDATA_CR_DDRCRCMDBUSTRAIN_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT;



























/* DDRDATA_CR_DDRCRDATACONTROLBYPASS_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG supported on:*/
/*      CWV (0x40807168)                                                        */
/* Register default value on CWV: 0x00000100                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrdata/data0ch0/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* ddrdata misc control
*/


#define DDRDATA_CR_DDRCRDATACONTROLBYPASS_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG 0x1BA40168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cwvdqdqsrxauxclkgate : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Gate auxclk for Rx DCD at DQSTOP. */
    UINT32 cwvdqdqstxauxclkgate : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Gate auxclk for Tx DCD at DQSTOP and DQTOP. */
    UINT32 cwvdqdqstxdcabypassen : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Disable and bypass Tx DCA in PBDTOP and DQSTOP. */
    UINT32 cwvdqssel : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Select nibble 0 bypass or SDL input between
                               nibble 0 DQS Rx and nibble 1 DQS Rx. 0: Nibble 0
                               bypass or SDL input is from nibble 0 DQS Rx. 1:
                               Nibble 0 bypass or SDL input is from nibble 1
                               DQS Rx.
                            */
    UINT32 cwvmdllbypassen : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Bypass MDLL and PI. */
    UINT32 cwvmdllenb : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Disable MDLL and PI. */
    UINT32 cwvnbl1selbypass : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Select nibble 1 bypass input between nibble 0
                               DQS Rx and nibble 1 DQS Rx. 0: Nibble 1 bypass
                               input is from nibble 0 DQS Rx. 1: Nibble 1
                               bypass input is from nibble 1 DQS Rx.
                            */
    UINT32 cwvrxpbdbypassen : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bypass Rx DCA in PBDTOP. This one goes to the
                               PBDTOP and DQTOP.
                            */
    UINT32 cwvrxpbdenable : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /* Enable Rx DCA in PBDTOP. */
    UINT32 cwvsdlbypassen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bypass SDL2PI in SDL2PITOP and SDL2PI replica in
                               DQTOP.
                            */
    UINT32 cwvsdlenb : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Enable SDL in SDL2PITOP. */
    UINT32 cwvxoverbypassen : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bypass XOVER. Din goes to the pclk negedge
                               directly.
                            */
    UINT32 cwvphsel : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               MDLL PI clk phase select. 1: 180deg shifted from
                               QCLK. 0: 0deg.
                            */
    UINT32 cwvrefphsel : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               MDLL reference clk phase select. 1: 180deg
                               shifted from QCLK. 0: 0deg.
                            */
    UINT32 cwvwriteleven : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Write Leveling Enable. */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRDATA_CR_DDRCRDATACONTROLBYPASS_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT;

#endif /* _CWV_DDRIO_DAT0_CH0_h */
