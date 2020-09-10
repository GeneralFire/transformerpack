
/** @file
  MCIO_DDRIOEXT.h

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
/* MEMIO_BOOT                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |   */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | UNCORE_PMA_SAI                          */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* MEMIO_BOOT                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |   */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | UNCORE_PMA_SAI                          */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* MEMIO_BOOT                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |   */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | UNCORE_PMA_SAI                          */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* MEMIO_BOOT                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |   */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | UNCORE_PMA_SAI                          */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* MEMIO_BOOT                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |   */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | UNCORE_PMA_SAI                          */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* MEMIO_BOOT                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI  */
/*    | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |   */
/*    DFX_UNTRUSTED_SAI | OOB_MSM_SAI | UNCORE_PMA_SAI                          */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _MCIO_DDRIOEXT_h
#define _MCIO_DDRIOEXT_h
#include <Base.h>

/* DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a00)                                                      */
/*      SPRB0 (0x20012a00)                                                      */
/*      SPRHBM (0x20012a00)                                                     */
/*      SPRC0 (0x20012a00)                                                      */
/*      SPRMCC (0x20012a00)                                                     */
/*      SPRUCC (0x20012a00)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_REG 0x08122A00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_1_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a04)                                                      */
/*      SPRB0 (0x20012a04)                                                      */
/*      SPRHBM (0x20012a04)                                                     */
/*      SPRC0 (0x20012a04)                                                      */
/*      SPRMCC (0x20012a04)                                                     */
/*      SPRUCC (0x20012a04)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_1_MCIO_DDRIOEXT_REG 0x08122A04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_2_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a08)                                                      */
/*      SPRB0 (0x20012a08)                                                      */
/*      SPRHBM (0x20012a08)                                                     */
/*      SPRC0 (0x20012a08)                                                      */
/*      SPRMCC (0x20012a08)                                                     */
/*      SPRUCC (0x20012a08)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_2_MCIO_DDRIOEXT_REG 0x08122A08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_2_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_3_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a0c)                                                      */
/*      SPRB0 (0x20012a0c)                                                      */
/*      SPRHBM (0x20012a0c)                                                     */
/*      SPRC0 (0x20012a0c)                                                      */
/*      SPRMCC (0x20012a0c)                                                     */
/*      SPRUCC (0x20012a0c)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_3_MCIO_DDRIOEXT_REG 0x08122A0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_3_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_4_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a10)                                                      */
/*      SPRB0 (0x20012a10)                                                      */
/*      SPRHBM (0x20012a10)                                                     */
/*      SPRC0 (0x20012a10)                                                      */
/*      SPRMCC (0x20012a10)                                                     */
/*      SPRUCC (0x20012a10)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_4_MCIO_DDRIOEXT_REG 0x08122A10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_4_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_5_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a14)                                                      */
/*      SPRB0 (0x20012a14)                                                      */
/*      SPRHBM (0x20012a14)                                                     */
/*      SPRC0 (0x20012a14)                                                      */
/*      SPRMCC (0x20012a14)                                                     */
/*      SPRUCC (0x20012a14)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_5_MCIO_DDRIOEXT_REG 0x08122A14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_5_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_6_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a18)                                                      */
/*      SPRB0 (0x20012a18)                                                      */
/*      SPRHBM (0x20012a18)                                                     */
/*      SPRC0 (0x20012a18)                                                      */
/*      SPRMCC (0x20012a18)                                                     */
/*      SPRUCC (0x20012a18)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_6_MCIO_DDRIOEXT_REG 0x08122A18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_6_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_7_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a1c)                                                      */
/*      SPRB0 (0x20012a1c)                                                      */
/*      SPRHBM (0x20012a1c)                                                     */
/*      SPRC0 (0x20012a1c)                                                      */
/*      SPRMCC (0x20012a1c)                                                     */
/*      SPRUCC (0x20012a1c)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_7_MCIO_DDRIOEXT_REG 0x08122A1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_7_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_8_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a20)                                                      */
/*      SPRB0 (0x20012a20)                                                      */
/*      SPRHBM (0x20012a20)                                                     */
/*      SPRC0 (0x20012a20)                                                      */
/*      SPRMCC (0x20012a20)                                                     */
/*      SPRUCC (0x20012a20)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_8_MCIO_DDRIOEXT_REG 0x08122A20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_8_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_9_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a24)                                                      */
/*      SPRB0 (0x20012a24)                                                      */
/*      SPRHBM (0x20012a24)                                                     */
/*      SPRC0 (0x20012a24)                                                      */
/*      SPRMCC (0x20012a24)                                                     */
/*      SPRUCC (0x20012a24)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_9_MCIO_DDRIOEXT_REG 0x08122A24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_9_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_10_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a28)                                                      */
/*      SPRB0 (0x20012a28)                                                      */
/*      SPRHBM (0x20012a28)                                                     */
/*      SPRC0 (0x20012a28)                                                      */
/*      SPRMCC (0x20012a28)                                                     */
/*      SPRUCC (0x20012a28)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_10_MCIO_DDRIOEXT_REG 0x08122A28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_10_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_11_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a2c)                                                      */
/*      SPRB0 (0x20012a2c)                                                      */
/*      SPRHBM (0x20012a2c)                                                     */
/*      SPRC0 (0x20012a2c)                                                      */
/*      SPRMCC (0x20012a2c)                                                     */
/*      SPRUCC (0x20012a2c)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_11_MCIO_DDRIOEXT_REG 0x08122A2C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_11_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_12_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a30)                                                      */
/*      SPRB0 (0x20012a30)                                                      */
/*      SPRHBM (0x20012a30)                                                     */
/*      SPRC0 (0x20012a30)                                                      */
/*      SPRMCC (0x20012a30)                                                     */
/*      SPRUCC (0x20012a30)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_12_MCIO_DDRIOEXT_REG 0x08122A30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_12_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_13_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a34)                                                      */
/*      SPRB0 (0x20012a34)                                                      */
/*      SPRHBM (0x20012a34)                                                     */
/*      SPRC0 (0x20012a34)                                                      */
/*      SPRMCC (0x20012a34)                                                     */
/*      SPRUCC (0x20012a34)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_13_MCIO_DDRIOEXT_REG 0x08122A34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_13_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_14_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a38)                                                      */
/*      SPRB0 (0x20012a38)                                                      */
/*      SPRHBM (0x20012a38)                                                     */
/*      SPRC0 (0x20012a38)                                                      */
/*      SPRMCC (0x20012a38)                                                     */
/*      SPRUCC (0x20012a38)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_14_MCIO_DDRIOEXT_REG 0x08122A38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_14_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a3c)                                                      */
/*      SPRB0 (0x20012a3c)                                                      */
/*      SPRHBM (0x20012a3c)                                                     */
/*      SPRC0 (0x20012a3c)                                                      */
/*      SPRMCC (0x20012a3c)                                                     */
/*      SPRUCC (0x20012a3c)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_REG 0x08122A3C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_15_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a40)                                                      */
/*      SPRB0 (0x20012a40)                                                      */
/*      SPRHBM (0x20012a40)                                                     */
/*      SPRC0 (0x20012a40)                                                      */
/*      SPRMCC (0x20012a40)                                                     */
/*      SPRUCC (0x20012a40)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_REG 0x08122A40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_16_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_17_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a44)                                                      */
/*      SPRB0 (0x20012a44)                                                      */
/*      SPRHBM (0x20012a44)                                                     */
/*      SPRC0 (0x20012a44)                                                      */
/*      SPRMCC (0x20012a44)                                                     */
/*      SPRUCC (0x20012a44)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_17_MCIO_DDRIOEXT_REG 0x08122A44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_17_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_18_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a48)                                                      */
/*      SPRB0 (0x20012a48)                                                      */
/*      SPRHBM (0x20012a48)                                                     */
/*      SPRC0 (0x20012a48)                                                      */
/*      SPRMCC (0x20012a48)                                                     */
/*      SPRUCC (0x20012a48)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_18_MCIO_DDRIOEXT_REG 0x08122A48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_18_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_19_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a4c)                                                      */
/*      SPRB0 (0x20012a4c)                                                      */
/*      SPRHBM (0x20012a4c)                                                     */
/*      SPRC0 (0x20012a4c)                                                      */
/*      SPRMCC (0x20012a4c)                                                     */
/*      SPRUCC (0x20012a4c)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_19_MCIO_DDRIOEXT_REG 0x08122A4C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_19_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_20_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a50)                                                      */
/*      SPRB0 (0x20012a50)                                                      */
/*      SPRHBM (0x20012a50)                                                     */
/*      SPRC0 (0x20012a50)                                                      */
/*      SPRMCC (0x20012a50)                                                     */
/*      SPRUCC (0x20012a50)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_20_MCIO_DDRIOEXT_REG 0x08122A50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_20_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL0_BIT_21_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a54)                                                      */
/*      SPRB0 (0x20012a54)                                                      */
/*      SPRHBM (0x20012a54)                                                     */
/*      SPRC0 (0x20012a54)                                                      */
/*      SPRMCC (0x20012a54)                                                     */
/*      SPRUCC (0x20012a54)                                                     */
/* Register default value on SPRA0: 0x00490000                                  */
/* Register default value on SPRB0: 0x00490000                                  */
/* Register default value on SPRHBM: 0x00490000                                 */
/* Register default value on SPRC0: 0x00490000                                  */
/* Register default value on SPRMCC: 0x00490000                                 */
/* Register default value on SPRUCC: 0x00490000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL0_BIT_21_MCIO_DDRIOEXT_REG 0x08122A54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly : 9;

                            /* Bits[8:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CMD delay control. The range is 0 to 8 UI with
                               step size of clkpi/128.[br] This field is added
                               with cmd_dly_adj to allow BIOS for margining
                               sweep (signed numer -128 to 127). [br] The final
                               delay - [br] 8:7 - is used for logic delay[br]
                               6:0 - The lower 7 bit is added with
                               cmd_piref_offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the cmd pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] Note: we
                               have 2 DLL in CMD fub, which generate 4 piref
                               clocks. Please refer to ddrcc_piref_offset0
                               register definition for piref assignment for
                               each bit[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_pie_offset : 7;

                            /* Bits[16:10], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the cmd_pio_code. The
                               result is the cmd pi even code (cmd_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and received data
                               (RX).[br] 0: CMD IO is used for RX [br] 1: CMD
                               IO is used for TX [br] Note: This signal need to
                               be qualify by IO direction valid control.[br]
                               Note: For ACIO, both RX/TX are enabled by ACIO
                               control.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state[br] 1: pull low (cke for
                               DDR4 or CS for DDR5)
                            */
    UINT32 pi_n_clk_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               PI N clock enable control. For IO that doesn't
                               require to have the PI N clock on. BIOS can set
                               the corresponding bit to 0 to save power. For
                               example DDRT REQ, alert input don't require PI N
                               clock to receive the data. They are recive on
                               the DCLK.
                            */
    UINT32 io_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable this IO if this CMD IO pin is
                               not used.[br] Currently use case is for DDR5
                               RDIMM. We only pin out 7 of the IOs. The other 7
                               IOs are not pinout.
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable to Command buf. */
    UINT32 rx_unsample_path_sel : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               0: sampled path - ERID (DDRT), REQ (DDRT2)[br]
                               1: unsampled path - ALERT, REQ (DDRT), ERR
                               (DDRT/DDRT2), RSP_A/RSP_B (DDR5)
                            */
    UINT32 cs_para_sel : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               0: this bit uses CA parameter for ca_imode_en,
                               ca_imode_eq_code, ca_txeq_1st_tap and
                               ca_txeq_2nd_tap.[br] 1: this bit uses CS
                               parameter for cs_imode_en, cs_imode_eq_code,
                               cs_txeq_1st_tap and cs_txeq_2nd_tap
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /* RX FIFO pointer separation control */
    UINT32 sa_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Sense Amplifier enable to Command buf. */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_ui : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /*
                               Transmit Unit Interval. [br] Bit 1: Set to 1 if
                               TX UI is 0.5 Dclk [br] Bit 0: Set to 1 if TX UI
                               is 2 DCLK. [br] - DDR5 RDIMM CMD bit (CA7) (1N
                               mode) : 2'b10 (0.5 Dclk) [br] - DDR5 UDIMM CMD
                               bit (CA14) (1N mode) : 2'b00 (1 Dclk) [br] -
                               DDR5 RDIMM CMD bit (CA7) (2N mode) : 2'b00 (1
                               Dclk) [br] - DDR5 UDIMM CMD bit (CA14) (2N mode)
                               : 2'b01 (2 Dclk) [br] - DDR5 CTL bit : 2'b00 (1
                               Dclk) [br] - DDR4 CMD/CTL bit (1N mode) : 2'b00
                               (1 Dclk) [br] - DDR4 CMD bit (2N mode) : 2'b01
                               (2 Dclk)
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL0_BIT_21_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_RXVREF_CTL_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012a58)                                                      */
/*      SPRB0 (0x20012a58)                                                      */
/*      SPRHBM (0x20012a58)                                                     */
/*      SPRC0 (0x20012a58)                                                      */
/*      SPRMCC (0x20012a58)                                                     */
/*      SPRUCC (0x20012a58)                                                     */
/* Register default value on SPRA0: 0x0000BFA0                                  */
/* Register default value on SPRB0: 0x0000BFA0                                  */
/* Register default value on SPRHBM: 0x0000BFA0                                 */
/* Register default value on SPRC0: 0x0000BFA0                                  */
/* Register default value on SPRMCC: 0x0000BFA0                                 */
/* Register default value on SPRUCC: 0x0000BFA0                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX VREF Control Register
*/


#define DDRCC_RXVREF_CTL_MCIO_DDRIOEXT_REG 0x08122A58

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_vref_ctl0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000A0*/

                            /*
                               RX Vref control 0 with step size is
                               vccddr_hv/256. Default is 62.5% vccddr_hv.[br]
                               These control is applied for cc bit 0 to 11[br]
                               Note: This vref is intended to use for alert_n
                            */
    UINT32 rx_vref_ctl1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x000000BF*/

                            /*
                               RX Vref control 1 with step size is
                               vccddr_hv/256. Default is 75% vccddr_hv.[br]
                               These control is applied for cc bit 12 to 21[br]
                               Note: This vref is intended to use for
                               REQ/ERR/RSP_A/RSP_B
                            */
    UINT32 rsvd : 14;

                            /* Bits[29:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_RXVREF_CTL_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012a5c)                                                      */
/*      SPRB0 (0x20012a5c)                                                      */
/*      SPRHBM (0x20012a5c)                                                     */
/*      SPRC0 (0x20012a5c)                                                      */
/*      SPRMCC (0x20012a5c)                                                     */
/*      SPRUCC (0x20012a5c)                                                     */
/* Register default value on SPRA0: 0x00000408                                  */
/* Register default value on SPRB0: 0x00000408                                  */
/* Register default value on SPRHBM: 0x00000408                                 */
/* Register default value on SPRC0: 0x00000408                                  */
/* Register default value on SPRMCC: 0x00000408                                 */
/* Register default value on SPRUCC: 0x00000408                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Late Action Register for RCOMP DRV UP/DN. It is updated either by COMP training FSM or SW. The promotion from late acton register to effective register is controlled by COMP FSM after quiescent request is granted from MC.
*/


#define DDRCC_COMP_LA0_MCIO_DDRIOEXT_REG 0x08122A5C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up : 5;

                            /* Bits[4:0], Access Type=RW/V, default=0x00000008*/

                            /*
                               CMD Rcomp Drive Up Comp Value. It is trained by
                               COMP FSM.[br] This value is added with
                               rcomp_drv_up_adj (signed number. Range is -8 to
                               7) before send it to the cmdbuf.[br] Note: HW
                               checks for overflow and underflow.
                            */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000008*/

                            /*
                               CMD Rcomp Drive Down Comp Value. It is trained
                               by COMP FSM.[br] This value is added with
                               rcomp_drv_dn_adj (signed number. Range is -8 to
                               7) before send it to the cmdbuf.[br] Note: HW
                               checks for overflow and underflow.
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_COMP_LA0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012a60)                                                      */
/*      SPRB0 (0x20012a60)                                                      */
/*      SPRHBM (0x20012a60)                                                     */
/*      SPRC0 (0x20012a60)                                                      */
/*      SPRMCC (0x20012a60)                                                     */
/*      SPRUCC (0x20012a60)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Late Action Register for SCOMP, and TCO COMP. It is updated by BIOS. The promotion from late acton register to effective register is controlled by COMP FSM after quiescent request is granted from MC.
*/


#define DDRCC_COMP_LA1_MCIO_DDRIOEXT_REG 0x08122A60

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tco_comp : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /*
                               CMD TCO Comp Value. It is program by BIOS but it
                               is not trained.[br] 0/64: fastest rise & fall
                               delay[br] 0->31: increasing fall delay[br]
                               64->95: increasing rise delay[br] Other values
                               are not used (32-63 & 96-127).[br] To get
                               monotonic increasing sweep of dutycycle, sweep
                               from (95 to 65, 0 to 31)
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scomp : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000000*/

                            /*
                               CMD Slew Rate Comp Value. It is trained by BIOS.
                               [br] 0 is fastest slew rate, 31 is slowest slew
                               rate. [br] The SCOMP Up/Down Code are generated
                               from the following equation:[br] SCOMP Up
                               code[lb]4:0[rb] = scomp[lb]4:0[rb] +
                               scomp_up_adj[lb]3:0[rb];[br] SCOMP Down
                               code[lb]4:0[rb] = scomp[lb]4:0[rb] +
                               scomp_dn_adj[lb]3:0[rb];[br] Note : both SCOMP
                               up Adjust (scomp_up_adj) and SCOMP down adjust
                               (scomp_dn_adj) are signed valued. The adjust
                               range is -8 to 7[br] Note: HW checks for
                               overflow and underflow.
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_COMP_LA1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012a64)                                                      */
/*      SPRB0 (0x20012a64)                                                      */
/*      SPRHBM (0x20012a64)                                                     */
/*      SPRC0 (0x20012a64)                                                      */
/*      SPRMCC (0x20012a64)                                                     */
/*      SPRUCC (0x20012a64)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* COMP offset values add on to SCOMP, RCOMP DRV up/down, and TCO COMP
*/


