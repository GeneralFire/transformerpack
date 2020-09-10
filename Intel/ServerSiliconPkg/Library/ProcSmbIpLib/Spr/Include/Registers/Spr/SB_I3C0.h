
/** @file
  SB_I3C0.h

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
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | UNCORE_PMA_SAI                   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | UNCORE_PMA_SAI                   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | UNCORE_PMA_SAI                   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | UNCORE_PMA_SAI                   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | UNCORE_PMA_SAI                   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* SB_I3C_MMIO_REGS_GP                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | UNCORE_PMA_SAI                   */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | OOB_MSM_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _SB_I3C0_h
#define _SB_I3C0_h
#include <Base.h>

/* HCI_VERSION_SB_I3C0_REG supported on:                                        */
/*      SPRA0 (0x200d8000)                                                      */
/*      SPRB0 (0x200d8000)                                                      */
/*      SPRHBM (0x200d8000)                                                     */
/*      SPRC0 (0x200d8000)                                                      */
/*      SPRMCC (0x200d8000)                                                     */
/*      SPRUCC (0x200d8000)                                                     */
/* Register default value on SPRA0: 0x00000052                                  */
/* Register default value on SPRB0: 0x00000052                                  */
/* Register default value on SPRHBM: 0x00000052                                 */
/* Register default value on SPRC0: 0x00000052                                  */
/* Register default value on SPRMCC: 0x00000052                                 */
/* Register default value on SPRUCC: 0x00000052                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                    HCI_VERSION
*/


#define HCI_VERSION_SB_I3C0_REG 0x2A028000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 version : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000052*/

                            /*  */

  } Bits;
  UINT32 Data;

} HCI_VERSION_SB_I3C0_STRUCT;

/* DEVICE_CONTROL_SB_I3C0_REG supported on:                                     */
/*      SPRA0 (0x200d8004)                                                      */
/*      SPRB0 (0x200d8004)                                                      */
/*      SPRHBM (0x200d8004)                                                     */
/*      SPRC0 (0x200d8004)                                                      */
/*      SPRMCC (0x200d8004)                                                     */
/*      SPRUCC (0x200d8004)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                 DEVICE_CONTROL
*/


#define DEVICE_CONTROL_SB_I3C0_REG 0x2A028004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iba_include : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 iba_arb_bits : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_4_6 : 3;

                            /* Bits[6:4], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 i2c_slave_present : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 hot_join_ctrl : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_9_28 : 20;

                            /* Bits[28:9], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 abort : 1;

                            /* Bits[29:29], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 resume : 1;

                            /* Bits[30:30], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 enable : 1;

                            /* Bits[31:31], Access Type=RW/V, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} DEVICE_CONTROL_SB_I3C0_STRUCT;

/* DEVICE_ADDR_SB_I3C0_REG supported on:                                        */
/*      SPRA0 (0x200d8008)                                                      */
/*      SPRB0 (0x200d8008)                                                      */
/*      SPRHBM (0x200d8008)                                                     */
/*      SPRC0 (0x200d8008)                                                      */
/*      SPRMCC (0x200d8008)                                                     */
/*      SPRUCC (0x200d8008)                                                     */
/* Register default value on SPRA0: 0x80000000                                  */
/* Register default value on SPRB0: 0x80000000                                  */
/* Register default value on SPRHBM: 0x80000000                                 */
/* Register default value on SPRC0: 0x80000000                                  */
/* Register default value on SPRMCC: 0x80000000                                 */
/* Register default value on SPRUCC: 0x80000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                    DEVICE_ADDR
*/


#define DEVICE_ADDR_SB_I3C0_REG 0x2A028008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved_0_15 : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 dynamic_addr : 7;

                            /* Bits[22:16], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 reserved_23_30 : 8;

                            /* Bits[30:23], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 dynamic_addr_valid : 1;

                            /* Bits[31:31], Access Type=RW/V, default=0x00000001*/

                            /*  */

  } Bits;
  UINT32 Data;

} DEVICE_ADDR_SB_I3C0_STRUCT;

/* DEVICE_CAPABILITIES_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d800c)                                                      */
/*      SPRB0 (0x200d800c)                                                      */
/*      SPRHBM (0x200d800c)                                                     */
/*      SPRC0 (0x200d800c)                                                      */
/*      SPRMCC (0x200d800c)                                                     */
/*      SPRUCC (0x200d800c)                                                     */
/* Register default value on SPRA0: 0x000000C4                                  */
/* Register default value on SPRB0: 0x000000C4                                  */
/* Register default value on SPRHBM: 0x000000C4                                 */
/* Register default value on SPRC0: 0x000000C4                                  */
/* Register default value on SPRMCC: 0x000000C4                                 */
/* Register default value on SPRUCC: 0x000000C4                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            DEVICE_CAPABILITIES
*/


#define DEVICE_CAPABILITIES_SB_I3C0_REG 0x2A02800C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved_0_1 : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 combo_command : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 auto_command : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 sg_for_ring_ptrs : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 non_current_master_cap : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 hdr_ddr_en : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 hdr_ts_en : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 reserved_8_31 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} DEVICE_CAPABILITIES_SB_I3C0_STRUCT;

/* RESET_CTRL_SB_I3C0_REG supported on:                                         */
/*      SPRA0 (0x200d8010)                                                      */
/*      SPRB0 (0x200d8010)                                                      */
/*      SPRHBM (0x200d8010)                                                     */
/*      SPRC0 (0x200d8010)                                                      */
/*      SPRMCC (0x200d8010)                                                     */
/*      SPRUCC (0x200d8010)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                     RESET_CTRL
*/


#define RESET_CTRL_SB_I3C0_REG 0x2A028010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 soft_rst : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 cmd_queue_rst : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 resp_queue_rst : 1;

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 tx_fifo_rst : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 rx_fifo_rst : 1;

                            /* Bits[4:4], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 ibi_queue_rst : 1;

                            /* Bits[5:5], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 reserved_6_28 : 23;

                            /* Bits[28:6], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 bus_reset_type : 2;

                            /* Bits[30:29], Access Type=RW/V, default=0x00000000*/

                            /*  */
    UINT32 bus_reset : 1;

                            /* Bits[31:31], Access Type=RW/V, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} RESET_CTRL_SB_I3C0_STRUCT;

/* PRESENT_STATE_SB_I3C0_REG supported on:                                      */
/*      SPRA0 (0x200d8014)                                                      */
/*      SPRB0 (0x200d8014)                                                      */
/*      SPRHBM (0x200d8014)                                                     */
/*      SPRC0 (0x200d8014)                                                      */
/*      SPRMCC (0x200d8014)                                                     */
/*      SPRUCC (0x200d8014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                  PRESENT_STATE
*/


#define PRESENT_STATE_SB_I3C0_REG 0x2A028014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved_0_1 : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 current_master : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*  */
    UINT32 reserved_3_31 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PRESENT_STATE_SB_I3C0_STRUCT;

/* COMMON_DEVICE_CTRL_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d8018)                                                      */
/*      SPRB0 (0x200d8018)                                                      */
/*      SPRHBM (0x200d8018)                                                     */
/*      SPRC0 (0x200d8018)                                                      */
/*      SPRMCC (0x200d8018)                                                     */
/*      SPRUCC (0x200d8018)                                                     */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRHBM: 0x00000005                                 */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRMCC: 0x00000005                                 */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             COMMON_DEVICE_CTRL
*/


#define COMMON_DEVICE_CTRL_SB_I3C0_REG 0x2A028018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ibi_payload : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 sir_reject : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 mr_reject : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 reserved_3_28 : 26;

                            /* Bits[28:3], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 dev_nack_retry_cnt : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_31_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} COMMON_DEVICE_CTRL_SB_I3C0_STRUCT;

/* INTR_STATUS_SB_I3C0_REG supported on:                                        */
/*      SPRA0 (0x200d8020)                                                      */
/*      SPRB0 (0x200d8020)                                                      */
/*      SPRHBM (0x200d8020)                                                     */
/*      SPRC0 (0x200d8020)                                                      */
/*      SPRMCC (0x200d8020)                                                     */
/*      SPRUCC (0x200d8020)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                    INTR_STATUS
*/


#define INTR_STATUS_SB_I3C0_REG 0x2A028020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_thld_stat : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 rx_thld_stat : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ibi_status_thld_stat : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 cmd_queue_ready_stat : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 resp_ready_stat : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 transfer_abort_stat : 1;

                            /* Bits[5:5], Access Type=RW/1C, default=0x00000000*/

                            /*  */
    UINT32 reserved_6_8 : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 transfer_err_stat : 1;

                            /* Bits[9:9], Access Type=RW/1C, default=0x00000000*/

                            /*  */
    UINT32 hc_internal_err_stat : 1;

                            /* Bits[10:10], Access Type=RW/1C, default=0x00000000*/

                            /*  */
    UINT32 reserved_11_13 : 3;

                            /* Bits[13:11], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ppoll_cmd_miss_stat : 1;

                            /* Bits[14:14], Access Type=RW/1C, default=0x00000000*/

                            /*  */
    UINT32 bus_reset_done_stat : 1;

                            /* Bits[15:15], Access Type=RW/1C, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} INTR_STATUS_SB_I3C0_STRUCT;

/* INTR_STATUS_ENABLE_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d8024)                                                      */
/*      SPRB0 (0x200d8024)                                                      */
/*      SPRHBM (0x200d8024)                                                     */
/*      SPRC0 (0x200d8024)                                                      */
/*      SPRMCC (0x200d8024)                                                     */
/*      SPRUCC (0x200d8024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             INTR_STATUS_ENABLE
*/


#define INTR_STATUS_ENABLE_SB_I3C0_REG 0x2A028024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_thld_stat_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 rx_thld_stat_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 ibi_thld_stat_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 cmd_queue_ready_stat_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 resp_ready_stat_intr_en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 transfer_abort_stat_en : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_6_8 : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 transfer_err_stat_en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 hc_internal_err_stat_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_11_13 : 3;

                            /* Bits[13:11], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ppoll_cmd_miss_stat_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 bus_reset_done_stat_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} INTR_STATUS_ENABLE_SB_I3C0_STRUCT;

/* INTR_SIGNAL_ENABLE_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d8028)                                                      */
/*      SPRB0 (0x200d8028)                                                      */
/*      SPRHBM (0x200d8028)                                                     */
/*      SPRC0 (0x200d8028)                                                      */
/*      SPRMCC (0x200d8028)                                                     */
/*      SPRUCC (0x200d8028)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             INTR_SIGNAL_ENABLE
*/


#define INTR_SIGNAL_ENABLE_SB_I3C0_REG 0x2A028028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_thld_signal_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 rx_thld_signal_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 ibi_thld_signal_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 cmd_queue_ready_signal_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 resp_ready_signal_en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 transfer_abort_signal_en : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_6_8 : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 transfer_err_signal_en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 hc_internal_err_signal_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_11_13 : 3;

                            /* Bits[13:11], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ppoll_cmd_miss_signal_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 bus_reset_done_signal_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} INTR_SIGNAL_ENABLE_SB_I3C0_STRUCT;

/* INTR_FORCE_SB_I3C0_REG supported on:                                         */
/*      SPRA0 (0x200d802c)                                                      */
/*      SPRB0 (0x200d802c)                                                      */
/*      SPRHBM (0x200d802c)                                                     */
/*      SPRC0 (0x200d802c)                                                      */
/*      SPRMCC (0x200d802c)                                                     */
/*      SPRUCC (0x200d802c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                     INTR_FORCE
*/


