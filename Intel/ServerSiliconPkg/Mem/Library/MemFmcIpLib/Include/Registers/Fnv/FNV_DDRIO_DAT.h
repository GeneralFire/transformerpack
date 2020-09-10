
/** @file
  FNV_DDRIO_DAT.h

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

/* BWV Security Policy Groups:                                                  */


#ifndef _FNV_DDRIO_DAT_h
#define _FNV_DDRIO_DAT_h
#include "DataTypes.h"

/* RXGROUP0N0RANK0_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6200)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank0. Broadcast reads are not supported.
*/


#define RXGROUP0N0RANK0_FNV_DDRIO_DAT_REG 0x1B040200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* readx4modeenb */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_P to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_N to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS (0: 1/16 UI offset, [br] 1:
                               3/16 UI Offset, 2: 5/16 UI Offsets, 3: 7/16 UI
                               Offsets, 4: 9/16 UI Offsets, 5,6,7: 11/16 UI
                               Offsets.)
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE current steering tuning (0=no eq, 7=max
                               eq). It is decoded to[br] CTLE resistor tuning:
                               00 - 3R (10.8Kohms), 01/10 - R (3.6Kohms), 11 -
                               R/3 (1.2Kohms)[br] CTLE capacitor tuning: 00 -
                               0C (not used), 01 - 1C (200ff), 10 - 2C (400ff),
                               11 - 3C (600ff)
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N0RANK0_FNV_DDRIO_DAT_STRUCT;

/* RXGROUP0N0RANK1_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6204)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank1. Broadcast reads are not supported.
*/


#define RXGROUP0N0RANK1_FNV_DDRIO_DAT_REG 0x1B040204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* readx4modeenb */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_P to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_N to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE current steering tuning (0=no eq, 7=max
                               eq). It is decoded to[br] CTLE resistor tuning:
                               00 - 3R (10.8Kohms), 01/10 - R (3.6Kohms), 11 -
                               R/3 (1.2Kohms)[br] CTLE capacitor tuning: 00 -
                               0C (not used), 01 - 1C (200ff), 10 - 2C (400ff),
                               11 - 3C (600ff)
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N0RANK1_FNV_DDRIO_DAT_STRUCT;

/* RXGROUP0N0RANK2_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6208)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank2. Broadcast reads are not supported.
*/


#define RXGROUP0N0RANK2_FNV_DDRIO_DAT_REG 0x1B040208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* readx4modeenb */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_P to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_P to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE current steering tuning (0=no eq, 7=max
                               eq). It is decoded to[br] CTLE resistor tuning:
                               00 - 3R (10.8Kohms), 01/10 - R (3.6Kohms), 11 -
                               R/3 (1.2Kohms)[br] CTLE capacitor tuning: 00 -
                               0C (not used), 01 - 1C (200ff), 10 - 2C (400ff),
                               11 - 3C (600ff)
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N0RANK2_FNV_DDRIO_DAT_STRUCT;

/* RXGROUP0N0RANK3_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b620c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank3. Broadcast reads are not supported.
*/


#define RXGROUP0N0RANK3_FNV_DDRIO_DAT_REG 0x1B04020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* readx4modeenb */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_P to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /*
                               per nibble READ timing control, delay DQS_N to
                               match the earlest arrival DQ, 0~1.5UI with step
                               size ClkPi/64
                            */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE current steering tuning (0=no eq, 7=max
                               eq). It is decoded to[br] CTLE resistor tuning:
                               00 - 3R (10.8Kohms), 01/10 - R (3.6Kohms), 11 -
                               R/3 (1.2Kohms)[br] CTLE capacitor tuning: 00 -
                               0C (not used), 01 - 1C (200ff), 10 - 2C (400ff),
                               11 - 3C (600ff)
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N0RANK3_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N0RANK0_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6240)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N0RANK0_FNV_DDRIO_DAT_REG 0x1B040240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* select one of the four DQ Clk */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N0RANK0_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N0RANK1_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6244)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N0RANK1_FNV_DDRIO_DAT_REG 0x1B040244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* Select one of the four DQ clks */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N0RANK1_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N0RANK2_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6248)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N0RANK2_FNV_DDRIO_DAT_REG 0x1B040248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* Select one of the four DQ clks */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N0RANK2_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N0RANK3_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b624c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N0RANK3_FNV_DDRIO_DAT_REG 0x1B04024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* Select one of the four DQ clk */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N0RANK3_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN0RANK0_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6280)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN0RANK0_FNV_DDRIO_DAT_REG 0x1B040280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQSP (to sample even bits)
                               is used in the RX FIFO. For proper operation,
                               always pick latest DQSP
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQSN (to sample odd bits)
                               is used in the RX FIFO. For proper operation,
                               always pick latest DQSN
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN0RANK0_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN0RANK1_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6284)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN0RANK1_FNV_DDRIO_DAT_REG 0x1B040284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               [0]=0 selects the raw output from the nibble DQS
                               amplifier; [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN0RANK1_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN0RANK2_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6288)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN0RANK2_FNV_DDRIO_DAT_REG 0x1B040288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               [0]=0 selects the raw output from the nibble DQS
                               amplifier; [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN0RANK2_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN0RANK3_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b628c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN0RANK3_FNV_DDRIO_DAT_REG 0x1B04028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               [0]=0 selects the raw output from the nibble DQS
                               amplifier; [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN0RANK3_FNV_DDRIO_DAT_STRUCT;

/* RXVREFCTRLN0_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b62a4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* RxVrefCtrl. Broadcast reads are not supported.
*/


#define RXVREFCTRLN0_FNV_DDRIO_DAT_REG 0x1B0402A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vrefperbit0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben0 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 vrefperbit1 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben1 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               enables fine grain Vref control, shift down the
                               Vref voltage by ~0.5 stepsize
                            */
    UINT32 vrefperbit2 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben2 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               enables fine grain Vref control, shift down the
                               Vref voltage by ~0.5 stepsize
                            */
    UINT32 vrefperbit3 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben3 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* shift down the Vref voltage by ~0.5 stepsize */

  } Bits;
  UINT32 Data;

} RXVREFCTRLN0_FNV_DDRIO_DAT_STRUCT;

/* DATATRAINFEEDBACKN0_FNV_DDRIO_DAT_REG supported on:                          */
/*      BWV (0x401b62a8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data Training Feedback Results. Broadcast reads are not supported.
*/


#define DATATRAINFEEDBACKN0_FNV_DDRIO_DAT_REG 0x1B0402A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datatrainfeedback : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Data Training Feedback Storage[br] [br]
                               DataTrainFeedback Description:[br] Training Step
                               Bits Nibble Description[br] -------------- ----
                               ------- --------------------------[br] Cross
                               Over Cal 0 0 Tx Dqs Phase Detection[br] Cross
                               Over Cal 1 0 Tx Dq Phase Detection[br] Cross
                               Over Cal 2 0 Rec En Phase Detection[br] RcvEn
                               8:0 0 Byte detection[br] [br] Wr Leveling 8:0 0
                               Byte detection[br] Senseamp offset 3:0 0 Dq
                               Senseamp offset Detection
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DATATRAINFEEDBACKN0_FNV_DDRIO_DAT_STRUCT;

