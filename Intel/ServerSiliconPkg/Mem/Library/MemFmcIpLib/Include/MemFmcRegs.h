/** @file
  MemFmcRegs.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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
**/

#ifndef _FMC_REGS_H_
#define _FMC_REGS_H_

#ifdef DDR5_SUPPORT
#include "Registers/Cwv/FmcIpRegisters.h"
#else
#include "Registers/Fnv/FmcIpRegisters.h"

#ifndef SKX_HOST

// BWV header aliases
#define   DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT    DDRCRCMDCONTROLS_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_STRUCT   DDRCRCMDCONTROLS2_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_STRUCT   DDRCRCMDCONTROLS3_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_STRUCT    DDRCRCMDTRAINING_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_STRUCT   DDRCRCTLRANKSUSED_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_STRUCT  DDRCRCMDPICODING_3_FNV_DDRIO_DAT7_CH_STRUCT
#define   DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_STRUCT DDRCRCMDPICODING2_3_FNV_DDRIO_DAT7_CH_STRUCT

#define   DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG   DDRCRCMDCONTROLS_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG  DDRCRCMDCONTROLS2_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG  DDRCRCMDCONTROLS3_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_REG   DDRCRCMDTRAINING_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_REG  DDRCRCTLRANKSUSED_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDPICODING_SA_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDPICODING_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDPICODING2_SA_FNV_DDRIO_DAT7_CH_REG    DDRCRCMDPICODING2_3_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_1_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS_NB_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_0_FNV_DDRIO_DAT7_CH_REG
#define   DDRCRCMDCONTROLS_SB_FNV_DDRIO_DAT7_CH_REG     DDRCRCMDCONTROLS_2_FNV_DDRIO_DAT7_CH_REG

#endif // !SKX_HOST
#endif //DDR5_SUPPORT
//
// Legacy EKV only register that does not exist on BWV header the code has to conditionally use this register
//

/* DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_REG supported on:                    */
/*       EKV (0x401B1A10)                                                     */
/* Register default value:              0x00066000                            */
#define DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_REG 0x1B0F4A10
/* Struct format extracted from XML file EKV\1.22.1.EKV.xml.
 * CLK Controls. Multicast read for CH012 and CH345 versions are not supported.
 */
typedef union {
  struct {
    UINT32 ddrphasextalkenableqnnnh : 1;
    /* ddrphasextalkenableqnnnh - Bits[0:0], RW, default = 1'b0 
       When Phased based xtalk cancellation is enabled, set this to delay cmd to 
       maintain constant Tcwl. 
     */
    UINT32 visabusenable : 1;
    /* visabusenable - Bits[1:1], RW, default = 1'h0 
       Enable Visa debug bus LCB for CLK fub
     */
    UINT32 pirefclkdivenable : 1;
    /* pirefclkdivenable - Bits[2:2], RW, default = 1'h0 
       enable PiRef clock divider in xover
     */
    UINT32 xoveropmode : 1;
    /* xoveropmode - Bits[3:3], RW, default = 1'h0 
       Xover Select between grid-PiRef and PiRef-PiN
     */
    UINT32 clkodtpdnsegen : 3;
    /* clkodtpdnsegen - Bits[6:4], RW, default = 3'b0 
       not used
     */
    UINT32 txon : 1;
    /* txon - Bits[7:7], RW, default = 1'b0 
       Forces the CLK transmitter on, overriding any power downs
     */
    UINT32 intclkon : 1;
    /* intclkon - Bits[8:8], RW, default = 1'b0 
       Forces on all PI enables and the LCB enables in the AFE fubs.  Does not force on 
       D0 PI enable or override weaklock mode. 
     */
    UINT32 repclkon : 1;
    /* repclkon - Bits[9:9], RW, default = 1'b0 
       not used
     */
    UINT32 iolbctl : 2;
    /* iolbctl - Bits[11:10], RW, default = 2'b0 
       Controls IO Loopback.  Ctl[1] enables IOLB mode and Ctl[0] controls if this is a 
       Setup(0) or Hold(1) test 
     */
    UINT32 odtmode : 1;
    /* odtmode - Bits[12:12], RW, default = 1'b0 
       Forces the driver in ODT mode, where both the pullup and pulldown are enabled.  
       Used for Rstray testing 
     */
    UINT32 odtpupsegen : 3;
    /* odtpupsegen - Bits[15:13], RW, default = 3'b011 
       not used
     */
    UINT32 is_rdimm : 1;
    /* is_rdimm - Bits[16:16], RW, default = 1'h0 
       set to drive DC on xxCLK during P6 and S3 if DDR3 + RDIMM is populated
     */
    UINT32 statlegen : 2;
    /* statlegen - Bits[18:17], RW, default = 2'b11 
       static leg enable
     */
    UINT32 ddr4extend : 1;
    /* ddr4extend - Bits[19:19], RW, default = 1'h0 
       Enable finer vref resolution in ddr4
     */
    UINT32 cttermination : 1;
    /* cttermination - Bits[20:20], RW, default = 1'h0 
       1: Center Tap Termination for DDR3; 0: Vddq Termination for DDR4/Intel SMI 2
     */
    UINT32 rxvref : 7;
    /* rxvref - Bits[27:21], RW, default = 7'b0 
       IOLB Vref Control.  2s compliment encoding from 1/3 Vdd to 2/3 Vdd with step 
       size of Vdd/192 (7.8mV) 
     */
    UINT32 halflsbbit : 1;
    /* halflsbbit - Bits[28:28], RW, default = 1'b0 
       enables fine grain Vref control, shift down the Vref voltage by ~0.5 stepsize
     */
    UINT32 rsvd_29 : 1;
    UINT32 lvlshft_holden : 1;
    /* lvlshft_holden - Bits[30:30], RW, default = 1'b0 
       level shift hold enable:[br]
                            Holds the HV control values and power down the level 
       shifters.[br] 
                            Apply '1' after training.  Slow timing - not advised for 
       fast enabling 
     */
    UINT32 xovercal : 1;
    /* xovercal - Bits[31:31], RW, default = 1'b0 
       Enables Xover Calibration Training Mode
     */
  } Bits;
  UINT32 Data;
} DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_STRUCT;

#endif //_FMC_REGS_H_