#define INTR_FORCE_SB_I3C0_REG 0x2A02802C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_thld_force_en : 1;

                            /* Bits[0:0], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 rx_thld_force_en : 1;

                            /* Bits[1:1], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 ibi_thld_force_en : 1;

                            /* Bits[2:2], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 cmd_queue_ready_force_en : 1;

                            /* Bits[3:3], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 resp_ready_force_en : 1;

                            /* Bits[4:4], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 transfer_abort_force_en : 1;

                            /* Bits[5:5], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 reserved_6_8 : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 transfer_err_force_en : 1;

                            /* Bits[9:9], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 hc_internal_err_force_en : 1;

                            /* Bits[10:10], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 reserved_11_13 : 3;

                            /* Bits[13:11], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ppoll_cmd_miss_force_en : 1;

                            /* Bits[14:14], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 bus_reset_done_force_en : 1;

                            /* Bits[15:15], Access Type=WO, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} INTR_FORCE_SB_I3C0_STRUCT;

/* RING_HEADERS_SECTION_OFFSET_SB_I3C0_REG supported on:                        */
/*      SPRA0 (0x200d8038)                                                      */
/*      SPRB0 (0x200d8038)                                                      */
/*      SPRHBM (0x200d8038)                                                     */
/*      SPRC0 (0x200d8038)                                                      */
/*      SPRMCC (0x200d8038)                                                     */
/*      SPRUCC (0x200d8038)                                                     */
/* Register default value on SPRA0: 0x000003C0                                  */
/* Register default value on SPRB0: 0x000003C0                                  */
/* Register default value on SPRHBM: 0x000003C0                                 */
/* Register default value on SPRC0: 0x000003C0                                  */
/* Register default value on SPRMCC: 0x000003C0                                 */
/* Register default value on SPRUCC: 0x000003C0                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*    RING_HEADERS_SECTION_OFFSET
*/


#define RING_HEADERS_SECTION_OFFSET_SB_I3C0_REG 0x2A028038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 section_offset : 16;

                            /* Bits[15:0], Access Type=RO, default=0x000003C0*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} RING_HEADERS_SECTION_OFFSET_SB_I3C0_STRUCT;

/* PIO_SECTION_OFFSET_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d803c)                                                      */
/*      SPRB0 (0x200d803c)                                                      */
/*      SPRHBM (0x200d803c)                                                     */
/*      SPRC0 (0x200d803c)                                                      */
/*      SPRMCC (0x200d803c)                                                     */
/*      SPRUCC (0x200d803c)                                                     */
/* Register default value on SPRA0: 0x000000C0                                  */
/* Register default value on SPRB0: 0x000000C0                                  */
/* Register default value on SPRHBM: 0x000000C0                                 */
/* Register default value on SPRC0: 0x000000C0                                  */
/* Register default value on SPRMCC: 0x000000C0                                 */
/* Register default value on SPRUCC: 0x000000C0                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             PIO_SECTION_OFFSET
*/


#define PIO_SECTION_OFFSET_SB_I3C0_REG 0x2A02803C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 section_offset : 16;

                            /* Bits[15:0], Access Type=RO, default=0x000000C0*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PIO_SECTION_OFFSET_SB_I3C0_STRUCT;

/* EXTCAPS_SECTION_OFFSET_SB_I3C0_REG supported on:                             */
/*      SPRA0 (0x200d8040)                                                      */
/*      SPRB0 (0x200d8040)                                                      */
/*      SPRHBM (0x200d8040)                                                     */
/*      SPRC0 (0x200d8040)                                                      */
/*      SPRMCC (0x200d8040)                                                     */
/*      SPRUCC (0x200d8040)                                                     */
/* Register default value on SPRA0: 0x00000200                                  */
/* Register default value on SPRB0: 0x00000200                                  */
/* Register default value on SPRHBM: 0x00000200                                 */
/* Register default value on SPRC0: 0x00000200                                  */
/* Register default value on SPRMCC: 0x00000200                                 */
/* Register default value on SPRUCC: 0x00000200                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*         EXTCAPS_SECTION_OFFSET
*/


#define EXTCAPS_SECTION_OFFSET_SB_I3C0_REG 0x2A028040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 section_offset : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000200*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} EXTCAPS_SECTION_OFFSET_SB_I3C0_STRUCT;

/* IBI_NOTIFY_CTRL_SB_I3C0_REG supported on:                                    */
/*      SPRA0 (0x200d8058)                                                      */
/*      SPRB0 (0x200d8058)                                                      */
/*      SPRHBM (0x200d8058)                                                     */
/*      SPRC0 (0x200d8058)                                                      */
/*      SPRMCC (0x200d8058)                                                     */
/*      SPRUCC (0x200d8058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                IBI_NOTIFY_CTRL
*/


#define IBI_NOTIFY_CTRL_SB_I3C0_REG 0x2A028058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 notify_hj_rejected : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 notify_mr_rejected : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_2_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 notify_sir_rejected : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_4_31 : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} IBI_NOTIFY_CTRL_SB_I3C0_STRUCT;

/* COMMAND_QUEUE_PORT_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d80c0)                                                      */
/*      SPRB0 (0x200d80c0)                                                      */
/*      SPRHBM (0x200d80c0)                                                     */
/*      SPRC0 (0x200d80c0)                                                      */
/*      SPRMCC (0x200d80c0)                                                     */
/*      SPRUCC (0x200d80c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             COMMAND_QUEUE_PORT
*/


#define COMMAND_QUEUE_PORT_SB_I3C0_REG 0x2A0280C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 command : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} COMMAND_QUEUE_PORT_SB_I3C0_STRUCT;

/* RESPONSE_QUEUE_PORT_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d80c4)                                                      */
/*      SPRB0 (0x200d80c4)                                                      */
/*      SPRHBM (0x200d80c4)                                                     */
/*      SPRC0 (0x200d80c4)                                                      */
/*      SPRMCC (0x200d80c4)                                                     */
/*      SPRUCC (0x200d80c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            RESPONSE_QUEUE_PORT
*/


#define RESPONSE_QUEUE_PORT_SB_I3C0_REG 0x2A0280C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data_length : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_23 : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 tid : 4;

                            /* Bits[27:24], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 err_status : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} RESPONSE_QUEUE_PORT_SB_I3C0_STRUCT;

/* DATA_PORT_SB_I3C0_REG supported on:                                          */
/*      SPRA0 (0x200d80c8)                                                      */
/*      SPRB0 (0x200d80c8)                                                      */
/*      SPRHBM (0x200d80c8)                                                     */
/*      SPRC0 (0x200d80c8)                                                      */
/*      SPRMCC (0x200d80c8)                                                     */
/*      SPRUCC (0x200d80c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                   TX_DATA_PORT
*/


#define DATA_PORT_SB_I3C0_REG 0x2A0280C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data_port : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} DATA_PORT_SB_I3C0_STRUCT;

/* IBI_PORT_SB_I3C0_REG supported on:                                           */
/*      SPRA0 (0x200d80cc)                                                      */
/*      SPRB0 (0x200d80cc)                                                      */
/*      SPRHBM (0x200d80cc)                                                     */
/*      SPRC0 (0x200d80cc)                                                      */
/*      SPRMCC (0x200d80cc)                                                     */
/*      SPRUCC (0x200d80cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                       IBI_PORT
*/


#define IBI_PORT_SB_I3C0_REG 0x2A0280CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ibi_data : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} IBI_PORT_SB_I3C0_STRUCT;

/* QUEUE_THLD_CTRL_SB_I3C0_REG supported on:                                    */
/*      SPRA0 (0x200d80d0)                                                      */
/*      SPRB0 (0x200d80d0)                                                      */
/*      SPRHBM (0x200d80d0)                                                     */
/*      SPRC0 (0x200d80d0)                                                      */
/*      SPRMCC (0x200d80d0)                                                     */
/*      SPRUCC (0x200d80d0)                                                     */
/* Register default value on SPRA0: 0x00200002                                  */
/* Register default value on SPRB0: 0x00200002                                  */
/* Register default value on SPRHBM: 0x00200002                                 */
/* Register default value on SPRC0: 0x00200002                                  */
/* Register default value on SPRMCC: 0x00200002                                 */
/* Register default value on SPRUCC: 0x00200002                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                QUEUE_THLD_CTRL
*/


#define QUEUE_THLD_CTRL_SB_I3C0_REG 0x2A0280D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_empty_buf_thld : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000002*/

                            /*  */
    UINT32 resp_buf_thld : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 ibi_data_thld : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000020*/

                            /*  */
    UINT32 ibi_status_thld : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} QUEUE_THLD_CTRL_SB_I3C0_STRUCT;

/* DATA_BUFFER_THLD_CTRL_SB_I3C0_REG supported on:                              */
/*      SPRA0 (0x200d80d4)                                                      */
/*      SPRB0 (0x200d80d4)                                                      */
/*      SPRHBM (0x200d80d4)                                                     */
/*      SPRC0 (0x200d80d4)                                                      */
/*      SPRMCC (0x200d80d4)                                                     */
/*      SPRUCC (0x200d80d4)                                                     */
/* Register default value on SPRA0: 0x01010404                                  */
/* Register default value on SPRB0: 0x01010404                                  */
/* Register default value on SPRHBM: 0x01010404                                 */
/* Register default value on SPRC0: 0x01010404                                  */
/* Register default value on SPRMCC: 0x01010404                                 */
/* Register default value on SPRUCC: 0x01010404                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*          DATA_BUFFER_THLD_CTRL
*/


#define DATA_BUFFER_THLD_CTRL_SB_I3C0_REG 0x2A0280D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_buf_thld : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000004*/

                            /*  */
    UINT32 reserved_3_7 : 5;

                            /* Bits[7:3], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 rx_buf_thld : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000004*/

                            /*  */
    UINT32 reserved_11_15 : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 tx_start_thld : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 reserved_19_23 : 5;

                            /* Bits[23:19], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 rx_start_thld : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 reserved_27_31 : 5;

                            /* Bits[31:27], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} DATA_BUFFER_THLD_CTRL_SB_I3C0_STRUCT;

/* QUEUE_SIZE_CTRL_SB_I3C0_REG supported on:                                    */
/*      SPRA0 (0x200d80d8)                                                      */
/*      SPRB0 (0x200d80d8)                                                      */
/*      SPRHBM (0x200d80d8)                                                     */
/*      SPRC0 (0x200d80d8)                                                      */
/*      SPRMCC (0x200d80d8)                                                     */
/*      SPRUCC (0x200d80d8)                                                     */
/* Register default value on SPRA0: 0x05051010                                  */
/* Register default value on SPRB0: 0x05051010                                  */
/* Register default value on SPRHBM: 0x05051010                                 */
/* Register default value on SPRC0: 0x05051010                                  */
/* Register default value on SPRMCC: 0x05051010                                 */
/* Register default value on SPRUCC: 0x05051010                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                QUEUE_SIZE_CTRL
*/


#define QUEUE_SIZE_CTRL_SB_I3C0_REG 0x2A0280D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cr_queue_size : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*  */
    UINT32 ibi_status_size : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000010*/

                            /*  */
    UINT32 rx_data_buffer_size : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000005*/

                            /*  */
    UINT32 tx_data_buffer_size : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000005*/

                            /*  */

  } Bits;
  UINT32 Data;

} QUEUE_SIZE_CTRL_SB_I3C0_STRUCT;

/* HW_IDENTIFICATION_HEADER_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d8200)                                                      */
/*      SPRB0 (0x200d8200)                                                      */
/*      SPRHBM (0x200d8200)                                                     */
/*      SPRC0 (0x200d8200)                                                      */
/*      SPRMCC (0x200d8200)                                                     */
/*      SPRUCC (0x200d8200)                                                     */
/* Register default value on SPRA0: 0x00000401                                  */
/* Register default value on SPRB0: 0x00000401                                  */
/* Register default value on SPRHBM: 0x00000401                                 */
/* Register default value on SPRC0: 0x00000401                                  */
/* Register default value on SPRMCC: 0x00000401                                 */
/* Register default value on SPRUCC: 0x00000401                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       HW_IDENTIFICATION_HEADER
*/