/* DDRCRFSMCAL_FNV_DDRIO_DAT_REG supported on:                                  */
/*      BWV (0x401b62c8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Fsm for  Voc and Xover caliberation, Voc caliberation is a procedure that cancel out intrinsic offset in analogue receivers and Xover training ,it aligns all the local dll to the global qclk
*/


#define DDRCRFSMCAL_FNV_DDRIO_DAT_REG 0x1B0402C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0fsm : 5;

                            /* Bits[4:0], Access Type=ROV, default=0x00000000*/

                            /*
                               Loads the value from fsm to Cr when caliberation
                               of first receiver is done(read only)
                            */
    UINT32 saoffset1fsm : 5;

                            /* Bits[9:5], Access Type=ROV, default=0x00000000*/

                            /*
                               Loads the value from fsm to Cr when caliberation
                               of second receiver is done(read only)
                            */
    UINT32 saoffset2fsm : 5;

                            /* Bits[14:10], Access Type=ROV, default=0x00000000*/

                            /*
                               Loads the value from fsm to Cr when caliberation
                               of third receiver is done(read only)
                            */
    UINT32 saoffset3fsm : 5;

                            /* Bits[19:15], Access Type=ROV, default=0x00000000*/

                            /*
                               Loads the value from fsm to Cr when caliberation
                               of fourth receiver is done(read only)
                            */
    UINT32 fsmcalcomplete : 1;

                            /* Bits[20:20], Access Type=ROV, default=0x00000000*/

                            /* when the caliberation is done */
    UINT32 refpiclkdelayfsm : 6;

                            /* Bits[26:21], Access Type=ROV, default=0x00000000*/

                            /*
                               Load FSM with a code at which , the transition
                               is detected.
                            */
    UINT32 fsmcaldone : 1;

                            /* Bits[27:27], Access Type=ROV, default=0x00000000*/

                            /* when the caliberation of voc or x-over is done */
    UINT32 fsmtimersel : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* Timer is set for either 8 cycles or 16 cycles */
    UINT32 fsmpatternsel : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               It selects whther the pattern of three bit or
                               two bit is selected when the transition happen
                               from 1-->0 for pdout and receiver , to avoid
                               glitch
                            */
    UINT32 fsmvoccal : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               This bit tells , wether the xover or voc
                               caliberation will be done, if its zero x-over
                               caliberation is done and vice-versa
                            */
    UINT32 fsmcalen : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* FsmCalEn starts the voc-Xover Fsm */

  } Bits;
  UINT32 Data;

} DDRCRFSMCAL_FNV_DDRIO_DAT_STRUCT;

/* DATACONTROL4_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b62cc)                                                        */
/* Register default value on BWV: 0x801F0000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* static configurations of tx and rx. Broadcast reads are not supported.
*/


#define DATACONTROL4_FNV_DDRIO_DAT_REG 0x1B0402CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vsshiorvrefctl1 : 8;

                            /* Bits[8:1], Access Type=RW, default=0x00000000*/

                            /*
                               VssHi control bits[br] Field Description: [br]
                               [br] Field Bits Description[br] ------- ----
                               ---------------------------------[br]
                               disoverflow 24 Disable overflow logic from
                               BDW[br] fast panic enable 25 Bypass 2 flops in
                               panic path for faster panic response[br]
                               vsshivccd2by3cfgen 26 Forces the driver DAC to
                               get supply from 2/3 vccd[br] vsshivccd2by3cfg
                               27:29 Current trim setting for 2/3 vccd mode,
                               001 fast, 011 typical, 111 slow silicon [br]
                               ddrvssh2ndcompen 30 Enable second comparator
                               (ZBB mode)[br] comp high bwen 31 Improve
                               comparator performance to support low digital
                               voltage range
                            */
    UINT32 dqtxeqsel : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* DQ TxEq setting selection to achieve constant EQ */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vrefpwrsave : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               If set to 0 (default), Vref generators selected
                               through RxVrefSel[3:0] are always on.[br] If set
                               to 1, Vref generators are switched off while in
                               Self Refresh.
                            */
    UINT32 vreftonblbus : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000008*/

                            /* Per-bit Enable connection to share Vref Bus */
    UINT32 rxvrefsel : 4;

                            /* Bits[20:17], Access Type=RW, default=0x0000000F*/

                            /* Per Bit Verf generator Enable */
    UINT32 refpiclkdelay : 6;

                            /* Bits[26:21], Access Type=RW, default=0x00000000*/

                            /* RefPiClkDelay */
    UINT32 fnvcrsdlbypassen : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* FNV Slave Delay Line Bypass Enable */
    UINT32 fnvcrdllbypassen : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* FNV DLL Bypass Enable */
    UINT32 fnvcrvisaen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               FNV Visa Fub Enable - data fub used for VISA if
                               asserted
                            */
    UINT32 writecrcenable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Enable write crc mode */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DATACONTROL4_FNV_DDRIO_DAT_STRUCT;

/* COMPDATA0_FNV_DDRIO_DAT_REG supported on:                                    */
/*      BWV (0x401b62d4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data Comp Values. Broadcast reads are not supported.
*/


#define COMPDATA0_FNV_DDRIO_DAT_REG 0x1B0402D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcompdrvup : 6;

                            /* Bits[5:0], Access Type=RWV, default=0x00000000*/

                            /*
                               Data Rcomp DrvUp Comp Value can be overriden via
                               message channel if periodical retrain is disable
                            */
    UINT32 spare0 : 3;

                            /* Bits[8:6], Access Type=RWV, default=0x00000000*/

                            /* Spare */
    UINT32 rcompdrvdown : 6;

                            /* Bits[14:9], Access Type=RWV, default=0x00000000*/

                            /* Data Rcomp DrvDown Comp Value */
    UINT32 vtcomp : 5;

                            /* Bits[19:15], Access Type=RWV, default=0x00000000*/

                            /* VT Comp Values */
    UINT32 tcocomp : 7;

                            /* Bits[26:20], Access Type=RWV, default=0x00000000*/

                            /* Data TCO Comp Value. */
    UINT32 slewratecomp : 5;

                            /* Bits[31:27], Access Type=RWV, default=0x00000000*/

                            /* Data Slew Rate Comp Value. */

  } Bits;
  UINT32 Data;

} COMPDATA0_FNV_DDRIO_DAT_STRUCT;

/* COMPDATA1_FNV_DDRIO_DAT_REG supported on:                                    */
/*      BWV (0x401b62d8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data Comp Values. Broadcast reads are not supported.
*/