#define DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_REG 0x08122A64

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up_adj : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to rcomp_drv_up to generate
                               the RCOMP Drive up code. The adjust range is -8
                               to 7.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn_adj : 4;

                            /* Bits[8:5], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to rcomp_drv_dn to generate
                               the RCOMP Drive down code. The adjust range is
                               -8 to 7.
                            */
    UINT32 rsvd_9 : 11;

                            /* Bits[19:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scomp_up_adj : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to scomp to generate the SCOMP
                               Up Code. The adjust range is -8 to 7.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scomp_dn_adj : 4;

                            /* Bits[28:25], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to scomp to generate the SCOMP
                               Down Code. The adjust range is -8 to 7.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_COMP_OFFSET_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012a68)                                                      */
/*      SPRB0 (0x20012a68)                                                      */
/*      SPRHBM (0x20012a68)                                                     */
/*      SPRC0 (0x20012a68)                                                      */
/*      SPRMCC (0x20012a68)                                                     */
/*      SPRUCC (0x20012a68)                                                     */
/* Register default value on SPRA0: 0x00000118                                  */
/* Register default value on SPRB0: 0x00000118                                  */
/* Register default value on SPRHBM: 0x00000118                                 */
/* Register default value on SPRC0: 0x00000118                                  */
/* Register default value on SPRMCC: 0x00000118                                 */
/* Register default value on SPRUCC: 0x00000118                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* VSSHI Control Register
*/


#define DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_REG 0x08122A68

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vsshi_target_code : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000018*/

                            /*
                               VSSHI target code to vsshi vref control. Step
                               size for 10nm VssHi is vccddr_hv/256.[br] Code
                               should be programmed according to analog_tuning
                               field in dll status register:[br] DDR5/T2
                               DDR4/T[br] 3'd3: 0x45 0x55[br] 3'd1,3'd2: 0x3a
                               0x4a[br] 3'd0: 0x2e 0x40[br] Note: the vref_ctrl
                               MSB (Bit 7) is hardtie to 0 inside VSSHI. Only
                               the lower 7-bit is controlled by this field.
                            */
    UINT32 vsshi_bias_ctl : 2;

                            /* Bits[8:7], Access Type=RW, default=0x00000002*/

                            /*
                               Vsshi Bias Current trim.[br] The field should be
                               programmed according to analog_tuning field.[br]
                               00: min current (if analog_tuning=3d3, fast
                               skew)[br] 01: mid current (if
                               analog_tuning=3d1/3d2, typ skew)[br] 11: max
                               current (if analog_tuning=3d0, slow skew)
                            */
    UINT32 vsshi_pulldown_en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* Pull VSSHI to VSS */
    UINT32 vsshi_close_loop : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Operates VssHi in Close loop mode */
    UINT32 rsvd : 19;

                            /* Bits[29:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vsshi_tmr_ext : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Double the timer for VSSHI settle time. Use for
                               failsafe in case settle time was
                               underestimated.[br] 0: 8K+1K=9K Dclk cycles[br]
                               1: 16K+2K=18K Dclk cycles[br] Doubles the VSSHI
                               settle time counter in case hard-coded value is
                               not adequate
                            */
    UINT32 vsshi_dclk_gate_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK for VSSHI
                               control logic. Default is disable clock gating.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_VSSHI_CTL_MCIO_DDRIOEXT_STRUCT;



/* DDRCC_DLL_CSR_0_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012a74)                                                      */
/*      SPRB0 (0x20012a74)                                                      */
/*      SPRHBM (0x20012a74)                                                     */
/*      SPRC0 (0x20012a74)                                                      */
/*      SPRMCC (0x20012a74)                                                     */
/*      SPRUCC (0x20012a74)                                                     */
/* Register default value on SPRA0: 0x28000000                                  */
/* Register default value on SPRB0: 0x28000000                                  */
/* Register default value on SPRHBM: 0x28000000                                 */
/* Register default value on SPRC0: 0x28000000                                  */
/* Register default value on SPRMCC: 0x28000000                                 */
/* Register default value on SPRUCC: 0x28000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per DLL Control and Status Register
*/


#define DDRCC_DLL_CSR_0_MCIO_DDRIOEXT_REG 0x08122A74

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 detrm_lock : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* DLL Deterministic Lock Status */
    UINT32 raw_lock : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* DLL Raw Lock Status Sticky bit. */
    UINT32 lock_timeout : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* DLL Lock timeout. */
    UINT32 dll_reset : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* DLL Reset. */
    UINT32 adcout : 10;

                            /* Bits[13:4], Access Type=RO/V, default=0x00000000*/

                            /* DLL ADC out. */
    UINT32 long_lock : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* DLL Long Lock Status. */
    UINT32 detlock_earlylockstky : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Detect lock CBB early lock sticky output */
    UINT32 weak_lock : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* DLL Weak Lock Status. */
    UINT32 adc_done : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* DLL ADC done. */
    UINT32 pilags : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               DLL output of PiDFx FSM to indciate Pi lags. In
                               order to read this debug register, clock gating
                               must be disabled.
                            */
    UINT32 openloop : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* MDLL open loop output */
    UINT32 lockrst_b : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Detect lock CBB lock reset output */
    UINT32 analog_tuning : 3;

                            /* Bits[23:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               BIOS should configure certain analog components
                               based on the value of this register.[br] Details
                               will be found in the description of any CSR that
                               relies on this value
                            */
    UINT32 rsvd : 3;

                            /* Bits[26:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flybyclk_sel : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bypasses flyby clock and selects H-tree clock
                               when disbled
                            */
    UINT32 iofreq_range : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /*
                               BIOS program this IO Frequency range field based
                               on PLL freq; It is half of the DRAM transfer
                               rate. For example DDR 4800. The PLL freq is
                               2400.[br] 2b00: 933MHz - 1333MHz (i.e. 1867MT/s
                               to 2667MT/s)[br] 2b01: 1467MHz 1600MHz (i.e.
                               2933MT/s to 3200MT/s)[br] 2b10: 1800MHz 2000MHz
                               (i.e. 3600MT/s to 4000MT/s)[br] 2b11: 2200MHz
                               2400MHz, or above (i.e. 4400MT/s to 4800MT/s)
                            */
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dll_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS must enable DLL when program the IO freq
                               range.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_DLL_CSR_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_DLL_CSR_1_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012a78)                                                      */
/*      SPRB0 (0x20012a78)                                                      */
/*      SPRHBM (0x20012a78)                                                     */
/*      SPRC0 (0x20012a78)                                                      */
/*      SPRMCC (0x20012a78)                                                     */
/*      SPRUCC (0x20012a78)                                                     */
/* Register default value on SPRA0: 0x28000000                                  */
/* Register default value on SPRB0: 0x28000000                                  */
/* Register default value on SPRHBM: 0x28000000                                 */
/* Register default value on SPRC0: 0x28000000                                  */
/* Register default value on SPRMCC: 0x28000000                                 */
/* Register default value on SPRUCC: 0x28000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per DLL Control and Status Register
*/


#define DDRCC_DLL_CSR_1_MCIO_DDRIOEXT_REG 0x08122A78

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 detrm_lock : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* DLL Deterministic Lock Status */
    UINT32 raw_lock : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* DLL Raw Lock Status Sticky bit. */
    UINT32 lock_timeout : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* DLL Lock timeout. */
    UINT32 dll_reset : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* DLL Reset. */
    UINT32 adcout : 10;

                            /* Bits[13:4], Access Type=RO/V, default=0x00000000*/

                            /* DLL ADC out. */
    UINT32 long_lock : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* DLL Long Lock Status. */
    UINT32 detlock_earlylockstky : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Detect lock CBB early lock sticky output */
    UINT32 weak_lock : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* DLL Weak Lock Status. */
    UINT32 adc_done : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* DLL ADC done. */
    UINT32 pilags : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               DLL output of PiDFx FSM to indciate Pi lags. In
                               order to read this debug register, clock gating
                               must be disabled.
                            */
    UINT32 openloop : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* MDLL open loop output */
    UINT32 lockrst_b : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Detect lock CBB lock reset output */
    UINT32 analog_tuning : 3;

                            /* Bits[23:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               BIOS should configure certain analog components
                               based on the value of this register.[br] Details
                               will be found in the description of any CSR that
                               relies on this value
                            */
    UINT32 rsvd : 3;

                            /* Bits[26:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flybyclk_sel : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bypasses flyby clock and selects H-tree clock
                               when disbled
                            */
    UINT32 iofreq_range : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /*
                               BIOS program this IO Frequency range field based
                               on PLL freq; It is half of the DRAM transfer
                               rate. For example DDR 4800. The PLL freq is
                               2400.[br] 2b00: 933MHz - 1333MHz (i.e. 1867MT/s
                               to 2667MT/s)[br] 2b01: 1467MHz 1600MHz (i.e.
                               2933MT/s to 3200MT/s)[br] 2b10: 1800MHz 2000MHz
                               (i.e. 3600MT/s to 4000MT/s)[br] 2b11: 2200MHz
                               2400MHz, or above (i.e. 4400MT/s to 4800MT/s)
                            */
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dll_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS must enable DLL when program the IO freq
                               range.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_DLL_CSR_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_OFFSET0_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20012a7c)                                                      */
/*      SPRB0 (0x20012a7c)                                                      */
/*      SPRHBM (0x20012a7c)                                                     */
/*      SPRC0 (0x20012a7c)                                                      */
/*      SPRMCC (0x20012a7c)                                                     */
/*      SPRUCC (0x20012a7c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training offset control CMD delay
*/


#define DDRCC_TRAIN_OFFSET0_MCIO_DDRIOEXT_REG 0x08122A7C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_dly_adj : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               CMD delay adjust value. It is added to all per
                               bit command delay. It is used by BIOS for
                               margining sweep. The adjust range is -128 to
                               127. The expected usage model is BIOS broadcast
                               the same adjust value to all the command fubs.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_OFFSET0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG supported on:                        */
/*      SPRA0 (0x20012a80)                                                      */
/*      SPRB0 (0x20012a80)                                                      */
/*      SPRHBM (0x20012a80)                                                     */
/*      SPRC0 (0x20012a80)                                                      */
/*      SPRMCC (0x20012a80)                                                     */
/*      SPRUCC (0x20012a80)                                                     */
/* Register default value on SPRA0: 0x3F002000                                  */
/* Register default value on SPRB0: 0x3F002000                                  */
/* Register default value on SPRHBM: 0x3F002000                                 */
/* Register default value on SPRC0: 0x3F002000                                  */
/* Register default value on SPRMCC: 0x3F002000                                 */
/* Register default value on SPRUCC: 0x3F002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for PI N clocks[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG 0x08122A80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pio_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI odd code to align the PIREF
                               odd clock to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_pio_code to DLL. The xover
                               input is driven from a failing edge of DCLK
                               flop. [br] By moving the piref odd clock to the
                               right by 63 ticks, we will have 63 pi tick of
                               setup margin.[br] This field can be programed by
                               BIOS or HW cal FSM.
                            */
    UINT32 piref_pie_offset : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the piref_pio_code. The
                               result is the piref pi even code
                               (piref_pie_code) to DLL. Default value is 64.
                            */
    UINT32 cmd_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This cmd piref offset is added with a signed
                               piref_setup_adj (default value is 63). [br] The
                               result is added to each cmd_dly[6:0], so each
                               cmd pi odd code to the DLL is relative to the
                               piref odd clock.[br] This field can be programed
                               by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 piref_setup_adj : 7;

                            /* Bits[30:24], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 63 ticks, and we will
                               have 63 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG supported on:                        */
/*      SPRA0 (0x20012a84)                                                      */
/*      SPRB0 (0x20012a84)                                                      */
/*      SPRHBM (0x20012a84)                                                     */
/*      SPRC0 (0x20012a84)                                                      */
/*      SPRMCC (0x20012a84)                                                     */
/*      SPRUCC (0x20012a84)                                                     */
/* Register default value on SPRA0: 0x3F002000                                  */
/* Register default value on SPRB0: 0x3F002000                                  */
/* Register default value on SPRHBM: 0x3F002000                                 */
/* Register default value on SPRC0: 0x3F002000                                  */
/* Register default value on SPRMCC: 0x3F002000                                 */
/* Register default value on SPRUCC: 0x3F002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for PI N clocks[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG 0x08122A84

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pio_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI odd code to align the PIREF
                               odd clock to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_pio_code to DLL. The xover
                               input is driven from a failing edge of DCLK
                               flop. [br] By moving the piref odd clock to the
                               right by 63 ticks, we will have 63 pi tick of
                               setup margin.[br] This field can be programed by
                               BIOS or HW cal FSM.
                            */
    UINT32 piref_pie_offset : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the piref_pio_code. The
                               result is the piref pi even code
                               (piref_pie_code) to DLL. Default value is 64.
                            */
    UINT32 cmd_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This cmd piref offset is added with a signed
                               piref_setup_adj (default value is 63). [br] The
                               result is added to each cmd_dly[6:0], so each
                               cmd pi odd code to the DLL is relative to the
                               piref odd clock.[br] This field can be programed
                               by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 piref_setup_adj : 7;

                            /* Bits[30:24], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 63 ticks, and we will
                               have 63 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG supported on:                        */
/*      SPRA0 (0x20012a88)                                                      */
/*      SPRB0 (0x20012a88)                                                      */
/*      SPRHBM (0x20012a88)                                                     */
/*      SPRC0 (0x20012a88)                                                      */
/*      SPRMCC (0x20012a88)                                                     */
/*      SPRUCC (0x20012a88)                                                     */
/* Register default value on SPRA0: 0x3F002000                                  */
/* Register default value on SPRB0: 0x3F002000                                  */
/* Register default value on SPRHBM: 0x3F002000                                 */
/* Register default value on SPRC0: 0x3F002000                                  */
/* Register default value on SPRMCC: 0x3F002000                                 */
/* Register default value on SPRUCC: 0x3F002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for PI N clocks[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG 0x08122A88

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pio_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI odd code to align the PIREF
                               odd clock to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_pio_code to DLL. The xover
                               input is driven from a failing edge of DCLK
                               flop. [br] By moving the piref odd clock to the
                               right by 63 ticks, we will have 63 pi tick of
                               setup margin.[br] This field can be programed by
                               BIOS or HW cal FSM.
                            */
    UINT32 piref_pie_offset : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the piref_pio_code. The
                               result is the piref pi even code
                               (piref_pie_code) to DLL. Default value is 64.
                            */
    UINT32 cmd_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This cmd piref offset is added with a signed
                               piref_setup_adj (default value is 63). [br] The
                               result is added to each cmd_dly[6:0], so each
                               cmd pi odd code to the DLL is relative to the
                               piref odd clock.[br] This field can be programed
                               by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 piref_setup_adj : 7;

                            /* Bits[30:24], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 63 ticks, and we will
                               have 63 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG supported on:                        */
/*      SPRA0 (0x20012a8c)                                                      */
/*      SPRB0 (0x20012a8c)                                                      */
/*      SPRHBM (0x20012a8c)                                                     */
/*      SPRC0 (0x20012a8c)                                                      */
/*      SPRMCC (0x20012a8c)                                                     */
/*      SPRUCC (0x20012a8c)                                                     */
/* Register default value on SPRA0: 0x3F002000                                  */
/* Register default value on SPRB0: 0x3F002000                                  */
/* Register default value on SPRHBM: 0x3F002000                                 */
/* Register default value on SPRC0: 0x3F002000                                  */
/* Register default value on SPRMCC: 0x3F002000                                 */
/* Register default value on SPRUCC: 0x3F002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for PI N clocks[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG 0x08122A8C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pio_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI odd code to align the PIREF
                               odd clock to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_pio_code to DLL. The xover
                               input is driven from a failing edge of DCLK
                               flop. [br] By moving the piref odd clock to the
                               right by 63 ticks, we will have 63 pi tick of
                               setup margin.[br] This field can be programed by
                               BIOS or HW cal FSM.
                            */
    UINT32 piref_pie_offset : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the piref_pio_code. The
                               result is the piref pi even code
                               (piref_pie_code) to DLL. Default value is 64.
                            */
    UINT32 cmd_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This cmd piref offset is added with a signed
                               piref_setup_adj (default value is 63). [br] The
                               result is added to each cmd_dly[6:0], so each
                               cmd pi odd code to the DLL is relative to the
                               piref odd clock.[br] This field can be programed
                               by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 piref_setup_adj : 7;

                            /* Bits[30:24], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 63 ticks, and we will
                               have 63 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG supported on:                                */
/*      SPRA0 (0x20012a90)                                                      */
/*      SPRB0 (0x20012a90)                                                      */
/*      SPRHBM (0x20012a90)                                                     */
/*      SPRC0 (0x20012a90)                                                      */
/*      SPRMCC (0x20012a90)                                                     */
/*      SPRUCC (0x20012a90)                                                     */
/* Register default value on SPRA0: 0x07C0900B                                  */
/* Register default value on SPRB0: 0x07C0900B                                  */
/* Register default value on SPRHBM: 0x07C0900B                                 */
/* Register default value on SPRC0: 0x07C09003                                  */
/* Register default value on SPRMCC: 0x07C09003                                 */
/* Register default value on SPRUCC: 0x07C09003                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX control 0 Register
*/