#define HW_IDENTIFICATION_HEADER_SB_I3C0_REG 0x2A028200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 cap_len : 16;

                            /* Bits[23:8], Access Type=RO, default=0x00000004*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} HW_IDENTIFICATION_HEADER_SB_I3C0_STRUCT;

/* COMP_MANUFACTURER_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d8204)                                                      */
/*      SPRB0 (0x200d8204)                                                      */
/*      SPRHBM (0x200d8204)                                                     */
/*      SPRC0 (0x200d8204)                                                      */
/*      SPRMCC (0x200d8204)                                                     */
/*      SPRUCC (0x200d8204)                                                     */
/* Register default value on SPRA0: 0x00000105                                  */
/* Register default value on SPRB0: 0x00000105                                  */
/* Register default value on SPRHBM: 0x00000105                                 */
/* Register default value on SPRC0: 0x00000105                                  */
/* Register default value on SPRMCC: 0x00000105                                 */
/* Register default value on SPRUCC: 0x00000105                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              COMP_MANUFACTURER
*/


#define COMP_MANUFACTURER_SB_I3C0_REG 0x2A028204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mipi_vendor_id : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000105*/

                            /*  */

  } Bits;
  UINT32 Data;

} COMP_MANUFACTURER_SB_I3C0_STRUCT;

/* COMP_VERSION_SB_I3C0_REG supported on:                                       */
/*      SPRA0 (0x200d8208)                                                      */
/*      SPRB0 (0x200d8208)                                                      */
/*      SPRHBM (0x200d8208)                                                     */
/*      SPRC0 (0x200d8208)                                                      */
/*      SPRMCC (0x200d8208)                                                     */
/*      SPRUCC (0x200d8208)                                                     */
/* Register default value on SPRA0: 0x3130302A                                  */
/* Register default value on SPRB0: 0x3130302A                                  */
/* Register default value on SPRHBM: 0x3130302A                                 */
/* Register default value on SPRC0: 0x3130302A                                  */
/* Register default value on SPRMCC: 0x3130302A                                 */
/* Register default value on SPRUCC: 0x3130302A                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                   COMP_VERSION
*/


#define COMP_VERSION_SB_I3C0_REG 0x2A028208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_ver_id : 32;

                            /* Bits[31:0], Access Type=RO, default=0x3130302A*/

                            /*  */

  } Bits;
  UINT32 Data;

} COMP_VERSION_SB_I3C0_STRUCT;

/* COMP_TYPE_SB_I3C0_REG supported on:                                          */
/*      SPRA0 (0x200d820c)                                                      */
/*      SPRB0 (0x200d820c)                                                      */
/*      SPRHBM (0x200d820c)                                                     */
/*      SPRC0 (0x200d820c)                                                      */
/*      SPRMCC (0x200d820c)                                                     */
/*      SPRUCC (0x200d820c)                                                     */
/* Register default value on SPRA0: 0x65613631                                  */
/* Register default value on SPRB0: 0x65613631                                  */
/* Register default value on SPRHBM: 0x65613631                                 */
/* Register default value on SPRC0: 0x65613631                                  */
/* Register default value on SPRMCC: 0x65613631                                 */
/* Register default value on SPRUCC: 0x65613631                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                      COMP_TYPE
*/


#define COMP_TYPE_SB_I3C0_REG 0x2A02820C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_ver_type : 32;

                            /* Bits[31:0], Access Type=RO, default=0x65613631*/

                            /*  */

  } Bits;
  UINT32 Data;

} COMP_TYPE_SB_I3C0_STRUCT;

/* BUS_TIMING_HEADER_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d8210)                                                      */
/*      SPRB0 (0x200d8210)                                                      */
/*      SPRHBM (0x200d8210)                                                     */
/*      SPRC0 (0x200d8210)                                                      */
/*      SPRMCC (0x200d8210)                                                     */
/*      SPRUCC (0x200d8210)                                                     */
/* Register default value on SPRA0: 0x00000CC0                                  */
/* Register default value on SPRB0: 0x00000CC0                                  */
/* Register default value on SPRHBM: 0x00000CC0                                 */
/* Register default value on SPRC0: 0x00000CC0                                  */
/* Register default value on SPRMCC: 0x00000CC0                                 */
/* Register default value on SPRUCC: 0x00000CC0                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              BUS_TIMING_HEADER
*/


#define BUS_TIMING_HEADER_SB_I3C0_REG 0x2A028210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000C0*/

                            /*  */
    UINT32 cap_len : 16;

                            /* Bits[23:8], Access Type=RO, default=0x0000000C*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} BUS_TIMING_HEADER_SB_I3C0_STRUCT;

/* SCL_I3C_OD_TIMING_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d8214)                                                      */
/*      SPRB0 (0x200d8214)                                                      */
/*      SPRHBM (0x200d8214)                                                     */
/*      SPRC0 (0x200d8214)                                                      */
/*      SPRMCC (0x200d8214)                                                     */
/*      SPRUCC (0x200d8214)                                                     */
/* Register default value on SPRA0: 0x00FA00FA                                  */
/* Register default value on SPRB0: 0x00FA00FA                                  */
/* Register default value on SPRHBM: 0x00FA00FA                                 */
/* Register default value on SPRC0: 0x00FA00FA                                  */
/* Register default value on SPRMCC: 0x00FA00FA                                 */
/* Register default value on SPRUCC: 0x00FA00FA                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              SCL_I3C_OD_TIMING
*/


#define SCL_I3C_OD_TIMING_SB_I3C0_REG 0x2A028214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_od_lcnt : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000FA*/

                            /*  */
    UINT32 reserved_8_15 : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 i3c_od_hcnt : 8;

                            /* Bits[23:16], Access Type=RW, default=0x000000FA*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_I3C_OD_TIMING_SB_I3C0_STRUCT;

/* SCL_I3C_PP_TIMING_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d8218)                                                      */
/*      SPRB0 (0x200d8218)                                                      */
/*      SPRHBM (0x200d8218)                                                     */
/*      SPRC0 (0x200d8218)                                                      */
/*      SPRMCC (0x200d8218)                                                     */
/*      SPRUCC (0x200d8218)                                                     */
/* Register default value on SPRA0: 0x00080008                                  */
/* Register default value on SPRB0: 0x00080008                                  */
/* Register default value on SPRHBM: 0x00080008                                 */
/* Register default value on SPRC0: 0x00080008                                  */
/* Register default value on SPRMCC: 0x00080008                                 */
/* Register default value on SPRUCC: 0x00080008                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              SCL_I3C_PP_TIMING
*/


#define SCL_I3C_PP_TIMING_SB_I3C0_REG 0x2A028218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_pp_lcnt : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000008*/

                            /*  */
    UINT32 reserved_8_15 : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 i3c_pp_hcnt : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000008*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_I3C_PP_TIMING_SB_I3C0_STRUCT;

/* SCL_I2C_FM_TIMING_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d821c)                                                      */
/*      SPRB0 (0x200d821c)                                                      */
/*      SPRHBM (0x200d821c)                                                     */
/*      SPRC0 (0x200d821c)                                                      */
/*      SPRMCC (0x200d821c)                                                     */
/*      SPRUCC (0x200d821c)                                                     */
/* Register default value on SPRA0: 0x00FA00FA                                  */
/* Register default value on SPRB0: 0x00FA00FA                                  */
/* Register default value on SPRHBM: 0x00FA00FA                                 */
/* Register default value on SPRC0: 0x00FA00FA                                  */
/* Register default value on SPRMCC: 0x00FA00FA                                 */
/* Register default value on SPRUCC: 0x00FA00FA                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              SCL_I2C_FM_TIMING
*/


#define SCL_I2C_FM_TIMING_SB_I3C0_REG 0x2A02821C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i2c_fm_lcnt : 16;

                            /* Bits[15:0], Access Type=RW, default=0x000000FA*/

                            /*  */
    UINT32 i2c_fm_hcnt : 8;

                            /* Bits[23:16], Access Type=RW, default=0x000000FA*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_I2C_FM_TIMING_SB_I3C0_STRUCT;

/* SCL_I2C_FMP_TIMING_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d8220)                                                      */
/*      SPRB0 (0x200d8220)                                                      */
/*      SPRHBM (0x200d8220)                                                     */
/*      SPRC0 (0x200d8220)                                                      */
/*      SPRMCC (0x200d8220)                                                     */
/*      SPRUCC (0x200d8220)                                                     */
/* Register default value on SPRA0: 0x00640064                                  */
/* Register default value on SPRB0: 0x00640064                                  */
/* Register default value on SPRHBM: 0x00640064                                 */
/* Register default value on SPRC0: 0x00640064                                  */
/* Register default value on SPRMCC: 0x00640064                                 */
/* Register default value on SPRUCC: 0x00640064                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             SCL_I2C_FMP_TIMING
*/


#define SCL_I2C_FMP_TIMING_SB_I3C0_REG 0x2A028220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i2c_fmp_lcnt : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000064*/

                            /*  */
    UINT32 reserved_8_15 : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 i2c_fmp_hcnt : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000064*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_I2C_FMP_TIMING_SB_I3C0_STRUCT;

/* SCL_I2C_SS_TIMING_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d8224)                                                      */
/*      SPRB0 (0x200d8224)                                                      */
/*      SPRHBM (0x200d8224)                                                     */
/*      SPRC0 (0x200d8224)                                                      */
/*      SPRMCC (0x200d8224)                                                     */
/*      SPRUCC (0x200d8224)                                                     */
/* Register default value on SPRA0: 0x032003AC                                  */
/* Register default value on SPRB0: 0x032003AC                                  */
/* Register default value on SPRHBM: 0x032003AC                                 */
/* Register default value on SPRC0: 0x032003AC                                  */
/* Register default value on SPRMCC: 0x032003AC                                 */
/* Register default value on SPRUCC: 0x032003AC                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              SCL_I2C_SS_TIMING
*/


#define SCL_I2C_SS_TIMING_SB_I3C0_REG 0x2A028224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i2c_ss_lcnt : 16;

                            /* Bits[15:0], Access Type=RW, default=0x000003AC*/

                            /*  */
    UINT32 i2c_ss_hcnt : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000320*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_I2C_SS_TIMING_SB_I3C0_STRUCT;

/* SCL_EXT_LCNT_TIMING_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8228)                                                      */
/*      SPRB0 (0x200d8228)                                                      */
/*      SPRHBM (0x200d8228)                                                     */
/*      SPRC0 (0x200d8228)                                                      */
/*      SPRMCC (0x200d8228)                                                     */
/*      SPRUCC (0x200d8228)                                                     */
/* Register default value on SPRA0: 0x20202020                                  */
/* Register default value on SPRB0: 0x20202020                                  */
/* Register default value on SPRHBM: 0x20202020                                 */
/* Register default value on SPRC0: 0x20202020                                  */
/* Register default value on SPRMCC: 0x20202020                                 */
/* Register default value on SPRUCC: 0x20202020                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            SCL_EXT_LCNT_TIMING
*/


#define SCL_EXT_LCNT_TIMING_SB_I3C0_REG 0x2A028228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_ext_lcnt_1 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000020*/

                            /*  */
    UINT32 i3c_ext_lcnt_2 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000020*/

                            /*  */
    UINT32 i3c_ext_lcnt_3 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000020*/

                            /*  */
    UINT32 i3c_ext_lcnt_4 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000020*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_EXT_LCNT_TIMING_SB_I3C0_STRUCT;

