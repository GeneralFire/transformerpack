
/** @file
  SB_I3C1_PVT.h

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
/* SB_I3C_FUSE_REGS_GP                                                          */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* SB_I3C_PVT_REGS_GP                                                           */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */

/* SPRHBM Security Policy Groups:                                               */

/* SPRC0 Security Policy Groups:                                                */

/* SPRMCC Security Policy Groups:                                               */

/* SPRUCC Security Policy Groups:                                               */


#ifndef _SB_I3C1_PVT_h
#define _SB_I3C1_PVT_h
#include <Base.h>

#ifdef SPRA0_HOST
/* PVT_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG supported on:                         */
/*      SPRA0 (0x20000000)                                                      */
/* Register default value on SPRA0: 0x01000208                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* Private Control Policy Low Register
*/


#define PVT_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pvt_control_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x01000208*/

                            /* Pvt Control Policy Low Field */

  } Bits;
  UINT32 Data;

} PVT_CONTROL_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* PVT_CONTROL_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                        */
/*      SPRA0 (0x20000004)                                                      */
/* Register default value on SPRA0: 0x00000400                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* Private Control Policy Low Register
*/


#define PVT_CONTROL_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A320004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pvt_control_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* Pvt Control Policy High Field */

  } Bits;
  UINT32 Data;

} PVT_CONTROL_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* PVT_READ_POLICY_LOW_SB_I3C1_PVT_REG supported on:                            */
/*      SPRA0 (0x20000008)                                                      */
/* Register default value on SPRA0: 0x0100020A                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* High Private Read Policy Low Register
*/


#define PVT_READ_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pvt_read_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100020A*/

                            /* High Private Read Policy Low Field */

  } Bits;
  UINT32 Data;

} PVT_READ_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* PVT_READ_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x2000000c)                                                      */
/* Register default value on SPRA0: 0x20000500                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* High Private Read Policy High Register
*/


#define PVT_READ_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A32000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pvt_read_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000500*/

                            /* High Private Read Policy High Field */

  } Bits;
  UINT32 Data;

} PVT_READ_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* PVT_WRITE_POLICY_LOW_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000010)                                                      */
/* Register default value on SPRA0: 0x0100020A                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* High Private Write Policy Low Register
*/


#define PVT_WRITE_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pvt_write_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100020A*/

                            /* High Private Write Policy Low Field */

  } Bits;
  UINT32 Data;

} PVT_WRITE_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* PVT_WRITE_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                          */
/*      SPRA0 (0x20000014)                                                      */
/* Register default value on SPRA0: 0x00000400                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* High Private Write Policy High Register
*/


#define PVT_WRITE_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A320014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pvt_write_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* High Private Write Policy High Field */

  } Bits;
  UINT32 Data;

} PVT_WRITE_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MMIO_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG supported on:                        */
/*      SPRA0 (0x20000018)                                                      */
/* Register default value on SPRA0: 0x0100021E                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* MMIO Control Policy Low Register
*/


#define MMIO_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mmio_control_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100021E*/

                            /* MMIO Control Policy Low Field */

  } Bits;
  UINT32 Data;

} MMIO_CONTROL_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MMIO_CONTROL_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                       */
/*      SPRA0 (0x2000001c)                                                      */
/* Register default value on SPRA0: 0x00000400                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* MMIO Control Policy High Register
*/


#define MMIO_CONTROL_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A32001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mmio_control_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* MMIO Control Policy High Field */

  } Bits;
  UINT32 Data;

} MMIO_CONTROL_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MMIO_READ_POLICY_LOW_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000020)                                                      */
/* Register default value on SPRA0: 0x0300021F                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* High Private Read Policy Low Register
*/


#define MMIO_READ_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mmio_read_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0300021F*/

                            /* High MMIO Read Policy Low Field */

  } Bits;
  UINT32 Data;

} MMIO_READ_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MMIO_READ_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                          */
/*      SPRA0 (0x20000024)                                                      */
/* Register default value on SPRA0: 0x20000D00                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* High Private Read Policy High Register
*/