#define DDRCC_RX_CTL0_MCIO_DDRIOEXT_REG 0x08122A90

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 odt_seg_en : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /*
                               Control number of ODT segment enable for cmd
                               buf.
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_combiner_alert_mode : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Force the Rx amplifier combiner to a lower
                               static power state during Alert and DDR-T
                               modes,[br] which reduces FiSH temperatures and
                               helps RV.
                            */
    UINT32 rsvd_4 : 6;

                            /* Bits[9:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_path_ungate : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               SW set this bit to 1 to ungate the DDRIO to MC
                               RX data path.
                            */
    UINT32 rxgainfuse_ovrd : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Repurposed for force local receiver bias on for
                               DQbuf.[br] When set to 1, it will keep the local
                               bias mirror always enabled.[br] When set to 0,
                               local bias gets enabled with senseamp_enable.
                            */
    UINT32 rxbias_vcm_ctl : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000001*/

                            /*
                               Rx summer output common mode control[br] 0:
                               0.25*vccddra[br] 1: 0.3*vccddra (default)[br] 2:
                               0.35*vccddra[br] 3: 0.4*vccddra
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxbias_rxgain_ctl : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000001*/

                            /*
                               00 1.5x (default for CMD/CLK)[br] 01 2x[br] 10 -
                               2.5x (default for DQ)[br] 11 3x
                            */
    UINT32 rsvd_17 : 5;

                            /* Bits[21:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 odt_static_leg_cfg : 5;

                            /* Bits[26:22], Access Type=RW, default=0x0000001F*/

                            /*
                               Sets number of ODT static legs. First 3 MSB bits
                               has weight of 4, 4th MSB has weight of 2 and LSB
                               bit has weight of 1. [br] Default of 0x1F has 15
                               static legs enabled.[br] Static leg training
                               starts with 0x1F (all 15 legs enabled)
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_RX_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_RX_CTL1_MCIO_DDRIOEXT_REG supported on:                                */
/*      SPRA0 (0x20012a94)                                                      */
/*      SPRB0 (0x20012a94)                                                      */
/*      SPRHBM (0x20012a94)                                                     */
/*      SPRC0 (0x20012a94)                                                      */
/*      SPRMCC (0x20012a94)                                                     */
/*      SPRUCC (0x20012a94)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX control 1 Register
*/


#define DDRCC_RX_CTL1_MCIO_DDRIOEXT_REG 0x08122A94

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_offset_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /* enable offset control in cmd buf and RX BIAS */
    UINT32 rsvd_7 : 22;

                            /* Bits[28:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_RX_CTL1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TX_CTL0_MCIO_DDRIOEXT_REG supported on:                                */
/*      SPRA0 (0x20012a9c)                                                      */
/*      SPRB0 (0x20012a9c)                                                      */
/*      SPRHBM (0x20012a9c)                                                     */
/*      SPRC0 (0x20012a9c)                                                      */
/*      SPRMCC (0x20012a9c)                                                     */
/*      SPRUCC (0x20012a9c)                                                     */
/* Register default value on SPRA0: 0x800000DF                                  */
/* Register default value on SPRB0: 0x800000DF                                  */
/* Register default value on SPRHBM: 0x800000DF                                 */
/* Register default value on SPRC0: 0x800000DF                                  */
/* Register default value on SPRMCC: 0x800000DF                                 */
/* Register default value on SPRUCC: 0x800000DF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX control 0 Register
*/


#define DDRCC_TX_CTL0_MCIO_DDRIOEXT_REG 0x08122A9C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 drv_static_leg_cfg : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001F*/

                            /*
                               Sets number of CmdCtl driver (Ron) static legs.
                               [br] First 3 MSB bits has weight of 4, 4th MSB
                               has weight of 2 and LSB bit has weight of 1.
                               [br] Hardware default of 0x1F has 15 static legs
                               enabled. [br] Static leg training starts with
                               0x1F (all 15 legs enabled)
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_seg_en : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /*
                               Control number of drive segment enable for CMD
                               buf. Also, it is used by COMP to TXEQ logic to
                               generate the EQCODE.
                            */
    UINT32 rsvd_8 : 2;

                            /* Bits[9:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cs_imode_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               CS Enable Imode EQ per byte lane. [br] 0: Imode
                               completely off [br] 1: Imode enabled (Xtalk
                               cancellation OR swing boost selected by ImodeCfg
                            */
    UINT32 ca_imode_en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               CA Enable Imode EQ per byte lane. [br] 0: Imode
                               completely off [br] 1: Imode enabled (Xtalk
                               cancellation OR swing boost selected by ImodeCfg
                            */
    UINT32 cs_eq_post2_sign : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               CS 2nd tap equalization sign bit[br] 0: +ve
                               equalization[br] 1: -ve equalization
                            */
    UINT32 ca_eq_post2_sign : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               CA 2nd tap equalization sign bit[br] 0: +ve
                               equalization[br] 1: -ve equalization
                            */
    UINT32 cs_eq_post2_sel : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               CS 2nd tap selection:[br] 0 txeq_2nd_tap used
                               for 2nd post cursor tap[br] 1 txeq_2nd_tap used
                               for extending range of 1st tap.
                            */
    UINT32 ca_eq_post2_sel : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               CA 2nd tap selection:[br] 0 txeq_2nd_tap used
                               for 2nd post cursor tap[br] 1 txeq_2nd_tap used
                               for extending range of 1st tap.
                            */
    UINT32 cs_imode_eq_code : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /*
                               CS Binary Imode Coefficient, sets Imode current
                               strength.[br] (used for either Xtalk
                               cancellation or swing boost)[br] Imode
                               coefficient, sets Imode current. Sweep values:
                               (0-15, 40-42, 48-63) for a linear increase of
                               current from 0 to max.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ca_imode_eq_code : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               CA Binary Imode Coefficient, sets Imode current
                               strength.[br] (used for either Xtalk
                               cancellation or swing boost)[br] Imode
                               coefficient, sets Imode current. Sweep values:
                               (0-15, 40-42, 48-63) for a linear increase of
                               current from 0 to max.
                            */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_serializer_pwradj : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to extend the pulse width of the pulsed
                               clocks to the Tx serializers.[br] No need skew
                               based settings, value of 1 seems to work across
                               PVT.[br] May need to set to 0 for slow silicon
                               if we see issues in post silicon.
                            */
    UINT32 scomp_tco_bypass_dis : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               0 : CMD BUF bypass SCOMP and TCO delay in the
                               transmitter.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_TX_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TX_CTL1_MCIO_DDRIOEXT_REG supported on:                                */
/*      SPRA0 (0x20012aa0)                                                      */
/*      SPRB0 (0x20012aa0)                                                      */
/*      SPRHBM (0x20012aa0)                                                     */
/*      SPRC0 (0x20012aa0)                                                      */
/*      SPRMCC (0x20012aa0)                                                     */
/*      SPRUCC (0x20012aa0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX control 1 Register
*/


#define DDRCC_TX_CTL1_MCIO_DDRIOEXT_REG 0x08122AA0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_dir_valid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Set by BIOS to indicate the io_dir control field
                               is valid.[br] - if this bit is not set, force
                               the receive data to 0[br] - it is used by the
                               command buf CBB to latch the io_pwrdn_pull_low
                               siganl when set to 1 by BIOS. That sequential is
                               power by always on power (s3localvcc), so when
                               analog power is down, the buf use it to
                               determine to either pull low or tri-state the
                               IO.
                            */
    UINT32 cs_txeq_1st_tap : 6;

                            /* Bits[6:1], Access Type=RW, default=0x00000000*/

                            /*
                               CS TxEQ 1st tap coefficient.The following list
                               all the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ca_txeq_1st_tap : 6;

                            /* Bits[13:8], Access Type=RW, default=0x00000000*/

                            /*
                               CA TxEQ 1st tap coefficient.The following list
                               all the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cs_txeq_2nd_tap : 4;

                            /* Bits[18:15], Access Type=RW, default=0x00000000*/

                            /*
                               CS TxEQ 2nd tap coefficient.The following list
                               all the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ca_txeq_2nd_tap : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               CA TxEQ 2nd tap coefficient.The following list
                               all the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_TX_CTL1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012aa8)                                                      */
/*      SPRB0 (0x20012aa8)                                                      */
/*      SPRHBM (0x20012aa8)                                                     */
/*      SPRC0 (0x20012aa8)                                                      */
/*      SPRMCC (0x20012aa8)                                                     */
/*      SPRUCC (0x20012aa8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DFX Control 0 Register
*/


#define DDRCC_DFX_CTL0_MCIO_DDRIOEXT_REG 0x08122AA8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tmr_test_mode : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Simulation-only control to shorten VSSHI and LDO
                               timers
                            */
    UINT32 sr_vref_dis : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in Self Refresh. The
                               cmd Vref generators is enabled by
                               rx_vref_en.[br] 1 - The cmd Vref generators are
                               switched off while in Self Refresh regardless
                               the setting of rx_vref_en.
                            */
    UINT32 ckepd_vref_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in CKE power down. All
                               CMDCTL Vref generators is enabled by
                               rx_vref_en.[br] 1 - CMDCTL Vref generators are
                               switched off while in CKE power down regardless
                               the setting of rx_vref_en.
                            */
    UINT32 rsvd_6 : 4;

                            /* Bits[9:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_voc_pass_gate_en : 22;

                            /* Bits[31:10], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit VOC RX pass gate enable for VOC training
                               and NTL testing.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_DFX_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20012aac)                                                      */
/*      SPRB0 (0x20012aac)                                                      */
/*      SPRHBM (0x20012aac)                                                     */
/*      SPRC0 (0x20012aac)                                                      */
/*      SPRMCC (0x20012aac)                                                     */
/*      SPRUCC (0x20012aac)                                                     */
/* Register default value on SPRA0: 0x00002000                                  */
/* Register default value on SPRB0: 0x00002000                                  */
/* Register default value on SPRHBM: 0x00002000                                 */
/* Register default value on SPRC0: 0x00002000                                  */
/* Register default value on SPRMCC: 0x00002000                                 */
/* Register default value on SPRUCC: 0x00002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW Training Control 0 Register
*/


#define DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG 0x08122AAC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_stage_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable xover training stage */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_summer_offset_stage_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable DFE summer offset training
                               stage for all bits.
                            */
    UINT32 rsvd_3 : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_5 : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 settling_time_x2 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to double the analog settling time.
                               [br] The default settling time for each stage
                               are [br] Xover : 32 Dclk for first iteration, 24
                               Dclk for subsequent [br] DFE summer offset : 24
                               Dclk [br] Txpiclk : 24 Dclk [br] Loop Delay: 8
                               Dclk
                            */
    UINT32 sample_cnt : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of Sample to evaluate for each iteration
                               step. It applied for all training stages. [br]
                               0: 16[br] 1: 32[br] 2: 64[br] 3: 2 (Should only
                               used to speed up digital simulation)
                            */
    UINT32 sample_thresh : 6;

                            /* Bits[15:10], Access Type=RW, default=0x00000008*/

                            /* Sample threshold value for voting logic. */
    UINT32 rsvd_16 : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_18 : 10;

                            /* Bits[27:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hw_cal_en : 4;

                            /* Bits[31:28], Access Type=RW/V, default=0x00000000*/

                            /*
                               HW calibation enable control per FSM (cmd has
                               4). Set to 1 to enable the HW calibration. [br]
                               They are 4 HW calibration stages that SW can
                               request HW to perform. It is defined in bit 0 to
                               4 in this register. [br] HW cal FSM will self
                               clear this bit when all the enabled calibration
                               stages are completed. [br] For BIOS training,
                               XOVER, sampler offset, and DFE summer offset
                               training can be enabled by SW.[br] For ACIO
                               testing, all 4 stages can be enabled by SW.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012ab0)                                                      */
/*      SPRB0 (0x20012ab0)                                                      */
/*      SPRHBM (0x20012ab0)                                                     */
/*      SPRC0 (0x20012ab0)                                                      */
/*      SPRMCC (0x20012ab0)                                                     */
/*      SPRUCC (0x20012ab0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW calibration status 0 Register[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_REG 0x08122AB0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed for any bit within this piref
                               group. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 rsvd_2 : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012ab4)                                                      */
/*      SPRB0 (0x20012ab4)                                                      */
/*      SPRHBM (0x20012ab4)                                                     */
/*      SPRC0 (0x20012ab4)                                                      */
/*      SPRMCC (0x20012ab4)                                                     */
/*      SPRUCC (0x20012ab4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW calibration status 0 Register[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_REG 0x08122AB4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed for any bit within this piref
                               group. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 rsvd_2 : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012ab8)                                                      */
/*      SPRB0 (0x20012ab8)                                                      */
/*      SPRHBM (0x20012ab8)                                                     */
/*      SPRC0 (0x20012ab8)                                                      */
/*      SPRMCC (0x20012ab8)                                                     */
/*      SPRUCC (0x20012ab8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW calibration status 0 Register[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_REG 0x08122AB8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed for any bit within this piref
                               group. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 rsvd_2 : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012abc)                                                      */
/*      SPRB0 (0x20012abc)                                                      */
/*      SPRHBM (0x20012abc)                                                     */
/*      SPRC0 (0x20012abc)                                                      */
/*      SPRMCC (0x20012abc)                                                     */
/*      SPRUCC (0x20012abc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW calibration status 0 Register[br]
                 reg 3: DLL 1 - piref for IO[lb]21:16[rb][br]
                 reg 2: DLL 1 - piref for IO[lb]15:12[rb][br]
                 reg 1: DLL 0 - piref for IO[lb]11:6[rb][br]
                 reg 0: DLL 0 - piref for IO[lb]5:0[rb]
*/


#define DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_REG 0x08122ABC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed for any bit within this piref
                               group. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 rsvd_2 : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_STRUCT;





/* DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012ad4)                                                      */
/*      SPRB0 (0x20012ad4)                                                      */
/*      SPRHBM (0x20012ad4)                                                     */
/*      SPRC0 (0x20012ad4)                                                      */
/*      SPRMCC (0x20012ad4)                                                     */
/*      SPRUCC (0x20012ad4)                                                     */
/* Register default value on SPRA0: 0x18000000                                  */
/* Register default value on SPRB0: 0x18000000                                  */
/* Register default value on SPRHBM: 0x18000000                                 */
/* Register default value on SPRC0: 0x18000000                                  */
/* Register default value on SPRMCC: 0x18000000                                 */
/* Register default value on SPRUCC: 0x18000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 1 Register
*/


#define DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG 0x08122AD4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_train_ctr_en : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Enables the Command Training Counter mode
                               (receivers, ODT needs to be enabled separately
                               before setting this bit), resets the sample
                               counter and starts the counting of the samples.
                               One of these per nibble that is used for
                               feedback (i.e. all DQ nibble and the fub with
                               the ALERT_n feedback. DQ nibbles can be set with
                               broadcast). The value of this field remains at 1
                               until after the Counting Window expires, then
                               returns to 0. Can be polled to determine when
                               the counting has completed. When the Counting
                               Window field is set to 0, this field will not
                               return to 0. In that case, this field must be
                               set to 0 to disable.[br] When the CS Assertion
                               Trigger Enable is set, the counting window start
                               is delayed until the first CS Assertion event
                               but this delay only applies for the ALERT_n,
                               RSP_A, and RSP_B signals. This does not apply to
                               the DQ counters.
                            */
    UINT32 ddrt2_req_train_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable DDR-T2 Request training */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_path_fb_en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable sample path and unsample path
                               feedback results. [br] Note: It is used for SW
                               SA training and SW ACIO txpiclk training.
                            */
    UINT32 xover_cal : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 will prepare data fub in a
                               state to allow BIOS only to perform Xover
                               calibraton training. - enable all the DCLK And
                               PI clocks. (may remove clock gating function)
                            */
    UINT32 vref_highz : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Tri-state the vref_hv output. Set to 1 for NTL
                               and Rx Summer Offset Cal.
                            */
    UINT32 rsvd_7 : 5;

                            /* Bits[11:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddr4 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               For SPR, 2 Channel DDR-MC sub-system. It can
                               either operate in DDR4/DDR-T or DDR5/DDR-T2. It
                               doesn't support DDR4/DDR-T mix with
                               DDR5/DDR-T2.[br] Setting this bit to 1 indicate
                               it is operating in DDR4/DDR-T. 0 inidate it is
                               operating in DDR5/DDR-T2.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 compeff_clk_gate_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate effective register
                               update CLK for power saving. Default is disable
                               clock gating.[br] Note: Clock gating must be
                               disable during any training steps. BIOS can
                               enable clock gating for power saving after all
                               the training steps are done.
                            */
    UINT32 bclk_gate_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable BCLK gating for Burnin/ADC
                               counter and DLL lock timer for power saving.
                            */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pi_clk_gate_en : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate all PI N clocks
                               for power saving. Default is disable clock
                               gating. [br] Note: Clock gating must be disable
                               during any training steps. BIOS can enable clock
                               gating for power saving after all the training
                               steps are done.
                            */
    UINT32 piref_clk_gate_en : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate PI REF clock for
                               power saving. Default is disable clock gating.
                               [br] Note: Clock gating must be disable during
                               any training steps. BIOS can enable clock gating
                               for power saving after all the training steps
                               are done.
                            */
    UINT32 tx_clk_gate_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate TX CLK for
                               transmit path logics. Default is disable clock
                               gating.[br] Note: Clock gating must be disable
                               during any training steps. BIOS can enable clock
                               gating for power saving after all the training
                               steps are done.
                            */
    UINT32 rx_clk_gate_en : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate RX CLK for receive
                               path logics. Default is disable clock gating.
                               [br] Note: Clock gating must be disable during
                               any training steps. BIOS can enable clock gating
                               for power saving after all the training steps
                               are done.
                            */
    UINT32 train_clk_gate_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate CLK for training
                               related logic (BIOS or HW training logics).
                               Default is disable clock gating. [br] Note:
                               Clock gating must be disable during any training
                               steps. BIOS can enable clock gating for power
                               saving after all the training steps are done.
                            */
    UINT32 csr_clk_gate_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate CSR CLK. Default
                               is disable clock gating. [br] Note: Clock gating
                               must be disable during any training steps. BIOS
                               can enable clock gating for power saving after
                               all the training steps are done.
                            */
    UINT32 rsvd_23 : 2;

                            /* Bits[24:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_strong_odt_on : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Force all the CMD IO strong ODT on. When set, it
                               always enable one segment for ODT regardless of
                               the state of drive enable. [br] The intend use
                               case is for ACIO with burnin
                            */
    UINT32 force_txon : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* Forces all the CMD transmitter driver on. */
    UINT32 force_odt_on : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Force all the CMD IO weak ODT on. The weak ODT
                               is ignored when drive enable is asserted. [br]
                               Note: For ACIO, ODT is provided by different DDR
                               channel.
                            */
    UINT32 force_rxbias_on : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /* Force Rx Bias and Rx Bias Vref On */
    UINT32 force_sa_on : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Force Senseamp on. Use for overclocking support
                               where we may not be able to hit the fast exit
                               latency.
                            */
    UINT32 rx_vref_en : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               Per Vref control to enable RX Vref.[br] Bit 0:
                               signal index 0-11 [br] Bit 1: signal index 12-21
                            */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012ad8)                                                      */