/* SCL_EXT_TERMN_LCNT_TIMING_SB_I3C0_REG supported on:                          */
/*      SPRA0 (0x200d822c)                                                      */
/*      SPRB0 (0x200d822c)                                                      */
/*      SPRHBM (0x200d822c)                                                     */
/*      SPRC0 (0x200d822c)                                                      */
/*      SPRMCC (0x200d822c)                                                     */
/*      SPRUCC (0x200d822c)                                                     */
/* Register default value on SPRA0: 0x00030000                                  */
/* Register default value on SPRB0: 0x00030000                                  */
/* Register default value on SPRHBM: 0x00030000                                 */
/* Register default value on SPRC0: 0x00030000                                  */
/* Register default value on SPRMCC: 0x00030000                                 */
/* Register default value on SPRUCC: 0x00030000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*      SCL_EXT_TERMN_LCNT_TIMING
*/


#define SCL_EXT_TERMN_LCNT_TIMING_SB_I3C0_REG 0x2A02822C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_ext_termn_lcnt : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_4_15 : 12;

                            /* Bits[15:4], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 i3c_ts_skew_cnt : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000003*/

                            /*  */
    UINT32 reserved_20_31 : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_EXT_TERMN_LCNT_TIMING_SB_I3C0_STRUCT;

/* SDA_HOLD_SWITCH_DLY_TIMING_SB_I3C0_REG supported on:                         */
/*      SPRA0 (0x200d8230)                                                      */
/*      SPRB0 (0x200d8230)                                                      */
/*      SPRHBM (0x200d8230)                                                     */
/*      SPRC0 (0x200d8230)                                                      */
/*      SPRMCC (0x200d8230)                                                     */
/*      SPRUCC (0x200d8230)                                                     */
/* Register default value on SPRA0: 0x00010000                                  */
/* Register default value on SPRB0: 0x00010000                                  */
/* Register default value on SPRHBM: 0x00010000                                 */
/* Register default value on SPRC0: 0x00010000                                  */
/* Register default value on SPRMCC: 0x00010000                                 */
/* Register default value on SPRUCC: 0x00010000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*     SDA_HOLD_SWITCH_DLY_TIMING
*/


#define SDA_HOLD_SWITCH_DLY_TIMING_SB_I3C0_REG 0x2A028230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sda_od_pp_switch_dly : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_3_7 : 5;

                            /* Bits[7:3], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 sda_pp_od_switch_dly : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_11_15 : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 sda_tx_hold : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 reserved_19_23 : 5;

                            /* Bits[23:19], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 sda_rx_hold : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_27_31 : 5;

                            /* Bits[31:27], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SDA_HOLD_SWITCH_DLY_TIMING_SB_I3C0_STRUCT;

/* BUS_FREE_TIMING_SB_I3C0_REG supported on:                                    */
/*      SPRA0 (0x200d8234)                                                      */
/*      SPRB0 (0x200d8234)                                                      */
/*      SPRHBM (0x200d8234)                                                     */
/*      SPRC0 (0x200d8234)                                                      */
/*      SPRMCC (0x200d8234)                                                     */
/*      SPRUCC (0x200d8234)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                BUS_FREE_TIMING
*/


#define BUS_FREE_TIMING_SB_I3C0_REG 0x2A028234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 i3c_mst_free : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000008*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} BUS_FREE_TIMING_SB_I3C0_STRUCT;

/* SLOT_TICK_TIME_CNT_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d8238)                                                      */
/*      SPRB0 (0x200d8238)                                                      */
/*      SPRHBM (0x200d8238)                                                     */
/*      SPRC0 (0x200d8238)                                                      */
/*      SPRMCC (0x200d8238)                                                     */
/*      SPRUCC (0x200d8238)                                                     */
/* Register default value on SPRA0: 0x000186A0                                  */
/* Register default value on SPRB0: 0x000186A0                                  */
/* Register default value on SPRHBM: 0x000186A0                                 */
/* Register default value on SPRC0: 0x000186A0                                  */
/* Register default value on SPRMCC: 0x000186A0                                 */
/* Register default value on SPRUCC: 0x000186A0                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             SLOT_TICK_TIME_CNT
*/


#define SLOT_TICK_TIME_CNT_SB_I3C0_REG 0x2A028238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tick_time_count : 20;

                            /* Bits[19:0], Access Type=RW, default=0x000186A0*/

                            /*  */
    UINT32 reserved_20_31 : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SLOT_TICK_TIME_CNT_SB_I3C0_STRUCT;

/* SCL_LOW_MST_EXT_TIMEOUT_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d823c)                                                      */
/*      SPRB0 (0x200d823c)                                                      */
/*      SPRHBM (0x200d823c)                                                     */
/*      SPRC0 (0x200d823c)                                                      */
/*      SPRMCC (0x200d823c)                                                     */
/*      SPRUCC (0x200d823c)                                                     */
/* Register default value on SPRA0: 0x003567E0                                  */
/* Register default value on SPRB0: 0x003567E0                                  */
/* Register default value on SPRHBM: 0x003567E0                                 */
/* Register default value on SPRC0: 0x003567E0                                  */
/* Register default value on SPRMCC: 0x003567E0                                 */
/* Register default value on SPRUCC: 0x003567E0                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        SCL_LOW_MST_EXT_TIMEOUT
*/


#define SCL_LOW_MST_EXT_TIMEOUT_SB_I3C0_REG 0x2A02823C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scl_low_mst_timeout_count : 26;

                            /* Bits[25:0], Access Type=RW, default=0x003567E0*/

                            /*  */
    UINT32 reserved_26_31 : 6;

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} SCL_LOW_MST_EXT_TIMEOUT_SB_I3C0_STRUCT;

/* DS_EXTCAP_HEADER_SB_I3C0_REG supported on:                                   */
/*      SPRA0 (0x200d8240)                                                      */
/*      SPRB0 (0x200d8240)                                                      */
/*      SPRHBM (0x200d8240)                                                     */
/*      SPRC0 (0x200d8240)                                                      */
/*      SPRMCC (0x200d8240)                                                     */
/*      SPRUCC (0x200d8240)                                                     */
/* Register default value on SPRA0: 0x0000050C                                  */
/* Register default value on SPRB0: 0x0000050C                                  */
/* Register default value on SPRHBM: 0x0000050C                                 */
/* Register default value on SPRC0: 0x0000050C                                  */
/* Register default value on SPRMCC: 0x0000050C                                 */
/* Register default value on SPRUCC: 0x0000050C                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*               DS_EXTCAP_HEADER
*/


#define DS_EXTCAP_HEADER_SB_I3C0_REG 0x2A028240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x0000000C*/

                            /*  */
    UINT32 cap_len : 16;

                            /* Bits[23:8], Access Type=RO, default=0x00000005*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} DS_EXTCAP_HEADER_SB_I3C0_STRUCT;

/* QUEUE_STATUS_LEVEL_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d8244)                                                      */
/*      SPRB0 (0x200d8244)                                                      */
/*      SPRHBM (0x200d8244)                                                     */
/*      SPRC0 (0x200d8244)                                                      */
/*      SPRMCC (0x200d8244)                                                     */
/*      SPRUCC (0x200d8244)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*             QUEUE_STATUS_LEVEL
*/


#define QUEUE_STATUS_LEVEL_SB_I3C0_REG 0x2A028244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_queue_free_lvl : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*  */
    UINT32 resp_buf_blr : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ibi_buf_blr : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ibi_status_cnt : 5;

                            /* Bits[28:24], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_29_31 : 3;

                            /* Bits[31:29], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} QUEUE_STATUS_LEVEL_SB_I3C0_STRUCT;

/* DATA_BUFFER_STATUS_LEVEL_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d8248)                                                      */
/*      SPRB0 (0x200d8248)                                                      */
/*      SPRHBM (0x200d8248)                                                     */
/*      SPRC0 (0x200d8248)                                                      */
/*      SPRMCC (0x200d8248)                                                     */
/*      SPRUCC (0x200d8248)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       DATA_BUFFER_STATUS_LEVEL
*/


#define DATA_BUFFER_STATUS_LEVEL_SB_I3C0_REG 0x2A028248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_buf_free_lvl : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

                            /*  */
    UINT32 rx_buf_lvl : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} DATA_BUFFER_STATUS_LEVEL_SB_I3C0_STRUCT;

/* PRESENT_STATE_DEBUG_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d824c)                                                      */
/*      SPRB0 (0x200d824c)                                                      */
/*      SPRHBM (0x200d824c)                                                     */
/*      SPRC0 (0x200d824c)                                                      */
/*      SPRMCC (0x200d824c)                                                     */
/*      SPRUCC (0x200d824c)                                                     */
/* Register default value on SPRA0: 0x00000003                                  */
/* Register default value on SPRB0: 0x00000003                                  */
/* Register default value on SPRHBM: 0x00000003                                 */
/* Register default value on SPRC0: 0x00000003                                  */
/* Register default value on SPRMCC: 0x00000003                                 */
/* Register default value on SPRUCC: 0x00000003                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PRESENT_STATE_DEBUG
*/


#define PRESENT_STATE_DEBUG_SB_I3C0_REG 0x2A02824C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scl_line_signal_level : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 sda_line_signal_level : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 reserved_2_7 : 6;

                            /* Bits[7:2], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 cm_tfr_status : 6;

                            /* Bits[13:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_14_15 : 2;

                            /* Bits[15:14], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 cm_tfr_st_status : 6;

                            /* Bits[21:16], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_22_23 : 2;

                            /* Bits[23:22], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 cmd_tid : 4;

                            /* Bits[27:24], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_28_31 : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PRESENT_STATE_DEBUG_SB_I3C0_STRUCT;

/* MASTER_EXT_HEADER_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d8254)                                                      */
/*      SPRB0 (0x200d8254)                                                      */
/*      SPRHBM (0x200d8254)                                                     */
/*      SPRC0 (0x200d8254)                                                      */
/*      SPRMCC (0x200d8254)                                                     */
/*      SPRUCC (0x200d8254)                                                     */
/* Register default value on SPRA0: 0x00001002                                  */
/* Register default value on SPRB0: 0x00001002                                  */
/* Register default value on SPRHBM: 0x00001002                                 */
/* Register default value on SPRC0: 0x00001002                                  */
/* Register default value on SPRMCC: 0x00001002                                 */
/* Register default value on SPRUCC: 0x00001002                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*              MASTER_EXT_HEADER
*/


#define MASTER_EXT_HEADER_SB_I3C0_REG 0x2A028254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000002*/

                            /*  */
    UINT32 cap_len : 16;

                            /* Bits[23:8], Access Type=RO, default=0x00000010*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} MASTER_EXT_HEADER_SB_I3C0_STRUCT;

/* MASTER_CONFIG_SB_I3C0_REG supported on:                                      */
/*      SPRA0 (0x200d8258)                                                      */
/*      SPRB0 (0x200d8258)                                                      */
/*      SPRHBM (0x200d8258)                                                     */
/*      SPRC0 (0x200d8258)                                                      */
/*      SPRMCC (0x200d8258)                                                     */
/*      SPRUCC (0x200d8258)                                                     */
/* Register default value on SPRA0: 0x00000039                                  */
/* Register default value on SPRB0: 0x00000039                                  */
/* Register default value on SPRHBM: 0x00000039                                 */
/* Register default value on SPRC0: 0x00000039                                  */
/* Register default value on SPRMCC: 0x00000039                                 */
/* Register default value on SPRUCC: 0x00000039                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*                  MASTER_CONFIG
*/


#define MASTER_CONFIG_SB_I3C0_REG 0x2A028258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 app_if_mode : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 app_if_data_width : 2;

                            /* Bits[3:2], Access Type=RO, default=0x00000002*/

                            /*  */
    UINT32 operation_mode : 2;

                            /* Bits[5:4], Access Type=RO, default=0x00000003*/

                            /*  */
    UINT32 reserved_6_31 : 26;

                            /* Bits[31:6], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} MASTER_CONFIG_SB_I3C0_STRUCT;

/* PERIODIC_POLL_HEADER_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d8260)                                                      */
/*      SPRB0 (0x200d8260)                                                      */
/*      SPRHBM (0x200d8260)                                                     */
/*      SPRC0 (0x200d8260)                                                      */
/*      SPRMCC (0x200d8260)                                                     */
/*      SPRUCC (0x200d8260)                                                     */
/* Register default value on SPRA0: 0x00019C04                                  */
/* Register default value on SPRB0: 0x00019C04                                  */
/* Register default value on SPRHBM: 0x00019C04                                 */
/* Register default value on SPRC0: 0x00019C04                                  */
/* Register default value on SPRMCC: 0x00019C04                                 */
/* Register default value on SPRUCC: 0x00019C04                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_HEADER
*/