#define MMIO_READ_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A320024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mmio_read_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000D00*/

                            /* High MMIO Read Policy High Field */

  } Bits;
  UINT32 Data;

} MMIO_READ_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MMIO_WRITE_POLICY_LOW_SB_I3C1_PVT_REG supported on:                          */
/*      SPRA0 (0x20000028)                                                      */
/* Register default value on SPRA0: 0x0100021E                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* High Private Write Policy Low Register
*/


#define MMIO_WRITE_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mmio_write_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100021E*/

                            /* High MMIO Write Policy Low Field */

  } Bits;
  UINT32 Data;

} MMIO_WRITE_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* MMIO_WRITE_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                         */
/*      SPRA0 (0x2000002c)                                                      */
/* Register default value on SPRA0: 0x20000C00                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* High Private Write Policy High Register
*/


#define MMIO_WRITE_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A32002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mmio_write_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000C00*/

                            /* High MMIO Write Policy High Field */

  } Bits;
  UINT32 Data;

} MMIO_WRITE_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG supported on:                        */
/*      SPRA0 (0x20000030)                                                      */
/* Register default value on SPRA0: 0x01000208                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse Control Policy Low Register
*/


#define FUSE_CONTROL_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_control_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x01000208*/

                            /* Fuse Control Policy Low Field */

  } Bits;
  UINT32 Data;

} FUSE_CONTROL_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE_CONTROL_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                       */
/*      SPRA0 (0x20000034)                                                      */
/* Register default value on SPRA0: 0x00000400                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse Control Policy High Register
*/


#define FUSE_CONTROL_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A320034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_control_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* Fuse Control Policy High Field */

  } Bits;
  UINT32 Data;

} FUSE_CONTROL_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE_READ_POLICY_LOW_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000038)                                                      */
/* Register default value on SPRA0: 0x0100020A                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse Read Policy Low Register
*/


#define FUSE_READ_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_read_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100020A*/

                            /* Fuse Read Policy Low Field */

  } Bits;
  UINT32 Data;

} FUSE_READ_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE_READ_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                          */
/*      SPRA0 (0x2000003c)                                                      */
/* Register default value on SPRA0: 0x20000500                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse Read Policy High Register
*/


#define FUSE_READ_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A32003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_read_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x20000500*/

                            /* Fuse Read Policy High Field */

  } Bits;
  UINT32 Data;

} FUSE_READ_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE_WRITE_POLICY_LOW_SB_I3C1_PVT_REG supported on:                          */
/*      SPRA0 (0x20000040)                                                      */
/* Register default value on SPRA0: 0x0100020A                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse Write Policy Low Register
*/


#define FUSE_WRITE_POLICY_LOW_SB_I3C1_PVT_REG 0x2A320040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_write_policy_low : 32;

                            /* Bits[31:0], Access Type=RW, default=0x0100020A*/

                            /* Fuse Write Policy Low Field */

  } Bits;
  UINT32 Data;

} FUSE_WRITE_POLICY_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE_WRITE_POLICY_HIGH_SB_I3C1_PVT_REG supported on:                         */
/*      SPRA0 (0x20000044)                                                      */
/* Register default value on SPRA0: 0x00000400                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse Write Policy High Register
*/


#define FUSE_WRITE_POLICY_HIGH_SB_I3C1_PVT_REG 0x2A320044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_write_policy_high : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000400*/

                            /* Fuse Write Policy High Field */

  } Bits;
  UINT32 Data;

} FUSE_WRITE_POLICY_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* FUSE1_SB_I3C1_PVT_REG supported on:                                          */
/*      SPRA0 (0x20000048)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_FUSE_REGS_GP                              */
/* Fuse 1
*/


#define FUSE1_SB_I3C1_PVT_REG 0x2A320048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_spd_i3c : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Stores the Fuse values pulled. 1 => IP disable,
                               0 => IP usable. This register is protected by a
                               separate policy, and agents in that policy group
                               can over-ride the fuse pulled value in this
                               field.
                            */
    UINT32 reserved0 : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} FUSE1_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* PMCTL_SB_I3C1_PVT_REG supported on:                                          */
/*      SPRA0 (0x2000006c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* Power Management control
*/