#define COMPDATA1_FNV_DDRIO_DAT_REG 0x1B0402D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcompodtup : 6;

                            /* Bits[5:0], Access Type=RWV, default=0x00000000*/

                            /* Data Rcomp OdtUp Comp Value */
    UINT32 rcompodtdown : 6;

                            /* Bits[11:6], Access Type=RWV, default=0x00000000*/

                            /* Data Rcomp Odt-Down Comp Value */
    UINT32 panicdrvdn : 10;

                            /* Bits[21:12], Access Type=RWV, default=0x00000000*/

                            /* Panic DrvDn RComp Value. */
    UINT32 panicdrvup : 10;

                            /* Bits[31:22], Access Type=RWV, default=0x00000000*/

                            /* Panic DrvUp RComp Comp Value. */

  } Bits;
  UINT32 Data;

} COMPDATA1_FNV_DDRIO_DAT_STRUCT;

/* DATAOFFSETTRAIN_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b62dc)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data offsets to training values. Broadcast reads are not supported.
*/


#define DATAOFFSETTRAIN_FNV_DDRIO_DAT_REG 0x1B0402DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvenoffset : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /* Offset added to Trained RxRcvEn Value. */
    UINT32 rxdqsoffset : 7;

                            /* Bits[12:6], Access Type=RW, default=0x00000000*/

                            /* Offset added to Trained Rx DQS Value. */
    UINT32 txdqoffset : 6;

                            /* Bits[18:13], Access Type=RW, default=0x00000000*/

                            /* Offset added to Trained Tx DQ Value. */
    UINT32 txdqsoffset : 6;

                            /* Bits[24:19], Access Type=RW, default=0x00000000*/

                            /* Ooffset added to Trained Tx DQS Value. */
    UINT32 vrefoffset : 7;

                            /* Bits[31:25], Access Type=RW, default=0x00000000*/

                            /*
                               2s Compliment offset added to Trained Vref
                               Value. Positive number increases Vref, and each
                               step is Vdd/384.
                            */

  } Bits;
  UINT32 Data;

} DATAOFFSETTRAIN_FNV_DDRIO_DAT_STRUCT;


/* DATACONTROL0_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b62e8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data offsets to training values. Broadcast reads are not supported.
*/


#define DATACONTROL0_FNV_DDRIO_DAT_REG 0x1B0402E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxtrainingmode : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enables Read DqDqs Training Mode */
    UINT32 wltrainingmode : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enables Write Leveling Training Mode */
    UINT32 rltrainingmode : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enables Read Leveling Training Mode. */
    UINT32 senseamptrainingmode : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Enables SenseAmp offset cancellation training
                               mode.
                            */
    UINT32 txon : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When set, forces DQ/DQS drive enable to active.
                               Used in power measurements and IO loopback mode
                            */
    UINT32 rfon : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down Disable: Forces register file read,
                               overriding the rank power down logic.
                            */
    UINT32 rxpion : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Power-down Disable: Forces RxDqs PI clocks on. */
    UINT32 txpion : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down Disable: Forces TxDq and TxDqs PI
                               clocks on.
                            */
    UINT32 internalclockson : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Forces on master DLL and all PI enables ON,
                               despite both channel in SelfRefresh, etc. low
                               power states.
                            */
    UINT32 saoffsettraintxon : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* SaOffsetTrainTxon */
    UINT32 txdisable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down: All write commands are ignored for
                               this data-byte. Used in single-channel MCI mode.
                            */
    UINT32 rxdisable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Power-down: All read commands are ignored for
                               this data-byte. Used in single-channel MCI mode.
                            */
    UINT32 txlong : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Sets the duration of the first transmitted DQ
                               bit of the burst. 0=1UI, 1=2UI.
                            */
    UINT32 rsvd_13 : 5;

                            /* Bits[17:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 driversegmentenable : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Controls which segements of the driver are
                               enabled: {0: All, 1: Only ODT}.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 readrfrd : 1;

                            /* Bits[20:20], Access Type=RWV, default=0x00000000*/

                            /*
                               Self clearing command bit. When set, it will
                               download the three 32 bit lines from the RegFile
                               associated with a read command to Rank =
                               ReadRFRank
                            */
    UINT32 readrfwr : 1;

                            /* Bits[21:21], Access Type=RWV, default=0x00000000*/

                            /*
                               Self clearing command bit. When set, it will
                               download the three 32 bit lines from the RegFile
                               associated with a write command to Rank =
                               ReadRFRank
                            */
    UINT32 readrfrank : 3;

                            /* Bits[24:22], Access Type=RWV, default=0x00000000*/

                            /*
                               Specifies the rank that ReadRFRd or ReadRFWr
                               will manually download the CR values from
                            */
    UINT32 forceodton : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* ODT is forced-on. */
    UINT32 odtsampoff : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* ODT and Senseamp are forced-off. */
    UINT32 disableodtstatic : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* not used */
    UINT32 ddrcrforceodton : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               force ODT to the transmitter on, regardless of
                               state of DriveEnable
                            */
    UINT32 spare : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* spare */
    UINT32 xovercal : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Enables Xover Calibration Training Mode */
    UINT32 longpreambleenable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Enables long preamble for DDR4 devices */

  } Bits;
  UINT32 Data;

} DATACONTROL0_FNV_DDRIO_DAT_STRUCT;

/* DATACONTROL1_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b62ec)                                                        */
/* Register default value on BWV: 0x0000003C                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data offsets to training values. Broadcast reads are not supported.
*/


#define DATACONTROL1_FNV_DDRIO_DAT_REG 0x1B0402EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 10;

                            /* Bits[9:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxbiasctl : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the Current and BW of the Receiver: {0:
                               0.34Idll, 1: 0.66Idll, 2: 0.88Idll, 3: Idll, 4:
                               1.33Idll, 5: 1.66Idll[br] , 6: 2.00Idll, 7:
                               2.33Idll}
                            */
    UINT32 odtdelay : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000000*/

                            /*
                               Controls ODT turn-on delay. To turn on N tQCK
                               before RcvEn, program to RcvEn[8:6] N + 4.
                            */
    UINT32 odtduration : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the ODT ON duration from (Code,
                               Duration) = (0x0, 11 tQCK) to (0x7, 18 tQCK)
                            */
    UINT32 senseampdelay : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               Controls Senseamp turn-on delay. To turn on N
                               tQCK before RcvEn, program to RcvEn[8:6] N + 4.
                            */
    UINT32 senseampduration : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the SenseAmp ON duration from (Code,
                               Duration) = (0x0, 11 tQCK) to (0x7, 18 tQCK)
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DATACONTROL1_FNV_DDRIO_DAT_STRUCT;

/* DATACONTROL2_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b62f0)                                                        */
/* Register default value on BWV: 0x07800000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data offsets to training values. Broadcast reads are not supported.
*/