#define PERIODIC_POLL_HEADER_SB_I3C0_REG 0x2A028260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000004*/

                            /*  */
    UINT32 cap_length : 16;

                            /* Bits[23:8], Access Type=RO, default=0x0000019C*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_HEADER_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CAPABILITY_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d8264)                                                      */
/*      SPRB0 (0x200d8264)                                                      */
/*      SPRHBM (0x200d8264)                                                     */
/*      SPRC0 (0x200d8264)                                                      */
/*      SPRMCC (0x200d8264)                                                     */
/*      SPRUCC (0x200d8264)                                                     */
/* Register default value on SPRA0: 0x00AF0F01                                  */
/* Register default value on SPRB0: 0x00AF0F01                                  */
/* Register default value on SPRHBM: 0x00AF0F01                                 */
/* Register default value on SPRC0: 0x00AF0F01                                  */
/* Register default value on SPRMCC: 0x00AF0F01                                 */
/* Register default value on SPRUCC: 0x00AF0F01                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CAPABILITY
*/


#define PERIODIC_POLL_CAPABILITY_SB_I3C0_REG 0x2A028264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tick_clk_period : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /*  */
    UINT32 max_slot_range : 4;

                            /* Bits[11:8], Access Type=RO, default=0x0000000F*/

                            /*  */
    UINT32 reserved_12_15 : 4;

                            /* Bits[15:12], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 max_poll_table_depth : 4;

                            /* Bits[19:16], Access Type=RO, default=0x0000000F*/

                            /*  */
    UINT32 reserved_20_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 max_poll_cmd_dl : 3;

                            /* Bits[23:21], Access Type=RO, default=0x00000005*/

                            /*  */
    UINT32 reserved_24_31 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CAPABILITY_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CONTROL_SB_I3C0_REG supported on:                              */
/*      SPRA0 (0x200d8268)                                                      */
/*      SPRB0 (0x200d8268)                                                      */
/*      SPRHBM (0x200d8268)                                                     */
/*      SPRC0 (0x200d8268)                                                      */
/*      SPRMCC (0x200d8268)                                                     */
/*      SPRUCC (0x200d8268)                                                     */
/* Register default value on SPRA0: 0x00784001                                  */
/* Register default value on SPRB0: 0x00784001                                  */
/* Register default value on SPRHBM: 0x00784001                                 */
/* Register default value on SPRC0: 0x00784001                                  */
/* Register default value on SPRMCC: 0x00784001                                 */
/* Register default value on SPRUCC: 0x00784001                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*          PERIODIC_POLL_CONTROL
*/


#define PERIODIC_POLL_CONTROL_SB_I3C0_REG 0x2A028268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot_interval_count : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 reserved_12_13 : 2;

                            /* Bits[13:12], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 slot_range_rcount : 5;

                            /* Bits[18:14], Access Type=RW, default=0x00000001*/

                            /*  */
    UINT32 slot_range : 4;

                            /* Bits[22:19], Access Type=RW, default=0x0000000F*/

                            /*  */
    UINT32 reserved_23_31 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CONTROL_SB_I3C0_STRUCT;

/* PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_REG supported on:                       */
/*      SPRA0 (0x200d826c)                                                      */
/*      SPRB0 (0x200d826c)                                                      */
/*      SPRHBM (0x200d826c)                                                     */
/*      SPRC0 (0x200d826c)                                                      */
/*      SPRMCC (0x200d826c)                                                     */
/*      SPRUCC (0x200d826c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*   PERIODIC_POLL_COMMAND_ENABLE
*/


#define PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_REG 0x2A02826C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd_en : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_COMMAND_ENABLE_SB_I3C0_STRUCT;

/* PERIODIC_POLL_DEBUG_REGISTER_SB_I3C0_REG supported on:                       */
/*      SPRA0 (0x200d8270)                                                      */
/*      SPRB0 (0x200d8270)                                                      */
/*      SPRHBM (0x200d8270)                                                     */
/*      SPRC0 (0x200d8270)                                                      */
/*      SPRMCC (0x200d8270)                                                     */
/*      SPRUCC (0x200d8270)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*   PERIODIC_POLL_DEBUG_REGISTER
*/


#define PERIODIC_POLL_DEBUG_REGISTER_SB_I3C0_REG 0x2A028270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot_id : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_4_7 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 ppcmd_id : 4;

                            /* Bits[11:8], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 reserved_12_30 : 19;

                            /* Bits[30:12], Access Type=RO, default=0x00000000*/

                            /*  */
    UINT32 debug_data_valid : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_DEBUG_REGISTER_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD0_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d8280)                                                      */
/*      SPRB0 (0x200d8280)                                                      */
/*      SPRHBM (0x200d8280)                                                     */
/*      SPRC0 (0x200d8280)                                                      */
/*      SPRMCC (0x200d8280)                                                     */
/*      SPRUCC (0x200d8280)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD0_LOC1
*/


#define PERIODIC_POLL_CMD0_LOC1_SB_I3C0_REG 0x2A028280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd0_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD0_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD0_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d8284)                                                      */
/*      SPRB0 (0x200d8284)                                                      */
/*      SPRHBM (0x200d8284)                                                     */
/*      SPRC0 (0x200d8284)                                                      */
/*      SPRMCC (0x200d8284)                                                     */
/*      SPRUCC (0x200d8284)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD0_LOC2
*/


#define PERIODIC_POLL_CMD0_LOC2_SB_I3C0_REG 0x2A028284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd0_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD0_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD1_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d8288)                                                      */
/*      SPRB0 (0x200d8288)                                                      */
/*      SPRHBM (0x200d8288)                                                     */
/*      SPRC0 (0x200d8288)                                                      */
/*      SPRMCC (0x200d8288)                                                     */
/*      SPRUCC (0x200d8288)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD1_LOC1
*/


#define PERIODIC_POLL_CMD1_LOC1_SB_I3C0_REG 0x2A028288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd1_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD1_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD1_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d828c)                                                      */
/*      SPRB0 (0x200d828c)                                                      */
/*      SPRHBM (0x200d828c)                                                     */
/*      SPRC0 (0x200d828c)                                                      */
/*      SPRMCC (0x200d828c)                                                     */
/*      SPRUCC (0x200d828c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD1_LOC2
*/


#define PERIODIC_POLL_CMD1_LOC2_SB_I3C0_REG 0x2A02828C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd1_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD1_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD2_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d8290)                                                      */
/*      SPRB0 (0x200d8290)                                                      */
/*      SPRHBM (0x200d8290)                                                     */
/*      SPRC0 (0x200d8290)                                                      */
/*      SPRMCC (0x200d8290)                                                     */
/*      SPRUCC (0x200d8290)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD2_LOC1
*/


#define PERIODIC_POLL_CMD2_LOC1_SB_I3C0_REG 0x2A028290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd2_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD2_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD2_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d8294)                                                      */
/*      SPRB0 (0x200d8294)                                                      */
/*      SPRHBM (0x200d8294)                                                     */
/*      SPRC0 (0x200d8294)                                                      */
/*      SPRMCC (0x200d8294)                                                     */
/*      SPRUCC (0x200d8294)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD2_LOC2
*/


#define PERIODIC_POLL_CMD2_LOC2_SB_I3C0_REG 0x2A028294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd2_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD2_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD3_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d8298)                                                      */
/*      SPRB0 (0x200d8298)                                                      */
/*      SPRHBM (0x200d8298)                                                     */
/*      SPRC0 (0x200d8298)                                                      */
/*      SPRMCC (0x200d8298)                                                     */
/*      SPRUCC (0x200d8298)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD3_LOC1
*/


#define PERIODIC_POLL_CMD3_LOC1_SB_I3C0_REG 0x2A028298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd3_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD3_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD3_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d829c)                                                      */
/*      SPRB0 (0x200d829c)                                                      */
/*      SPRHBM (0x200d829c)                                                     */
/*      SPRC0 (0x200d829c)                                                      */
/*      SPRMCC (0x200d829c)                                                     */
/*      SPRUCC (0x200d829c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD3_LOC2
*/


#define PERIODIC_POLL_CMD3_LOC2_SB_I3C0_REG 0x2A02829C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd3_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD3_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD4_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82a0)                                                      */
/*      SPRB0 (0x200d82a0)                                                      */
/*      SPRHBM (0x200d82a0)                                                     */
/*      SPRC0 (0x200d82a0)                                                      */
/*      SPRMCC (0x200d82a0)                                                     */
/*      SPRUCC (0x200d82a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD4_LOC1
*/


#define PERIODIC_POLL_CMD4_LOC1_SB_I3C0_REG 0x2A0282A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd4_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD4_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD4_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82a4)                                                      */
/*      SPRB0 (0x200d82a4)                                                      */
/*      SPRHBM (0x200d82a4)                                                     */
/*      SPRC0 (0x200d82a4)                                                      */
/*      SPRMCC (0x200d82a4)                                                     */
/*      SPRUCC (0x200d82a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD4_LOC2
*/


#define PERIODIC_POLL_CMD4_LOC2_SB_I3C0_REG 0x2A0282A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd4_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD4_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD5_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82a8)                                                      */
/*      SPRB0 (0x200d82a8)                                                      */
/*      SPRHBM (0x200d82a8)                                                     */
/*      SPRC0 (0x200d82a8)                                                      */
/*      SPRMCC (0x200d82a8)                                                     */
/*      SPRUCC (0x200d82a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD5_LOC1
*/


#define PERIODIC_POLL_CMD5_LOC1_SB_I3C0_REG 0x2A0282A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd5_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD5_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD5_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82ac)                                                      */
/*      SPRB0 (0x200d82ac)                                                      */
/*      SPRHBM (0x200d82ac)                                                     */
/*      SPRC0 (0x200d82ac)                                                      */
/*      SPRMCC (0x200d82ac)                                                     */
/*      SPRUCC (0x200d82ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD5_LOC2
*/


#define PERIODIC_POLL_CMD5_LOC2_SB_I3C0_REG 0x2A0282AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd5_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD5_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD6_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82b0)                                                      */
/*      SPRB0 (0x200d82b0)                                                      */
/*      SPRHBM (0x200d82b0)                                                     */
/*      SPRC0 (0x200d82b0)                                                      */
/*      SPRMCC (0x200d82b0)                                                     */
/*      SPRUCC (0x200d82b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD6_LOC1
*/


#define PERIODIC_POLL_CMD6_LOC1_SB_I3C0_REG 0x2A0282B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd6_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD6_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD6_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82b4)                                                      */
/*      SPRB0 (0x200d82b4)                                                      */
/*      SPRHBM (0x200d82b4)                                                     */
/*      SPRC0 (0x200d82b4)                                                      */
/*      SPRMCC (0x200d82b4)                                                     */
/*      SPRUCC (0x200d82b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD6_LOC2
*/


#define PERIODIC_POLL_CMD6_LOC2_SB_I3C0_REG 0x2A0282B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd6_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD6_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD7_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82b8)                                                      */
/*      SPRB0 (0x200d82b8)                                                      */
/*      SPRHBM (0x200d82b8)                                                     */
/*      SPRC0 (0x200d82b8)                                                      */
/*      SPRMCC (0x200d82b8)                                                     */
/*      SPRUCC (0x200d82b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD7_LOC1
*/