#define PMCTL_SB_I3C1_PVT_REG 0x2A32006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iosf_sb_local_gate_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               when set to 1, dynamic intenal clock gating is
                               enabled to the clock that is going to the I3C
                               controller. When set to 0, this clock gating is
                               disabled.
                            */
    UINT32 iosf_sb_trunk_gate_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               when set to 1, auto control of side_clk_req/ack
                               through SBEP is enabled. When set to 0, clkereq
                               will not be de-asserted dynamically.
                            */
    UINT32 reserved0 : 30;

                            /* Bits[31:2], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} PMCTL_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_HW_CTRL_SB_I3C1_PVT_REG supported on:                                 */
/*      SPRA0 (0x20000070)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C HW control register
*/


#define SB_I3C_HW_CTRL_SB_I3C1_PVT_REG 0x2A320070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_poll_us : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 0, HW monitors poll results in IBIQ,
                               and are converted to u/s pvt writes to PUNIT/MC.
                               Normal IBI (not polled results), if received,
                               will be flushed by logic and the last flushed
                               IBI will be logged in MMIO conv layer registers
                               - and a PCIe_AER message would be send upstream
                               to IEH. When set to 1, HW does not monitor IBIQ,
                               nor generate any U/S transactions.
                            */
    UINT32 ts_poll_dest : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, polled results would be send to
                               PUNIT only When set to 0, polled results would
                               be send to PUNIT & MC. This bit is applicable
                               only in PkgC0 state.
                            */
    UINT32 enb_nor_ibi : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 0, SW access to IBI_PORT is masked,
                               any accesses would be U/R-ed. When set to 1, SW
                               can access IBI_PORT and service normal IBIs.
                               This is intended to enable normal IBI usages on
                               SPR w/SW assistance. In this mode, U/S
                               transaction generation for polled results must
                               be disabled through DISABLE_POLL_US (Set to 1)
                            */
    UINT32 reserved0 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SB_I3C_HW_CTRL_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_PORTID_10_SB_I3C1_PVT_REG supported on:                            */
/*      SPRA0 (0x20000074)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC Portid 10
*/


#define SB_I3C_MC_PORTID_10_SB_I3C1_PVT_REG 0x2A320074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_portid_0 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 0 */
    UINT32 mc_portid_1 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 1 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_PORTID_10_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_PORTID_32_SB_I3C1_PVT_REG supported on:                            */
/*      SPRA0 (0x20000078)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC Portid 32
*/


#define SB_I3C_MC_PORTID_32_SB_I3C1_PVT_REG 0x2A320078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_portid_2 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 2 */
    UINT32 mc_portid_3 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 3 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_PORTID_32_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_PORTID_54_SB_I3C1_PVT_REG supported on:                            */
/*      SPRA0 (0x2000007c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC Portid 54
*/


#define SB_I3C_MC_PORTID_54_SB_I3C1_PVT_REG 0x2A32007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_portid_4 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 4 */
    UINT32 mc_portid_5 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 5 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_PORTID_54_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_PORTID_76_SB_I3C1_PVT_REG supported on:                            */
/*      SPRA0 (0x20000080)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC Portid 76
*/


#define SB_I3C_MC_PORTID_76_SB_I3C1_PVT_REG 0x2A320080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_portid_6 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 6 */
    UINT32 mc_portid_7 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* PORT_ID programming for DIMM 7 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_PORTID_76_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_00_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000084)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_00
*/


#define SB_I3C_MC_ADDROFF_00_SB_I3C1_PVT_REG 0x2A320084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 0 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_00_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_01_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000088)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_01
*/


#define SB_I3C_MC_ADDROFF_01_SB_I3C1_PVT_REG 0x2A320088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h0 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 0 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_01_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_10_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x2000008c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_10
*/


#define SB_I3C_MC_ADDROFF_10_SB_I3C1_PVT_REG 0x2A32008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 1 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_10_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_11_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000090)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_11
*/


#define SB_I3C_MC_ADDROFF_11_SB_I3C1_PVT_REG 0x2A320090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h1 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 1 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_11_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_20_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000094)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_20
*/


#define SB_I3C_MC_ADDROFF_20_SB_I3C1_PVT_REG 0x2A320094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 2 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_20_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_21_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000098)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_21
*/


#define SB_I3C_MC_ADDROFF_21_SB_I3C1_PVT_REG 0x2A320098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h2 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 2 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_21_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_30_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x2000009c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_30
*/


#define SB_I3C_MC_ADDROFF_30_SB_I3C1_PVT_REG 0x2A32009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l3 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 3 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_30_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_31_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000100)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_31
*/