/*      SPRB0 (0x20012ad8)                                                      */
/*      SPRHBM (0x20012ad8)                                                     */
/*      SPRC0 (0x20012ad8)                                                      */
/*      SPRMCC (0x20012ad8)                                                     */
/*      SPRUCC (0x20012ad8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 2 Register
*/


#define DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG 0x08122AD8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 train_cnt_io_sel0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select which IO to count during DCS and DCA
                               training. The count result is stored in
                               ddr_train_cnt_result0 register 0.[br] Note: The
                               current supporting mode is counting only ALERT_n
                               or counting on both RSP_A and RSP_B signals.
                            */
    UINT32 train_cnt_io_sel1 : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /*
                               Select which IO to count during DCS and DCA
                               training. The count result is stored in
                               ddr_train_cnt_result0 register 1.[br] Note: The
                               current supporting mode is counting only ALERT_n
                               or counting on both RSP_A and RSP_B signals.
                            */
    UINT32 csa_trigger_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, the counting window does not
                               start until the first CS assertion (any CS
                               assertion). This trigger only applies for the
                               counters associated with the ALERT_n, RSP_A, and
                               RSP_B signals. This does not apply to the DQ
                               counters.
                            */
    UINT32 sample_sel : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Determines if the command training counter will
                               count 1 or 0.[br] 0: count 1[br] 1: count 0
                            */
    UINT32 dll0_top_xover_cal_mux_sel : 5;

                            /* Bits[16:12], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for DLL0
                               Top piref calibration. Legal value is 0 to 17.
                            */
    UINT32 dll0_bot_xover_cal_mux_sel : 5;

                            /* Bits[21:17], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for DLL0
                               Bottom piref calibration. Legal value is 0 to
                               17.
                            */
    UINT32 dll1_top_xover_cal_mux_sel : 5;

                            /* Bits[26:22], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for DLL1
                               Top piref calibration. Legal value is 0 to 11.
                            */
    UINT32 dll1_bot_xover_cal_mux_sel : 5;

                            /* Bits[31:27], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for DLL1
                               Bottom piref calibration. Legal value is 0 to
                               17.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_REG supported on:                      */
/*      SPRA0 (0x20012adc)                                                      */
/*      SPRB0 (0x20012adc)                                                      */
/*      SPRHBM (0x20012adc)                                                     */
/*      SPRC0 (0x20012adc)                                                      */
/*      SPRMCC (0x20012adc)                                                     */
/*      SPRUCC (0x20012adc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training Counting Window 0 Register
*/


#define DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_REG 0x08122ADC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt_window : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               value that sets the number of DCLK cycles that
                               the selected CMDCTL input samples will be
                               counted. Linear setting. This is broadcast to
                               all the CMDCTL fub. When this field is set to 0
                               the counter will run until it saturates, or
                               until it the CMD Training Counter Mode field is
                               cleared.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_CNT_WINDOW0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_CNT_RESULT_0_MCIO_DDRIOEXT_REG supported on:                     */
/*      SPRA0 (0x20012ae0)                                                      */
/*      SPRB0 (0x20012ae0)                                                      */
/*      SPRHBM (0x20012ae0)                                                     */
/*      SPRC0 (0x20012ae0)                                                      */
/*      SPRMCC (0x20012ae0)                                                     */
/*      SPRUCC (0x20012ae0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Sample counter result for CMD Training Counter Mode
*/


#define DDRCC_TRAIN_CNT_RESULT_0_MCIO_DDRIOEXT_REG 0x08122AE0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 train_cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Value that indicates the number of logic 1 or 0
                               values sampled during the Counting Window (based
                               on sample_sel). This is cleared when the CMD
                               Training Counter mode is enabled.. Whten the
                               cnt_window is set to 0, the counter will run
                               until it saturates.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_CNT_RESULT_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_CNT_RESULT_1_MCIO_DDRIOEXT_REG supported on:                     */
/*      SPRA0 (0x20012ae4)                                                      */
/*      SPRB0 (0x20012ae4)                                                      */
/*      SPRHBM (0x20012ae4)                                                     */
/*      SPRC0 (0x20012ae4)                                                      */
/*      SPRMCC (0x20012ae4)                                                     */
/*      SPRUCC (0x20012ae4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Sample counter result for CMD Training Counter Mode
*/


#define DDRCC_TRAIN_CNT_RESULT_1_MCIO_DDRIOEXT_REG 0x08122AE4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 train_cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Value that indicates the number of logic 1 or 0
                               values sampled during the Counting Window (based
                               on sample_sel). This is cleared when the CMD
                               Training Counter mode is enabled.. Whten the
                               cnt_window is set to 0, the counter will run
                               until it saturates.
                            */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_CNT_RESULT_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20012ae8)                                                      */
/*      SPRB0 (0x20012ae8)                                                      */
/*      SPRHBM (0x20012ae8)                                                     */
/*      SPRC0 (0x20012ae8)                                                      */
/*      SPRMCC (0x20012ae8)                                                     */
/*      SPRUCC (0x20012ae8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training Result 0 Register
*/


#define DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_REG 0x08122AE8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_cal_pd_result : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Xover Calibration Phase Detected Result[br] Bit
                               3 : DLL 1 - piref for IO[lb]21:16[rb][br] Bit 2
                               : DLL 1 - piref for IO[lb]15:12[rb][br] Bit 1 :
                               DLL 0 - piref for IO[lb]11:6[rb][br] Bit 0 : DLL
                               0 - piref for IO[lb]5:0[rb]
                            */
    UINT32 sa_train_result : 22;

                            /* Bits[25:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Per bit senseamp train result[br] Note: SW must
                               set the ddrcc_train_ctl1.rx_path_fb_en to 1
                               before reading this fields.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_TRAIN_RESULT1_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20012aec)                                                      */
/*      SPRB0 (0x20012aec)                                                      */
/*      SPRHBM (0x20012aec)                                                     */
/*      SPRC0 (0x20012aec)                                                      */
/*      SPRMCC (0x20012aec)                                                     */
/*      SPRUCC (0x20012aec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc0_type/ddrcc0_ch0a/ddrcc0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training Result 1 Register
*/


#define DDRCC_TRAIN_RESULT1_MCIO_DDRIOEXT_REG 0x08122AEC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_even_sample_path_fb : 22;

                            /* Bits[21:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               RX even sample path feedback. It is used in SW
                               ACIO loopback txpiclk stage training. [br] Note:
                               SW must set the ddrcc_train_ctl1.rx_path_fb_en
                               to 1 before reading this fields.
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_TRAIN_RESULT1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b00)                                                      */
/*      SPRB0 (0x20012b00)                                                      */
/*      SPRHBM (0x20012b00)                                                     */
/*      SPRC0 (0x20012b00)                                                      */
/*      SPRMCC (0x20012b00)                                                     */
/*      SPRUCC (0x20012b00)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_REG 0x08122B00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_1_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b04)                                                      */
/*      SPRB0 (0x20012b04)                                                      */
/*      SPRHBM (0x20012b04)                                                     */
/*      SPRC0 (0x20012b04)                                                      */
/*      SPRMCC (0x20012b04)                                                     */
/*      SPRUCC (0x20012b04)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_1_MCIO_DDRIOEXT_REG 0x08122B04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_2_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b08)                                                      */
/*      SPRB0 (0x20012b08)                                                      */
/*      SPRHBM (0x20012b08)                                                     */
/*      SPRC0 (0x20012b08)                                                      */
/*      SPRMCC (0x20012b08)                                                     */
/*      SPRUCC (0x20012b08)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_2_MCIO_DDRIOEXT_REG 0x08122B08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_2_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_3_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b0c)                                                      */
/*      SPRB0 (0x20012b0c)                                                      */
/*      SPRHBM (0x20012b0c)                                                     */
/*      SPRC0 (0x20012b0c)                                                      */
/*      SPRMCC (0x20012b0c)                                                     */
/*      SPRUCC (0x20012b0c)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_3_MCIO_DDRIOEXT_REG 0x08122B0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_3_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_4_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b10)                                                      */
/*      SPRB0 (0x20012b10)                                                      */
/*      SPRHBM (0x20012b10)                                                     */
/*      SPRC0 (0x20012b10)                                                      */
/*      SPRMCC (0x20012b10)                                                     */
/*      SPRUCC (0x20012b10)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_4_MCIO_DDRIOEXT_REG 0x08122B10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_4_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_5_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b14)                                                      */
/*      SPRB0 (0x20012b14)                                                      */
/*      SPRHBM (0x20012b14)                                                     */
/*      SPRC0 (0x20012b14)                                                      */
/*      SPRMCC (0x20012b14)                                                     */
/*      SPRUCC (0x20012b14)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_5_MCIO_DDRIOEXT_REG 0x08122B14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_5_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_6_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b18)                                                      */
/*      SPRB0 (0x20012b18)                                                      */
/*      SPRHBM (0x20012b18)                                                     */
/*      SPRC0 (0x20012b18)                                                      */
/*      SPRMCC (0x20012b18)                                                     */
/*      SPRUCC (0x20012b18)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_6_MCIO_DDRIOEXT_REG 0x08122B18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_6_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_7_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b1c)                                                      */
/*      SPRB0 (0x20012b1c)                                                      */
/*      SPRHBM (0x20012b1c)                                                     */
/*      SPRC0 (0x20012b1c)                                                      */
/*      SPRMCC (0x20012b1c)                                                     */
/*      SPRUCC (0x20012b1c)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_7_MCIO_DDRIOEXT_REG 0x08122B1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_7_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_8_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b20)                                                      */
/*      SPRB0 (0x20012b20)                                                      */
/*      SPRHBM (0x20012b20)                                                     */
/*      SPRC0 (0x20012b20)                                                      */
/*      SPRMCC (0x20012b20)                                                     */
/*      SPRUCC (0x20012b20)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_8_MCIO_DDRIOEXT_REG 0x08122B20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_8_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_9_MCIO_DDRIOEXT_REG supported on:                             */
/*      SPRA0 (0x20012b24)                                                      */
/*      SPRB0 (0x20012b24)                                                      */
/*      SPRHBM (0x20012b24)                                                     */
/*      SPRC0 (0x20012b24)                                                      */
/*      SPRMCC (0x20012b24)                                                     */
/*      SPRUCC (0x20012b24)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_9_MCIO_DDRIOEXT_REG 0x08122B24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_9_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_10_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b28)                                                      */
/*      SPRB0 (0x20012b28)                                                      */
/*      SPRHBM (0x20012b28)                                                     */
/*      SPRC0 (0x20012b28)                                                      */
/*      SPRMCC (0x20012b28)                                                     */
/*      SPRUCC (0x20012b28)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_10_MCIO_DDRIOEXT_REG 0x08122B28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_10_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_11_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b2c)                                                      */
/*      SPRB0 (0x20012b2c)                                                      */
/*      SPRHBM (0x20012b2c)                                                     */
/*      SPRC0 (0x20012b2c)                                                      */
/*      SPRMCC (0x20012b2c)                                                     */
/*      SPRUCC (0x20012b2c)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_11_MCIO_DDRIOEXT_REG 0x08122B2C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_11_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_12_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b30)                                                      */
/*      SPRB0 (0x20012b30)                                                      */
/*      SPRHBM (0x20012b30)                                                     */
/*      SPRC0 (0x20012b30)                                                      */
/*      SPRMCC (0x20012b30)                                                     */
/*      SPRUCC (0x20012b30)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_12_MCIO_DDRIOEXT_REG 0x08122B30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_12_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_13_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b34)                                                      */
/*      SPRB0 (0x20012b34)                                                      */
/*      SPRHBM (0x20012b34)                                                     */
/*      SPRC0 (0x20012b34)                                                      */
/*      SPRMCC (0x20012b34)                                                     */
/*      SPRUCC (0x20012b34)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_13_MCIO_DDRIOEXT_REG 0x08122B34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_13_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_14_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b38)                                                      */
/*      SPRB0 (0x20012b38)                                                      */
/*      SPRHBM (0x20012b38)                                                     */
/*      SPRC0 (0x20012b38)                                                      */
/*      SPRMCC (0x20012b38)                                                     */
/*      SPRUCC (0x20012b38)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_14_MCIO_DDRIOEXT_REG 0x08122B38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_14_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_15_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b3c)                                                      */
/*      SPRB0 (0x20012b3c)                                                      */
/*      SPRHBM (0x20012b3c)                                                     */
/*      SPRC0 (0x20012b3c)                                                      */
/*      SPRMCC (0x20012b3c)                                                     */
/*      SPRUCC (0x20012b3c)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_15_MCIO_DDRIOEXT_REG 0x08122B3C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_15_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_16_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b40)                                                      */
/*      SPRB0 (0x20012b40)                                                      */
/*      SPRHBM (0x20012b40)                                                     */
/*      SPRC0 (0x20012b40)                                                      */
/*      SPRMCC (0x20012b40)                                                     */
/*      SPRUCC (0x20012b40)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_16_MCIO_DDRIOEXT_REG 0x08122B40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_16_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_17_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b44)                                                      */
/*      SPRB0 (0x20012b44)                                                      */
/*      SPRHBM (0x20012b44)                                                     */
/*      SPRC0 (0x20012b44)                                                      */
/*      SPRMCC (0x20012b44)                                                     */
/*      SPRUCC (0x20012b44)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_17_MCIO_DDRIOEXT_REG 0x08122B44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_17_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_18_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b48)                                                      */
/*      SPRB0 (0x20012b48)                                                      */
/*      SPRHBM (0x20012b48)                                                     */
/*      SPRC0 (0x20012b48)                                                      */
/*      SPRMCC (0x20012b48)                                                     */
/*      SPRUCC (0x20012b48)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_18_MCIO_DDRIOEXT_REG 0x08122B48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_18_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_19_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b4c)                                                      */
/*      SPRB0 (0x20012b4c)                                                      */
/*      SPRHBM (0x20012b4c)                                                     */
/*      SPRC0 (0x20012b4c)                                                      */
/*      SPRMCC (0x20012b4c)                                                     */
/*      SPRUCC (0x20012b4c)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_19_MCIO_DDRIOEXT_REG 0x08122B4C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_19_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_20_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b50)                                                      */
/*      SPRB0 (0x20012b50)                                                      */
/*      SPRHBM (0x20012b50)                                                     */
/*      SPRC0 (0x20012b50)                                                      */
/*      SPRMCC (0x20012b50)                                                     */
/*      SPRUCC (0x20012b50)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_20_MCIO_DDRIOEXT_REG 0x08122B50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_20_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_CTL1_BIT_21_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012b54)                                                      */
/*      SPRB0 (0x20012b54)                                                      */
/*      SPRHBM (0x20012b54)                                                     */
/*      SPRC0 (0x20012b54)                                                      */
/*      SPRMCC (0x20012b54)                                                     */
/*      SPRUCC (0x20012b54)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* CMD Pi Settings
*/


#define DDRCC_CTL1_BIT_21_MCIO_DDRIOEXT_REG 0x08122B54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61,62,63=not used)[br] This field can be
                               programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_CTL1_BIT_21_MCIO_DDRIOEXT_STRUCT;

/* DDRCC_DLL_CTL0_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012b58)                                                      */
/*      SPRB0 (0x20012b58)                                                      */
/*      SPRHBM (0x20012b58)                                                     */
/*      SPRC0 (0x20012b58)                                                      */
/*      SPRMCC (0x20012b58)                                                     */
/*      SPRUCC (0x20012b58)                                                     */
/* Register default value on SPRA0: 0x00001E24                                  */
/* Register default value on SPRB0: 0x00001E24                                  */
/* Register default value on SPRHBM: 0x00001E24                                 */
/* Register default value on SPRC0: 0x00001E24                                  */
/* Register default value on SPRMCC: 0x00001E24                                 */
/* Register default value on SPRUCC: 0x00001E24                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcc1_type/ddrcc1_ch0a/ddrcc1_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DLL Control 0 Register
*/