#define PERIODIC_POLL_CMD7_LOC1_SB_I3C0_REG 0x2A0282B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd7_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD7_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD7_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82bc)                                                      */
/*      SPRB0 (0x200d82bc)                                                      */
/*      SPRHBM (0x200d82bc)                                                     */
/*      SPRC0 (0x200d82bc)                                                      */
/*      SPRMCC (0x200d82bc)                                                     */
/*      SPRUCC (0x200d82bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD7_LOC2
*/


#define PERIODIC_POLL_CMD7_LOC2_SB_I3C0_REG 0x2A0282BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd7_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD7_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD8_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82c0)                                                      */
/*      SPRB0 (0x200d82c0)                                                      */
/*      SPRHBM (0x200d82c0)                                                     */
/*      SPRC0 (0x200d82c0)                                                      */
/*      SPRMCC (0x200d82c0)                                                     */
/*      SPRUCC (0x200d82c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD8_LOC1
*/


#define PERIODIC_POLL_CMD8_LOC1_SB_I3C0_REG 0x2A0282C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd8_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD8_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD8_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82c4)                                                      */
/*      SPRB0 (0x200d82c4)                                                      */
/*      SPRHBM (0x200d82c4)                                                     */
/*      SPRC0 (0x200d82c4)                                                      */
/*      SPRMCC (0x200d82c4)                                                     */
/*      SPRUCC (0x200d82c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD8_LOC2
*/


#define PERIODIC_POLL_CMD8_LOC2_SB_I3C0_REG 0x2A0282C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd8_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD8_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD9_LOC1_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82c8)                                                      */
/*      SPRB0 (0x200d82c8)                                                      */
/*      SPRHBM (0x200d82c8)                                                     */
/*      SPRC0 (0x200d82c8)                                                      */
/*      SPRMCC (0x200d82c8)                                                     */
/*      SPRUCC (0x200d82c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD9_LOC1
*/


#define PERIODIC_POLL_CMD9_LOC1_SB_I3C0_REG 0x2A0282C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd9_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD9_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD9_LOC2_SB_I3C0_REG supported on:                            */
/*      SPRA0 (0x200d82cc)                                                      */
/*      SPRB0 (0x200d82cc)                                                      */
/*      SPRHBM (0x200d82cc)                                                     */
/*      SPRC0 (0x200d82cc)                                                      */
/*      SPRMCC (0x200d82cc)                                                     */
/*      SPRUCC (0x200d82cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*        PERIODIC_POLL_CMD9_LOC2
*/


#define PERIODIC_POLL_CMD9_LOC2_SB_I3C0_REG 0x2A0282CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd9_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD9_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD10_LOC1_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82d0)                                                      */
/*      SPRB0 (0x200d82d0)                                                      */
/*      SPRHBM (0x200d82d0)                                                     */
/*      SPRC0 (0x200d82d0)                                                      */
/*      SPRMCC (0x200d82d0)                                                     */
/*      SPRUCC (0x200d82d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD10_LOC1
*/


#define PERIODIC_POLL_CMD10_LOC1_SB_I3C0_REG 0x2A0282D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd10_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD10_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD10_LOC2_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82d4)                                                      */
/*      SPRB0 (0x200d82d4)                                                      */
/*      SPRHBM (0x200d82d4)                                                     */
/*      SPRC0 (0x200d82d4)                                                      */
/*      SPRMCC (0x200d82d4)                                                     */
/*      SPRUCC (0x200d82d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD10_LOC2
*/


#define PERIODIC_POLL_CMD10_LOC2_SB_I3C0_REG 0x2A0282D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd10_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD10_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD11_LOC1_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82d8)                                                      */
/*      SPRB0 (0x200d82d8)                                                      */
/*      SPRHBM (0x200d82d8)                                                     */
/*      SPRC0 (0x200d82d8)                                                      */
/*      SPRMCC (0x200d82d8)                                                     */
/*      SPRUCC (0x200d82d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD11_LOC1
*/


#define PERIODIC_POLL_CMD11_LOC1_SB_I3C0_REG 0x2A0282D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd11_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD11_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD11_LOC2_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82dc)                                                      */
/*      SPRB0 (0x200d82dc)                                                      */
/*      SPRHBM (0x200d82dc)                                                     */
/*      SPRC0 (0x200d82dc)                                                      */
/*      SPRMCC (0x200d82dc)                                                     */
/*      SPRUCC (0x200d82dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD11_LOC2
*/


#define PERIODIC_POLL_CMD11_LOC2_SB_I3C0_REG 0x2A0282DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd11_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD11_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD12_LOC1_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82e0)                                                      */
/*      SPRB0 (0x200d82e0)                                                      */
/*      SPRHBM (0x200d82e0)                                                     */
/*      SPRC0 (0x200d82e0)                                                      */
/*      SPRMCC (0x200d82e0)                                                     */
/*      SPRUCC (0x200d82e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD12_LOC1
*/


#define PERIODIC_POLL_CMD12_LOC1_SB_I3C0_REG 0x2A0282E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd12_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD12_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD12_LOC2_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82e4)                                                      */
/*      SPRB0 (0x200d82e4)                                                      */
/*      SPRHBM (0x200d82e4)                                                     */
/*      SPRC0 (0x200d82e4)                                                      */
/*      SPRMCC (0x200d82e4)                                                     */
/*      SPRUCC (0x200d82e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD12_LOC2
*/


#define PERIODIC_POLL_CMD12_LOC2_SB_I3C0_REG 0x2A0282E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd12_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD12_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD13_LOC1_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82e8)                                                      */
/*      SPRB0 (0x200d82e8)                                                      */
/*      SPRHBM (0x200d82e8)                                                     */
/*      SPRC0 (0x200d82e8)                                                      */
/*      SPRMCC (0x200d82e8)                                                     */
/*      SPRUCC (0x200d82e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD13_LOC1
*/


#define PERIODIC_POLL_CMD13_LOC1_SB_I3C0_REG 0x2A0282E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd13_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD13_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD13_LOC2_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82ec)                                                      */
/*      SPRB0 (0x200d82ec)                                                      */
/*      SPRHBM (0x200d82ec)                                                     */
/*      SPRC0 (0x200d82ec)                                                      */
/*      SPRMCC (0x200d82ec)                                                     */
/*      SPRUCC (0x200d82ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD13_LOC2
*/


#define PERIODIC_POLL_CMD13_LOC2_SB_I3C0_REG 0x2A0282EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd13_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD13_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD14_LOC1_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82f0)                                                      */
/*      SPRB0 (0x200d82f0)                                                      */
/*      SPRHBM (0x200d82f0)                                                     */
/*      SPRC0 (0x200d82f0)                                                      */
/*      SPRMCC (0x200d82f0)                                                     */
/*      SPRUCC (0x200d82f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD14_LOC1
*/


#define PERIODIC_POLL_CMD14_LOC1_SB_I3C0_REG 0x2A0282F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd14_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD14_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD14_LOC2_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82f4)                                                      */
/*      SPRB0 (0x200d82f4)                                                      */
/*      SPRHBM (0x200d82f4)                                                     */
/*      SPRC0 (0x200d82f4)                                                      */
/*      SPRMCC (0x200d82f4)                                                     */
/*      SPRUCC (0x200d82f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD14_LOC2
*/


#define PERIODIC_POLL_CMD14_LOC2_SB_I3C0_REG 0x2A0282F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd14_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD14_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD15_LOC1_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82f8)                                                      */
/*      SPRB0 (0x200d82f8)                                                      */
/*      SPRHBM (0x200d82f8)                                                     */
/*      SPRC0 (0x200d82f8)                                                      */
/*      SPRMCC (0x200d82f8)                                                     */
/*      SPRUCC (0x200d82f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD15_LOC1
*/


#define PERIODIC_POLL_CMD15_LOC1_SB_I3C0_REG 0x2A0282F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd15_loc1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD15_LOC1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_CMD15_LOC2_SB_I3C0_REG supported on:                           */
/*      SPRA0 (0x200d82fc)                                                      */
/*      SPRB0 (0x200d82fc)                                                      */
/*      SPRHBM (0x200d82fc)                                                     */
/*      SPRC0 (0x200d82fc)                                                      */
/*      SPRMCC (0x200d82fc)                                                     */
/*      SPRUCC (0x200d82fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*       PERIODIC_POLL_CMD15_LOC2
*/


#define PERIODIC_POLL_CMD15_LOC2_SB_I3C0_REG 0x2A0282FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_cmd15_loc2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_CMD15_LOC2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT0_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8380)                                                      */
/*      SPRB0 (0x200d8380)                                                      */
/*      SPRHBM (0x200d8380)                                                     */
/*      SPRC0 (0x200d8380)                                                      */
/*      SPRMCC (0x200d8380)                                                     */
/*      SPRUCC (0x200d8380)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT0
*/


#define PERIODIC_POLL_SLOT0_SB_I3C0_REG 0x2A028380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot0 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT0_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT1_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8384)                                                      */
/*      SPRB0 (0x200d8384)                                                      */
/*      SPRHBM (0x200d8384)                                                     */
/*      SPRC0 (0x200d8384)                                                      */
/*      SPRMCC (0x200d8384)                                                     */
/*      SPRUCC (0x200d8384)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT1
*/


#define PERIODIC_POLL_SLOT1_SB_I3C0_REG 0x2A028384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot1 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT1_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT2_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8388)                                                      */
/*      SPRB0 (0x200d8388)                                                      */
/*      SPRHBM (0x200d8388)                                                     */
/*      SPRC0 (0x200d8388)                                                      */
/*      SPRMCC (0x200d8388)                                                     */
/*      SPRUCC (0x200d8388)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT2
*/


#define PERIODIC_POLL_SLOT2_SB_I3C0_REG 0x2A028388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot2 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT2_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT3_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d838c)                                                      */
/*      SPRB0 (0x200d838c)                                                      */
/*      SPRHBM (0x200d838c)                                                     */
/*      SPRC0 (0x200d838c)                                                      */
/*      SPRMCC (0x200d838c)                                                     */
/*      SPRUCC (0x200d838c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT3
*/


#define PERIODIC_POLL_SLOT3_SB_I3C0_REG 0x2A02838C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot3 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT3_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT4_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8390)                                                      */
/*      SPRB0 (0x200d8390)                                                      */
/*      SPRHBM (0x200d8390)                                                     */
/*      SPRC0 (0x200d8390)                                                      */
/*      SPRMCC (0x200d8390)                                                     */
/*      SPRUCC (0x200d8390)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT4
*/


#define PERIODIC_POLL_SLOT4_SB_I3C0_REG 0x2A028390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot4 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT4_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT5_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8394)                                                      */
/*      SPRB0 (0x200d8394)                                                      */
/*      SPRHBM (0x200d8394)                                                     */
/*      SPRC0 (0x200d8394)                                                      */
/*      SPRMCC (0x200d8394)                                                     */
/*      SPRUCC (0x200d8394)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT5
*/


#define PERIODIC_POLL_SLOT5_SB_I3C0_REG 0x2A028394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot5 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT5_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT6_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8398)                                                      */
/*      SPRB0 (0x200d8398)                                                      */
/*      SPRHBM (0x200d8398)                                                     */
/*      SPRC0 (0x200d8398)                                                      */
/*      SPRMCC (0x200d8398)                                                     */
/*      SPRUCC (0x200d8398)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT6
*/


#define PERIODIC_POLL_SLOT6_SB_I3C0_REG 0x2A028398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot6 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT6_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT7_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d839c)                                                      */
/*      SPRB0 (0x200d839c)                                                      */
/*      SPRHBM (0x200d839c)                                                     */
/*      SPRC0 (0x200d839c)                                                      */
/*      SPRMCC (0x200d839c)                                                     */
/*      SPRUCC (0x200d839c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT7
*/


