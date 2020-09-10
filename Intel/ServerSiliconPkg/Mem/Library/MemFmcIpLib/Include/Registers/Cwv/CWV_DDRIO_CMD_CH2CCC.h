
/** @file
  CWV_DDRIO_CMD_CH2CCC.h

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


#ifndef _CWV_DDRIO_CMD_CH2CCC_h
#define _CWV_DDRIO_CMD_CH2CCC_h
#include <DataTypes.h>








/* DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG supported on:*/
/*      CWV (0x40807818)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch2ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* CCC Pi Settings
*/


#define DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG 0x1BA43818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 picode0 : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /* Maps to original Picode[7:1] */
    UINT32 picode1 : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /* Maps to original Picode[7:1] */
    UINT32 picode2 : 8;

                            /* Bits[23:16], Access Type=RW/V, default=0x00000000*/

                            /* Maps to original Picode[7:1] */
    UINT32 picodeextra : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Maps to {2'b0, PiCode2[8,0], PiCode1[8,0],
                               PiCode0[8,0]}
                            */

  } Bits;
  UINT32 Data;

} DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT;







/* DDRCCC_CR_DDRCRPINSUSED_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG supported on:  */
/*      CWV (0x40807830)                                                        */
/* Register default value on CWV: 0x010F9FFF                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch2ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* Pins used in the CCC fub as well as various control TX control settings for slew rate and swing
*/


#define DDRCCC_CR_DDRCRPINSUSED_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG 0x1BA43830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txen : 13;

                            /* Bits[12:0], Access Type=RW, default=0x00001FFF*/

                            /*
                               Static enable for CCC TX[x] based on DRAM
                               technology, system population (ranks used), etc.
                               This doesn't override the dynamic SPID DrvEn.
                            */
    UINT32 cccmuxselect : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /*
                               CCC ISpid pin mux select {0: DDR4, 1: LP4, 2:
                               LP5 Ascending, 3: LP5 Decending} (UY) {0: DDR4,
                               1: LP4, 2: LP5, 3: DDR5} (SH)
                            */
    UINT32 pien : 5;

                            /* Bits[19:15], Access Type=RW, default=0x0000001F*/

                            /* PI clock enable - direct control over PI clocks */
    UINT32 pienovrd : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Value of 0 means PiEn[4:0] only forces the PI
                               on. Value of 1 means PiEn[4:0] forces the PI on
                               or off.
                            */
    UINT32 ddrcaslwdlybypass : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Increase the slew rate for the CA transmitters
                               by bypassing all of the delay cells
                            */
    UINT32 ddrctlslwdlybypass : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Increase the slew rate for the CTL transmitters
                               by bypassing all of the delay cells
                            */
    UINT32 ddrclkslwdlybypass : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Increase the slew rate for the CLK transmitters
                               by bypassing all of the delay cells
                            */
    UINT32 gear1 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /* 1'b1: gear 1 mode. 1'b0: gear 2 mode. */
    UINT32 timerxxclk : 3;

                            /* Bits[27:25], Access Type=RW, default=0x00000000*/

                            /*
                               DDR4 xxCLK takes time to stabilize AC
                               termination. Timer creates xxClkGood after
                               2^(2+Timer) QXClks. (Time==0) disables checks
                               for systems w/o this issue
                            */
    UINT32 keepxxclkon : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Keeps the XXClk DrvEn on in LPMode and DVFS
                               switching. Spid_ClkStop still controls toggling
                               but this forces it to stop differential low
                               instead of HiZ
                            */
    UINT32 spare : 3;

                            /* Bits[31:29], Access Type=RW, default=0x00000000*/

                            /* Spare */

  } Bits;
  UINT32 Data;

} DDRCCC_CR_DDRCRPINSUSED_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT;

























/* DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG supported on:  */
/*      CWV (0x40807894)                                                        */
/* Register default value on CWV: 0x20202020                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch2ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* RX vref controls.
*/


#define DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG 0x1BA43894

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vrefctl_rx5 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000020*/

                            /* Vref code. */
    UINT32 vrefctl_rx6 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000020*/

                            /* Vref code. */
    UINT32 vrefctl_rx7 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000020*/

                            /* Vref code. */
    UINT32 vrefctl_rx8 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000020*/

                            /* Vref code. */

  } Bits;
  UINT32 Data;

} DDRCCC_CR_DDRCRCCCVREF1_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT;

/* DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG supported on:  */
/*      CWV (0x40807898)                                                        */
/* Register default value on CWV: 0x20202020                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch2ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* RX vref controls.
*/


#define DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG 0x1BA43898

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vrefctl_rx9 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000020*/

                            /* Vref code. */
    UINT32 vrefctl_rx10 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000020*/

                            /* Vref code. */
    UINT32 vrefctl_rx11 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000020*/

                            /* Vref code. */
    UINT32 vrefctl_rx12 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000020*/

                            /* Vref code. */

  } Bits;
  UINT32 Data;

} DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT;














/* DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG supported on:*/
/*      CWV (0x408078d0)                                                        */
/* Register default value on CWV: 0x0000020A                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch2ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* RX enable signals for DFE, offset cancellation, SAL, sampler, summer.
*/


#define DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG 0x1BA438D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxgainctlbiasadj : 8;

                            /* Bits[7:0], Access Type=RW, default=0x0000000A*/

                            /* RX Bias Adj for gain modulation. */
    UINT32 imodebiasen : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Enable Imode bias. */
    UINT32 rxbiasen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /* Enable RX bias gen. */
    UINT32 anaviewmuxctl : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /* Observability. */
    UINT32 rxsamptailfuse : 8;

                            /* Bits[20:13], Access Type=RW, default=0x00000000*/

                            /* Enable half of SAL current source legs */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vrefadj : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /* signed offset to per bit Vref values */

  } Bits;
  UINT32 Data;

} DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT;






#endif /* _CWV_DDRIO_CMD_CH2CCC_h */