#define DDRCC_DLL_CTL0_MCIO_DDRIOEXT_REG 0x08122B58

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dll_discharge_tmr : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000024*/

                            /*
                               BIOS must program this DLL discharge timer based
                               on the analog tuning value and hclk freq before
                               enable the DLL. The default value is based on
                               hclk is 1200 MHz and discharge time is 30ns.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dll_lock_tmr : 7;

                            /* Bits[13:7], Access Type=RW, default=0x0000003C*/

                            /*
                               BIOS must program this DLL lock timer based on
                               the analog tuning value and hclk freq before
                               enable the DLL. The default value is based on
                               hclk is 1200 MHz and lock time is 50ns.
                            */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCC_DLL_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012c00)                                                      */
/*      SPRB0 (0x20012c00)                                                      */
/*      SPRHBM (0x20012c00)                                                     */
/*      SPRC0 (0x20012c00)                                                      */
/*      SPRMCC (0x20012c00)                                                     */
/*      SPRUCC (0x20012c00)                                                     */
/* Register default value on SPRA0: 0x0001C000                                  */
/* Register default value on SPRB0: 0x0001C000                                  */
/* Register default value on SPRHBM: 0x0001C000                                 */
/* Register default value on SPRC0: 0x0001C000                                  */
/* Register default value on SPRMCC: 0x0001C000                                 */
/* Register default value on SPRUCC: 0x0001C000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Clock Pair Control 0 Register
*/


#define DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_REG 0x08122C00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clk_dly : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CLK delay with step size of clkpi/128.[br] This
                               value is added with clk_piref_offset and
                               piref_setup_adj to generate the clk_pio_code
                               before send to DLL , so the programed value is
                               relative to the piref odd clock.[br] Note: bit 6
                               (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] Note: Each
                               clock pair P/N pin share the same delay.[br]
                               This field can be programed by BIOS or HW cal
                               FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clk_pie_offset : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the clk_pio_code. The
                               result is the clk pi even code (clk_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and enable
                               senseamp/ODT (RX).[br] 0: CLK IO is used for RX
                               [br] 1: CLK IO is used for TX [br] Note: This
                               signal need to be qualify by IO direction valid
                               control.[br] Note: For ACIO, both RX/TX are
                               enabled by ACIO control.
                            */
    UINT32 clk_pair_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enables output buffers, PI clocks and output
                               datapaths for this clock pair.
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               In DDRT mode, two clock paris are assigned to
                               receive ERID for slot 0/1. This bit sets the
                               RXFIFO pointer initialization value.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state (DDRT ERID ONLY)[br] 1:
                               pull low
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable per clock pair */
    UINT32 sa_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Senseamp enable per clock pair */
    UINT32 rsvd_23 : 8;

                            /* Bits[30:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_PAIR_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_PAIR_1_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012c04)                                                      */
/*      SPRB0 (0x20012c04)                                                      */
/*      SPRHBM (0x20012c04)                                                     */
/*      SPRC0 (0x20012c04)                                                      */
/*      SPRMCC (0x20012c04)                                                     */
/*      SPRUCC (0x20012c04)                                                     */
/* Register default value on SPRA0: 0x0001C000                                  */
/* Register default value on SPRB0: 0x0001C000                                  */
/* Register default value on SPRHBM: 0x0001C000                                 */
/* Register default value on SPRC0: 0x0001C000                                  */
/* Register default value on SPRMCC: 0x0001C000                                 */
/* Register default value on SPRUCC: 0x0001C000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Clock Pair Control 0 Register
*/


#define DDRCLK_CTL0_PAIR_1_MCIO_DDRIOEXT_REG 0x08122C04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clk_dly : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CLK delay with step size of clkpi/128.[br] This
                               value is added with clk_piref_offset and
                               piref_setup_adj to generate the clk_pio_code
                               before send to DLL , so the programed value is
                               relative to the piref odd clock.[br] Note: bit 6
                               (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] Note: Each
                               clock pair P/N pin share the same delay.[br]
                               This field can be programed by BIOS or HW cal
                               FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clk_pie_offset : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the clk_pio_code. The
                               result is the clk pi even code (clk_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and enable
                               senseamp/ODT (RX).[br] 0: CLK IO is used for RX
                               [br] 1: CLK IO is used for TX [br] Note: This
                               signal need to be qualify by IO direction valid
                               control.[br] Note: For ACIO, both RX/TX are
                               enabled by ACIO control.
                            */
    UINT32 clk_pair_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enables output buffers, PI clocks and output
                               datapaths for this clock pair.
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               In DDRT mode, two clock paris are assigned to
                               receive ERID for slot 0/1. This bit sets the
                               RXFIFO pointer initialization value.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state (DDRT ERID ONLY)[br] 1:
                               pull low
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable per clock pair */
    UINT32 sa_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Senseamp enable per clock pair */
    UINT32 rsvd_23 : 8;

                            /* Bits[30:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_PAIR_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_PAIR_2_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012c08)                                                      */
/*      SPRB0 (0x20012c08)                                                      */
/*      SPRHBM (0x20012c08)                                                     */
/*      SPRC0 (0x20012c08)                                                      */
/*      SPRMCC (0x20012c08)                                                     */
/*      SPRUCC (0x20012c08)                                                     */
/* Register default value on SPRA0: 0x0001C000                                  */
/* Register default value on SPRB0: 0x0001C000                                  */
/* Register default value on SPRHBM: 0x0001C000                                 */
/* Register default value on SPRC0: 0x0001C000                                  */
/* Register default value on SPRMCC: 0x0001C000                                 */
/* Register default value on SPRUCC: 0x0001C000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Clock Pair Control 0 Register
*/


#define DDRCLK_CTL0_PAIR_2_MCIO_DDRIOEXT_REG 0x08122C08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clk_dly : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CLK delay with step size of clkpi/128.[br] This
                               value is added with clk_piref_offset and
                               piref_setup_adj to generate the clk_pio_code
                               before send to DLL , so the programed value is
                               relative to the piref odd clock.[br] Note: bit 6
                               (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] Note: Each
                               clock pair P/N pin share the same delay.[br]
                               This field can be programed by BIOS or HW cal
                               FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clk_pie_offset : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the clk_pio_code. The
                               result is the clk pi even code (clk_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and enable
                               senseamp/ODT (RX).[br] 0: CLK IO is used for RX
                               [br] 1: CLK IO is used for TX [br] Note: This
                               signal need to be qualify by IO direction valid
                               control.[br] Note: For ACIO, both RX/TX are
                               enabled by ACIO control.
                            */
    UINT32 clk_pair_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enables output buffers, PI clocks and output
                               datapaths for this clock pair.
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               In DDRT mode, two clock paris are assigned to
                               receive ERID for slot 0/1. This bit sets the
                               RXFIFO pointer initialization value.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state (DDRT ERID ONLY)[br] 1:
                               pull low
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable per clock pair */
    UINT32 sa_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Senseamp enable per clock pair */
    UINT32 rsvd_23 : 8;

                            /* Bits[30:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_PAIR_2_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_PAIR_3_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012c0c)                                                      */
/*      SPRB0 (0x20012c0c)                                                      */
/*      SPRHBM (0x20012c0c)                                                     */
/*      SPRC0 (0x20012c0c)                                                      */
/*      SPRMCC (0x20012c0c)                                                     */
/*      SPRUCC (0x20012c0c)                                                     */
/* Register default value on SPRA0: 0x0001C000                                  */
/* Register default value on SPRB0: 0x0001C000                                  */
/* Register default value on SPRHBM: 0x0001C000                                 */
/* Register default value on SPRC0: 0x0001C000                                  */
/* Register default value on SPRMCC: 0x0001C000                                 */
/* Register default value on SPRUCC: 0x0001C000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Clock Pair Control 0 Register
*/


#define DDRCLK_CTL0_PAIR_3_MCIO_DDRIOEXT_REG 0x08122C0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clk_dly : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               CLK delay with step size of clkpi/128.[br] This
                               value is added with clk_piref_offset and
                               piref_setup_adj to generate the clk_pio_code
                               before send to DLL , so the programed value is
                               relative to the piref odd clock.[br] Note: bit 6
                               (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] Note: Each
                               clock pair P/N pin share the same delay.[br]
                               This field can be programed by BIOS or HW cal
                               FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clk_pie_offset : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the clk_pio_code. The
                               result is the clk pi even code (clk_pie_code) to
                               DLL. Default value is 64.
                            */
    UINT32 io_dir : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               IO direction control during normal function
                               mode. This bit is used to qualify the DDRINTF
                               command output enable (TX) and enable
                               senseamp/ODT (RX).[br] 0: CLK IO is used for RX
                               [br] 1: CLK IO is used for TX [br] Note: This
                               signal need to be qualify by IO direction valid
                               control.[br] Note: For ACIO, both RX/TX are
                               enabled by ACIO control.
                            */
    UINT32 clk_pair_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Enables output buffers, PI clocks and output
                               datapaths for this clock pair.
                            */
    UINT32 rxfifo_ptr_ctl : 3;

                            /* Bits[19:17], Access Type=RW, default=0x00000000*/

                            /*
                               In DDRT mode, two clock paris are assigned to
                               receive ERID for slot 0/1. This bit sets the
                               RXFIFO pointer initialization value.
                            */
    UINT32 io_pwrdn_pull_low : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               IO Analog Power Down Pull low control. This
                               control determines the IO behavior (pull low or
                               tri-state) when analog power domain is down (C6
                               or S3)[br] 0: tri-state (DDRT ERID ONLY)[br] 1:
                               pull low
                            */
    UINT32 odt_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* ODT enable per clock pair */
    UINT32 sa_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Senseamp enable per clock pair */
    UINT32 rsvd_23 : 8;

                            /* Bits[30:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_PAIR_3_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012c20)                                                      */
/*      SPRB0 (0x20012c20)                                                      */
/*      SPRHBM (0x20012c20)                                                     */
/*      SPRC0 (0x20012c20)                                                      */
/*      SPRMCC (0x20012c20)                                                     */
/*      SPRUCC (0x20012c20)                                                     */
/* Register default value on SPRA0: 0x000000BF                                  */
/* Register default value on SPRB0: 0x000000BF                                  */
/* Register default value on SPRHBM: 0x000000BF                                 */
/* Register default value on SPRC0: 0x000000BF                                  */
/* Register default value on SPRMCC: 0x000000BF                                 */
/* Register default value on SPRUCC: 0x000000BF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX VREF Control Register
*/


#define DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_REG 0x08122C20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_vref_ctl : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000BF*/

                            /*
                               RX Vref control with step size is vccddr_hv/256.
                               Default is 75% vccddr_hv.
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_RXVREF_CTL_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_COMP_LA0_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012c24)                                                      */
/*      SPRB0 (0x20012c24)                                                      */
/*      SPRHBM (0x20012c24)                                                     */
/*      SPRC0 (0x20012c24)                                                      */
/*      SPRMCC (0x20012c24)                                                     */
/*      SPRUCC (0x20012c24)                                                     */
/* Register default value on SPRA0: 0x00000408                                  */
/* Register default value on SPRB0: 0x00000408                                  */
/* Register default value on SPRHBM: 0x00000408                                 */
/* Register default value on SPRC0: 0x00000408                                  */
/* Register default value on SPRMCC: 0x00000408                                 */
/* Register default value on SPRUCC: 0x00000408                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Late Action Register for RCOMP DRV UP/DN. It is updated either by COMP training FSM or SW. The promotion from late acton register to effective register is controlled by COMP FSM after quiescent request is granted from MC.
*/


#define DDRCLK_COMP_LA0_MCIO_DDRIOEXT_REG 0x08122C24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up : 5;

                            /* Bits[4:0], Access Type=RW/V, default=0x00000008*/

                            /*
                               Clk Rcomp Drive Up Comp Value. It is trained by
                               COMP FSM.[br] This value is added with
                               rcomp_drv_up_adj (signed number. Range is -8 to
                               7) before send it to the clkbuf.[br] Note: HW
                               checks for overflow and underflow.
                            */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000008*/

                            /*
                               Clk Rcomp Drive Down Comp Value. It is trained
                               by COMP FSM.[br] This value is added with
                               rcomp_drv_dn_adj (signed number. Range is -8 to
                               7) before send it to the clkbuf.[br] Note: HW
                               checks for overflow and underflow.
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_COMP_LA0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_COMP_LA1_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012c28)                                                      */
/*      SPRB0 (0x20012c28)                                                      */
/*      SPRHBM (0x20012c28)                                                     */
/*      SPRC0 (0x20012c28)                                                      */
/*      SPRMCC (0x20012c28)                                                     */
/*      SPRUCC (0x20012c28)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Late Action Register for SCOMP, and TCO COMP. It is updated by BIOS. The promotion from late acton register to effective register is controlled by COMP FSM after quiescent request is granted from MC.
*/


#define DDRCLK_COMP_LA1_MCIO_DDRIOEXT_REG 0x08122C28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tco_comp : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /*
                               Clk TCO Comp Value. It is programmed by BIOS but
                               it is not trained.[br] 0/64: fastest rise & fall
                               delay[br] 0->31: increasing fall delay[br]
                               64->95: increasing rise delay[br] Other values
                               are not used (32-63 & 96-127).[br] To get
                               monotonic increasing sweep of dutycycle, sweep
                               from (95 to 65, 0 to 31)
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scomp : 5;

                            /* Bits[12:8], Access Type=RW, default=0x00000000*/

                            /*
                               Clk Slew Rate Comp Value.[br] It is program by
                               BIOS but it is not trained.[br] 0 is fastest
                               slew rate, 31 is slowest slew rate.[br] The
                               SCOMP Up/Down Code are generated from the
                               following equation:[br] SCOMP Up code[lb]4:0[rb]
                               = scomp[lb]4:0[rb] +
                               scomp_up_adj[lb]3:0[rb];[br] SCOMP Down
                               code[lb]4:0[rb] = scomp[lb]4:0[rb] +
                               scomp_dn_adj[lb]3:0[rb];[br] Note : both SCOMP
                               up Adjust (scomp_up_adj) and SCOMP down adjust
                               (scomp_dn_adj) are signed valued. The adjust
                               range is -8 to 7[br] Note: HW checks for
                               overflow and underflow.
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_COMP_LA1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_COMP_OFFSET_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20012c2c)                                                      */
/*      SPRB0 (0x20012c2c)                                                      */
/*      SPRHBM (0x20012c2c)                                                     */
/*      SPRC0 (0x20012c2c)                                                      */
/*      SPRMCC (0x20012c2c)                                                     */
/*      SPRUCC (0x20012c2c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* COMP offset values add on to SCOMP, RCOMP DRV up/down, and TCO COMP
*/


#define DDRCLK_COMP_OFFSET_MCIO_DDRIOEXT_REG 0x08122C2C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up_adj : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to rcomp_drv_up to generate
                               the RCOMP Drive up code. The adjust range is -8
                               to 7.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn_adj : 4;

                            /* Bits[8:5], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to rcomp_drv_dn to generate
                               the RCOMP Drive down code. The adjust range is
                               -8 to 7.
                            */
    UINT32 rsvd_9 : 11;

                            /* Bits[19:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scomp_up_adj : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to scomp to generate the SCOMP
                               Up Code. The adjust range is -8 to 7.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 scomp_dn_adj : 4;

                            /* Bits[28:25], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to scomp to generate the SCOMP
                               Down Code. The adjust range is -8 to 7.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_COMP_OFFSET_MCIO_DDRIOEXT_STRUCT;


/* DDRCLK_DLL_CSR_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012c34)                                                      */
/*      SPRB0 (0x20012c34)                                                      */
/*      SPRHBM (0x20012c34)                                                     */
/*      SPRC0 (0x20012c34)                                                      */
/*      SPRMCC (0x20012c34)                                                     */
/*      SPRUCC (0x20012c34)                                                     */
/* Register default value on SPRA0: 0x28000000                                  */
/* Register default value on SPRB0: 0x28000000                                  */
/* Register default value on SPRHBM: 0x28000000                                 */
/* Register default value on SPRC0: 0x28000000                                  */
/* Register default value on SPRMCC: 0x28000000                                 */
/* Register default value on SPRUCC: 0x28000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DLL Control and Status Register
*/


#define DDRCLK_DLL_CSR_MCIO_DDRIOEXT_REG 0x08122C34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 detrm_lock : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* DLL Deterministic Lock Status */
    UINT32 raw_lock : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* DLL Raw Lock Status Sticky bit. */
    UINT32 lock_timeout : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* DLL Lock timeout. */
    UINT32 dll_reset : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* DLL Reset. */
    UINT32 adcout : 10;

                            /* Bits[13:4], Access Type=RO/V, default=0x00000000*/

                            /* DLL ADC out. */
    UINT32 long_lock : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* DLL Long Lock Status. */
    UINT32 detlock_earlylockstky : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Detect lock CBB early lock sticky output */
    UINT32 weak_lock : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* DLL Weak Lock Status. */
    UINT32 adc_done : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* DLL ADC done. */
    UINT32 pilags : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               DLL output of PiDFx FSM to indciate Pi lags. In
                               order to read this debug register, clock gating
                               must be disabled.
                            */
    UINT32 openloop : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* MDLL open loop output */
    UINT32 lockrst_b : 1;

                            /* Bits[20:20], Access Type=RO/V, default=0x00000000*/

                            /* Detect lock CBB lock reset output */
    UINT32 analog_tuning : 3;

                            /* Bits[23:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               BIOS should configure certain analog components
                               based on the value of this register.[br] Details
                               will be found in the description of any CSR that
                               relies on this value
                            */
    UINT32 rsvd : 3;

                            /* Bits[26:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flybyclk_sel : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bypasses flyby clock and selects H-tree clock
                               when disbled
                            */
    UINT32 iofreq_range : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000002*/

                            /*
                               BIOS program this IO Frequency range field based
                               on PLL freq. It is half of the DRAM transfer
                               rate.[br] For example DDR 4800. The PLL freq is
                               2400.[br] 2b00: 933MHz - 1333MHz (i.e. 1867MT/s
                               to 2667MT/s)[br] 2b01: 1467MHz 1600MHz (i.e.
                               2933MT/s to 3200MT/s)[br] 2b10: 1800MHz 2000MHz
                               (i.e. 3600MT/s to 4000MT/s)[br] 2b11: 2200MHz
                               2400MHz, or above (i.e. 4400MT/s to 4800MT/s)
                            */
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dll_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS must enable DLL when program the IO freq
                               range.
                            */

  } Bits;
  UINT32 Data;

} DDRCLK_DLL_CSR_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_DLL_CTL0_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012c38)                                                      */
/*      SPRB0 (0x20012c38)                                                      */
/*      SPRHBM (0x20012c38)                                                     */
/*      SPRC0 (0x20012c38)                                                      */
/*      SPRMCC (0x20012c38)                                                     */
/*      SPRUCC (0x20012c38)                                                     */
/* Register default value on SPRA0: 0x00001E24                                  */
/* Register default value on SPRB0: 0x00001E24                                  */
/* Register default value on SPRHBM: 0x00001E24                                 */
/* Register default value on SPRC0: 0x00001E24                                  */
/* Register default value on SPRMCC: 0x00001E24                                 */
/* Register default value on SPRUCC: 0x00001E24                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DLL Control 0 Register
*/


