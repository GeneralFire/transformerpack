
/** @file
  SB_CXPSMB.h

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
/* CXPSMB                                                                       */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* CXPSMB                                                                       */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* CXPSMB                                                                       */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* CXPSMB                                                                       */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* CXPSMB                                                                       */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* CXPSMB                                                                       */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | HOSTIA_BOOT_SAI  */
/*    | HOSTIA_SMM_SAI | OOB_MSM_SAI                                            */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _SB_CXPSMB_h
#define _SB_CXPSMB_h
#include <Base.h>





/* SPGT_SB_CXPSMB_REG supported on:                                             */
/*      SPRA0 (0x20000214)                                                      */
/*      SPRB0 (0x20000214)                                                      */
/*      SPRHBM (0x20000214)                                                     */
/*      SPRC0 (0x20000214)                                                      */
/*      SPRMCC (0x20000214)                                                     */
/*      SPRUCC (0x20000214)                                                     */
/* Register default value on SPRA0: 0x40000005                                  */
/* Register default value on SPRB0: 0x40000005                                  */
/* Register default value on SPRHBM: 0x40000005                                 */
/* Register default value on SPRC0: 0x40000005                                  */
/* Register default value on SPRMCC: 0x40000005                                 */
/* Register default value on SPRUCC: 0x40000005                                 */
/* SPRA0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRB0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRHBM Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRC0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRMCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRUCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CXPSMB                                           */
/* SPRB0 Security PolicyGroup: CXPSMB                                           */
/* SPRHBM Security PolicyGroup: CXPSMB                                          */
/* SPRC0 Security PolicyGroup: CXPSMB                                           */
/* SPRMCC Security PolicyGroup: CXPSMB                                          */
/* SPRUCC Security PolicyGroup: CXPSMB                                          */
/* Setting of these timing parameters by FW, will affect both master and target timing on the physical. Hence these values must only be changed when the physical is IDLE.  Timings are based off a 100-MHz Clock, i.e. 1 clock period is 10 ns.[br]FW must read the SPGT.SPD register to determine what speed the PHY must be run on and then program the other register bits accordingly.
*/


#define SPGT_SB_CXPSMB_REG 0x19520214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 20;

                            /* Bits[19:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_20 : 10;

                            /* Bits[29:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spd : 2;

                            /* Bits[31:30], Access Type=RW/V/P, default=0x00000001*/

                            /*
                               Value to indicate what speed physical bus must
                               operate. It is up to the system designer to
                               ensure that all devices on the same SMBus
                               segment are capable of operating at the
                               programmed speed.[br][br]00: Standard (80
                               kHz)[br]01: Standard (100 kHz)[br]10: Fast Mode
                               (400 kHz)[br]11: Fast Mode Plus (1
                               MHz)[br][br]Note: The ability to write to this
                               register to dynamically alter the speeds is a
                               test mode capability. The expectation is that
                               the SMBus speed is set using soft strap and/or
                               BIOS when in production.[br][br]This register is
                               hardware loaded by soft strap pull.
                            */

  } Bits;
  UINT32 Data;

} SPGT_SB_CXPSMB_STRUCT;




/* DBSTS_SB_CXPSMB_REG supported on:                                            */
/*      SPRA0 (0x20000220)                                                      */
/*      SPRB0 (0x20000220)                                                      */
/*      SPRHBM (0x20000220)                                                     */
/*      SPRC0 (0x20000220)                                                      */
/*      SPRMCC (0x20000220)                                                     */
/*      SPRUCC (0x20000220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRB0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRHBM Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRC0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRMCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRUCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CXPSMB                                           */
/* SPRB0 Security PolicyGroup: CXPSMB                                           */
/* SPRHBM Security PolicyGroup: CXPSMB                                          */
/* SPRC0 Security PolicyGroup: CXPSMB                                           */
/* SPRMCC Security PolicyGroup: CXPSMB                                          */
/* SPRUCC Security PolicyGroup: CXPSMB                                          */
/* Debug Status Register
*/


#define DBSTS_SB_CXPSMB_REG 0x19520220