#define DATACONTROL2_FNV_DDRIO_DAT_REG 0x1B0402F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxvocselqnnnh : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enables SenseAmp offset cancellation training
                               mode
                            */
    UINT32 forcebiason : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Force on the internal Vref and Rx bias circuit,
                               regardless of any other power downs
                            */
    UINT32 forcerxon : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Force on the RxAmp only (as opposed to
                               OdtSampOn, which turns on both ODT and the
                               amplifier).[br] Use for overclocking support
                               where we may not be able to hit the fast exit
                               latency
                            */
    UINT32 ddr4extend : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable finer vref resolution in ddr4 */
    UINT32 cttermination : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* DDRCOMP_CR_DDRCRCOMPOVR CTTermination */
    UINT32 dfelben : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Digital far end loopback enable */
    UINT32 dfelbtxdmuxsel : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Digital far end loopback TX FIFO data mux sel */
    UINT32 spare1 : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /* Spare1 */
    UINT32 txddjcen : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000000*/

                            /* TxddjcEn */
    UINT32 dfeenable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* DfeEnable */
    UINT32 dfedlysel : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* DfeDlySel */
    UINT32 dfecode : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               DfeCode: binary code 0000 to 1000 for DFE
                               offset, mid-code code for zero offset is 0100
                            */
    UINT32 rxdqssaoffset : 5;

                            /* Bits[27:23], Access Type=RW, default=0x0000000F*/

                            /* strobe Rx Amp offset control */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DATACONTROL2_FNV_DDRIO_DAT_STRUCT;

/* DATACONTROL3_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b62f4)                                                        */
/* Register default value on BWV: 0x00000FCF                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* static configurations of tx and rx. Broadcast reads are not supported.
*/


#define DATACONTROL3_FNV_DDRIO_DAT_REG 0x1B0402F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 drvstaticlegcfg : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /*
                               binary default setting for static legs in DRV
                               mode[br] 0 0 0 legs (EQ disabled)[br] 0 1 3 legs
                               (EQ max 3 legs)[br] 1 0 6 legs (EQ max 6
                               legs)[br] 1 1 12 legs (EQ max 12 legs)[br] In
                               most cases, we would set this to be 11, but for
                               some configurations that require a higher [br]
                               Ron value, we need to reduce the number of
                               static legs enabled so that fast corner can hit
                               [br] the higher Ron target value.
                            */
    UINT32 odtstaticlegcfg : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000003*/

                            /*
                               binary default setting for static legs in ODT
                               mode[br] 0 0 0 legs (EQ disabled)[br] 0 1 3 legs
                               (EQ max 3 legs)[br] 1 0 6 legs (EQ max 6
                               legs)[br] 1 1 12 legs (EQ max 12 legs)[br] In
                               most cases, we would set this to be 11, but for
                               some configurations that require a higher [br]
                               Ron value, we need to reduce the number of
                               static legs enabled so that fast corner can hit
                               [br] the higher Ron target value.
                            */
    UINT32 lvlshft_holden : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               level shift hold enable[br] Holds the HV control
                               values and power down mode the level
                               shifters.[br] Apply '1' after training. Slow
                               timing - not advised for fast enabling
                            */
    UINT32 rsvd_5 : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 odtsegmentenable : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000007*/

                            /*
                               This will tell which segments are enabled out of
                               reset. [br] After that, a shift register will
                               determine the enabled segments
                            */
    UINT32 datasegmentenable : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000007*/

                            /*
                               This will tell which segments are enabled out of
                               reset.
                            */
    UINT32 longodtr2w : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               When set, it will extends ODT by 1 qclk on read
                               to write turnarounds. hsd 230773
                            */
    UINT32 spare1 : 8;

                            /* Bits[20:13], Access Type=RW, default=0x00000000*/

                            /* Spare1 */
    UINT32 ddrcrctlecapen : 2;

                            /* Bits[22:21], Access Type=RW, default=0x00000000*/

                            /* ctle capacitor setting */
    UINT32 ddrcrctleresen : 2;

                            /* Bits[24:23], Access Type=RW, default=0x00000000*/

                            /* ctle resistor setting */
    UINT32 pcasbiasclosedloopen : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* RxBias CR */
    UINT32 rxbiassel : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* RxBias CR */
    UINT32 rxbiasfoldedlegtrim : 2;

                            /* Bits[28:27], Access Type=RW, default=0x00000000*/

                            /* RxBias CR */
    UINT32 rxbiasgcncomp : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /* RxBias CR */
    UINT32 postambleenable : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Spare */

  } Bits;
  UINT32 Data;

} DATACONTROL3_FNV_DDRIO_DAT_STRUCT;

/* VSSHIORVREFCONTROL_DAT_FNV_DDRIO_DAT_REG supported on:                       */
/*      BWV (0x401b62f8)                                                        */
/* Register default value on BWV: 0x004D8238                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, only. Broadcast reads are not supported.
*/


#define VSSHIORVREFCONTROL_DAT_FNV_DDRIO_DAT_REG 0x1B0402F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vsshiorvrefctl : 24;

                            /* Bits[23:0], Access Type=RW, default=0x004D8238*/

                            /*
                               Channel 0 fubs : VssHi adjust setting. Channel1
                               fubs: DIMM VREF adjust settings.[br] [br] VssHi
                               Field Description: [br] [br] Field Bits
                               Description[br] ------- ---- -------------------
                               ------------------------------------------------
                               ------------------------------------------------
                               --------------------------------------------[br]
                               SelCode 23 Selects the code to output: {0: Code,
                               1: TCode}[br] GainBoost 22 Enable gain boost is
                               panic mode by updating code/tcode [br]
                               PanicVoltage 21:18 Unsigned integer controlling
                               the voltage error on VssHi that will trigger the
                               panic driver. Step size of VccDDQ/192 (0 to
                               117mV)[br] CloseLoop 17 Enables the VssHi close
                               loop tracking [br] PanicEn 16 Enables the VssHi
                               Panic Driver[br] BWError 15:14 Limits the loop
                               gain (ie: difference between code and tcode) to
                               +/- 2^(SlowBWError)[br] OpenLoop 13 Operates
                               VssHi in open loop mode with the target Vref
                               code directly driving the output DAC with no
                               feedback involved[br] SampleDivider 12:10
                               Controls the loop bandwidth by dividing down the
                               input clock: {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3:
                               Qclk/8, 4: Qclk/16, 5: Qclk/32, 6: Qclk/64, 7:
                               Qclk/128}[br] LoBWDivider 9:8 For the slow
                               integrator, selects the net number of samples in
                               a given direction to trip the filter. {0: 4
                               samples, 1: 16 samples, 2: 32 samples, 3: 64
                               samples}[br] HiBWDivider 7:6 For the fast
                               integrator, selects the net number of samples in
                               a given direction to trip the filter. {0: 4
                               samples, 1: 16 samples, 2: 32 samples, 3: 64
                               samples}[br] Target 5:0 Unsigned integer
                               controlling the target VssHi voltage. Step size
                               of VccDDQ/192 and starting value of
                               VccDDQ*20/192
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VSSHIORVREFCONTROL_DAT_FNV_DDRIO_DAT_STRUCT;


/* RXGROUP0N1RANK0_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6300)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank0. Broadcast reads are not supported.
*/