#define DDRCLK_DLL_CTL0_MCIO_DDRIOEXT_REG 0x08122C38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dll_discharge_tmr : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000024*/

                            /*
                               BIOS must program this DLL discharge timer based
                               on the analog tuning value and hclk freq before
                               enable the DLL. The default value is based on
                               hclk is 1200 MHz and discharge time is 30ns.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dll_lock_tmr : 7;

                            /* Bits[13:7], Access Type=RW, default=0x0000003C*/

                            /*
                               BIOS must program this DLL lock timer based on
                               the analog tuning value and hclk freq before
                               enable the DLL. The default value is based on
                               hclk is 1200 MHz and lock lock is 50ns.
                            */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_DLL_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG supported on:                       */
/*      SPRA0 (0x20012c3c)                                                      */
/*      SPRB0 (0x20012c3c)                                                      */
/*      SPRHBM (0x20012c3c)                                                     */
/*      SPRC0 (0x20012c3c)                                                      */
/*      SPRMCC (0x20012c3c)                                                     */
/*      SPRUCC (0x20012c3c)                                                     */
/* Register default value on SPRA0: 0x3F002000                                  */
/* Register default value on SPRB0: 0x3F002000                                  */
/* Register default value on SPRHBM: 0x3F002000                                 */
/* Register default value on SPRC0: 0x3F002000                                  */
/* Register default value on SPRMCC: 0x3F002000                                 */
/* Register default value on SPRUCC: 0x3F002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for PI N clocks[br]
                 reg 1: DLL 0 - piref for clk pair [lb]3:2[rb][br]
                 reg 0: DLL 0 - piref for clk pair [br]1:0[rb]
*/


#define DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG 0x08122C3C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pio_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI odd code to align the PIREF
                               odd clock to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_pio_code to DLL. The xover
                               input is driven from a failing edge of DCLK
                               flop. [br] By moving the piref odd clock to the
                               right by 63 ticks, we will have 63 pi tick of
                               setup margin.[br] This field can be programed by
                               BIOS or HW cal FSM.
                            */
    UINT32 piref_pie_offset : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the piref_pio_code. The
                               result is the piref pi even code
                               (piref_pie_code) to DLL. Default value is 64.
                            */
    UINT32 clk_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This clk piref offset is added with a signed
                               piref_setup_adj (default value is 63). [br] The
                               result is added to each clk pair clk_dly[6:0]
                               field, so each clk pair pi odd code to the DLL
                               is relative to the piref odd clock.[br] This
                               field can be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 piref_setup_adj : 7;

                            /* Bits[30:24], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 63 ticks, and we will
                               have 63 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG supported on:                       */
/*      SPRA0 (0x20012c40)                                                      */
/*      SPRB0 (0x20012c40)                                                      */
/*      SPRHBM (0x20012c40)                                                     */
/*      SPRC0 (0x20012c40)                                                      */
/*      SPRMCC (0x20012c40)                                                     */
/*      SPRUCC (0x20012c40)                                                     */
/* Register default value on SPRA0: 0x3F002000                                  */
/* Register default value on SPRB0: 0x3F002000                                  */
/* Register default value on SPRHBM: 0x3F002000                                 */
/* Register default value on SPRC0: 0x3F002000                                  */
/* Register default value on SPRMCC: 0x3F002000                                 */
/* Register default value on SPRUCC: 0x3F002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for PI N clocks[br]
                 reg 1: DLL 0 - piref for clk pair [lb]3:2[rb][br]
                 reg 0: DLL 0 - piref for clk pair [br]1:0[rb]
*/


#define DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG 0x08122C40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pio_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI odd code to align the PIREF
                               odd clock to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_pio_code to DLL. The xover
                               input is driven from a failing edge of DCLK
                               flop. [br] By moving the piref odd clock to the
                               right by 63 ticks, we will have 63 pi tick of
                               setup margin.[br] This field can be programed by
                               BIOS or HW cal FSM.
                            */
    UINT32 piref_pie_offset : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is added to the piref_pio_code. The
                               result is the piref pi even code
                               (piref_pie_code) to DLL. Default value is 64.
                            */
    UINT32 clk_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This clk piref offset is added with a signed
                               piref_setup_adj (default value is 63). [br] The
                               result is added to each clk pair clk_dly[6:0]
                               field, so each clk pair pi odd code to the DLL
                               is relative to the piref odd clock.[br] This
                               field can be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 piref_setup_adj : 7;

                            /* Bits[30:24], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 63 ticks, and we will
                               have 63 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012c44)                                                      */
/*      SPRB0 (0x20012c44)                                                      */
/*      SPRHBM (0x20012c44)                                                     */
/*      SPRC0 (0x20012c44)                                                      */
/*      SPRMCC (0x20012c44)                                                     */
/*      SPRUCC (0x20012c44)                                                     */
/* Register default value on SPRA0: 0x07C0900B                                  */
/* Register default value on SPRB0: 0x07C0900B                                  */
/* Register default value on SPRHBM: 0x07C0900B                                 */
/* Register default value on SPRC0: 0x07C0900B                                  */
/* Register default value on SPRMCC: 0x07C0900B                                 */
/* Register default value on SPRUCC: 0x07C0900B                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX control 0 Register
*/


#define DDRCLK_RX_CTL0_MCIO_DDRIOEXT_REG 0x08122C44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 odt_seg_en : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /*
                               Control number of ODT segment enable for clk
                               buf.
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_combiner_alert_mode : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Force the Rx amplifier combiner to a lower
                               static power state during Alert and DDR-T
                               modes,[br] which reduces FiSH temperatures and
                               helps RV.
                            */
    UINT32 rsvd_4 : 6;

                            /* Bits[9:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_path_ungate : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               SW set this bit to 1 to ungate the DDRIO to MC
                               RX data path.
                            */
    UINT32 rxgainfuse_ovrd : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Repurposed for force local receiver bias on for
                               DQbuf.[br] When set to 1, it will keep the local
                               bias mirror always enabled.[br] When set to 0,
                               local bias gets enabled with senseamp_enable.
                            */
    UINT32 rxbias_vcm_ctl : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000001*/

                            /*
                               Rx summer output common mode control[br] 0:
                               0.25*vccddra[br] 1: 0.3*vccddra (default)[br] 2:
                               0.35*vccddra[br] 3: 0.4*vccddra
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxbias_rxgain_ctl : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000001*/

                            /* 3 Rload Settings (K- 1.5, 2, and 3) */
    UINT32 rsvd_17 : 5;

                            /* Bits[21:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 odt_static_leg_cfg : 5;

                            /* Bits[26:22], Access Type=RW, default=0x0000001F*/

                            /*
                               Sets number of ODT static legs. First 3 MSB bits
                               has weight of 4, 4th MSB has weight of 2 and LSB
                               bit has weight of 1. [br] Default of 0x1F has 15
                               static legs enabled.[br] Static leg training
                               starts with 0x1F (all 15 legs enabled)
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_RX_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_RX_CTL1_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012c48)                                                      */
/*      SPRB0 (0x20012c48)                                                      */
/*      SPRHBM (0x20012c48)                                                     */
/*      SPRC0 (0x20012c48)                                                      */
/*      SPRMCC (0x20012c48)                                                     */
/*      SPRUCC (0x20012c48)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX control 1 Register
*/


#define DDRCLK_RX_CTL1_MCIO_DDRIOEXT_REG 0x08122C48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_offset_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /* enable offset control in clk buf and RX BIAS */
    UINT32 rsvd_7 : 22;

                            /* Bits[28:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_RX_CTL1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_TX_CTL0_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012c50)                                                      */
/*      SPRB0 (0x20012c50)                                                      */
/*      SPRHBM (0x20012c50)                                                     */
/*      SPRC0 (0x20012c50)                                                      */
/*      SPRMCC (0x20012c50)                                                     */
/*      SPRUCC (0x20012c50)                                                     */
/* Register default value on SPRA0: 0x800000DF                                  */
/* Register default value on SPRB0: 0x800000DF                                  */
/* Register default value on SPRHBM: 0x800000DF                                 */
/* Register default value on SPRC0: 0x800000DF                                  */
/* Register default value on SPRMCC: 0x800000DF                                 */
/* Register default value on SPRUCC: 0x800000DF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX control 0 Register
*/


#define DDRCLK_TX_CTL0_MCIO_DDRIOEXT_REG 0x08122C50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 drv_static_leg_cfg : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001F*/

                            /*
                               Sets number of CLK driver (Ron) static legs.[br]
                               First 3 MSB bits has weight of 4, 4th MSB has
                               weight of 2 and LSB bit has weight of 1.[br] RTL
                               default of 0x1F has 15 static legs enabled.[br]
                               Static leg training starts with 0x1F (all 15
                               legs enabled)
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_seg_en : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /*
                               Control number of data sement enable for CLK
                               buf.
                            */
    UINT32 rsvd_8 : 22;

                            /* Bits[29:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_serializer_pwradj : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to extend the pulse width of the pulsed
                               clocks to the Tx serializers.[br] No need skew
                               based settings, value of 1 seems to work across
                               PVT.[br] May need to set to 0 for slow silicon
                               if we see issues in post silicon.
                            */
    UINT32 scomp_tco_bypass_dis : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               0 : CLK BUF bypass SCOMP and TCO delay in the
                               transmitter.
                            */

  } Bits;
  UINT32 Data;

} DDRCLK_TX_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG supported on:                               */
/*      SPRA0 (0x20012c54)                                                      */
/*      SPRB0 (0x20012c54)                                                      */
/*      SPRHBM (0x20012c54)                                                     */
/*      SPRC0 (0x20012c54)                                                      */
/*      SPRMCC (0x20012c54)                                                     */
/*      SPRUCC (0x20012c54)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX control 1 Register
*/


#define DDRCLK_TX_CTL1_MCIO_DDRIOEXT_REG 0x08122C54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_dir_valid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               IODirectionValid.[br] SW must program the
                               io_pwrdn_pull_low control before set this bit to
                               1 to ensure the analog logic capture the pwr
                               down pin behavior.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_TX_CTL1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_DFX_CTL0_MCIO_DDRIOEXT_REG supported on:                              */
/*      SPRA0 (0x20012c5c)                                                      */
/*      SPRB0 (0x20012c5c)                                                      */
/*      SPRHBM (0x20012c5c)                                                     */
/*      SPRC0 (0x20012c5c)                                                      */
/*      SPRMCC (0x20012c5c)                                                     */
/*      SPRUCC (0x20012c5c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DFX Control 0 Register
*/


#define DDRCLK_DFX_CTL0_MCIO_DDRIOEXT_REG 0x08122C5C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tmr_test_mode : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Simulation-only control to shorten VSSHI and LDO
                               timers
                            */
    UINT32 sr_vref_dis : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in Self Refresh. The
                               clk Vref generators is enabled by
                               rx_vref_en.[br] 1 - The clk Vref generators are
                               switched off while in Self Refresh regardless
                               the setting of rx_vref_en.
                            */
    UINT32 ckepd_vref_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in CKE power down. All
                               CLK Vref generators is enabled by
                               rx_vref_en.[br] 1 - CLK Vref generators are
                               switched off while in CKE power down regardless
                               the setting of rx_vref_en.
                            */
    UINT32 rsvd_6 : 18;

                            /* Bits[23:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_voc_pass_gate_en : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit VOC RX pass gate enable for VOC training
                               and NTL testing.
                            */

  } Bits;
  UINT32 Data;

} DDRCLK_DFX_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG supported on:                         */
/*      SPRA0 (0x20012c60)                                                      */
/*      SPRB0 (0x20012c60)                                                      */
/*      SPRHBM (0x20012c60)                                                     */
/*      SPRC0 (0x20012c60)                                                      */
/*      SPRMCC (0x20012c60)                                                     */
/*      SPRUCC (0x20012c60)                                                     */
/* Register default value on SPRA0: 0x00002000                                  */
/* Register default value on SPRB0: 0x00002000                                  */
/* Register default value on SPRHBM: 0x00002000                                 */
/* Register default value on SPRC0: 0x00002000                                  */
/* Register default value on SPRMCC: 0x00002000                                 */
/* Register default value on SPRUCC: 0x00002000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW Training Control 0 Register
*/


#define DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG 0x08122C60

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_stage_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable xover training stage */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_summer_offset_stage_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable DFE summer offset training
                               stage for all clock bits.
                            */
    UINT32 rsvd_3 : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_5 : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 settling_time_x2 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to double the analog settling time.
                               [br] The default settling time for each stage
                               are [br] Xover : 32 Dclk for first iteration, 24
                               Dclk for subsequent [br] DFE summer offset : 24
                               Dclk [br] Txpiclk : 24 Dclk [br] Loop Delay: 8
                               Dclk
                            */
    UINT32 sample_cnt : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of Sample to evaluate for each iteration
                               step. It applied for all training stages. [br]
                               0: 16[br] 1: 32[br] 2: 64[br] 3: 2 (Should only
                               used to speed up digital simulation)
                            */
    UINT32 sample_thresh : 6;

                            /* Bits[15:10], Access Type=RW, default=0x00000008*/

                            /* Sample threshold value for voting logic. */
    UINT32 rsvd_16 : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hw_cal_en : 2;

                            /* Bits[31:30], Access Type=RW/V, default=0x00000000*/

                            /*
                               HW calibation enable control per FSM (clk has
                               2). Set to 1 to enable the HW calibration. [br]
                               They are 4 HW calibration stages that SW can
                               request HW to perform. It is defined in bit 0 to
                               4 in this register. [br] HW cal FSM will self
                               clear this bit when all the enabled calibration
                               stages are completed. [br] For BIOS training,
                               XOVER, sampler offset, and DFE summer offset
                               training can be enabled by SW.[br] For ACIO
                               testing, all 4 stages can be enabled by SW.
                            */

  } Bits;
  UINT32 Data;

} DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_HWCAL_STS0_0_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20012c64)                                                      */
/*      SPRB0 (0x20012c64)                                                      */
/*      SPRHBM (0x20012c64)                                                     */
/*      SPRC0 (0x20012c64)                                                      */
/*      SPRMCC (0x20012c64)                                                     */
/*      SPRUCC (0x20012c64)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW calibration status 0 Register[br]
                 reg 1: DLL 0 - piref for clk pair [lb]3:2[rb][br]
                 reg 0: DLL 0 - piref for clk pair [br]1:0[rb]
*/


#define DDRCLK_HWCAL_STS0_0_MCIO_DDRIOEXT_REG 0x08122C64

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed for any clock pair within
                               this piref group. SW must clear this bit to 0
                               when enable HW calibration.
                            */
    UINT32 rsvd_2 : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_HWCAL_STS0_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_HWCAL_STS0_1_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20012c68)                                                      */
/*      SPRB0 (0x20012c68)                                                      */
/*      SPRHBM (0x20012c68)                                                     */
/*      SPRC0 (0x20012c68)                                                      */
/*      SPRMCC (0x20012c68)                                                     */
/*      SPRUCC (0x20012c68)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW calibration status 0 Register[br]
                 reg 1: DLL 0 - piref for clk pair [lb]3:2[rb][br]
                 reg 0: DLL 0 - piref for clk pair [br]1:0[rb]
*/


#define DDRCLK_HWCAL_STS0_1_MCIO_DDRIOEXT_REG 0x08122C68

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed for any clock pair within
                               this piref group. SW must clear this bit to 0
                               when enable HW calibration.
                            */
    UINT32 rsvd_2 : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_HWCAL_STS0_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012c78)                                                      */
/*      SPRB0 (0x20012c78)                                                      */
/*      SPRHBM (0x20012c78)                                                     */
/*      SPRC0 (0x20012c78)                                                      */
/*      SPRMCC (0x20012c78)                                                     */
/*      SPRUCC (0x20012c78)                                                     */
/* Register default value on SPRA0: 0x18000000                                  */
/* Register default value on SPRB0: 0x18000000                                  */
/* Register default value on SPRHBM: 0x18000000                                 */
/* Register default value on SPRC0: 0x18000000                                  */
/* Register default value on SPRMCC: 0x18000000                                 */
/* Register default value on SPRUCC: 0x18000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 1 Register
*/