#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lsmbs : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This register reflects the last seen state of
                               the SMBus. Since the SMBus state is dynamic, it
                               is advised that FW use its judgement when
                               interpreting this data.[br][br]000: SMBus
                               Idle[br]001: Start Seen[br]010: ACK Seen[br]011:
                               Repeated Start Seen[br]100: Stop Seen[br]101:
                               Reserved[br]110: Reserved[br]111: Reserved
                            */
    UINT32 rsvd : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 smbdc : 1;

                            /* Bits[5:5], Access Type=RO/C/V, default=0x00000000*/

                            /*
                               This register when set indicates that there was
                               a 1->0 or 0->1 transition on SMBus data
                               line.[br][br]This register is cleared by HW on
                               reads by FW.
                            */
    UINT32 smbckc : 1;

                            /* Bits[6:6], Access Type=RO/C/V, default=0x00000000*/

                            /*
                               This register when set indicates that there was
                               a 1->0 or 0->1 transition on SMBus clock
                               line.[br][br]This register is cleared by HW on
                               reads by FW.
                            */
    UINT32 smbac : 1;

                            /* Bits[7:7], Access Type=RO/C/V, default=0x00000000*/

                            /*
                               This register when set indicates that there was
                               a 1->0 or 0->1 transition on SMBus alert
                               line.[br][br]This register is cleared by HW on
                               reads by FW.
                            */
    UINT32 smbbc : 1;

                            /* Bits[8:8], Access Type=RO/C/V, default=0x00000000*/

                            /*
                               This register when set indicates that there was
                               a 1->0 or 0->1 transition on SMBus busy
                               line.[br][br]This register is cleared by HW on
                               reads by FW.
                            */
    UINT32 smbdt : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               This register shows the live state of SMBus data
                               signal as seen by HW from the buffer after HW
                               deglitch and synchronization.
                            */
    UINT32 smbck : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               This register shows the live state of SMBus
                               clock signal as seen by HW from the buffer after
                               HW deglitch and synchronization.
                            */
    UINT32 smbat : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               This register shows the live state of SMBus
                               SMBALERT signal as seen by HW from the buffer
                               after HW deglitch and synchronization.
                            */
    UINT32 smbbu : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               This register shows the live state of SMBus Busy
                               signal as seen by HW from the buffer after HW
                               deglitch and synchronization.
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DBSTS_SB_CXPSMB_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */


/* SMTCTL_SB_CXPSMB_REG supported on:                                           */
/*      SPRA0 (0x20000224)                                                      */
/*      SPRB0 (0x20000224)                                                      */
/*      SPRHBM (0x20000224)                                                     */
/*      SPRC0 (0x20000224)                                                      */
/*      SPRMCC (0x20000224)                                                     */
/*      SPRUCC (0x20000224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRB0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRHBM Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRC0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRMCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRUCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CXPSMB                                           */
/* SPRB0 Security PolicyGroup: CXPSMB                                           */
/* SPRHBM Security PolicyGroup: CXPSMB                                          */
/* SPRC0 Security PolicyGroup: CXPSMB                                           */
/* SPRMCC Security PolicyGroup: CXPSMB                                          */
/* SPRUCC Security PolicyGroup: CXPSMB                                          */
/* Note regarding bus reset modes: Three bus reset modes are implemented. On setting SMBEN from 0 to 1, the bus reset modes, if individually enabled, are executed in the following order: 1) I2CBUSCLR, 2) SMBTORSTMODE, 3) MUXDISRSTMODE. I2CBUSCLR can be issued 1 to 4 by configuring I2BCRSTMODECNT.
*/