#define RXGROUP0N1RANK0_FNV_DDRIO_DAT_REG 0x1B040300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* ReadX8ModeEn */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /* common DQS_P delay control, per nibble */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /* common DQS_N delay control, per nibble */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               Rx Equalization code: {4:3} controls the cap
                               tuning 0: (not used), 1: 200ff, 2: 400ff, 3:
                               600ff, [br] controls the resistor tuning (0: 3R,
                               1: 1R, 2: 1R, 3: R/3).
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N1RANK0_FNV_DDRIO_DAT_STRUCT;

/* RXGROUP0N1RANK1_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6304)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank0. Broadcast reads are not supported.
*/


#define RXGROUP0N1RANK1_FNV_DDRIO_DAT_REG 0x1B040304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* ReadX8ModeEn */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /* common DQS_P delay control, per nibble */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /* common DQS_N delay control, per nibble */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               Rx Equalization code: {4:3} controls the cap
                               tuning 0: (not used), 1: 200ff, 2: 400ff, 3:
                               600ff, [br] controls the resistor tuning (0: 3R,
                               1: 1R, 2: 1R, 3: R/3).
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N1RANK1_FNV_DDRIO_DAT_STRUCT;

/* RXGROUP0N1RANK2_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6308)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank0.Broadcast reads are not supported.
*/


#define RXGROUP0N1RANK2_FNV_DDRIO_DAT_REG 0x1B040308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* ReadX8ModeEn */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /* common DQS_P delay control, per nibble */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /* common DQS_N delay control, per nibble */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               Rx Equalization code: {4:3} controls the cap
                               tuning 0: (not used), 1: 200ff, 2: 400ff, 3:
                               600ff, [br] controls the resistor tuning (0: 3R,
                               1: 1R, 2: 1R, 3: R/3).
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N1RANK2_FNV_DDRIO_DAT_STRUCT;

/* RXGROUP0N1RANK3_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b630c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data RX training values for Rank0. Broadcast reads are not supported.
*/


#define RXGROUP0N1RANK3_FNV_DDRIO_DAT_REG 0x1B04030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcvendelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               RcvEn timing control. Unsigned number from 0 to
                               8 tQCK with step size of tQCK/64
                            */
    UINT32 readx8modeen : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* ReadX8ModeEn */
    UINT32 rxdqspdelay : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000000*/

                            /* common DQS_P delay control, per nibble */
    UINT32 rxdqsndelay : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /* common DQS_N delay control, per nibble */
    UINT32 rxdqdelay : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQ delay, per nibble, used in case if DQ is
                               too early than DQS
                            */
    UINT32 rxeq : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               Rx Equalization code: {4:3} controls the cap
                               tuning 0: (not used), 1: 200ff, 2: 400ff, 3:
                               600ff, [br] controls the resistor tuning (0: 3R,
                               1: 1R, 2: 1R, 3: R/3).
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXGROUP0N1RANK3_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N1RANK0_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6340)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N1RANK0_FNV_DDRIO_DAT_REG 0x1B040340

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* one of the four clocks */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N1RANK0_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N1RANK1_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6344)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N1RANK1_FNV_DDRIO_DAT_REG 0x1B040344

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* one of the four clocks */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N1RANK1_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N1RANK2_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6348)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N1RANK2_FNV_DDRIO_DAT_REG 0x1B040348

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* one of the 4 clocks */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N1RANK2_FNV_DDRIO_DAT_STRUCT;

/* TXGROUP0N1RANK3_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b634c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Timing and Voltage control for writing one data byte on Rank. Broadcast reads are not supported.
*/


#define TXGROUP0N1RANK3_FNV_DDRIO_DAT_REG 0x1B04034C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqsdelay : 9;

                            /* Bits[17:9], Access Type=RW, default=0x00000000*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 8 tQCK with step size of tQCK/64
                            */
    UINT32 txdqpiclksel : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from PiRef to PiN for bit 1 */
    UINT32 rsvd_20 : 2;

                            /* Bits[21:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqsxsel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* xelect for crossover from Qclk to CkPi4Dqs */
    UINT32 txeq : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Write Equalization codes. 5 bit control, but
                               only 24 of them are meaningful.[br] The decoding
                               is 1-1 matched, means 0 is 0 legs enabled for
                               deemphasized, 1 is 1 leg enabled[br] for
                               deemphasized, and so on.[br] BDX-change.
                               Increased the width by 1bit for TXEQ tracking
                            */
    UINT32 x8writeen : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXGROUP0N1RANK3_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN1RANK0_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6380)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN1RANK0_FNV_DDRIO_DAT_REG 0x1B040380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               x4/x8 DQS mux select[br] [0]=1 selects the raw
                               output from the nibble0 DQS amplifier in x8
                               mode; [br] [0]=0 select nibble1 amplifier output
                               in x4 mode; [br] [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN1RANK0_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN1RANK1_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6384)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN1RANK1_FNV_DDRIO_DAT_REG 0x1B040384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               x4/x8 DQS mux select[br] [0]=1 selects the raw
                               output from the nibble0 DQS amplifier in x8
                               mode; [br] [0]=0 select nibble1 amplifier output
                               in x4 mode; [br] [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN1RANK1_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN1RANK2_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b6388)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN1RANK2_FNV_DDRIO_DAT_REG 0x1B040388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               x4/x8 DQS mux select[br] [0]=1 selects the raw
                               output from the nibble0 DQS amplifier in x8
                               mode; [br] [0]=0 select nibble1 amplifier output
                               in x4 mode; [br] [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN1RANK2_FNV_DDRIO_DAT_STRUCT;

/* RXOFFSETN1RANK3_FNV_DDRIO_DAT_REG supported on:                              */
/*      BWV (0x401b638c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Read Per Lane Amplifier Voltage Offset. Broadcast reads are not supported.
*/


#define RXOFFSETN1RANK3_FNV_DDRIO_DAT_REG 0x1B04038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 saoffset0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[0] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[1] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset2 : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[2] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 saoffset3 : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier voltage offset for bit[3] of the DQ
                               byte. {0: Most negative offset,... 8: 0 offset,
                               ... 63: Most postive offset}
                            */
    UINT32 rxdqspsel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /*
                               x4/x8 DQS mux select[br] [0]=1 selects the raw
                               output from the nibble0 DQS amplifier in x8
                               mode; [br] [0]=0 select nibble1 amplifier output
                               in x4 mode; [br] [1] is not used.
                            */
    UINT32 rxdqsnsel : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               select the phase2 dqs clock skewed with max
                               delay to drive rxFIFO write
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXOFFSETN1RANK3_FNV_DDRIO_DAT_STRUCT;

/* RXVREFCTRLN1_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b63a4)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* RxVrefCtrl. Broadcast reads are not supported.
*/


#define RXVREFCTRLN1_FNV_DDRIO_DAT_REG 0x1B0403A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vrefperbit0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben0 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 vrefperbit1 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben1 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 vrefperbit2 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben2 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /* Rsvd */
    UINT32 vrefperbit3 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Per bit Rx Vref Control */
    UINT32 halflsben3 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* shift down the Vref voltage by ~0.5 stepsize */

  } Bits;
  UINT32 Data;

} RXVREFCTRLN1_FNV_DDRIO_DAT_STRUCT;