#define PERIODIC_POLL_SLOT7_SB_I3C0_REG 0x2A02839C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot7 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT7_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT8_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d83a0)                                                      */
/*      SPRB0 (0x200d83a0)                                                      */
/*      SPRHBM (0x200d83a0)                                                     */
/*      SPRC0 (0x200d83a0)                                                      */
/*      SPRMCC (0x200d83a0)                                                     */
/*      SPRUCC (0x200d83a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT8
*/


#define PERIODIC_POLL_SLOT8_SB_I3C0_REG 0x2A0283A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot8 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT8_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT9_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d83a4)                                                      */
/*      SPRB0 (0x200d83a4)                                                      */
/*      SPRHBM (0x200d83a4)                                                     */
/*      SPRC0 (0x200d83a4)                                                      */
/*      SPRMCC (0x200d83a4)                                                     */
/*      SPRUCC (0x200d83a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*            PERIODIC_POLL_SLOT9
*/


#define PERIODIC_POLL_SLOT9_SB_I3C0_REG 0x2A0283A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot9 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT9_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT10_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83a8)                                                      */
/*      SPRB0 (0x200d83a8)                                                      */
/*      SPRHBM (0x200d83a8)                                                     */
/*      SPRC0 (0x200d83a8)                                                      */
/*      SPRMCC (0x200d83a8)                                                     */
/*      SPRUCC (0x200d83a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_SLOT10
*/


#define PERIODIC_POLL_SLOT10_SB_I3C0_REG 0x2A0283A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot10 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT10_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT11_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83ac)                                                      */
/*      SPRB0 (0x200d83ac)                                                      */
/*      SPRHBM (0x200d83ac)                                                     */
/*      SPRC0 (0x200d83ac)                                                      */
/*      SPRMCC (0x200d83ac)                                                     */
/*      SPRUCC (0x200d83ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_SLOT11
*/


#define PERIODIC_POLL_SLOT11_SB_I3C0_REG 0x2A0283AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot11 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT11_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT12_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83b0)                                                      */
/*      SPRB0 (0x200d83b0)                                                      */
/*      SPRHBM (0x200d83b0)                                                     */
/*      SPRC0 (0x200d83b0)                                                      */
/*      SPRMCC (0x200d83b0)                                                     */
/*      SPRUCC (0x200d83b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_SLOT12
*/


#define PERIODIC_POLL_SLOT12_SB_I3C0_REG 0x2A0283B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot12 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT12_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT13_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83b4)                                                      */
/*      SPRB0 (0x200d83b4)                                                      */
/*      SPRHBM (0x200d83b4)                                                     */
/*      SPRC0 (0x200d83b4)                                                      */
/*      SPRMCC (0x200d83b4)                                                     */
/*      SPRUCC (0x200d83b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_SLOT13
*/


#define PERIODIC_POLL_SLOT13_SB_I3C0_REG 0x2A0283B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot13 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT13_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT14_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83b8)                                                      */
/*      SPRB0 (0x200d83b8)                                                      */
/*      SPRHBM (0x200d83b8)                                                     */
/*      SPRC0 (0x200d83b8)                                                      */
/*      SPRMCC (0x200d83b8)                                                     */
/*      SPRUCC (0x200d83b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_SLOT14
*/


#define PERIODIC_POLL_SLOT14_SB_I3C0_REG 0x2A0283B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot14 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT14_SB_I3C0_STRUCT;

/* PERIODIC_POLL_SLOT15_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83bc)                                                      */
/*      SPRB0 (0x200d83bc)                                                      */
/*      SPRHBM (0x200d83bc)                                                     */
/*      SPRC0 (0x200d83bc)                                                      */
/*      SPRMCC (0x200d83bc)                                                     */
/*      SPRUCC (0x200d83bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim          */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_ctrl_prim         */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/*           PERIODIC_POLL_SLOT15
*/


#define PERIODIC_POLL_SLOT15_SB_I3C0_REG 0x2A0283BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 periodic_poll_slot15 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*  */
    UINT32 reserved_16_31 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /*  */

  } Bits;
  UINT32 Data;

} PERIODIC_POLL_SLOT15_SB_I3C0_STRUCT;

/* MULT_BUS_INST_EXT_CAP_SB_I3C0_REG supported on:                              */
/*      SPRA0 (0x200d83d0)                                                      */
/*      SPRB0 (0x200d83d0)                                                      */
/*      SPRHBM (0x200d83d0)                                                     */
/*      SPRC0 (0x200d83d0)                                                      */
/*      SPRMCC (0x200d83d0)                                                     */
/*      SPRUCC (0x200d83d0)                                                     */
/* Register default value on SPRA0: 0x00000803                                  */
/* Register default value on SPRB0: 0x00000803                                  */
/* Register default value on SPRHBM: 0x00000803                                 */
/* Register default value on SPRC0: 0x00000803                                  */
/* Register default value on SPRMCC: 0x00000803                                 */
/* Register default value on SPRUCC: 0x00000803                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* Multi Bus Instance Extended Capability ID
*/


#define MULT_BUS_INST_EXT_CAP_SB_I3C0_REG 0x2A0283D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mult_bus_inst_cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000003*/

                            /* Multi Bus Instance Extended Capability ID */
    UINT32 mult_bus_inst_cap_len : 16;

                            /* Bits[23:8], Access Type=RO, default=0x00000008*/

                            /* Multi Bus Instance Extended Capability ID */
    UINT32 reserved0 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Multi Bus Instance Extended Capability ID */

  } Bits;
  UINT32 Data;

} MULT_BUS_INST_EXT_CAP_SB_I3C0_STRUCT;

/* MULT_BUS_INST_CNT_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d83d4)                                                      */
/*      SPRB0 (0x200d83d4)                                                      */
/*      SPRHBM (0x200d83d4)                                                     */
/*      SPRC0 (0x200d83d4)                                                      */
/*      SPRMCC (0x200d83d4)                                                     */
/*      SPRUCC (0x200d83d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* Multi Bus Instance Count
*/


#define MULT_BUS_INST_CNT_SB_I3C0_REG 0x2A0283D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mult_bus_inst_cnt : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Multi Bus Instance Count */
    UINT32 reserved0 : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Multi Bus Instance Count */

  } Bits;
  UINT32 Data;

} MULT_BUS_INST_CNT_SB_I3C0_STRUCT;

/* MULT_BUS_INST_OFFSET_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d83d8)                                                      */
/*      SPRB0 (0x200d83d8)                                                      */
/*      SPRHBM (0x200d83d8)                                                     */
/*      SPRC0 (0x200d83d8)                                                      */
/*      SPRMCC (0x200d83d8)                                                     */
/*      SPRUCC (0x200d83d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* Multi Bus Instance Offset
*/


#define MULT_BUS_INST_OFFSET_SB_I3C0_REG 0x2A0283D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mult_bus_inst_offset : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Multi Bus Instance Offset */

  } Bits;
  UINT32 Data;

} MULT_BUS_INST_OFFSET_SB_I3C0_STRUCT;

/* VND_SPEC_EXT_CAP_2_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d83dc)                                                      */
/*      SPRB0 (0x200d83dc)                                                      */
/*      SPRHBM (0x200d83dc)                                                     */
/*      SPRC0 (0x200d83dc)                                                      */
/*      SPRMCC (0x200d83dc)                                                     */
/*      SPRUCC (0x200d83dc)                                                     */
/* Register default value on SPRA0: 0x000040C1                                  */
/* Register default value on SPRB0: 0x000040C1                                  */
/* Register default value on SPRHBM: 0x000040C1                                 */
/* Register default value on SPRC0: 0x000040C1                                  */
/* Register default value on SPRMCC: 0x000040C1                                 */
/* Register default value on SPRUCC: 0x000040C1                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* Vendor Specific Capability Register 2
*/


#define VND_SPEC_EXT_CAP_2_SB_I3C0_REG 0x2A0283DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vnd_spc_ext_cap_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000C1*/

                            /* Vendor Specific Capability Register 2 */
    UINT32 vnd_spc_ext_cap_len : 16;

                            /* Bits[23:8], Access Type=RO, default=0x00000040*/

                            /* Vendor Specific Capability Register 2 */
    UINT32 reserved0 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Vendor Specific Capability Register 2 */

  } Bits;
  UINT32 Data;

} VND_SPEC_EXT_CAP_2_SB_I3C0_STRUCT;

/* SB_I3C_LAST_AER_STS_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d83e0)                                                      */
/*      SPRB0 (0x200d83e0)                                                      */
/*      SPRHBM (0x200d83e0)                                                     */
/*      SPRC0 (0x200d83e0)                                                      */
/*      SPRMCC (0x200d83e0)                                                     */
/*      SPRUCC (0x200d83e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C Last AER status data
*/


#define SB_I3C_LAST_AER_STS_SB_I3C0_REG 0x2A0283E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_send_aer_sts : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               SB I3C Last sent PCIE AER - errors. Bit0- error
                               in IBI status for polled data, bit1- normal IBI
                               sent during polling enable and bit2- I3C
                               controller interrupt
                            */
    UINT32 reserved0 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* SB I3C Last sent PCIE AER status data */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_AER_STS_SB_I3C0_STRUCT;

/* SB_I3C_LAST_IBI_STS_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d83e4)                                                      */
/*      SPRB0 (0x200d83e4)                                                      */
/*      SPRHBM (0x200d83e4)                                                     */
/*      SPRC0 (0x200d83e4)                                                      */
/*      SPRMCC (0x200d83e4)                                                     */
/*      SPRUCC (0x200d83e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last sent IBI Status
*/


#define SB_I3C_LAST_IBI_STS_SB_I3C0_REG 0x2A0283E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_read_ibi_sts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               IBI Status value of normal IBI received during
                               poll enable.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_IBI_STS_SB_I3C0_STRUCT;

/* SB_I3C_PKG_C_STATE_SB_I3C0_REG supported on:                                 */
/*      SPRA0 (0x200d83ec)                                                      */
/*      SPRB0 (0x200d83ec)                                                      */
/*      SPRHBM (0x200d83ec)                                                     */
/*      SPRC0 (0x200d83ec)                                                      */
/*      SPRMCC (0x200d83ec)                                                     */
/*      SPRUCC (0x200d83ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C PKG C state register
*/


#define SB_I3C_PKG_C_STATE_SB_I3C0_REG 0x2A0283EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pkgc_state : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               SB I3C last received PkG C state register -
                               PM_REQ_PS_VALUE[2:0] is captured in this
                               register when PM_req message is received.
                            */
    UINT32 reserved0 : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SB_I3C_PKG_C_STATE_SB_I3C0_STRUCT;

/* SB_I3C_DISABLE_POLLING_SB_I3C0_REG supported on:                             */
/*      SPRA0 (0x200d83f0)                                                      */
/*      SPRB0 (0x200d83f0)                                                      */
/*      SPRHBM (0x200d83f0)                                                     */
/*      SPRC0 (0x200d83f0)                                                      */
/*      SPRMCC (0x200d83f0)                                                     */
/*      SPRUCC (0x200d83f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C disable polling
*/


#define SB_I3C_DISABLE_POLLING_SB_I3C0_REG 0x2A0283F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_polling : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 0 - Polling is enabled. When set to
                               1 - Pollibng is disabled.
                            */
    UINT32 disable_polled_value_pre_processing : 8;

                            /* Bits[8:1], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 0 - polled data will be processed
                               before sending upstream message. When set to 1 -
                               polled data will be sent as is in the upstream
                               message.
                            */
    UINT32 enable_swap_polled_data : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 0 - swapping of 2B polled data
                               before rounding is disabled. When set to 1 -
                               swapping of 2B polled data before rounding is
                               enabled.
                            */
    UINT32 reserved0 : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SB_I3C_DISABLE_POLLING_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM0_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d83f4)                                                      */
