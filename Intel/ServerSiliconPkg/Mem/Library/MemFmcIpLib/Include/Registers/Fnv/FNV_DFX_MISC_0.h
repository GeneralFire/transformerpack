
/** @file
  FNV_DFX_MISC_0.h

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


#ifndef _FNV_DFX_MISC_0_h
#define _FNV_DFX_MISC_0_h
#include "DataTypes.h"


/* VENDOR_DEVICE_ID_FNV_DFX_MISC_0_REG supported on:                            */
/*      BWV (0x4000c008)                                                        */
/* Register default value on BWV: 0x8086097B                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register contains the Vendor/Device ID
*/


#define VENDOR_DEVICE_ID_FNV_DFX_MISC_0_REG 0x1BC40008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000097B*/

                            /*
                               Device ID.[br] FNV - 0x0979[br] EKV - 0x097A[br]
                               BWV - 0x097B
                            */
    UINT32 vendor_id : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00008086*/

                            /* Vendor ID */

  } Bits;
  UINT32 Data;

} VENDOR_DEVICE_ID_FNV_DFX_MISC_0_STRUCT;

/* REVISION_MFG_ID_FNV_DFX_MISC_0_REG supported on:                             */
/*      BWV (0x4000c00c)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* MFG Stepping ID value.Revision ID control and values.
*/


#define REVISION_MFG_ID_FNV_DFX_MISC_0_REG 0x1BC4000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rev_id_sel : 2;

                            /* Bits[1:0], Access Type=RWO, default=0x00000000*/

                            /*
                               Selects RevID[br] 00 : SRID (default)[br] 01 :
                               CRID[br] NOTE: In the current implementation,
                               CRID and SRID contain the same value. Meaning
                               RevID select programming will not change RevID
                               value.[br] USER WARNING: Host reads may
                               inadvertently lock this register in an undesired
                               mode as some reads are realized as R-Ws.[br]
                               Therefore, should CRID differ from SRID at some
                               point, FW needs to lock the appropriate mode
                               before host accesses this CSR, for example in FW
                               init.[br] The mode may be determined by a
                               revenue SKU or other similar function
                            */
    UINT32 revision_id : 8;

                            /* Bits[9:2], Access Type=ROV, default=0x00000000*/

                            /*
                               OS accessible RevID selected by RevID Sel[br]
                               The 8-bit REVID is split into 2 sections:[br]
                               The following bits are Reserved, [7:6] and
                               [3:2].[br] Upper 2-bits [5:4], translates to
                               base layer stepping[br] 00 A-step[br] 01
                               S-step[br] 10 C-step[br] 11 D-step[br] Lower
                               2-bits [1:0], translates to metal layer
                               stepping[br] 00 Metal-step 0[br] 01 Metal-step
                               1[br] 10 Metal-step 2[br] 11 Metal-step 3[br]
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} REVISION_MFG_ID_FNV_DFX_MISC_0_STRUCT;












/* FNV_DEBUG_LOCK_FNV_DFX_MISC_0_REG supported on:                              */
/*      BWV (0x4000c048)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* This register controls access to DFx functionality. Mainly JTAG tunneling.
*/


#define FNV_DEBUG_LOCK_FNV_DFX_MISC_0_REG 0x1BC40048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dbg_en : 1;

                            /* Bits[0:0], Access Type=RWV, default=0x00000000*/

                            /*
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_jtag_tunneling_dbg_en[br]
                               0=Debug Disabled[br] 1=Debug Enabled[br] Note:
                               The default value is loaded from Fuses. When
                               lock is set, this field will not change until
                               next assertion of RST_N.
                            */
    UINT32 rsvd : 29;

                            /* Bits[29:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lock : 1;

                            /* Bits[30:30], Access Type=RWV, default=0x00000000*/

                            /*
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_CR_locked[br] When set,
                               locks this register from further changes until
                               next assertion of RST_N. The default value is
                               loaded from fuses[br] 0=unlocked[br]
                               1=locked[br]
                            */
    UINT32 dwe : 1;

                            /* Bits[31:31], Access Type=ROV, default=0x00000000*/

                            /*
                               HW sets this bit to indicate that Debug was
                               Enabled.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_DWE_False
                            */

  } Bits;
  UINT32 Data;

} FNV_DEBUG_LOCK_FNV_DFX_MISC_0_STRUCT;