/* DATATRAINFEEDBACKN1_FNV_DDRIO_DAT_REG supported on:                          */
/*      BWV (0x401b63a8)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Data Training Feedback Results. Broadcast reads are not supported.
*/


#define DATATRAINFEEDBACKN1_FNV_DDRIO_DAT_REG 0x1B0403A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datatrainfeedback : 9;

                            /* Bits[8:0], Access Type=RW, default=0x00000000*/

                            /*
                               Data Training Feedback Storage [br] Training
                               Step Bits Nibble Description[br] --------------
                               ---- ------- ---------------[br] Cross Over Cal
                               0 1 Tx Dqs Phase Detection[br] Cross Over Cal 1
                               1 Tx Dq Phase Detection[br] Cross Over Cal 2 1
                               Rec En Phase Detection[br] RcvEn 8:0 1 Byte
                               detection[br] Wr Leveling 8:0 1 Byte
                               detection[br] Senseamp offset 3:0 1 Dq Senseamp
                               offset Detection
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DATATRAINFEEDBACKN1_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDCOMP_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b6800)                                                        */
/* Register default value on BWV: 0x04000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Comp Values. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDCOMP_FNV_DDRIO_DAT_REG 0x1B040800

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scomp : 6;

                            /* Bits[5:0], Access Type=RWV, default=0x00000000*/

                            /*
                               Scomp Value, Normally updated by Comp FSM via
                               CompBus. can be overriden via message channel if
                               periodical retrain is disabled.
                            */
    UINT32 tcocomp : 7;

                            /* Bits[12:6], Access Type=RWV, default=0x00000000*/

                            /*
                               Tco Comp Value, Normally updated by Comp FSM via
                               CompBus. can be overriden via message channel if
                               periodical retrain is disabled.
                            */
    UINT32 rcompdrvup : 6;

                            /* Bits[18:13], Access Type=RWV, default=0x00000000*/

                            /*
                               Rcomp Drv Up Value, Normally updated by Comp FSM
                               via CompBus. can be overriden via message
                               channel if periodical retrain is disabled.
                            */
    UINT32 rcompdrvdown : 6;

                            /* Bits[24:19], Access Type=RWV, default=0x00000000*/

                            /*
                               Rcomp Drv Dn Value, Normally updated by Comp FSM
                               via CompBus. can be overriden via message
                               channel if periodical retrain is disabled.
                            */
    UINT32 lscomp : 3;

                            /* Bits[27:25], Access Type=RWV, default=0x00000002*/

                            /* not used */
    UINT32 spare : 4;

                            /* Bits[31:28], Access Type=RWV, default=0x00000000*/

                            /* Spare */

  } Bits;
  UINT32 Data;

} DDRCRCMDCOMP_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDCOMPOFFSET_FNV_DDRIO_DAT_REG supported on:                           */
/*      BWV (0x401b6804)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Comp Offset Values. Multicast read for CMDCTL_CH012 and CMDCTL_CH345 versions are not supported.
*/


#define DDRCRCMDCOMPOFFSET_FNV_DDRIO_DAT_REG 0x1B040804

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 17;

                            /* Bits[16:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrcmdctlrx_en : 12;

                            /* Bits[28:17], Access Type=RW, default=0x00000000*/

                            /* Enable DDRT Parity in CMDCTL */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCMDCOMPOFFSET_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDPICODING_FNV_DDRIO_DAT_REG supported on:                             */
/*      BWV (0x401b6808)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Pi Settings. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDPICODING_FNV_DDRIO_DAT_REG 0x1B040808

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmdpicode0 : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay0 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay - add 1 qclk cycle delay. */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmdpicode1 : 6;

                            /* Bits[14:9], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay1 : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmdpicode2 : 6;

                            /* Bits[23:18], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay2 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCMDPICODING_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDCONTROLS_FNV_DDRIO_DAT_REG supported on:                             */
/*      BWV (0x401b680c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Controls. Multicast read for CMDCTL_CH012 and CMDCTL_CH345 versions are not supported.
*/


#define DDRCRCMDCONTROLS_FNV_DDRIO_DAT_REG 0x1B04080C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 visabusenable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable Visa debug bus LCB for CMD fub */
    UINT32 pirefclkdivenable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* enable PiRef clock divider in xover */
    UINT32 xoveropmode : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Xover Select between grid-PiRef and PiRef-PiN */
    UINT32 dllmask : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               On weak lock exit, controls how long the
                               feedback to the phase detector is masked: {0: 1
                               qclk (required for delayline latency), 1: 2
                               qclk, 2: 3 qclk, 3: 4 qclk}
                            */
    UINT32 drvpupdis : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               This will be connected to all tx, and must
                               toggle 1->0 after a power state
                            */
    UINT32 txon : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Forces the CMD transmitter on, overriding any
                               power downs
                            */
    UINT32 intclkon : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Forces on all PI enables and the LCB enables in
                               the AFE fubs. Does not force on D0 PI enable or
                               override weaklock mode.
                            */
    UINT32 repclkon : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* not used */
    UINT32 iolbctl : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /*
                               Controls IO Loopback. Ctl[1] enables IOLB mode
                               and Ctl[0] controls if this is a Setup(0) or
                               Hold(1) test
                            */
    UINT32 odtmode : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Forces the driver in ODT mode, where both the
                               pullup and pulldown are enabled. Used for Rstray
                               testing
                            */
    UINT32 cmdtxeq : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /* not used */
    UINT32 rsvd_15 : 2;

                            /* Bits[16:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lvmode : 2;

                            /* Bits[18:17], Access Type=RW, default=0x00000000*/

                            /* not used */
    UINT32 lvmodevalid : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* not used */
    UINT32 lvlshft_holden : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               level shift hold enable[br] Holds the HV control
                               values and power down mode the level
                               shifters.[br] Apply '1' after training. Slow
                               timing - not advised for fast enabling
                            */
    UINT32 rxvref : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000000*/

                            /*
                               IOLB Vref Control. 2s compliment encoding from
                               1/3 Vdd to 2/3 Vdd with step size of Vdd/192
                               (7.8mV)
                            */
    UINT32 halflsbbit : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               enables fine grain Vref control, shift down the
                               Vref voltage by ~0.5 stepsize
                            */
    UINT32 vrefdisable : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Vref Disable */
    UINT32 ddr4modeenable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Ddr4 Mode Enable, set to 1 in CMD2
                               (Func:5.Addr:C00 and Func:7.Addr:C00) fub to
                               bypass extra delay on parity bit
                            */
    UINT32 xovercal : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Enables Xover Calibration Training Mode */

  } Bits;
  UINT32 Data;

} DDRCRCMDCONTROLS_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDCONTROLS2_FNV_DDRIO_DAT_REG supported on:                            */
/*      BWV (0x401b6810)                                                        */
/* Register default value on BWV: 0x01400000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Controls. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDCONTROLS2_FNV_DDRIO_DAT_REG 0x1B040810

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 20;

                            /* Bits[19:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fnvddrcrdllpibypassen : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* FNV DDR/PI Bypass Enable */
    UINT32 ddr4extend : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enable finer vref resolution in ddr4 */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmdrxbiassel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /* CmdRxBiasSel for Cmd Receiver */
    UINT32 forcecmdbiason : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /* ForceCmdBiason for Cmd Receiver */
    UINT32 fnvcmdcrxoverbypassen : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* bypass ddrcmdctl xover for FNV */
    UINT32 cmdctlecapen : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /* CmdCtl Buffer CtleCapEn */
    UINT32 cmdctleresen : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /* CmdCtl Buffer CtleResEn */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCMDCONTROLS2_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCTLCOMP_FNV_DDRIO_DAT_REG supported on:                                 */