/*      SPRB0 (0x200d83f4)                                                      */
/*      SPRHBM (0x200d83f4)                                                     */
/*      SPRC0 (0x200d83f4)                                                      */
/*      SPRMCC (0x200d83f4)                                                     */
/*      SPRUCC (0x200d83f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM0 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM0_TS_VALUE_SB_I3C0_REG 0x2A0283F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm0_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 0 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM0_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM1_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d83f8)                                                      */
/*      SPRB0 (0x200d83f8)                                                      */
/*      SPRHBM (0x200d83f8)                                                     */
/*      SPRC0 (0x200d83f8)                                                      */
/*      SPRMCC (0x200d83f8)                                                     */
/*      SPRUCC (0x200d83f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM1 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM1_TS_VALUE_SB_I3C0_REG 0x2A0283F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm1_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 1 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM1_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM2_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d83fc)                                                      */
/*      SPRB0 (0x200d83fc)                                                      */
/*      SPRHBM (0x200d83fc)                                                     */
/*      SPRC0 (0x200d83fc)                                                      */
/*      SPRMCC (0x200d83fc)                                                     */
/*      SPRUCC (0x200d83fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM2 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM2_TS_VALUE_SB_I3C0_REG 0x2A0283FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm2_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 2 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM2_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM3_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d8400)                                                      */
/*      SPRB0 (0x200d8400)                                                      */
/*      SPRHBM (0x200d8400)                                                     */
/*      SPRC0 (0x200d8400)                                                      */
/*      SPRMCC (0x200d8400)                                                     */
/*      SPRUCC (0x200d8400)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM3 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM3_TS_VALUE_SB_I3C0_REG 0x2A028400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm3_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 3 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM3_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM4_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d8404)                                                      */
/*      SPRB0 (0x200d8404)                                                      */
/*      SPRHBM (0x200d8404)                                                     */
/*      SPRC0 (0x200d8404)                                                      */
/*      SPRMCC (0x200d8404)                                                     */
/*      SPRUCC (0x200d8404)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM4 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM4_TS_VALUE_SB_I3C0_REG 0x2A028404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm4_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 4 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM4_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM5_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d8408)                                                      */
/*      SPRB0 (0x200d8408)                                                      */
/*      SPRHBM (0x200d8408)                                                     */
/*      SPRC0 (0x200d8408)                                                      */
/*      SPRMCC (0x200d8408)                                                     */
/*      SPRUCC (0x200d8408)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM5 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM5_TS_VALUE_SB_I3C0_REG 0x2A028408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm5_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 5 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM5_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM6_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d840c)                                                      */
/*      SPRB0 (0x200d840c)                                                      */
/*      SPRHBM (0x200d840c)                                                     */
/*      SPRC0 (0x200d840c)                                                      */
/*      SPRMCC (0x200d840c)                                                     */
/*      SPRUCC (0x200d840c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM6 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM6_TS_VALUE_SB_I3C0_REG 0x2A02840C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm6_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 6 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM6_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMM7_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d8410)                                                      */
/*      SPRB0 (0x200d8410)                                                      */
/*      SPRHBM (0x200d8410)                                                     */
/*      SPRC0 (0x200d8410)                                                      */
/*      SPRMCC (0x200d8410)                                                     */
/*      SPRUCC (0x200d8410)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled DIMM7 temperature sensor
*/


#define SB_I3C_LAST_POLLED_DIMM7_TS_VALUE_SB_I3C0_REG 0x2A028410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_16b_dimm7_ts : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* SB I3C last sent IBI Data for DIMM 7 */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMM7_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_LAST_POLLED_DIMMX_TS_VALUE_SB_I3C0_REG supported on:                  */
/*      SPRA0 (0x200d8414)                                                      */
/*      SPRB0 (0x200d8414)                                                      */
/*      SPRHBM (0x200d8414)                                                     */
/*      SPRC0 (0x200d8414)                                                      */
/*      SPRMCC (0x200d8414)                                                     */
/*      SPRUCC (0x200d8414)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last polled value DIMMx TS message data
*/


#define SB_I3C_LAST_POLLED_DIMMX_TS_VALUE_SB_I3C0_REG 0x2A028414

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_polled_value_dimmx_ts_msg_send : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               SB I3C last sent IBI Status read from SB I3C
                               controller.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_POLLED_DIMMX_TS_VALUE_SB_I3C0_STRUCT;

/* SB_I3C_IBI_STATUS_MASK_ERR_SB_I3C0_REG supported on:                         */
/*      SPRA0 (0x200d8418)                                                      */
/*      SPRB0 (0x200d8418)                                                      */
/*      SPRHBM (0x200d8418)                                                     */
/*      SPRC0 (0x200d8418)                                                      */
/*      SPRMCC (0x200d8418)                                                     */
/*      SPRUCC (0x200d8418)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C IBI status mask error
*/


#define SB_I3C_IBI_STATUS_MASK_ERR_SB_I3C0_REG 0x2A028418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ibi_status_mask_err : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               This register is used to mask the errors
                               received in IBI status during polled data read.
                               Currently only 1 error bit is is valid in IBI
                               status. Bit [0] of this register will be used to
                               mask that. There is one more error in IBI
                               status, which is not applicable for SPR. only
                               mask_err[0] is used. when set to 0: error in the
                               IBI status will be ignored when set to 1: error
                               in the IBI status, data will be discarded and
                               pcie_aer message will be sent out.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_IBI_STATUS_MASK_ERR_SB_I3C0_STRUCT;

/* SB_I3C_IBI_STATUS_SB_I3C0_REG supported on:                                  */
/*      SPRA0 (0x200d841c)                                                      */
/*      SPRB0 (0x200d841c)                                                      */
/*      SPRHBM (0x200d841c)                                                     */
/*      SPRC0 (0x200d841c)                                                      */
/*      SPRMCC (0x200d841c)                                                     */
/*      SPRUCC (0x200d841c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C IBI status
*/


#define SB_I3C_IBI_STATUS_SB_I3C0_REG 0x2A02841C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ibi_error_status : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This bit will be set when polled data received
                               has error set in the status.
                            */
    UINT32 normal_ibi_rcvd : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               This bit will be set by design when normal IBI
                               is received during polling and status and data
                               will be logged.
                            */
    UINT32 reserved0 : 30;

                            /* Bits[31:2], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SB_I3C_IBI_STATUS_SB_I3C0_STRUCT;

/* APCTL_SB_I3C0_REG supported on:                                              */
/*      SPRA0 (0x200d8420)                                                      */
/*      SPRB0 (0x200d8420)                                                      */
/*      SPRHBM (0x200d8420)                                                     */
/*      SPRC0 (0x200d8420)                                                      */
/*      SPRMCC (0x200d8420)                                                     */
/*      SPRUCC (0x200d8420)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* DIMM TYPE Register
*/


#define APCTL_SB_I3C0_REG 0x2A028420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dimm_type : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This is 8 bits, 1 bit per DIMM and is applicable
                               only to PkgC6 (not C0) states. When set to 0, HW
                               continues polling to these DIMMs (if already
                               enabled in controller.POLL_EN) in PkgC6, and U/S
                               messages would be send to PUNIT & IEH (for
                               PCIe_AER) When set to 1, polling is stopped for
                               the relevant DIMMs - and hence no U/S PvtWr to
                               PUNIT. PCIe_AER would be send if normal IBIs are
                               received.
                            */
    UINT32 reserved0 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* RESERVED */

  } Bits;
  UINT32 Data;

} APCTL_SB_I3C0_STRUCT;

/* SB_I3C_LAST_IBI_DATA_SB_I3C0_REG supported on:                               */
/*      SPRA0 (0x200d8424)                                                      */
/*      SPRB0 (0x200d8424)                                                      */
/*      SPRHBM (0x200d8424)                                                     */
/*      SPRC0 (0x200d8424)                                                      */
/*      SPRMCC (0x200d8424)                                                     */
/*      SPRUCC (0x200d8424)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last sent IBI Data
*/


#define SB_I3C_LAST_IBI_DATA_SB_I3C0_REG 0x2A028424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_read_ibi_data : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               IBI Data value of normal IBI received during
                               poll enable.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_IBI_DATA_SB_I3C0_STRUCT;

/* SB_I3C_LAST_IBI_DATA1_SB_I3C0_REG supported on:                              */
/*      SPRA0 (0x200d8428)                                                      */
/*      SPRB0 (0x200d8428)                                                      */
/*      SPRHBM (0x200d8428)                                                     */
/*      SPRC0 (0x200d8428)                                                      */
/*      SPRMCC (0x200d8428)                                                     */
/*      SPRUCC (0x200d8428)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last sent IBI Data1
*/


#define SB_I3C_LAST_IBI_DATA1_SB_I3C0_REG 0x2A028428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_read_ibi_data1 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               IBI Data 2DW value of normal IBI received during
                               poll enable.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_IBI_DATA1_SB_I3C0_STRUCT;

/* SB_I3C_LAST_IBI_DATA2_SB_I3C0_REG supported on:                              */
/*      SPRA0 (0x200d842c)                                                      */
/*      SPRB0 (0x200d842c)                                                      */
/*      SPRHBM (0x200d842c)                                                     */
/*      SPRC0 (0x200d842c)                                                      */
/*      SPRMCC (0x200d842c)                                                     */
/*      SPRUCC (0x200d842c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last sent IBI Data2
*/


#define SB_I3C_LAST_IBI_DATA2_SB_I3C0_REG 0x2A02842C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_read_ibi_data2 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               IBI Data 3DW value of normal IBI received during
                               poll enable.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_IBI_DATA2_SB_I3C0_STRUCT;

/* SB_I3C_LAST_IBI_DATA3_SB_I3C0_REG supported on:                              */
/*      SPRA0 (0x200d8430)                                                      */
/*      SPRB0 (0x200d8430)                                                      */
/*      SPRHBM (0x200d8430)                                                     */
/*      SPRC0 (0x200d8430)                                                      */
/*      SPRMCC (0x200d8430)                                                     */
/*      SPRUCC (0x200d8430)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C last sent IBI Data3
*/


#define SB_I3C_LAST_IBI_DATA3_SB_I3C0_REG 0x2A028430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 last_read_ibi_data3 : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               IBI Data 4DW value of normal IBI received during
                               poll enable.
                            */

  } Bits;
  UINT32 Data;

} SB_I3C_LAST_IBI_DATA3_SB_I3C0_STRUCT;

/* SB_I3C_RST_PAT_TRIG_SB_I3C0_REG supported on:                                */
/*      SPRA0 (0x200d8434)                                                      */
/*      SPRB0 (0x200d8434)                                                      */
/*      SPRHBM (0x200d8434)                                                     */
/*      SPRC0 (0x200d8434)                                                      */
/*      SPRMCC (0x200d8434)                                                     */
/*      SPRUCC (0x200d8434)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRB0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRHBM Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRC0 Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim            */
/* SPRMCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* SPRUCC Register File:    sprsp_top/lpss[0]/sb_i3c_0/sb_i3c_cl_prim           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRB0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRHBM Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRC0 Security PolicyGroup: SB_I3C_MMIO_REGS_GP                              */
/* SPRMCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SPRUCC Security PolicyGroup: SB_I3C_MMIO_REGS_GP                             */
/* SB I3C Reset Pattern Trigger
*/


#define SB_I3C_RST_PAT_TRIG_SB_I3C0_REG 0x2A028434

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reset_pattern_trig : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               When set to 0 - no effect. When set to 1 - reset
                               pattern gen is enabled. Writing one to this bit
                               internally pull down the signal and again set by
                               hardware once reset pattern generation is done.
                            */
    UINT32 reserved1 : 7;

                            /* Bits[7:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 reset_pattern_ctrl : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               This field holds the count value for getting
                               HIGH/LOW pulse
                            */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SB_I3C_RST_PAT_TRIG_SB_I3C0_STRUCT;
#endif /* _SB_I3C0_h */