#define SMTCTL_SB_CXPSMB_REG 0x19520224

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 smben : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to block activity on SMBus,
                               including any bus reset (if enabled), on
                               side_rst_b deassertion.[br][br]This creates a
                               configuration window where the SMBus speed, bus
                               reset, and any other config registers can be
                               programmed by BIOS/pCode prior to SMBus
                               activity.[br]When the parameter USE_SMBUS_ENABLE
                               is 1, BIOS/pCode must set this field to 1 to
                               enable activity on SMBus. When set to 1, the
                               programmed bus reset mode, if enabled, is
                               executed, and SMBus/I2C commands from sideband
                               can be processed by SMBus.[br]This field is
                               self-locking and can only be set to 1 a single
                               time. Following being set to 1, writes shall
                               have no effect and reads shall return 1.[br]When
                               the parameter USE_SMBUS_ENABLE is 0, this field
                               is loaded to a constant value of 1 by hardware.
                            */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 i2cbcrstmodeen : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the I2C
                               bus clear plus STOP condition reset mode. See
                               description in INITI2CBUSCLR.
                            */
    UINT32 smbtorstmodeen : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the SMBus
                               Timeout reset mode. The SMBus timeout is simply
                               a pause on the SMBCLK and SMBDAT outputs, which
                               SMBus slaves may use as a self reset. This
                               timeout is only specified in the SMBus spec and
                               not the I2C spec. The SMBus timeout defaults to
                               a 25ms pause (controlled in TIMERCTL) on the
                               SMBCLK and SMBDAT outputs.
                            */
    UINT32 muxdisrstmodeen : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the mux
                               disable reset mode. See description in
                               INITMUXDIS
                            */
    UINT32 i2cbcrstmodecnt : 2;

                            /* Bits[8:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Controls the number of I2C bus clears that are
                               executed when I2CBCRSTMODEEN is enabled. Zero
                               based number: 2'h0 = 1 bus clear, 2'h1 = 2 bus
                               clears, 2'h2 = 3 bus clears, 2'h3 = 4 bus
                               clears.
                            */
    UINT32 rsvd_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMTCTL_SB_CXPSMB_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 smben : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to block activity on SMBus,
                               including any bus reset (if enabled), on
                               side_rst_b deassertion.[br][br]This creates a
                               configuration window where the SMBus speed, bus
                               reset, and any other config registers can be
                               programmed by BIOS/pCode prior to SMBus
                               activity.[br]When the parameter USE_SMBUS_ENABLE
                               is 1, BIOS/pCode must set this field to 1 to
                               enable activity on SMBus. When set to 1, the
                               programmed bus reset mode, if enabled, is
                               executed, and SMBus/I2C commands from sideband
                               can be processed by SMBus.[br]This field is
                               self-locking and can only be set to 1 a single
                               time. Following being set to 1, writes shall
                               have no effect and reads shall return 1.[br]When
                               the parameter USE_SMBUS_ENABLE is 0, this field
                               is loaded to a constant value of 1 by hardware.
                            */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 i2cbcrstmodeen : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the I2C
                               bus clear plus STOP condition reset mode. See
                               description in INITI2CBUSCLR.
                            */
    UINT32 smbtorstmodeen : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the SMBus
                               Timeout reset mode. The SMBus timeout is simply
                               a pause on the SMBCLK and SMBDAT outputs, which
                               SMBus slaves may use as a self reset. This
                               timeout is only specified in the SMBus spec and
                               not the I2C spec. The SMBus timeout defaults to
                               a 25ms pause (controlled in TIMERCTL) on the
                               SMBCLK and SMBDAT outputs.
                            */
    UINT32 muxdisrstmodeen : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the mux
                               disable reset mode. See description in
                               INITMUXDIS
                            */
    UINT32 i2cbcrstmodecnt : 2;

                            /* Bits[8:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Controls the number of I2C bus clears that are
                               executed when I2CBCRSTMODEEN is enabled. Zero
                               based number: 2'h0 = 1 bus clear, 2'h1 = 2 bus
                               clears, 2'h2 = 3 bus clears, 2'h3 = 4 bus
                               clears.
                            */
    UINT32 rsvd_9 : 2;

                            /* Bits[10:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMTCTL_SB_CXPSMB_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 smben : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used to block activity on SMBus,
                               including any bus reset (if enabled), on
                               side_rst_b deassertion.[br][br]This creates a
                               configuration window where the SMBus speed, bus
                               reset, and any other config registers can be
                               programmed by BIOS/pCode prior to SMBus
                               activity.[br]When the parameter USE_SMBUS_ENABLE
                               is 1, BIOS/pCode must set this field to 1 to
                               enable activity on SMBus. When set to 1, the
                               programmed bus reset mode, if enabled, is
                               executed, and SMBus/I2C commands from sideband
                               can be processed by SMBus.[br]This field is
                               self-locking and can only be set to 1 a single
                               time. Following being set to 1, writes shall
                               have no effect and reads shall return 1.[br]When
                               the parameter USE_SMBUS_ENABLE is 0, this field
                               is loaded to a constant value of 1 by hardware.
                            */
    UINT32 rsvd_1 : 3;

                            /* Bits[3:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 i2cbcrstmodeen : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the I2C
                               bus clear plus STOP condition reset mode. See
                               description in INITI2CBUSCLR.
                            */
    UINT32 smbtorstmodeen : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the SMBus
                               Timeout reset mode. The SMBus timeout is simply
                               a pause on the SMBCLK and SMBDAT outputs, which
                               SMBus slaves may use as a self reset. This
                               timeout is only specified in the SMBus spec and
                               not the I2C spec. The SMBus timeout defaults to
                               a 25ms pause (controlled in TIMERCTL) on the
                               SMBCLK and SMBDAT outputs.
                            */
    UINT32 muxdisrstmodeen : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               When SMBEN is set, this field enables the mux
                               disable reset mode. See description in
                               INITMUXDIS
                            */
    UINT32 i2cbcrstmodecnt : 2;

                            /* Bits[8:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Controls the number of I2C bus clears that are
                               executed when I2CBCRSTMODEEN is enabled. Zero
                               based number: 2'h0 = 1 bus clear, 2'h1 = 2 bus
                               clears, 2'h2 = 3 bus clears, 2'h3 = 4 bus
                               clears.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMTCTL_SB_CXPSMB_STRUCT;






















/* SEGMAP0_SB_CXPSMB_REG supported on:                                          */
/*      SPRA0 (0x27c)                                                           */
/*      SPRB0 (0x27c)                                                           */
/*      SPRHBM (0x27c)                                                          */
/*      SPRC0 (0x27c)                                                           */
/*      SPRMCC (0x27c)                                                          */
/*      SPRUCC (0x27c)                                                          */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRB0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRHBM Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRC0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRMCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRUCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CXPSMB                                           */
/* SPRB0 Security PolicyGroup: CXPSMB                                           */
/* SPRHBM Security PolicyGroup: CXPSMB                                          */
/* SPRC0 Security PolicyGroup: CXPSMB                                           */
/* SPRMCC Security PolicyGroup: CXPSMB                                          */
/* SPRUCC Security PolicyGroup: CXPSMB                                          */
/* Segment Map
*/