/*      BWV (0x401b6814)                                                        */
/* Register default value on BWV: 0x04000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CTL Comp Values. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCTLCOMP_FNV_DDRIO_DAT_REG 0x1B040814

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scomp : 6;

                            /* Bits[5:0], Access Type=RWV, default=0x00000000*/

                            /* Scomp Value */
    UINT32 tcocomp : 7;

                            /* Bits[12:6], Access Type=RWV, default=0x00000000*/

                            /* Tco Comp Value */
    UINT32 rcompdrvup : 6;

                            /* Bits[18:13], Access Type=RWV, default=0x00000000*/

                            /* Rcomp Drv Up Value */
    UINT32 rcompdrvdown : 6;

                            /* Bits[24:19], Access Type=RWV, default=0x00000000*/

                            /* Rcomp Drv Dn Value */
    UINT32 lscomp : 3;

                            /* Bits[27:25], Access Type=RWV, default=0x00000002*/

                            /* LevelShift Comp Value */
    UINT32 spare : 4;

                            /* Bits[31:28], Access Type=RWV, default=0x00000000*/

                            /* Spare */

  } Bits;
  UINT32 Data;

} DDRCRCTLCOMP_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCTLCOMPOFFSET_FNV_DDRIO_DAT_REG supported on:                           */
/*      BWV (0x401b6818)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CTL Comp Offset Values. Multicast read for CMDCTL_CH012 and CMDCTL_CH345 versions are not supported.
*/


#define DDRCRCTLCOMPOFFSET_FNV_DDRIO_DAT_REG 0x1B040818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 18;

                            /* Bits[17:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmdctlbufsel : 6;

                            /* Bits[23:18], Access Type=RW, default=0x00000000*/

                            /* CmdBuf or CtlBuf Selection */
    UINT32 iodirectionvalid : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* Is the Direction (RX/TX Valid) */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCTLCOMPOFFSET_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDCONTROLS3_FNV_DDRIO_DAT_REG supported on:                            */
/*      BWV (0x401b681c)                                                        */
/* Register default value on BWV: 0x0000007F                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Controls 3. Multicast read for CMDCTL_CH012 and CMDCTL_CH345 versions are not supported.
*/


#define DDRCRCMDCONTROLS3_FNV_DDRIO_DAT_REG 0x1B04081C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bufdrvsegen : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001F*/

                            /* Drive strength enable for CmdBuf/CtlBuf. */
    UINT32 cmdvrefpgen : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               control the DdrVrefSelDQQnnnH pin on the
                               ddrvrefint generator
                            */
    UINT32 odtstrength : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /* OdtStrength */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clkpibypass : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Clk Pi Bypass for DIV2 */
    UINT32 divby2alignctl : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Invert DAlign */
    UINT32 divby2disable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* select qclk for cmdxover instead of Dclk */
    UINT32 odten : 12;

                            /* Bits[22:11], Access Type=RW, default=0x00000000*/

                            /* ODTDis for Cmd and Ctl buf */
    UINT32 cmdrefpiclkdelay : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /* CmdRefClk */
    UINT32 cmdbypass : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Contols to bypass entire XOVER CMD */
    UINT32 drvencrovrd : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               Override for DriveEn-controlled delayed command
                               bit. [br] bit30 -> set to 1 in order to let
                               bit31 control the path (0 lets DriveEn control
                               it)[br] bit31 -> set to 1 for delayed command
                               bit, 0 for un-delayed command bit (from xover to
                               AFE)
                            */

  } Bits;
  UINT32 Data;

} DDRCRCMDCONTROLS3_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCTLRANKSUSED_FNV_DDRIO_DAT_REG supported on:                            */
/*      BWV (0x401b6820)                                                        */
/* Register default value on BWV: 0x0000003F                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CTL Ranks Used. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCTLRANKSUSED_FNV_DDRIO_DAT_REG 0x1B040820

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ranken : 6;

                            /* Bits[5:0], Access Type=RW, default=0x0000003F*/

                            /*
                               Enables output buffers for ctlbufpair 0 to
                               ctlbufpair6 , PI clocks and output datapaths for
                               this rank
                            */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lvlshft_holden : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               level shift hold enable[br] Holds the HV control
                               values and power down mode the level
                               shifters.[br] Apply '1' after training. Slow
                               timing - not advised for fast enabling
                            */
    UINT32 rxbiasgcncomp : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /*
                               used to compensate the GCN skew. The GCN skew
                               information should be[br] extracted by reading
                               and analyzing the final Rcomp code value. The
                               target spec[br] is to categorize the GCN skew in
                               3 process condition (mainly slow, fast,
                               typical).[br] (one example can be just reading
                               the 2 MLS bit of Rcomp code and determine if
                               the[br] GCN is slow, fast or typical). The
                               gcncomp is used for CTLE.[br] 11: gcn in slow
                               corner[br] 10: gcn in typical corner[br] 01: gcn
                               in fast corner
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 imodebiashighcm : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* Imode bias control */
    UINT32 rxbiassel : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* Rx bias Sel */
    UINT32 cmdrxeqqnnnl : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /* CmdRxEqQnnnL for RX CmdCtl Buf */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrcmdrxbiasctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* CmdRxBiasCtl for RX biasgen */
    UINT32 cmdpcasbiasclosedloopen : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* CmdPcasBiasClosedLoopEnQnnnH for RX biasgen */
    UINT32 ddrcmdrxbiasfoldedlegtrim : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /* DdrCmdRxBiasFoldedLegTrim for RX biasgen */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCTLRANKSUSED_FNV_DDRIO_DAT_STRUCT;