/* MSC_TEMP_CRIT_CSR_FNV_DFX_MISC_0_REG supported on:                           */
/*      BWV (0x4000c074)                                                        */
/* Register default value on BWV: 0x000000FF                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Critical trip point.
*/


#define MSC_TEMP_CRIT_CSR_FNV_DFX_MISC_0_REG 0x1BC40074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tempcrit : 9;

                            /* Bits[8:0], Access Type=RW, default=0x000000FF*/

                            /* Critical threshold trip point. */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_CRIT_CSR_FNV_DFX_MISC_0_STRUCT;

















/* MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG supported on:                    */
/*      BWV (0x4000c180)                                                        */
/* Register default value on BWV: 0x54414269                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* FNVIO Interface Initialization 
*/


#define MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG 0x1BC40180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc2ddrttrainreset : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               DDR IO Training Reset. This should be pulsed
                               after each IO Training Stage to reload picodes
                               updates.[br] Note that the actual training reset
                               signal to fnvio is an OR function of this CSR
                               and normal reset to DDRT IO
                            */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrtclkdetectdetlev : 5;

                            /* Bits[7:3], Access Type=RW, default=0x0000000D*/

                            /*
                               Analog control: DDRT Ref. CLK Detect Lev. [br]
                               ddrtclkdetectdetlev[1:0] - Real Usage. [br]
                               ddrtclkdetectdetlev[2] - Spare Usage. [br]
                               ddrtclkdetectdetlev[4:3] - Real Usage. [br]
                               Spare Usage Criteria: [br]
                               ddrtclkdetectdetlev[2] - rxbiasadj_pq[0]
                               (default 1'b1). [br]
                            */
    UINT32 ddrtclkdetectfiltlev : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000002*/

                            /* Analog control: DDRT Ref. CLK Filter Lev */
    UINT32 ddrtrefclkbypass : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Enable bit for DDRT reference clock divider
                               bypass
                            */
    UINT32 ddrtrefclkdivratio : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000008*/

                            /*
                               DDRT Reference clock divider ratio[br] 5'h06:
                               divide by 6 - Input DClk 800 MHz[br] 5'h07:
                               divide by 7 - Input DClk 933 MHz[br] 5'h08:
                               divide by 8 - Input DClk 1067 MHz[br] 5'h09:
                               divide by 9 - Input DClk 1200 MHz[br] 5'h0A:
                               divide by 10 - Input Dclk 1333 MHz [br] *This
                               CSR needs to be programmed during init flow*[br]
                               *This CSR needs to be programmed during ddrt
                               frequency change*
                            */
    UINT32 ddrvrefgenenableqnnnh : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               set to 1 along with the other ddrtclk
                               controls(like bias and SA enable)
                            */
    UINT32 ddrrxvrefctlqnnnh : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000020*/

                            /* Default: 7'b010_0000 */
    UINT32 ddrvrefhalfupqnnnh : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* DDR VREF Half Up - Set to 0) */
    UINT32 ddrtclkdetectbiascon : 4;

                            /* Bits[28:25], Access Type=RW, default=0x0000000A*/

                            /*
                               Analog control: DDRT Ref. CLK BIAS Con. [br]
                               ddrtclkdetectbiascon[2:0] - Real Usage. [br]
                               ddrtclkdetectbiascon[3] - Spare Usage. [br]
                               Spare Usage Criteria: [br]
                               ddrtclkdetectbiascon[3] - rxbiasadj_pq[1]
                               (default 1'b1). [br]
                            */
    UINT32 ddrtclkdetecthyscon : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000002*/

                            /* Analog control: DDRT Ref. CLK HYS Con */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT;


/* MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG supported on:                    */
/*      BWV (0x4000c188)                                                        */
/* Register default value on BWV: 0x2E410805                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Connects to FNVIO Interface Initialization 
*/


#define MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG 0x1BC40188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 30;

                            /* Bits[29:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrt_resync : 1;

                            /* Bits[30:30], Access Type=RWV, default=0x00000000*/

                            /*
                               DDRT Frequency change mode is enabled On entry
                               of DDRT Frequency change, set Clock Idle Mask.
                               Clear Clock Idle Mask when new DDRT Frequencies
                               are correctly programmed
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_STRUCT;



/* MSC_NGN_PLL_CTRL1_FNV_DFX_MISC_0_REG supported on:                           */
/*      BWV (0x4000c194)                                                        */
/* Register default value on BWV: 0x0000000A                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* Current NGN feedback divider ratio
	    7'h08 : NGN CLK frequency is 533 Mhz
	    7'h0A : NGN CLK frequency is 667 Mhz
	    7'h0B : NGN CLK frequency is 733 Mhz
*/


#define MSC_NGN_PLL_CTRL1_FNV_DFX_MISC_0_REG 0x1BC40194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 freqcur : 7;

                            /* Bits[6:0], Access Type=RWV, default=0x0000000A*/

                            /* Feedback divider ratio value */
    UINT32 rsvd_7 : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_NGN_PLL_CTRL1_FNV_DFX_MISC_0_STRUCT;

/* MSC_PLL_CTRL_FNV_DFX_MISC_0_REG supported on:                                */
/*      BWV (0x4000c198)                                                        */
/* Register default value on BWV: 0xE0003000                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* NGN/DDRT PLL Control Register
*/


#define MSC_PLL_CTRL_FNV_DFX_MISC_0_REG 0x1BC40198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 daunit_pmreset_mask : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               When set: RFSM will reset DAUNIT in DDRT PM
                               Modes [br] If this bit is cleared(set to zero),
                               then da_ckgate_ctrl.dclk_gate_dis must be set to
                               0.
                            */
    UINT32 ddrtio_pmreset_mask : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               When set: RFSM will reset DDRT IO in DDRT Reset
                               PM Mode
                            */
    UINT32 rsvd_14 : 17;

                            /* Bits[30:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrtio_ckidle_mask : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               When this bit is set, clock idle from ddrio is
                               masked. Firmware should only clear this bit when
                               correct DDRT Frequencies are programmed in boot
                               flow. Set this bit when entering DDRT Frequency
                               Change 0 : No Mask 1 : MASK Clock Idle (Default)
                            */

  } Bits;
  UINT32 Data;

} MSC_PLL_CTRL_FNV_DFX_MISC_0_STRUCT;








/* MSC_FNVIO_DDRTDLL_CSR_FNV_DFX_MISC_0_REG supported on:                       */
/*      BWV (0x4000c1b8)                                                        */
/* Register default value on BWV: 0x00000020                                    */
/* BWV Register File:    0_1_4_BWV                                              */
/* Struct generated from BWV BDF: 0_1_4                                         */
/* BWV Security PolicyGroup:                                                    */
/* FNVIO interface Ddrtdll override signals register
*/


#define MSC_FNVIO_DDRTDLL_CSR_FNV_DFX_MISC_0_REG 0x1BC401B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable to override signals with CSR value */
    UINT32 ddrtdllpowerdown01qnnnh_val : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* DDRT DLL power down control */
    UINT32 ddrtdllpowerdown23qnnnh_val : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* DDRT DLL power down control */
    UINT32 ddrtdllpowerdown4qnnnh_val : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* DDRT DLL power down control */
    UINT32 rsvd_4 : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_FNVIO_DDRTDLL_CSR_FNV_DFX_MISC_0_STRUCT;





































































#endif /* _FNV_DFX_MISC_0_h */