#define SEGMAP0_SB_CXPSMB_REG 0x1950027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 smap : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Indicates the segment row to which the source is
                               mapped.
                            */
    UINT8 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} SEGMAP0_SB_CXPSMB_STRUCT;
















































/* SEGROW0_SB_CXPSMB_REG supported on:                                          */
/*      SPRA0 (0x100002ac)                                                      */
/*      SPRB0 (0x100002ac)                                                      */
/*      SPRHBM (0x100002ac)                                                     */
/*      SPRC0 (0x100002ac)                                                      */
/*      SPRMCC (0x100002ac)                                                     */
/*      SPRUCC (0x100002ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRB0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRHBM Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRC0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRMCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRUCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: CXPSMB                                           */
/* SPRB0 Security PolicyGroup: CXPSMB                                           */
/* SPRHBM Security PolicyGroup: CXPSMB                                          */
/* SPRC0 Security PolicyGroup: CXPSMB                                           */
/* SPRMCC Security PolicyGroup: CXPSMB                                          */
/* SPRUCC Security PolicyGroup: CXPSMB                                          */
/* Segment Row
*/


#define SEGROW0_SB_CXPSMB_REG 0x195102AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 valid : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Indicates this table entry is valid. */
    UINT16 mp : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* Indicates if a mux is present on this segment. */
    UINT16 mdisrst : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Indicates that the mux at this address, if
                               present, shall be disabled by writing a channel
                               select of DEVARBCTL.MUXNOCHANSELVAL (ignoring
                               SEGROW.MCHANSEL value) during mux disable reset
                               mode, or as directed by system software.
                            */
    UINT16 maddr : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Indicates the mux slave address when a mux is
                               present on this segment, used as bits 3:1 in the
                               programed slave address.
                            */
    UINT16 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 mchansel : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Indicates the channel select to be programmed to
                               the mux when a mux is present on this segment.
                            */

  } Bits;
  UINT16 Data;

} SEGROW0_SB_CXPSMB_STRUCT;




























































































#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* TIMERCTL1_SB_CXPSMB_REG supported on:                                        */
/*      SPRB0 (0x200003c4)                                                      */
/*      SPRHBM (0x200003c4)                                                     */
/*      SPRC0 (0x200003c4)                                                      */
/*      SPRMCC (0x200003c4)                                                     */
/*      SPRUCC (0x200003c4)                                                     */
/* Register default value on SPRB0: 0x0000020A                                  */
/* Register default value on SPRHBM: 0x0000020A                                 */
/* Register default value on SPRC0: 0x0000020A                                  */
/* Register default value on SPRMCC: 0x0000020A                                 */
/* Register default value on SPRUCC: 0x0000020A                                 */
/* SPRB0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRHBM Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRC0 Register File:    sprsp_top/cxpsmb_top/priv                            */
/* SPRMCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* SPRUCC Register File:    sprsp_top/cxpsmb_top/priv                           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: CXPSMB                                           */
/* SPRHBM Security PolicyGroup: CXPSMB                                          */
/* SPRC0 Security PolicyGroup: CXPSMB                                           */
/* SPRMCC Security PolicyGroup: CXPSMB                                          */
/* SPRUCC Security PolicyGroup: CXPSMB                                          */
/* Timer Control
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#endif /* _SB_CXPSMB_h */