#define SB_I3C_MC_ADDROFF_31_SB_I3C1_PVT_REG 0x2A320100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h3 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 3 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_31_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_40_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000104)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_40
*/


#define SB_I3C_MC_ADDROFF_40_SB_I3C1_PVT_REG 0x2A320104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l4 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 4 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_40_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_41_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000108)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_41
*/


#define SB_I3C_MC_ADDROFF_41_SB_I3C1_PVT_REG 0x2A320108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h4 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 4 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_41_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_50_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x2000010c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_50
*/


#define SB_I3C_MC_ADDROFF_50_SB_I3C1_PVT_REG 0x2A32010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l5 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 5 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_50_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_51_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000110)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_51
*/


#define SB_I3C_MC_ADDROFF_51_SB_I3C1_PVT_REG 0x2A320110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h5 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 5 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_51_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_60_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000114)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_60
*/


#define SB_I3C_MC_ADDROFF_60_SB_I3C1_PVT_REG 0x2A320114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l6 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 6 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_60_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_61_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000118)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_61
*/


#define SB_I3C_MC_ADDROFF_61_SB_I3C1_PVT_REG 0x2A320118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h6 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 6 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_61_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_70_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x2000011c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_70
*/


#define SB_I3C_MC_ADDROFF_70_SB_I3C1_PVT_REG 0x2A32011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_l7 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 7 */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_70_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_MC_ADDROFF_71_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000120)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C MC ADDROFF_71
*/


#define SB_I3C_MC_ADDROFF_71_SB_I3C1_PVT_REG 0x2A320120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mc_addroff_h7 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* ADDRESS_OFFSET programming for DIMM 7 */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_MC_ADDROFF_71_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_PCIE_ERR_DEST_ID_SB_I3C1_PVT_REG supported on:                        */
/*      SPRA0 (0x20000124)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C PCIE ERR DEST ID
*/


#define SB_I3C_PCIE_ERR_DEST_ID_SB_I3C1_PVT_REG 0x2A320124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pcie_err_dest_id : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* PCIE AER message destination ID */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_PCIE_ERR_DEST_ID_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_PUNIT_DEST_ID_SB_I3C1_PVT_REG supported on:                           */
/*      SPRA0 (0x20000128)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C PUNIT DEST ID
*/


#define SB_I3C_PUNIT_DEST_ID_SB_I3C1_PVT_REG 0x2A320128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 punit_dest_id : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* PUNIT_DEST_ID */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_PUNIT_DEST_ID_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_PUNIT_ADDROFF_LOW_SB_I3C1_PVT_REG supported on:                       */
/*      SPRA0 (0x2000012c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C PUNIT ADDROFF LOW
*/


#define SB_I3C_PUNIT_ADDROFF_LOW_SB_I3C1_PVT_REG 0x2A32012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 punit_addroff_l0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               PUNIT_ADDROFF_L0 PUNIT address offset is
                               configurable for only first DIMM0, for the
                               remaining DIMMs address offset is incremented by
                               4. eg. DIMM1_addr_off = DIMM0_addr_off + 4
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_PUNIT_ADDROFF_LOW_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */

#ifdef SPRA0_HOST
/* SB_I3C_PUNIT_ADDROFF_HIGH_SB_I3C1_PVT_REG supported on:                      */
/*      SPRA0 (0x20000130)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* SPRA0 Register File:    sprsp_top/lpss[1]/sb_i3c_pvt                         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_PVT_REGS_GP                               */
/* SB I3C PUNIT ADDROFF HIGH
*/


#define SB_I3C_PUNIT_ADDROFF_HIGH_SB_I3C1_PVT_REG 0x2A320130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 punit_addroff_h0 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               PUNIT_ADDROFF_H0 PUNIT address offset is
                               configurable for only first DIMM0, for the
                               remaining DIMMs address offset is incremented by
                               4. eg. DIMM1_addr_off = DIMM0_addr_off + 4
                               DIMM2_addr_off = DIMM1_addr_off + 4
                            */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} SB_I3C_PUNIT_ADDROFF_HIGH_SB_I3C1_PVT_STRUCT;
#endif /* (SPRA0_HOST) */
#endif /* _SB_I3C1_PVT_h */