#define DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_REG 0x08122C78

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_path_fb_en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable sample path and unsample path
                               feedback results. [br] Note: It is used for SW
                               SA training and SW ACIO txpiclk training.
                            */
    UINT32 xover_cal : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 will prepare data fub in a
                               state to allow BIOS only to perform Xover
                               calibraton training. - enable all the DCLK And
                               PI clocks. (may remove clock gating function)
                            */
    UINT32 vref_highz : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Tri-state the vref_hv output. Set to 1 for NTL
                               and Rx Summer Offset Cal.
                            */
    UINT32 rsvd_7 : 5;

                            /* Bits[11:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddr4 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               For SPR, 2 Channel DDR-MC sub-system. It can
                               either operate in DDR4/DDR-T or DDR5/DDR-T2. It
                               doesn't support DDR4/DDR-T mix with
                               DDR5/DDR-T2.[br] Setting this bit to 1 indicate
                               it is operating in DDR4/DDR-T. 0 inidate it is
                               operating in DDR5/DDR-T2.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 compeff_clk_gate_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate effective register
                               update CLK for power saving. Default is disable
                               clock gating.[br] Note: Clock gating must be
                               disable during any training steps. BIOS can
                               enable clock gating for power saving after all
                               the training steps are done.
                            */
    UINT32 bclk_gate_en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable BCLK gating for Burnin/ADC
                               counter and DLL lock timer for power saving.
                            */
    UINT32 rsvd_16 : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pi_clk_gate_en : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate all PI N clocks
                               for power saving. Default is disable clock
                               gating. [br] Note: Clock gating must be disable
                               during any training steps. BIOS can enable clock
                               gating for power saving after all the training
                               steps are done.
                            */
    UINT32 piref_clk_gate_en : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate PI REF clock for
                               power saving. Default is disable clock gating.
                               [br] Note: Clock gating must be disable during
                               any training steps. BIOS can enable clock gating
                               for power saving after all the training steps
                               are done.
                            */
    UINT32 tx_clk_gate_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate TX CLK for
                               transmit path logics. Default is disable clock
                               gating.[br] Note: Clock gating must be disable
                               during any training steps. BIOS can enable clock
                               gating for power saving after all the training
                               steps are done.
                            */
    UINT32 rx_clk_gate_en : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate RX CLK for receive
                               path logics. Default is disable clock gating.
                               [br] Note: Clock gating must be disable during
                               any training steps. BIOS can enable clock gating
                               for power saving after all the training steps
                               are done.
                            */
    UINT32 train_clk_gate_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate CLK for training
                               related logic (BIOS or HW training logics).
                               Default is disable clock gating. [br] Note:
                               Clock gating must be disable during any training
                               steps. BIOS can enable clock gating for power
                               saving after all the training steps are done.
                            */
    UINT32 csr_clk_gate_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate CSR CLK. Default
                               is disable clock gating. [br] Note: Clock gating
                               must be disable during any training steps. BIOS
                               can enable clock gating for power saving after
                               all the training steps are done.
                            */
    UINT32 rsvd_23 : 3;

                            /* Bits[25:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_txon : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* Forces all the clk transmitter driver on. */
    UINT32 force_odt_on : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Force all the CLK IO weak ODT on. The weak ODT
                               is ignored when drive enable is asserted. [br]
                               Note: For ACIO, ODT is provided by different DDR
                               channel.
                            */
    UINT32 force_rxbias_on : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /* Force Rx Bias and Rx Bias Vref On */
    UINT32 force_sa_on : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Force Senseamp on. Use for overclocking support
                               where we may not be able to hit the fast exit
                               latency.
                            */
    UINT32 force_strong_odt_on : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Force all the CLK IO strong ODT on. When set, it
                               always enable one segment for ODT regardless the
                               state of the drive enable. [br] The intend use
                               case is for ACIO with burnin.
                            */
    UINT32 rx_vref_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* set to 1 to enable CLK vref */

  } Bits;
  UINT32 Data;

} DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_TRAIN_CTL2_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012c7c)                                                      */
/*      SPRB0 (0x20012c7c)                                                      */
/*      SPRHBM (0x20012c7c)                                                     */
/*      SPRC0 (0x20012c7c)                                                      */
/*      SPRMCC (0x20012c7c)                                                     */
/*      SPRUCC (0x20012c7c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 2 Register
*/


#define DDRCLK_TRAIN_CTL2_MCIO_DDRIOEXT_REG 0x08122C7C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dll_top_xover_cal_mux_sel : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for DLL
                               Top piref calibration. Legal value is 0 to 9.
                            */
    UINT32 dll_bot_xover_cal_mux_sel : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for DLL
                               Bottom piref calibration. Legal value is 0 to 9.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_TRAIN_RESULT0_MCIO_DDRIOEXT_REG supported on:                         */
/*      SPRA0 (0x20012c80)                                                      */
/*      SPRB0 (0x20012c80)                                                      */
/*      SPRHBM (0x20012c80)                                                     */
/*      SPRC0 (0x20012c80)                                                      */
/*      SPRMCC (0x20012c80)                                                     */
/*      SPRUCC (0x20012c80)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training Result 0 Register
*/


#define DDRCLK_TRAIN_RESULT0_MCIO_DDRIOEXT_REG 0x08122C80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_cal_pd_result : 2;

                            /* Bits[1:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Xover Calibration Phase Detected Result[br] Bit
                               1 : DLL 0 - piref for clk pair [lb]3:2[rb][br]
                               Bit 0 : DLL 0 - piref for clk pair [lb]1:0[rb]
                            */
    UINT32 sa_train_result : 8;

                            /* Bits[9:2], Access Type=RO/V, default=0x00000000*/

                            /* Per bit senseamp train result */
    UINT32 rx_even_sample_path_fb : 8;

                            /* Bits[17:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               RX even sample path feedback. It is used in SW
                               ACIO loopback txpiclk stage training. [br] Note:
                               SW must set the ddrcc_train_ctl1.rx_path_fb_en
                               to 1 before reading this fields.
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_0_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012ce0)                                                      */
/*      SPRB0 (0x20012ce0)                                                      */
/*      SPRHBM (0x20012ce0)                                                     */
/*      SPRC0 (0x20012ce0)                                                      */
/*      SPRMCC (0x20012ce0)                                                     */
/*      SPRUCC (0x20012ce0)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_0_MCIO_DDRIOEXT_REG 0x08122CE0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_0_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_1_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012ce4)                                                      */
/*      SPRB0 (0x20012ce4)                                                      */
/*      SPRHBM (0x20012ce4)                                                     */
/*      SPRC0 (0x20012ce4)                                                      */
/*      SPRMCC (0x20012ce4)                                                     */
/*      SPRUCC (0x20012ce4)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_1_MCIO_DDRIOEXT_REG 0x08122CE4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_1_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_2_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012ce8)                                                      */
/*      SPRB0 (0x20012ce8)                                                      */
/*      SPRHBM (0x20012ce8)                                                     */
/*      SPRC0 (0x20012ce8)                                                      */
/*      SPRMCC (0x20012ce8)                                                     */
/*      SPRUCC (0x20012ce8)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_2_MCIO_DDRIOEXT_REG 0x08122CE8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_2_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_3_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012cec)                                                      */
/*      SPRB0 (0x20012cec)                                                      */
/*      SPRHBM (0x20012cec)                                                     */
/*      SPRC0 (0x20012cec)                                                      */
/*      SPRMCC (0x20012cec)                                                     */
/*      SPRUCC (0x20012cec)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_3_MCIO_DDRIOEXT_REG 0x08122CEC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_3_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_4_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012cf0)                                                      */
/*      SPRB0 (0x20012cf0)                                                      */
/*      SPRHBM (0x20012cf0)                                                     */
/*      SPRC0 (0x20012cf0)                                                      */
/*      SPRMCC (0x20012cf0)                                                     */
/*      SPRUCC (0x20012cf0)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_4_MCIO_DDRIOEXT_REG 0x08122CF0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_4_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_5_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012cf4)                                                      */
/*      SPRB0 (0x20012cf4)                                                      */
/*      SPRHBM (0x20012cf4)                                                     */
/*      SPRC0 (0x20012cf4)                                                      */
/*      SPRMCC (0x20012cf4)                                                     */
/*      SPRUCC (0x20012cf4)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_5_MCIO_DDRIOEXT_REG 0x08122CF4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_5_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_6_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012cf8)                                                      */
/*      SPRB0 (0x20012cf8)                                                      */
/*      SPRHBM (0x20012cf8)                                                     */
/*      SPRC0 (0x20012cf8)                                                      */
/*      SPRMCC (0x20012cf8)                                                     */
/*      SPRUCC (0x20012cf8)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_6_MCIO_DDRIOEXT_REG 0x08122CF8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_6_MCIO_DDRIOEXT_STRUCT;

/* DDRCLK_CTL0_BIT_7_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20012cfc)                                                      */
/*      SPRB0 (0x20012cfc)                                                      */
/*      SPRHBM (0x20012cfc)                                                     */
/*      SPRC0 (0x20012cfc)                                                      */
/*      SPRMCC (0x20012cfc)                                                     */
/*      SPRUCC (0x20012cfc)                                                     */
/* Register default value on SPRA0: 0x0000001E                                  */
/* Register default value on SPRB0: 0x0000001E                                  */
/* Register default value on SPRHBM: 0x0000001E                                 */
/* Register default value on SPRC0: 0x0000001E                                  */
/* Register default value on SPRMCC: 0x0000001E                                 */
/* Register default value on SPRUCC: 0x0000001E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrclk_type/ddrclk_ch0/ddrclk_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per bit Control 0 Register
*/


#define DDRCLK_CTL0_BIT_7_MCIO_DDRIOEXT_REG 0x08122CFC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCLK_CTL0_BIT_7_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_DATA_COMP0_MCIO_DDRIOEXT_REG supported on:                           */
/*      SPRA0 (0x20017f00)                                                      */
/*      SPRB0 (0x20017f00)                                                      */
/*      SPRHBM (0x20017f00)                                                     */
/*      SPRC0 (0x20017f00)                                                      */
/*      SPRMCC (0x20017f00)                                                     */
/*      SPRUCC (0x20017f00)                                                     */
/* Register default value on SPRA0: 0x00000408                                  */
/* Register default value on SPRB0: 0x00000408                                  */
/* Register default value on SPRHBM: 0x00000408                                 */
/* Register default value on SPRC0: 0x00000408                                  */
/* Register default value on SPRMCC: 0x00000408                                 */
/* Register default value on SPRUCC: 0x00000408                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds the value set for Data fub Compensation
*/


#define DDRCOMP_DATA_COMP0_MCIO_DDRIOEXT_REG 0x08127F00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up : 5;

                            /* Bits[4:0], Access Type=RW/V, default=0x00000008*/

                            /*
                               Sets the resistive value (aka driving strength)
                               of the pull-up block of the DQ/DQS transmitter.
                               When [br] dq_drv_up_ovr_en = 1, drive strength
                               value is programmed by software[br]
                               dq_drv_up_ovr_en = 0, drive strength value is
                               computed by Comp FSM
                            */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000008*/

                            /*
                               Sets the resistive value (aka driving strength)
                               of the pull-down block of the DQ/DQS
                               transmitter. When [br] dq_drv_dn_ovr_en = 1,
                               drive strength value is programmed by
                               software[br] dq_drv_dn_ovr_en = 0, drive
                               strength value is computed by Comp FSM
                            */
    UINT32 rsvd_12 : 2;

                            /* Bits[13:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_odt_up : 5;

                            /* Bits[18:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the resistive value of the pull-up block of
                               DQ On die termination.When [br] dq_odt_up_ovr_en
                               = 1, ODT value is programmed by software[br]
                               dq_odt_up_ovr_en = 0, ODT value is computed by
                               Comp FSM
                            */
    UINT32 rsvd_19 : 2;

                            /* Bits[20:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_odt_dn : 5;

                            /* Bits[25:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the resistive value of the pull-dn block of
                               DQ On die termination. When [br]
                               dq_odt_dn_ovr_en = 1, ODT value is programmed by
                               software [br] dq_odt_dn_ovr_en = 0, ODT value is
                               computed by Comp FSM
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_DATA_COMP0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20017f04)                                                      */
/*      SPRB0 (0x20017f04)                                                      */
/*      SPRHBM (0x20017f04)                                                     */
/*      SPRC0 (0x20017f04)                                                      */
/*      SPRMCC (0x20017f04)                                                     */
/*      SPRUCC (0x20017f04)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds the value set for CMD fub Compensation
*/


#define DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_REG 0x08127F04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up : 5;

                            /* Bits[4:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the resistive value (aka driving strength)
                               of the pull-Up block of the CMD transmitter.
                               When[br] cmd_drv_up_ovr_en = 1, drive strength
                               value is programmed by software[br]
                               cmd_drv_up_ovr_en = 0, drive strength value is
                               computed by Comp FSM.[br] Note: CMD transmitter
                               uses rcomp codes for ODT codes too.
                            */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the resistive value (aka driving strength)
                               of the pull-down block of the CMD transmitter.
                               When[br] cmd_drv_dn_ovr_en = 1, drive strength
                               value is programmed by software[br]
                               cmd_drv_dn_ovr_en = 0, drive strength value is
                               computed by Comp FSM.[br] Note: CMD transmitter
                               uses rcomp codes for ODT codes too.
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_CMD_COMP0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_CLK_COMP0_MCIO_DDRIOEXT_REG supported on:                            */
/*      SPRA0 (0x20017f08)                                                      */
/*      SPRB0 (0x20017f08)                                                      */
/*      SPRHBM (0x20017f08)                                                     */
/*      SPRC0 (0x20017f08)                                                      */
/*      SPRMCC (0x20017f08)                                                     */
/*      SPRUCC (0x20017f08)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds the value set for CLK fub Compensation
*/


#define DDRCOMP_CLK_COMP0_MCIO_DDRIOEXT_REG 0x08127F08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up : 5;

                            /* Bits[4:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the resistive value (aka driving strength)
                               of the pull-Up block of the CLK transmitter.
                               When[br] clk_drv_up_ovr_en = 1, drive strength
                               value is programmed by software[br]
                               clk_drv_up_ovr_en = 0, drive strength value is
                               computed by Comp FSM
                            */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the resistive value (aka driving strength)
                               of the pull-down block of the CLK transmitter.
                               When[br] clk_drv_dn_ovr_en = 1, drive strength
                               value is programmed by software[br]
                               clk_drv_dn_ovr_en = 0, drive strength value is
                               computed by CompFSM
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_CLK_COMP0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_DQ_VREF_CTL_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20017f0c)                                                      */
/*      SPRB0 (0x20017f0c)                                                      */
/*      SPRHBM (0x20017f0c)                                                     */
/*      SPRC0 (0x20017f0c)                                                      */
/*      SPRMCC (0x20017f0c)                                                     */
/*      SPRUCC (0x20017f0c)                                                     */
/* Register default value on SPRA0: 0x4040404D                                  */
/* Register default value on SPRB0: 0x4040404D                                  */
/* Register default value on SPRHBM: 0x4040404D                                 */
/* Register default value on SPRC0: 0x4040404D                                  */
/* Register default value on SPRMCC: 0x4040404D                                 */
/* Register default value on SPRUCC: 0x4040404D                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Comp DQ Vref control Register
*/


#define DDRCOMP_DQ_VREF_CTL_MCIO_DDRIOEXT_REG 0x08127F0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_drv_pup_vref : 7;

                            /* Bits[6:0], Access Type=RW, default=0x0000004D*/

                            /*
                               DqRcompDrvUp Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_drv_pdn_vref : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               DqRcompDrvDown Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_odt_pup_vref : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000040*/

                            /*
                               DqOdtcompDrvUp Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_odt_pdn_vref : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000040*/

                            /*
                               DqOdtRcompDrvDown Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_DQ_VREF_CTL_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_CMD_VREF_CTL_MCIO_DDRIOEXT_REG supported on:                         */
/*      SPRA0 (0x20017f10)                                                      */
/*      SPRB0 (0x20017f10)                                                      */
/*      SPRHBM (0x20017f10)                                                     */
/*      SPRC0 (0x20017f10)                                                      */
/*      SPRMCC (0x20017f10)                                                     */
/*      SPRUCC (0x20017f10)                                                     */
/* Register default value on SPRA0: 0x00004050                                  */
/* Register default value on SPRB0: 0x00004050                                  */
/* Register default value on SPRHBM: 0x00004050                                 */
/* Register default value on SPRC0: 0x00004050                                  */
/* Register default value on SPRMCC: 0x00004050                                 */
/* Register default value on SPRUCC: 0x00004050                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Comp CMD Vref control Register
*/


#define DDRCOMP_CMD_VREF_CTL_MCIO_DDRIOEXT_REG 0x08127F10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_drv_pup_vref : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000050*/

                            /*
                               CmdRcompDrvUp Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cmd_drv_pdn_vref : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               CmdRcompDrvDown Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_CMD_VREF_CTL_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_CLK_VREF_CTL_MCIO_DDRIOEXT_REG supported on:                         */
/*      SPRA0 (0x20017f14)                                                      */
/*      SPRB0 (0x20017f14)                                                      */
/*      SPRHBM (0x20017f14)                                                     */
/*      SPRC0 (0x20017f14)                                                      */
/*      SPRMCC (0x20017f14)                                                     */
/*      SPRUCC (0x20017f14)                                                     */
/* Register default value on SPRA0: 0x0000404D                                  */
/* Register default value on SPRB0: 0x0000404D                                  */
/* Register default value on SPRHBM: 0x0000404D                                 */
/* Register default value on SPRC0: 0x0000404D                                  */
/* Register default value on SPRMCC: 0x0000404D                                 */
/* Register default value on SPRUCC: 0x0000404D                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Comp CLK Vref control Register
*/