/* DDRCRCMDTRAINING_FNV_DDRIO_DAT_REG supported on:                             */
/*      BWV (0x401b682c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* Cmd Training Register. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDTRAINING_FNV_DDRIO_DAT_REG 0x1B04082C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 trainingoffset : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /* Offset added to Trained PiCode Value. */
    UINT32 spare0 : 2;

                            /* Bits[7:6], Access Type=WO, default=0x00000000*/

                            /* Spare */
    UINT32 xoverphasedet : 14;

                            /* Bits[21:8], Access Type=ROV, default=0x00000000*/

                            /*
                               bits13:6 unused[br] bit5 output of phase
                               detector for I/O Pair 0[br] bit4 output of phase
                               detector for I/O Pair 1[br] bit3 output of phase
                               detector for I/O Pair 2[br] bit2 output of phase
                               detector for I/O Pair 3[br] bit1 output of phase
                               detector for I/O Pair 4[br] bit0 output of phase
                               detector for I/O Pair 5 (only on cmdctla FUBs)
                            */
    UINT32 trainingoffset2 : 6;

                            /* Bits[27:22], Access Type=RW, default=0x00000000*/

                            /*
                               Offset added to PiCode. Obtained via xover
                               calibration such that ClkPi aligns with grid
                               clock when PiCode=0.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCMDTRAINING_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDCONTROLS1_FNV_DDRIO_DAT_REG supported on:                            */
/*      BWV (0x401b6830)                                                        */
/* Register default value on BWV: 0x03C01000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Controls 1. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDCONTROLS1_FNV_DDRIO_DAT_REG 0x1B040830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmdoutputensel : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* CmdOutputEnable */
    UINT32 selectcompcode : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /* Select CompCode between cmd or ctl */
    UINT32 rsvd_13 : 9;

                            /* Bits[21:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqstatdflt : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000003*/

                            /* Static leg enable */
    UINT32 odtstatdflt : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000003*/

                            /* Static leg enable */
    UINT32 spare2 : 6;

                            /* Bits[31:26], Access Type=RW, default=0x00000000*/

                            /* Spare2 */

  } Bits;
  UINT32 Data;

} DDRCRCMDCONTROLS1_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDPICODING2_FNV_DDRIO_DAT_REG supported on:                            */
/*      BWV (0x401b6834)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Pi Settings. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDPICODING2_FNV_DDRIO_DAT_REG 0x1B040834

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmdpicode3 : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay3 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmdpicode4 : 6;

                            /* Bits[14:9], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay4 : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmdpicode5 : 6;

                            /* Bits[23:18], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay5 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCMDPICODING2_FNV_DDRIO_DAT_STRUCT;

/* DDRCRCMDPICODING3_FNV_DDRIO_DAT_REG supported on:                            */
/*      BWV (0x401b6838)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* CMD Pi Settings. Multicast read for CH012 and CH345 versions are not supported.
*/


#define DDRCRCMDPICODING3_FNV_DDRIO_DAT_REG 0x1B040838

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmdpicode6 : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Cmd Pi Code[br] [5:0] - Pi setting with [5]
                               being the PiMSB sel
                            */
    UINT32 cmdpilogicdelay6 : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /* Cmd Pi Logic Delay */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCRCMDPICODING3_FNV_DDRIO_DAT_STRUCT;


/* VSSHIORVREFCONTROL_CMD_FNV_DDRIO_DAT_REG supported on:                       */
/*      BWV (0x401b6840)                                                        */
/* Register default value on BWV: 0x004D8238                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, only. Multicast read for CH012 and CH345 versions are not supported.
*/


#define VSSHIORVREFCONTROL_CMD_FNV_DDRIO_DAT_REG 0x1B040840

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vsshiorvrefctl : 24;

                            /* Bits[23:0], Access Type=RW, default=0x004D8238*/

                            /*
                               Channel 0 fubs : VssHi adjust setting. Channel1
                               fubs: DIMM VREF adjust settings.[br] [br] VssHi
                               Field Description:[br] [br] Field Bits
                               Description[br] ------- ---- -------------------
                               ------------------------------------------------
                               ------------------------------------------------
                               --------------------------------------------[br]
                               SelCode 23 Selects the code to output: {0: Code,
                               1: TCode}[br] GainBoost 22 Enable gain boost is
                               panic mode by updating code/tcode[br]
                               PanicVoltage 21:18 Unsigned integer controlling
                               the voltage error on VssHi that will trigger the
                               panic driver. Step size of VccDDQ/192 (0 to
                               117mV)[br] CloseLoop 17 Enables the VssHi close
                               loop tracking[br] PanicEn 16 Enables the VssHi
                               Panic Driver[br] BWError 15:14 Limits the loop
                               gain (ie: difference between code and tcode) to
                               +/- 2^(SlowBWError)[br] OpenLoop 13 Operates
                               VssHi in open loop mode with the target Vref
                               code directly driving the output DAC with no
                               feedback involved[br] SampleDivider 12:10
                               Controls the loop bandwidth by dividing down the
                               input clock: {0: Qclk, 1: Qclk/2, 2: Qclk/4, 3:
                               Qclk/8, 4: Qclk/16, 5: Qclk/32, 6: Qclk/64, 7:
                               Qclk/128}[br] LoBWDivider 9:8 For the slow
                               integrator, selects the net number of samples in
                               a given direction to trip the filter. {0: 4
                               samples, 1: 16 samples, 2: 32 samples, 3: 64
                               samples}[br] HiBWDivider 7:6 For the fast
                               integrator, selects the net number of samples in
                               a given direction to trip the filter. {0: 4
                               samples, 1: 16 samples, 2: 32 samples, 3: 64
                               samples}[br] Target 5:0 Unsigned integer
                               controlling the target VssHi voltage. Step size
                               of VccDDQ/192 and starting value of
                               VccDDQ*20/192
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VSSHIORVREFCONTROL_CMD_FNV_DDRIO_DAT_STRUCT;


/* VSSHIORVREFCONTROL1_FNV_DDRIO_DAT_REG supported on:                          */
/*      BWV (0x401b6848)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    1_22_6_BWV                                             */
/* Struct generated from BWV BDF: 1_22_6                                        */
/* BWV Security PolicyGroup:                                                    */
/* VssHi Controls on Channel 0 only. DIMM Vref Controls on  Channel1, databyte 7, only. Extension for BDX. Multicast read for CH012 and CH345 versions are not supported.
*/


#define VSSHIORVREFCONTROL1_FNV_DDRIO_DAT_REG 0x1B040848

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vsshiorvrefctl : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* BDX-specific extended control signal. */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VSSHIORVREFCONTROL1_FNV_DDRIO_DAT_STRUCT;
#endif /* _FNV_DDRIO_DAT_h */