#define DDRCOMP_CLK_VREF_CTL_MCIO_DDRIOEXT_REG 0x08127F14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clk_drv_pup_vref : 7;

                            /* Bits[6:0], Access Type=RW, default=0x0000004D*/

                            /*
                               ClkRcompDrvUp Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clk_drv_pdn_vref : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000040*/

                            /*
                               ClkRcompDrvDown Vref with step size of
                               Vccddr_hv/256
                            */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_CLK_VREF_CTL_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG supported on:                      */
/*      SPRA0 (0x20017f18)                                                      */
/*      SPRB0 (0x20017f18)                                                      */
/*      SPRHBM (0x20017f18)                                                     */
/*      SPRC0 (0x20017f18)                                                      */
/*      SPRMCC (0x20017f18)                                                     */
/*      SPRUCC (0x20017f18)                                                     */
/* Register default value on SPRA0: 0x000FFFFF                                  */
/* Register default value on SPRB0: 0x000FFFFF                                  */
/* Register default value on SPRHBM: 0x000FFFFF                                 */
/* Register default value on SPRC0: 0x000FFFFF                                  */
/* Register default value on SPRMCC: 0x000FFFFF                                 */
/* Register default value on SPRUCC: 0x000FFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds compensation controls for static leg training
*/


#define DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_REG 0x08127F18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_static_leg_ctl : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001F*/

                            /*
                               DQ Comp Loop Staic Leg Control value for data
                               comp.[br] Sets number of DRV static legs. First
                               3 MSB bits have weight of 4[br] 4th MSB has
                               weight of 2 and LSB has weight of 1.[br] Default
                               of 0x1F has 15 static legs enabled.[br] Static
                               leg training starts with 0x1F (all 15 legs
                               enabled)
                            */
    UINT32 cmd_static_leg_ctl : 5;

                            /* Bits[9:5], Access Type=RW, default=0x0000001F*/

                            /*
                               Command Static Leg value used for cmd comp[br]
                               Sets number of DRV static legs. First 3 MSB bits
                               have weight of 4[br] 4th MSB has weight of 2 and
                               LSB has weight of 1.[br] Default of 0x1F has 15
                               static legs enabled.[br] Static leg training
                               starts with 0x1F (all 15 legs enabled)
                            */
    UINT32 clk_static_leg_ctl : 5;

                            /* Bits[14:10], Access Type=RW, default=0x0000001F*/

                            /*
                               Clock Static Leg value used for CLK comp[br]
                               Sets number of DRV static legs. First 3 MSB bits
                               have weight of 4[br] 4th MSB has weight of 2 and
                               LSB has weight of 1.[br] Default of 0x1F has 15
                               static legs enabled.[br] Static leg training
                               starts with 0x1F (all 15 legs enabled)
                            */
    UINT32 odt_static_leg_ctl : 5;

                            /* Bits[19:15], Access Type=RW, default=0x0000001F*/

                            /*
                               ODT static leg value used for ODT comp[br] Sets
                               number of ODT static legs. First 3 MSB bits have
                               weight of 4[br] 4th MSB has weight of 2 and LSB
                               has weight of 1.[br] Default of 0x1F has 15
                               static legs enabled.[br] Static leg training
                               starts with 0x1F (all 15 legs enabled)
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_STATIC_LEG_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_SEG_OVR_CTL0_MCIO_DDRIOEXT_REG supported on:                         */
/*      SPRA0 (0x20017f1c)                                                      */
/*      SPRB0 (0x20017f1c)                                                      */
/*      SPRHBM (0x20017f1c)                                                     */
/*      SPRC0 (0x20017f1c)                                                      */
/*      SPRMCC (0x20017f1c)                                                     */
/*      SPRUCC (0x20017f1c)                                                     */
/* Register default value on SPRA0: 0x00555F00                                  */
/* Register default value on SPRB0: 0x00555F00                                  */
/* Register default value on SPRHBM: 0x00555F00                                 */
/* Register default value on SPRC0: 0x00555F00                                  */
/* Register default value on SPRMCC: 0x00555F00                                 */
/* Register default value on SPRUCC: 0x00555F00                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds the segment override control
*/


#define DDRCOMP_SEG_OVR_CTL0_MCIO_DDRIOEXT_REG 0x08127F1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_seg_ovr_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Dq segment Override Enable */
    UINT32 odt_seg_ovr_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Dq Odt Segment Override Enable */
    UINT32 cmd_seg_ovr_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Cmd Segment Override Enable */
    UINT32 clk_seg_ovr_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Clk Segment Override */
    UINT32 rsvd : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_up_seg_ovr_value : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000003*/

                            /*
                               when dq_seg_ovr_en = 0, Dq Up segment Enable
                               value = 1;[br] dq_seg_ovr_en = 1, Override value
                            */
    UINT32 dq_dn_seg_ovr_value : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000003*/

                            /*
                               when dq_seg_ovr_en = 0, Dq down segment Enable
                               value = 1;[br] dq_seg_ovr_en = 1, Override value
                            */
    UINT32 odt_up_seg_ovr_value : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000001*/

                            /*
                               when odt_seg_ovr_en = 0, Odt Up segment Enable
                               value = 1;[br] odt_seg_ovr_en = 1, Override
                               value
                            */
    UINT32 odt_dn_seg_ovr_value : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000001*/

                            /*
                               when odt_seg_ovr_en = 0, Odt down segment Enable
                               value = 1;[br] odt_seg_ovr_en = 1, Override
                               value
                            */
    UINT32 cmd_up_seg_ovr_value : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000001*/

                            /*
                               when cmd_seg_ovr_en = 0, Cmd Up segment Enable
                               value = 1;[br] cmd_seg_ovr_en = 1, Override
                               value
                            */
    UINT32 cmd_dn_seg_ovr_value : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000001*/

                            /*
                               when cmd_seg_ovr_en = 0, Cmd down segment Enable
                               value = 1;[br] cmd_seg_ovr_en = 1, Override
                               value
                            */
    UINT32 clk_up_seg_ovr_value : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000001*/

                            /*
                               when clk_seg_ovr_en= 0; Clk Up segment Enable
                               value = 1;[br] clk_seg_ovr_en = 1, Override
                               value
                            */
    UINT32 clk_dn_seg_ovr_value : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000001*/

                            /*
                               when clk_seg_ovr_en = 0; Clk down segment Enable
                               value = 1;[br] clk_seg_ovr_en = 1, Override
                               value
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_SEG_OVR_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_DRV_OVR_CTL0_MCIO_DDRIOEXT_REG supported on:                         */
/*      SPRA0 (0x20017f20)                                                      */
/*      SPRB0 (0x20017f20)                                                      */
/*      SPRHBM (0x20017f20)                                                     */
/*      SPRC0 (0x20017f20)                                                      */
/*      SPRMCC (0x20017f20)                                                     */
/*      SPRUCC (0x20017f20)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds bits for Drive Override Control
*/


#define DDRCOMP_DRV_OVR_CTL0_MCIO_DDRIOEXT_REG 0x08127F20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_drv_up_ovr_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Override enable for Data Rcomp DriveUp Value */
    UINT32 dq_drv_dn_ovr_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Override enable for Data Rcomp DriveDn Value */
    UINT32 dq_odt_up_ovr_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Override enable for Data Rcomp OdtUp Value */
    UINT32 dq_odt_dn_ovr_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Override enable for Data Rcomp OdtDn Value */
    UINT32 cmd_drv_up_ovr_en : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Override enable for Cmd Rcomp DriveUp Value */
    UINT32 cmd_drv_dn_ovr_en : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Override enable for Cmd Rcomp DriveDn Value */
    UINT32 clk_drv_up_ovr_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Override enable for Clk Rcomp DriveUp Value */
    UINT32 clk_drv_dn_ovr_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Override enable for Clk Rcomp DriveDn value */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqscomp_ovr_en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* DQS Comp Override */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_DRV_OVR_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_CTL0_MCIO_DDRIOEXT_REG supported on:                                 */
/*      SPRA0 (0x20017f24)                                                      */
/*      SPRB0 (0x20017f24)                                                      */
/*      SPRHBM (0x20017f24)                                                     */
/*      SPRC0 (0x20017f24)                                                      */
/*      SPRMCC (0x20017f24)                                                     */
/*      SPRUCC (0x20017f24)                                                     */
/* Register default value on SPRA0: 0x01000200                                  */
/* Register default value on SPRB0: 0x01000200                                  */
/* Register default value on SPRHBM: 0x01000200                                 */
/* Register default value on SPRC0: 0x01000200                                  */
/* Register default value on SPRMCC: 0x01000200                                 */
/* Register default value on SPRUCC: 0x01000200                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Comp Control 0 Register
*/


#define DDRCOMP_CTL0_MCIO_DDRIOEXT_REG 0x08127F24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dclk_gate_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK. Default is
                               disable clock gating.
                            */
    UINT32 csr_clk_gate_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate CSR CLK. Default
                               is disable clock gating. [br] Note: Clock gating
                               must be disable during any training steps. BIOS
                               can enable clock gating for power saving after
                               all the training steps are done.
                            */
    UINT32 rsvd_2 : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_dfx_mode : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               when rcomp_dfx_mode =0; switch cap comparator
                               clock= fsm_clk; rcomp_dfx_mode=1; switch cap
                               comparator clock= fsm_clk/3
                            */
    UINT32 force_odt_on : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Force ODT on used in dq/clk/cmd rcomp AIP for
                               debug purpose. Sets the pull down segment as
                               inverted pull up.
                            */
    UINT32 bubble_tmr_ext : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               Extend the FSM bubble timer by.[br] 0 - 1x
                               Default [br] 1 - 1.5x [br] 2 - 2x [br] 3 - 2.5
                            */
    UINT32 sw_cap_cmp_clk_sel : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000002*/

                            /*
                               0: reset = 1 cycle, PreCharge = 1, cycle,
                               evaluate = 1 cycle[br] 1: reset = 2 cycle,
                               PreCharge = 2, cycle, evaluate = 1 cycle[br] 2:
                               reset = 3 cycle, PreCharge = 3, cycle, evaluate
                               = 1 cycle[br] 3: Reserved
                            */
    UINT32 bubble_cbb_clk_gate_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable cbb clk gating during bubble
                               state.
                            */
    UINT32 eval_sample_x2 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Changes the number of samples dropped during the
                               binary FSM eval state. Setting this bit
                               increases the number of dropped samples from 14
                               to 256 for RCOMP and from 7 to 14 for RXDQSCOMP
                            */
    UINT32 rsvd : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_odt_up_dn_offset : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /*
                               Offset between DqOdt Up and Down when fix OdtDn
                               enable is set. Unused in wave3
                            */
    UINT32 force_comp_vref_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to force both comp's vref enable to 1,
                               regardless of the comp main fsm state
                            */
    UINT32 fix_odt_dn : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Skip DqRcomOdtDn compensation and use [br]
                               DqOdtDn = ddrcomp_data_comp0.rcomp_odt_up +
                               dq_odt_up_dn_offset. Unused in wave3
                            */
    UINT32 dram_rst_n : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               software de-asserts o_reset_n_ddr_pad_hv by
                               toggling (1 -> 0 ->1 ) this CR field
                            */
    UINT32 rsvd_25 : 6;

                            /* Bits[30:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_REG supported on:                       */
/*      SPRA0 (0x20017f28)                                                      */
/*      SPRB0 (0x20017f28)                                                      */
/*      SPRHBM (0x20017f28)                                                     */
/*      SPRC0 (0x20017f28)                                                      */
/*      SPRMCC (0x20017f28)                                                     */
/*      SPRUCC (0x20017f28)                                                     */
/* Register default value on SPRA0: 0x00090000                                  */
/* Register default value on SPRB0: 0x00090000                                  */
/* Register default value on SPRHBM: 0x00090000                                 */
/* Register default value on SPRC0: 0x00090000                                  */
/* Register default value on SPRMCC: 0x00090000                                 */
/* Register default value on SPRUCC: 0x00090000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DimmVref Adjust Controls.
*/


#define DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_REG 0x08127F28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ch0_dimm_vref_ctl : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               CH0 DIMM Vref Control.[br] For 10nm Vref Gen
                               step size is Vccddr_hv/256 and valid range for
                               DimmVref is Vccddr_hv/3 2*Vccddr_hv/3. The
                               target code to vref decoding: Vref =
                               VrefCtrl*Vccddr_hv/256
                            */
    UINT32 ch1_dimm_vref_ctl : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               CH1 DIMM Vref Control.[br] For 10nm Vref Gen
                               step size is Vccddr_hv/256 and valid range for
                               DimmVref is Vccddr_hv/3 2*Vccddr_hv/3. The
                               target code to vref decoding: Vref =
                               VrefCtrl*Vccddr_hv/256
                            */
    UINT32 ch0_vref_bias_ctl : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000001*/

                            /* Current control for channel 0 DIMM Vref */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ch1_vref_bias_ctl : 2;

                            /* Bits[20:19], Access Type=RW, default=0x00000001*/

                            /* Current control for channel 1 DIMM Vref */
    UINT32 rsvd_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ch0_dimm_vref_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Enables Ch0 DIMM Vref */
    UINT32 ch1_dimm_vref_en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /* Enables Ch1 DIMM Vref */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_DIMM_VREF_CTL0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_RX_RETRAIN0_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20017f2c)                                                      */
/*      SPRB0 (0x20017f2c)                                                      */
/*      SPRHBM (0x20017f2c)                                                     */
/*      SPRC0 (0x20017f2c)                                                      */
/*      SPRMCC (0x20017f2c)                                                     */
/*      SPRUCC (0x20017f2c)                                                     */
/* Register default value on SPRA0: 0x08001000                                  */
/* Register default value on SPRB0: 0x08001000                                  */
/* Register default value on SPRHBM: 0x08001000                                 */
/* Register default value on SPRC0: 0x08001000                                  */
/* Register default value on SPRMCC: 0x08001000                                 */
/* Register default value on SPRUCC: 0x08001000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX retrain 0 Register
*/


#define DDRCOMP_RX_RETRAIN0_MCIO_DDRIOEXT_REG 0x08127F2C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxrt_init : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Set to 1 to indicate this is the 1st retraining
                               to initialize the rxrt_init_picode. HW will not
                               calculate and apply the rxrt_dqs_dly_adj. Once
                               HW finish the initialization, it clears this
                               bit.
                            */
    UINT32 logicpath_train_en : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               HW only auto train the RxDqs logic path after
                               prim reset deassertion. For debug purpose, SW
                               can set this bit to enable logic path training.
                               HW clear this bit after logic path training is
                               completed.
                            */
    UINT32 rxrt_dqs_dly_adj : 7;

                            /* Bits[8:2], Access Type=RW/V, default=0x00000000*/

                            /*
                               Rx Retraining DQS delay adjust value for DQS N
                               and P. This field is calcuated by DDRCOMP fub
                               and broadcast to all the data fub. The adjust
                               range is -64 to 63.
                            */
    UINT32 rsvd : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxrt_logic_delay : 3;

                            /* Bits[12:10], Access Type=RW/V, default=0x00000004*/

                            /*
                               Set this to 100 for 1 DCLK delay. 010 for 2 DCLK
                               delay and 001 for 3 DCLK delay
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxrt_init_picode : 8;

                            /* Bits[21:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial RX training unmatch reciever
                               delay in PI ticks. It is used by HW logic to
                               calculate the rxrt_dqs_dly_adj for each
                               retraining.[br] Note: The attribte is RW/V to
                               allow SW to write to this field but we don't
                               have any use case yet.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pbias_stg2amp_trim : 3;

                            /* Bits[25:23], Access Type=RW, default=0x00000000*/

                            /*
                               pbias current mirror for the 2nd stage amplifier
                               in the DQS COMP CBB. Adjusts the bias current.
                               0=min, 7=max.
                            */
    UINT32 rsvd_26 : 1;

                            /* Bits[26:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqscomp_rxgain_ctl : 2;

                            /* Bits[28:27], Access Type=RW, default=0x00000001*/

                            /*
                               00 - 1.5x (default for CMD/CLK)[br] 01 - 2x[br]
                               10 - 2.5x (default for DQ)[br] 11 - 3x
                            */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxrt_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable Rx DQS retraining */

  } Bits;
  UINT32 Data;

} DDRCOMP_RX_RETRAIN0_MCIO_DDRIOEXT_STRUCT;

/* DDRCOMP_RX_RETRAIN1_MCIO_DDRIOEXT_REG supported on:                          */
/*      SPRA0 (0x20017f30)                                                      */
/*      SPRB0 (0x20017f30)                                                      */
/*      SPRHBM (0x20017f30)                                                     */
/*      SPRC0 (0x20017f30)                                                      */
/*      SPRMCC (0x20017f30)                                                     */
/*      SPRUCC (0x20017f30)                                                     */
/* Register default value on SPRA0: 0x00000018                                  */
/* Register default value on SPRB0: 0x00000018                                  */
/* Register default value on SPRHBM: 0x00000018                                 */
/* Register default value on SPRC0: 0x00000018                                  */
/* Register default value on SPRMCC: 0x00000018                                 */
/* Register default value on SPRUCC: 0x00000018                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrcomp_type/ddrcomp/ddrcomp_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX retrain 1 Register
*/


#define DDRCOMP_RX_RETRAIN1_MCIO_DDRIOEXT_REG 0x08127F30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxrt_logic_path_train_picode : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000018*/

                            /*
                               Rx Retrain logic path training PI code starting
                               value
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRCOMP_RX_RETRAIN1_MCIO_DDRIOEXT_STRUCT;

#endif /* _MCIO_DDRIOEXT_h */
