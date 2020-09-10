
/** @file
  MCIO_DDRIO.h

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


#ifndef _MCIO_DDRIO_h
#define _MCIO_DDRIO_h
#include <Base.h>

/* DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011400)                                                      */
/*      SPRB0 (0x20011400)                                                      */
/*      SPRHBM (0x20011400)                                                     */
/*      SPRC0 (0x20011400)                                                      */
/*      SPRMCC (0x20011400)                                                     */
/*      SPRUCC (0x20011400)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_REG 0x08021400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_1_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011404)                                                      */
/*      SPRB0 (0x20011404)                                                      */
/*      SPRHBM (0x20011404)                                                     */
/*      SPRC0 (0x20011404)                                                      */
/*      SPRMCC (0x20011404)                                                     */
/*      SPRUCC (0x20011404)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_1_MCIO_DDRIO_REG 0x08021404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_2_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011408)                                                      */
/*      SPRB0 (0x20011408)                                                      */
/*      SPRHBM (0x20011408)                                                     */
/*      SPRC0 (0x20011408)                                                      */
/*      SPRMCC (0x20011408)                                                     */
/*      SPRUCC (0x20011408)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_2_MCIO_DDRIO_REG 0x08021408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_3_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001140c)                                                      */
/*      SPRB0 (0x2001140c)                                                      */
/*      SPRHBM (0x2001140c)                                                     */
/*      SPRC0 (0x2001140c)                                                      */
/*      SPRMCC (0x2001140c)                                                     */
/*      SPRUCC (0x2001140c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_3_MCIO_DDRIO_REG 0x0802140C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_4_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011410)                                                      */
/*      SPRB0 (0x20011410)                                                      */
/*      SPRHBM (0x20011410)                                                     */
/*      SPRC0 (0x20011410)                                                      */
/*      SPRMCC (0x20011410)                                                     */
/*      SPRUCC (0x20011410)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_4_MCIO_DDRIO_REG 0x08021410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_5_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011414)                                                      */
/*      SPRB0 (0x20011414)                                                      */
/*      SPRHBM (0x20011414)                                                     */
/*      SPRC0 (0x20011414)                                                      */
/*      SPRMCC (0x20011414)                                                     */
/*      SPRUCC (0x20011414)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_5_MCIO_DDRIO_REG 0x08021414

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_6_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011418)                                                      */
/*      SPRB0 (0x20011418)                                                      */
/*      SPRHBM (0x20011418)                                                     */
/*      SPRC0 (0x20011418)                                                      */
/*      SPRMCC (0x20011418)                                                     */
/*      SPRUCC (0x20011418)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_6_MCIO_DDRIO_REG 0x08021418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_RANK_7_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001141c)                                                      */
/*      SPRB0 (0x2001141c)                                                      */
/*      SPRHBM (0x2001141c)                                                     */
/*      SPRC0 (0x2001141c)                                                      */
/*      SPRMCC (0x2001141c)                                                     */
/*      SPRUCC (0x2001141c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for x8 Deive, RX DQ, DQSP/N and Receive enable Delay
*/


#define DDRD_N0_RX_CTL0_RANK_7_MCIO_DDRIO_REG 0x0802141C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcven_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Receive Enable Delay. The range is 0 to 16UI
                               with step size clkpi/128.[br] DDR4 range is <
                               10UI. (max of 4 logic delay) [br] DDR5 range is
                               < 16UI. (max of 7 logic delay) [br] 10:7 - It is
                               used for logic delay.[br] 6:0 - The lower 7 bit
                               is added with rcven_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. The result is sent
                               to the rcven crossover logic and DLL.[br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rxdqsp_dly : 8;

                            /* Bits[18:11], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_P
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqp_dly + rxdqsp_dly[lb]0..3[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdqsn_dly : 8;

                            /* Bits[26:19], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by RX DQS deskew logic to delay DQS_N
                               to match the earlest arrival DQ.[br] The per bit
                               range is 0 to 1.375 dclk with step size
                               clkpi/128. [br] The per bit calculation is [br]
                               rxdqsn_dly + rxdqsn_dly[lb]3..0[rb] (perbit
                               delay) + rxrt_dqs_dly_adj (singed number for rx
                               retraining. Range is -64 to 63) + rxdqs_dly_adj
                               (signed number for BIOS margining sweep. Range
                               is -128 to 127)[br] Note: BIOS must make sure
                               the total delay not more than 1.375 dclk.
                            */
    UINT32 rxdq_dly : 3;

                            /* Bits[29:27], Access Type=RW, default=0x00000000*/

                            /*
                               It is used by the RX deskew CBB. Normal usage
                               (when DFE is disabled) adds delay to the DQ
                               path, when DQ is earlier than DQS. This control
                               only apply for DDR4.[br] 0: 1/16 UI offset,[br]
                               1: 3/16 UI offset,[br] 2: 5/16 UI offset,[br] 3:
                               7/16 UI offset,[br] 4: 9/16 UI offset,[br] 5:
                               11/16 UI offset,[br] 6,7: 13/16 UI offset.[br]
                               When DFE is enabled, adds delay on the DFE
                               feedback path (after the DFE sampler).[br] 0: 0
                               offset (bypass),[br] 1: 2/16 UI offset,[br] 2:
                               4/16 UI offset,[br] 3: 6/16 UI offset,[br] 4:
                               8/16 UI offset,[br] 5: 10/16 UI offset,[br] 6,7:
                               12/16 UI offset).
                            */
    UINT32 rsvd : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_strobe_inv : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* RX Strobe Inversion for DFE */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_0_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011420)                                                      */
/*      SPRB0 (0x20011420)                                                      */
/*      SPRHBM (0x20011420)                                                     */
/*      SPRC0 (0x20011420)                                                      */
/*      SPRMCC (0x20011420)                                                     */
/*      SPRUCC (0x20011420)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_0_MCIO_DDRIO_REG 0x08021420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_1_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011424)                                                      */
/*      SPRB0 (0x20011424)                                                      */
/*      SPRHBM (0x20011424)                                                     */
/*      SPRC0 (0x20011424)                                                      */
/*      SPRMCC (0x20011424)                                                     */
/*      SPRUCC (0x20011424)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_1_MCIO_DDRIO_REG 0x08021424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_2_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011428)                                                      */
/*      SPRB0 (0x20011428)                                                      */
/*      SPRHBM (0x20011428)                                                     */
/*      SPRC0 (0x20011428)                                                      */
/*      SPRMCC (0x20011428)                                                     */
/*      SPRUCC (0x20011428)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_2_MCIO_DDRIO_REG 0x08021428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_3_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001142c)                                                      */
/*      SPRB0 (0x2001142c)                                                      */
/*      SPRHBM (0x2001142c)                                                     */
/*      SPRC0 (0x2001142c)                                                      */
/*      SPRMCC (0x2001142c)                                                     */
/*      SPRUCC (0x2001142c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_3_MCIO_DDRIO_REG 0x0802142C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_4_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011430)                                                      */
/*      SPRB0 (0x20011430)                                                      */
/*      SPRHBM (0x20011430)                                                     */
/*      SPRC0 (0x20011430)                                                      */
/*      SPRMCC (0x20011430)                                                     */
/*      SPRUCC (0x20011430)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_4_MCIO_DDRIO_REG 0x08021430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_5_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011434)                                                      */
/*      SPRB0 (0x20011434)                                                      */
/*      SPRHBM (0x20011434)                                                     */
/*      SPRC0 (0x20011434)                                                      */
/*      SPRMCC (0x20011434)                                                     */
/*      SPRUCC (0x20011434)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_5_MCIO_DDRIO_REG 0x08021434

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_6_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011438)                                                      */
/*      SPRB0 (0x20011438)                                                      */
/*      SPRHBM (0x20011438)                                                     */
/*      SPRC0 (0x20011438)                                                      */
/*      SPRMCC (0x20011438)                                                     */
/*      SPRUCC (0x20011438)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_6_MCIO_DDRIO_REG 0x08021438

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_RANK_7_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001143c)                                                      */
/*      SPRB0 (0x2001143c)                                                      */
/*      SPRHBM (0x2001143c)                                                     */
/*      SPRC0 (0x2001143c)                                                      */
/*      SPRMCC (0x2001143c)                                                     */
/*      SPRUCC (0x2001143c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX EQ, RX DQSN delay for DQ0-3
*/


#define DDRD_N0_RX_CTL1_RANK_7_MCIO_DDRIO_REG 0x0802143C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsn_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsn_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsn delay control to add on to
                               rxdqsn_dly to delay DQS_N to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_cap_en : 2;

                            /* Bits[26:25], Access Type=RW, default=0x00000000*/

                            /* CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctle_res_en : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_0_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011440)                                                      */
/*      SPRB0 (0x20011440)                                                      */
/*      SPRHBM (0x20011440)                                                     */
/*      SPRC0 (0x20011440)                                                      */
/*      SPRMCC (0x20011440)                                                     */
/*      SPRUCC (0x20011440)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_0_MCIO_DDRIO_REG 0x08021440

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_1_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011444)                                                      */
/*      SPRB0 (0x20011444)                                                      */
/*      SPRHBM (0x20011444)                                                     */
/*      SPRC0 (0x20011444)                                                      */
/*      SPRMCC (0x20011444)                                                     */
/*      SPRUCC (0x20011444)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_1_MCIO_DDRIO_REG 0x08021444

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_2_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011448)                                                      */
/*      SPRB0 (0x20011448)                                                      */
/*      SPRHBM (0x20011448)                                                     */
/*      SPRC0 (0x20011448)                                                      */
/*      SPRMCC (0x20011448)                                                     */
/*      SPRUCC (0x20011448)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_2_MCIO_DDRIO_REG 0x08021448

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_3_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001144c)                                                      */
/*      SPRB0 (0x2001144c)                                                      */
/*      SPRHBM (0x2001144c)                                                     */
/*      SPRC0 (0x2001144c)                                                      */
/*      SPRMCC (0x2001144c)                                                     */
/*      SPRUCC (0x2001144c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_3_MCIO_DDRIO_REG 0x0802144C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_4_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011450)                                                      */
/*      SPRB0 (0x20011450)                                                      */
/*      SPRHBM (0x20011450)                                                     */
/*      SPRC0 (0x20011450)                                                      */
/*      SPRMCC (0x20011450)                                                     */
/*      SPRUCC (0x20011450)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_4_MCIO_DDRIO_REG 0x08021450

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_5_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011454)                                                      */
/*      SPRB0 (0x20011454)                                                      */
/*      SPRHBM (0x20011454)                                                     */
/*      SPRC0 (0x20011454)                                                      */
/*      SPRMCC (0x20011454)                                                     */
/*      SPRUCC (0x20011454)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_5_MCIO_DDRIO_REG 0x08021454

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_6_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011458)                                                      */
/*      SPRB0 (0x20011458)                                                      */
/*      SPRHBM (0x20011458)                                                     */
/*      SPRC0 (0x20011458)                                                      */
/*      SPRMCC (0x20011458)                                                     */
/*      SPRUCC (0x20011458)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_6_MCIO_DDRIO_REG 0x08021458

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL2_RANK_7_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001145c)                                                      */
/*      SPRB0 (0x2001145c)                                                      */
/*      SPRHBM (0x2001145c)                                                     */
/*      SPRC0 (0x2001145c)                                                      */
/*      SPRMCC (0x2001145c)                                                     */
/*      SPRUCC (0x2001145c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control For DFE, RX DQSP delay for DQ0-3
*/


#define DDRD_N0_RX_CTL2_RANK_7_MCIO_DDRIO_REG 0x0802145C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqsp_dly0 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ0. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly1 : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ1. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly2 : 5;

                            /* Bits[17:13], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ2. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_dly3 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Per bit rxdqsp delay control to add on to
                               rxdqsp_dly to delay DQS_P to match on DQ3. [br]
                               The range is 0 to 0.25 dclk with step size
                               clkpi/128. [br] Note: BIOS must make sure the
                               total delay not more than 1.375 dclk.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL2_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET0_RANK_0_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x20011460)                                                      */
/*      SPRB0 (0x20011460)                                                      */
/*      SPRHBM (0x20011460)                                                     */
/*      SPRC0 (0x20011460)                                                      */
/*      SPRMCC (0x20011460)                                                     */
/*      SPRUCC (0x20011460)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset0 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET0_RANK_0_MCIO_DDRIO_REG 0x08021460

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c1_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET0_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET0_RANK_1_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x20011464)                                                      */
/*      SPRB0 (0x20011464)                                                      */
/*      SPRHBM (0x20011464)                                                     */
/*      SPRC0 (0x20011464)                                                      */
/*      SPRMCC (0x20011464)                                                     */
/*      SPRUCC (0x20011464)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset0 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET0_RANK_1_MCIO_DDRIO_REG 0x08021464

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c1_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET0_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET0_RANK_2_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x20011468)                                                      */
/*      SPRB0 (0x20011468)                                                      */
/*      SPRHBM (0x20011468)                                                     */
/*      SPRC0 (0x20011468)                                                      */
/*      SPRMCC (0x20011468)                                                     */
/*      SPRUCC (0x20011468)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset0 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET0_RANK_2_MCIO_DDRIO_REG 0x08021468

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c1_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET0_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET0_RANK_3_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x2001146c)                                                      */
/*      SPRB0 (0x2001146c)                                                      */
/*      SPRHBM (0x2001146c)                                                     */
/*      SPRC0 (0x2001146c)                                                      */
/*      SPRMCC (0x2001146c)                                                     */
/*      SPRUCC (0x2001146c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset0 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET0_RANK_3_MCIO_DDRIO_REG 0x0802146C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c1_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c1_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C1 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c2_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C2 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET0_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET1_RANK_0_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x20011470)                                                      */
/*      SPRB0 (0x20011470)                                                      */
/*      SPRHBM (0x20011470)                                                     */
/*      SPRC0 (0x20011470)                                                      */
/*      SPRMCC (0x20011470)                                                     */
/*      SPRUCC (0x20011470)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset1 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET1_RANK_0_MCIO_DDRIO_REG 0x08021470

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c3_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET1_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET1_RANK_1_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x20011474)                                                      */
/*      SPRB0 (0x20011474)                                                      */
/*      SPRHBM (0x20011474)                                                     */
/*      SPRC0 (0x20011474)                                                      */
/*      SPRMCC (0x20011474)                                                     */
/*      SPRUCC (0x20011474)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset1 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET1_RANK_1_MCIO_DDRIO_REG 0x08021474

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c3_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET1_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET1_RANK_2_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x20011478)                                                      */
/*      SPRB0 (0x20011478)                                                      */
/*      SPRHBM (0x20011478)                                                     */
/*      SPRC0 (0x20011478)                                                      */
/*      SPRMCC (0x20011478)                                                     */
/*      SPRUCC (0x20011478)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset1 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET1_RANK_2_MCIO_DDRIO_REG 0x08021478

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c3_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET1_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_OFFSET1_RANK_3_MCIO_DDRIO_REG supported on:                */
/*      SPRA0 (0x2001147c)                                                      */
/*      SPRB0 (0x2001147c)                                                      */
/*      SPRHBM (0x2001147c)                                                     */
/*      SPRC0 (0x2001147c)                                                      */
/*      SPRMCC (0x2001147c)                                                     */
/*      SPRUCC (0x2001147c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE per bit offset1 (Only for DDR5, so suuport up to 4 rank)
*/


#define DDRD_N0_DFE_COEFF_OFFSET1_RANK_3_MCIO_DDRIO_REG 0x0802147C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_c3_offset0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c3_offset3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C3 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset0 : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ0. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset1 : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ1. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset2 : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ2. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 dfe_c4_offset3 : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DFE C4 offset for DQ3. It is a positive
                               adjustment. BIOS will train to min value.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_OFFSET1_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_0_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011480)                                                      */
/*      SPRB0 (0x20011480)                                                      */
/*      SPRHBM (0x20011480)                                                     */
/*      SPRC0 (0x20011480)                                                      */
/*      SPRMCC (0x20011480)                                                     */
/*      SPRUCC (0x20011480)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_0_MCIO_DDRIO_REG 0x08021480

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_1_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011484)                                                      */
/*      SPRB0 (0x20011484)                                                      */
/*      SPRHBM (0x20011484)                                                     */
/*      SPRC0 (0x20011484)                                                      */
/*      SPRMCC (0x20011484)                                                     */
/*      SPRUCC (0x20011484)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_1_MCIO_DDRIO_REG 0x08021484

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_2_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011488)                                                      */
/*      SPRB0 (0x20011488)                                                      */
/*      SPRHBM (0x20011488)                                                     */
/*      SPRC0 (0x20011488)                                                      */
/*      SPRMCC (0x20011488)                                                     */
/*      SPRUCC (0x20011488)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_2_MCIO_DDRIO_REG 0x08021488

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_3_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001148c)                                                      */
/*      SPRB0 (0x2001148c)                                                      */
/*      SPRHBM (0x2001148c)                                                     */
/*      SPRC0 (0x2001148c)                                                      */
/*      SPRMCC (0x2001148c)                                                     */
/*      SPRUCC (0x2001148c)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_3_MCIO_DDRIO_REG 0x0802148C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_4_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011490)                                                      */
/*      SPRB0 (0x20011490)                                                      */
/*      SPRHBM (0x20011490)                                                     */
/*      SPRC0 (0x20011490)                                                      */
/*      SPRMCC (0x20011490)                                                     */
/*      SPRUCC (0x20011490)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_4_MCIO_DDRIO_REG 0x08021490

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_5_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011494)                                                      */
/*      SPRB0 (0x20011494)                                                      */
/*      SPRHBM (0x20011494)                                                     */
/*      SPRC0 (0x20011494)                                                      */
/*      SPRMCC (0x20011494)                                                     */
/*      SPRUCC (0x20011494)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_5_MCIO_DDRIO_REG 0x08021494

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_6_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x20011498)                                                      */
/*      SPRB0 (0x20011498)                                                      */
/*      SPRHBM (0x20011498)                                                     */
/*      SPRC0 (0x20011498)                                                      */
/*      SPRMCC (0x20011498)                                                     */
/*      SPRUCC (0x20011498)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_6_MCIO_DDRIO_REG 0x08021498

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_RANK_7_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001149c)                                                      */
/*      SPRB0 (0x2001149c)                                                      */
/*      SPRHBM (0x2001149c)                                                     */
/*      SPRC0 (0x2001149c)                                                      */
/*      SPRMCC (0x2001149c)                                                     */
/*      SPRUCC (0x2001149c)                                                     */
/* Register default value on SPRA0: 0x00020000                                  */
/* Register default value on SPRB0: 0x00020000                                  */
/* Register default value on SPRHBM: 0x00020000                                 */
/* Register default value on SPRC0: 0x00020000                                  */
/* Register default value on SPRMCC: 0x00020000                                 */
/* Register default value on SPRUCC: 0x00020000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for 1st/2nd TX EQ Tap, TXDQS delay
*/


#define DDRD_N0_TX_CTL0_RANK_7_MCIO_DDRIO_REG 0x0802149C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqs_dly : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQS delay control. The range is 0 to 16 UI
                               with step size of clkpi/128. [br] DDR4 range is
                               < 10UI. (max of 4 logic delay) [br] DDR5 range
                               is < 16UI. (max of 7 logic delay) [br] 10:7 - It
                               is used for logic delay.[br] 6:0 - The lower 7
                               bit is added with txdqs_piref_offset and
                               piref_setup_adj, so the programed value is
                               relative to the piref clock. Then The result is
                               the txdqs pi odd code (txdqs_pio_code) that sent
                               to the txdqs crossover logic and DLL. Note: bit
                               6 (MSB) is inverted before the addition, so the
                               delay see by BIOS is linear.[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdqs_pie_offset : 7;

                            /* Bits[17:11], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdqs_pio_code . The
                               result is the txdqs pi even code
                               (txdqs_pie_code) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_2nd_tap : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 2nd tap coefficient.The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_2nd_tap:
                               0,1,2,3,4,5,6,7,12,13,14,15
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txeq_1st_tap : 6;

                            /* Bits[29:24], Access Type=RW, default=0x00000000*/

                            /*
                               TxEQ 1st tap coefficient. The following list all
                               the legal programming values. During TxEQ
                               training BIOS should sweep the following codes
                               in accending order: txeq_1st_tap: 0,1,2,3,4,5,6,
                               7,12,13,14,15,28,29,30,31,60,61,62,63
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114a0)                                                      */
/*      SPRB0 (0x200114a0)                                                      */
/*      SPRHBM (0x200114a0)                                                     */
/*      SPRC0 (0x200114a0)                                                      */
/*      SPRMCC (0x200114a0)                                                     */
/*      SPRUCC (0x200114a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_REG 0x080214A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_1_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114a4)                                                      */
/*      SPRB0 (0x200114a4)                                                      */
/*      SPRHBM (0x200114a4)                                                     */
/*      SPRC0 (0x200114a4)                                                      */
/*      SPRMCC (0x200114a4)                                                     */
/*      SPRUCC (0x200114a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_1_MCIO_DDRIO_REG 0x080214A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_2_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114a8)                                                      */
/*      SPRB0 (0x200114a8)                                                      */
/*      SPRHBM (0x200114a8)                                                     */
/*      SPRC0 (0x200114a8)                                                      */
/*      SPRMCC (0x200114a8)                                                     */
/*      SPRUCC (0x200114a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_2_MCIO_DDRIO_REG 0x080214A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_3_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114ac)                                                      */
/*      SPRB0 (0x200114ac)                                                      */
/*      SPRHBM (0x200114ac)                                                     */
/*      SPRC0 (0x200114ac)                                                      */
/*      SPRMCC (0x200114ac)                                                     */
/*      SPRUCC (0x200114ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_3_MCIO_DDRIO_REG 0x080214AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_4_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114b0)                                                      */
/*      SPRB0 (0x200114b0)                                                      */
/*      SPRHBM (0x200114b0)                                                     */
/*      SPRC0 (0x200114b0)                                                      */
/*      SPRMCC (0x200114b0)                                                     */
/*      SPRUCC (0x200114b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_4_MCIO_DDRIO_REG 0x080214B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_5_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114b4)                                                      */
/*      SPRB0 (0x200114b4)                                                      */
/*      SPRHBM (0x200114b4)                                                     */
/*      SPRC0 (0x200114b4)                                                      */
/*      SPRMCC (0x200114b4)                                                     */
/*      SPRUCC (0x200114b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_5_MCIO_DDRIO_REG 0x080214B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_6_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114b8)                                                      */
/*      SPRB0 (0x200114b8)                                                      */
/*      SPRHBM (0x200114b8)                                                     */
/*      SPRC0 (0x200114b8)                                                      */
/*      SPRMCC (0x200114b8)                                                     */
/*      SPRUCC (0x200114b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_6_MCIO_DDRIO_REG 0x080214B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_RANK_7_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114bc)                                                      */
/*      SPRB0 (0x200114bc)                                                      */
/*      SPRHBM (0x200114bc)                                                     */
/*      SPRC0 (0x200114bc)                                                      */
/*      SPRMCC (0x200114bc)                                                     */
/*      SPRUCC (0x200114bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for PXC delta, TXDQ delay for DQ0-1
*/


#define DDRD_N0_TX_CTL1_RANK_7_MCIO_DDRIO_REG 0x080214BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly0 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 0 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly1 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 1 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 pxc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 pxc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               PXC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114c0)                                                      */
/*      SPRB0 (0x200114c0)                                                      */
/*      SPRHBM (0x200114c0)                                                     */
/*      SPRC0 (0x200114c0)                                                      */
/*      SPRMCC (0x200114c0)                                                     */
/*      SPRUCC (0x200114c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_REG 0x080214C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_1_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114c4)                                                      */
/*      SPRB0 (0x200114c4)                                                      */
/*      SPRHBM (0x200114c4)                                                     */
/*      SPRC0 (0x200114c4)                                                      */
/*      SPRMCC (0x200114c4)                                                     */
/*      SPRUCC (0x200114c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_1_MCIO_DDRIO_REG 0x080214C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_2_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114c8)                                                      */
/*      SPRB0 (0x200114c8)                                                      */
/*      SPRHBM (0x200114c8)                                                     */
/*      SPRC0 (0x200114c8)                                                      */
/*      SPRMCC (0x200114c8)                                                     */
/*      SPRUCC (0x200114c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_2_MCIO_DDRIO_REG 0x080214C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_3_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114cc)                                                      */
/*      SPRB0 (0x200114cc)                                                      */
/*      SPRHBM (0x200114cc)                                                     */
/*      SPRC0 (0x200114cc)                                                      */
/*      SPRMCC (0x200114cc)                                                     */
/*      SPRUCC (0x200114cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_3_MCIO_DDRIO_REG 0x080214CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_4_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114d0)                                                      */
/*      SPRB0 (0x200114d0)                                                      */
/*      SPRHBM (0x200114d0)                                                     */
/*      SPRC0 (0x200114d0)                                                      */
/*      SPRMCC (0x200114d0)                                                     */
/*      SPRUCC (0x200114d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_4_MCIO_DDRIO_REG 0x080214D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_5_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114d4)                                                      */
/*      SPRB0 (0x200114d4)                                                      */
/*      SPRHBM (0x200114d4)                                                     */
/*      SPRC0 (0x200114d4)                                                      */
/*      SPRMCC (0x200114d4)                                                     */
/*      SPRUCC (0x200114d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_5_MCIO_DDRIO_REG 0x080214D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_6_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114d8)                                                      */
/*      SPRB0 (0x200114d8)                                                      */
/*      SPRHBM (0x200114d8)                                                     */
/*      SPRC0 (0x200114d8)                                                      */
/*      SPRMCC (0x200114d8)                                                     */
/*      SPRUCC (0x200114d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_6_MCIO_DDRIO_REG 0x080214D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL2_RANK_7_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200114dc)                                                      */
/*      SPRB0 (0x200114dc)                                                      */
/*      SPRHBM (0x200114dc)                                                     */
/*      SPRC0 (0x200114dc)                                                      */
/*      SPRMCC (0x200114dc)                                                     */
/*      SPRUCC (0x200114dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for DDJC delta, TXDQ delay for DQ2-3
*/


#define DDRD_N0_TX_CTL2_RANK_7_MCIO_DDRIO_REG 0x080214DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_dly2 : 11;

                            /* Bits[10:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 2 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 txdq_dly3 : 11;

                            /* Bits[21:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Write DQ 3 delay control. The range is 0 to 16
                               UI with step size of clkpi/128.[br] DDR4 range
                               is < 10UI. (max of 4 logic delay) [br] DDR5
                               range is < 16UI. (max of 7 logic delay) [br]
                               This field is added with txdq_dly_adj to allow
                               BIOS for margining sweep (signed numer -128 to
                               127). It, also, adds with txrt_dq_dly_adj for tx
                               retraning (signed numer -64 to 63).[br] The
                               final delay - [br] 10:7 - is used for logic
                               delay[br] 6:0 - The lower 7 bit is added with
                               txdq_piref offset and piref_setup_adj, so the
                               programmed value is relative to the piref clock.
                               The result is the txdq pi odd code. [br] Note:
                               bit 6 (MSB) is inverted before the addition, so
                               the delay see by BIOS is linear.[br] This field
                               can be programmed by BIOS or HW cal FSM.
                            */
    UINT32 ddjc_delta0 : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ0 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta1 : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ1 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta2 : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ2 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 ddjc_delta3 : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               DDJC delta for DQ3 inside the nibble.[br] Note:
                               BIOS msut make sure the combine DDJC and PXC
                               delta value doesnt overflow or underflow the
                               analog logic support range.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL2_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114e0)                                                      */
/*      SPRB0 (0x200114e0)                                                      */
/*      SPRHBM (0x200114e0)                                                     */
/*      SPRC0 (0x200114e0)                                                      */
/*      SPRMCC (0x200114e0)                                                     */
/*      SPRUCC (0x200114e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_REG 0x080214E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_1_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114e4)                                                      */
/*      SPRB0 (0x200114e4)                                                      */
/*      SPRHBM (0x200114e4)                                                     */
/*      SPRC0 (0x200114e4)                                                      */
/*      SPRMCC (0x200114e4)                                                     */
/*      SPRUCC (0x200114e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_1_MCIO_DDRIO_REG 0x080214E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_2_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114e8)                                                      */
/*      SPRB0 (0x200114e8)                                                      */
/*      SPRHBM (0x200114e8)                                                     */
/*      SPRC0 (0x200114e8)                                                      */
/*      SPRMCC (0x200114e8)                                                     */
/*      SPRUCC (0x200114e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_2_MCIO_DDRIO_REG 0x080214E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_3_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114ec)                                                      */
/*      SPRB0 (0x200114ec)                                                      */
/*      SPRHBM (0x200114ec)                                                     */
/*      SPRC0 (0x200114ec)                                                      */
/*      SPRMCC (0x200114ec)                                                     */
/*      SPRUCC (0x200114ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_3_MCIO_DDRIO_REG 0x080214EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_4_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114f0)                                                      */
/*      SPRB0 (0x200114f0)                                                      */
/*      SPRHBM (0x200114f0)                                                     */
/*      SPRC0 (0x200114f0)                                                      */
/*      SPRMCC (0x200114f0)                                                     */
/*      SPRUCC (0x200114f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_4_MCIO_DDRIO_REG 0x080214F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_5_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114f4)                                                      */
/*      SPRB0 (0x200114f4)                                                      */
/*      SPRHBM (0x200114f4)                                                     */
/*      SPRC0 (0x200114f4)                                                      */
/*      SPRMCC (0x200114f4)                                                     */
/*      SPRUCC (0x200114f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_5_MCIO_DDRIO_REG 0x080214F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_6_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114f8)                                                      */
/*      SPRB0 (0x200114f8)                                                      */
/*      SPRHBM (0x200114f8)                                                     */
/*      SPRC0 (0x200114f8)                                                      */
/*      SPRMCC (0x200114f8)                                                     */
/*      SPRUCC (0x200114f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_6_MCIO_DDRIO_REG 0x080214F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN_RANK_7_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200114fc)                                                      */
/*      SPRB0 (0x200114fc)                                                      */
/*      SPRHBM (0x200114fc)                                                     */
/*      SPRC0 (0x200114fc)                                                      */
/*      SPRMCC (0x200114fc)                                                     */
/*      SPRUCC (0x200114fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd0_type/ddrd0_n0_ch0a_0/ddrd0_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TX Retrain
*/


#define DDRD_N0_TX_RETRAIN_RANK_7_MCIO_DDRIO_REG 0x080214FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dq_dly_adj : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Retraining DQ delay adjust value for all the
                               DQ bits. This field is updated by tx retraining
                               HW logic. The adjust range is -64 to 63
                            */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init_picode : 10;

                            /* Bits[17:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This is the initial MRC training unmatch DRAM
                               reciever delay in PI ticks with 1 fractional bit
                               (bit 0). It is used by HW logic to calculate the
                               txrt_dq_dly_adj for each retraining.[br] Note:
                               The attribte is RW/V to allow SW to write to
                               this field but we don't have any use case yet.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               TX retrain enable. It should be set for DDR5 and
                               not LRDIMM type to enable TX retraining.
                            */
    UINT32 txrt_adj_byte_sel : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Selects between the local byte or the adjacent
                               byte to generate the RO count for tx retraining
                               calculation. Used for x16 devices.[br] Note:
                               This control doesn't apply for ECC byte.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x20011500)                                                      */
/*      SPRB0 (0x20011500)                                                      */
/*      SPRHBM (0x20011500)                                                     */
/*      SPRC0 (0x20011500)                                                      */
/*      SPRMCC (0x20011500)                                                     */
/*      SPRUCC (0x20011500)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_REG 0x08021500

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_1_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x20011504)                                                      */
/*      SPRB0 (0x20011504)                                                      */
/*      SPRHBM (0x20011504)                                                     */
/*      SPRC0 (0x20011504)                                                      */
/*      SPRMCC (0x20011504)                                                     */
/*      SPRUCC (0x20011504)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_1_MCIO_DDRIO_REG 0x08021504

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_2_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x20011508)                                                      */
/*      SPRB0 (0x20011508)                                                      */
/*      SPRHBM (0x20011508)                                                     */
/*      SPRC0 (0x20011508)                                                      */
/*      SPRMCC (0x20011508)                                                     */
/*      SPRUCC (0x20011508)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_2_MCIO_DDRIO_REG 0x08021508

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_3_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x2001150c)                                                      */
/*      SPRB0 (0x2001150c)                                                      */
/*      SPRHBM (0x2001150c)                                                     */
/*      SPRC0 (0x2001150c)                                                      */
/*      SPRMCC (0x2001150c)                                                     */
/*      SPRUCC (0x2001150c)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_3_MCIO_DDRIO_REG 0x0802150C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_4_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x20011510)                                                      */
/*      SPRB0 (0x20011510)                                                      */
/*      SPRHBM (0x20011510)                                                     */
/*      SPRC0 (0x20011510)                                                      */
/*      SPRMCC (0x20011510)                                                     */
/*      SPRUCC (0x20011510)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_4_MCIO_DDRIO_REG 0x08021510

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_5_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x20011514)                                                      */
/*      SPRB0 (0x20011514)                                                      */
/*      SPRHBM (0x20011514)                                                     */
/*      SPRC0 (0x20011514)                                                      */
/*      SPRMCC (0x20011514)                                                     */
/*      SPRUCC (0x20011514)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_5_MCIO_DDRIO_REG 0x08021514

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_6_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x20011518)                                                      */
/*      SPRB0 (0x20011518)                                                      */
/*      SPRHBM (0x20011518)                                                     */
/*      SPRC0 (0x20011518)                                                      */
/*      SPRMCC (0x20011518)                                                     */
/*      SPRUCC (0x20011518)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_6_MCIO_DDRIO_REG 0x08021518

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TXDQ_PIE_OFFSET_RANK_7_MCIO_DDRIO_REG supported on:                  */
/*      SPRA0 (0x2001151c)                                                      */
/*      SPRB0 (0x2001151c)                                                      */
/*      SPRHBM (0x2001151c)                                                     */
/*      SPRC0 (0x2001151c)                                                      */
/*      SPRMCC (0x2001151c)                                                     */
/*      SPRUCC (0x2001151c)                                                     */
/* Register default value on SPRA0: 0x08102040                                  */
/* Register default value on SPRB0: 0x08102040                                  */
/* Register default value on SPRHBM: 0x08102040                                 */
/* Register default value on SPRC0: 0x08102040                                  */
/* Register default value on SPRMCC: 0x08102040                                 */
/* Register default value on SPRUCC: 0x08102040                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank TXDQ PI Even offset
*/


#define DDRD_N0_TXDQ_PIE_OFFSET_RANK_7_MCIO_DDRIO_REG 0x0802151C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdq_pie_offset0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code0. The
                               result is the txdq0 pi even code
                               (txdq_pie_code0) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code1. The
                               result is the txdq1 pi even code
                               (txdq_pie_code1) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code2. The
                               result is the txdq2 pi even code
                               (txdq_pie_code2) to DLL.. Default value is 64.
                            */
    UINT32 txdq_pie_offset3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x00000040*/

                            /*
                               This offset is add to the txdq_pio_code3. The
                               result is the txdq3 pi even code
                               (txdq_pie_code3) to DLL.. Default value is 64.
                            */
    UINT32 rsvd : 3;

                            /* Bits[30:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 x8_device : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* x8 device enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_TXDQ_PIE_OFFSET_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011520)                                                      */
/*      SPRB0 (0x20011520)                                                      */
/*      SPRHBM (0x20011520)                                                     */
/*      SPRC0 (0x20011520)                                                      */
/*      SPRMCC (0x20011520)                                                     */
/*      SPRUCC (0x20011520)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_REG 0x08021520

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_1_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011524)                                                      */
/*      SPRB0 (0x20011524)                                                      */
/*      SPRHBM (0x20011524)                                                     */
/*      SPRC0 (0x20011524)                                                      */
/*      SPRMCC (0x20011524)                                                     */
/*      SPRUCC (0x20011524)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_1_MCIO_DDRIO_REG 0x08021524

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_2_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011528)                                                      */
/*      SPRB0 (0x20011528)                                                      */
/*      SPRHBM (0x20011528)                                                     */
/*      SPRC0 (0x20011528)                                                      */
/*      SPRMCC (0x20011528)                                                     */
/*      SPRUCC (0x20011528)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_2_MCIO_DDRIO_REG 0x08021528

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_3_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x2001152c)                                                      */
/*      SPRB0 (0x2001152c)                                                      */
/*      SPRHBM (0x2001152c)                                                     */
/*      SPRC0 (0x2001152c)                                                      */
/*      SPRMCC (0x2001152c)                                                     */
/*      SPRUCC (0x2001152c)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_3_MCIO_DDRIO_REG 0x0802152C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_4_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011530)                                                      */
/*      SPRB0 (0x20011530)                                                      */
/*      SPRHBM (0x20011530)                                                     */
/*      SPRC0 (0x20011530)                                                      */
/*      SPRMCC (0x20011530)                                                     */
/*      SPRUCC (0x20011530)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_4_MCIO_DDRIO_REG 0x08021530

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_5_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011534)                                                      */
/*      SPRB0 (0x20011534)                                                      */
/*      SPRHBM (0x20011534)                                                     */
/*      SPRC0 (0x20011534)                                                      */
/*      SPRMCC (0x20011534)                                                     */
/*      SPRUCC (0x20011534)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_5_MCIO_DDRIO_REG 0x08021534

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_6_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011538)                                                      */
/*      SPRB0 (0x20011538)                                                      */
/*      SPRHBM (0x20011538)                                                     */
/*      SPRC0 (0x20011538)                                                      */
/*      SPRMCC (0x20011538)                                                     */
/*      SPRUCC (0x20011538)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_6_MCIO_DDRIO_REG 0x08021538

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SA_OFFSET_RANK_7_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x2001153c)                                                      */
/*      SPRB0 (0x2001153c)                                                      */
/*      SPRHBM (0x2001153c)                                                     */
/*      SPRC0 (0x2001153c)                                                      */
/*      SPRMCC (0x2001153c)                                                     */
/*      SPRUCC (0x2001153c)                                                     */
/* Register default value on SPRA0: 0x03C78F1E                                  */
/* Register default value on SPRB0: 0x03C78F1E                                  */
/* Register default value on SPRHBM: 0x03C78F1E                                 */
/* Register default value on SPRC0: 0x03C78F1E                                  */
/* Register default value on SPRMCC: 0x03C78F1E                                 */
/* Register default value on SPRUCC: 0x03C78F1E                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank Control for RX DQSN/P Select and Senseamp offset for DQ0-3
*/


#define DDRD_N0_SA_OFFSET_RANK_7_MCIO_DDRIO_REG 0x0802153C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sa_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max -ve offset, 30=no offset, 60=max +ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sa_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001E*/

                            /*
                               Rxamp (summer) offset Control. Mid code is 30.
                               (0=max +ve offset, 30=no offset, 60=max -ve
                               offset, 61, 62, 63=not used)[br] This field can
                               be programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxdqsp_sel : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS P for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS P.
                            */
    UINT32 rxdqsn_sel : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /*
                               selects which skewed DQS N for post receive
                               enable counter clock in the RX Deskew logic. For
                               proper operation, always pick latest DQS N.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_SA_OFFSET_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011540)                                                      */
/*      SPRB0 (0x20011540)                                                      */
/*      SPRHBM (0x20011540)                                                     */
/*      SPRC0 (0x20011540)                                                      */
/*      SPRMCC (0x20011540)                                                     */
/*      SPRUCC (0x20011540)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_REG 0x08021540

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_1_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011544)                                                      */
/*      SPRB0 (0x20011544)                                                      */
/*      SPRHBM (0x20011544)                                                     */
/*      SPRC0 (0x20011544)                                                      */
/*      SPRMCC (0x20011544)                                                     */
/*      SPRUCC (0x20011544)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_1_MCIO_DDRIO_REG 0x08021544

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_2_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011548)                                                      */
/*      SPRB0 (0x20011548)                                                      */
/*      SPRHBM (0x20011548)                                                     */
/*      SPRC0 (0x20011548)                                                      */
/*      SPRMCC (0x20011548)                                                     */
/*      SPRUCC (0x20011548)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_2_MCIO_DDRIO_REG 0x08021548

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_3_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x2001154c)                                                      */
/*      SPRB0 (0x2001154c)                                                      */
/*      SPRHBM (0x2001154c)                                                     */
/*      SPRC0 (0x2001154c)                                                      */
/*      SPRMCC (0x2001154c)                                                     */
/*      SPRUCC (0x2001154c)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_3_MCIO_DDRIO_REG 0x0802154C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_4_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011550)                                                      */
/*      SPRB0 (0x20011550)                                                      */
/*      SPRHBM (0x20011550)                                                     */
/*      SPRC0 (0x20011550)                                                      */
/*      SPRMCC (0x20011550)                                                     */
/*      SPRUCC (0x20011550)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_4_MCIO_DDRIO_REG 0x08021550

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_4_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_5_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011554)                                                      */
/*      SPRB0 (0x20011554)                                                      */
/*      SPRHBM (0x20011554)                                                     */
/*      SPRC0 (0x20011554)                                                      */
/*      SPRMCC (0x20011554)                                                     */
/*      SPRUCC (0x20011554)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_5_MCIO_DDRIO_REG 0x08021554

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_5_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_6_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x20011558)                                                      */
/*      SPRB0 (0x20011558)                                                      */
/*      SPRHBM (0x20011558)                                                     */
/*      SPRC0 (0x20011558)                                                      */
/*      SPRMCC (0x20011558)                                                     */
/*      SPRUCC (0x20011558)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_6_MCIO_DDRIO_REG 0x08021558

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_6_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFE_COEFF_RANK_7_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x2001155c)                                                      */
/*      SPRB0 (0x2001155c)                                                      */
/*      SPRHBM (0x2001155c)                                                     */
/*      SPRC0 (0x2001155c)                                                      */
/*      SPRMCC (0x2001155c)                                                     */
/*      SPRUCC (0x2001155c)                                                     */
/* Register default value on SPRA0: 0x00084188                                  */
/* Register default value on SPRB0: 0x00084188                                  */
/* Register default value on SPRHBM: 0x00084188                                 */
/* Register default value on SPRC0: 0x00084188                                  */
/* Register default value on SPRMCC: 0x00084188                                 */
/* Register default value on SPRUCC: 0x00084188                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Rank DFE Coefficient
*/


#define DDRD_N0_DFE_COEFF_RANK_7_MCIO_DDRIO_REG 0x0802155C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfe_coeff1 : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 1st post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -120mV to +35mV in 5mV
                               steps.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff2 : 4;

                            /* Bits[9:6], Access Type=RW, default=0x00000006*/

                            /*
                               DFE 2nd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff3 : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 3rd post cursor tap coefficient. Used in
                               DDR5 mode only.[br] -40mV to +35mV in 5 mV steps
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfe_coeff4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000008*/

                            /*
                               DFE 4th post cursor tap coefficient. (In DDR4
                               mode this is the floating tap coefficient).[br]
                               -40mV to +35mV in 5mV steps
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFE_COEFF_RANK_7_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DLL_CTL0_MCIO_DDRIO_REG supported on:                                */
/*      SPRA0 (0x20011570)                                                      */
/*      SPRB0 (0x20011570)                                                      */
/*      SPRHBM (0x20011570)                                                     */
/*      SPRC0 (0x20011570)                                                      */
/*      SPRMCC (0x20011570)                                                     */
/*      SPRUCC (0x20011570)                                                     */
/* Register default value on SPRA0: 0x00001E24                                  */
/* Register default value on SPRB0: 0x00001E24                                  */
/* Register default value on SPRHBM: 0x00001E24                                 */
/* Register default value on SPRC0: 0x00001E24                                  */
/* Register default value on SPRMCC: 0x00001E24                                 */
/* Register default value on SPRUCC: 0x00001E24                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DLL Control 0 Register
*/


#define DDRD_N0_DLL_CTL0_MCIO_DDRIO_REG 0x08021570

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

} DDRD_N0_DLL_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG supported on:                             */
/*      SPRA0 (0x20011574)                                                      */
/*      SPRB0 (0x20011574)                                                      */
/*      SPRHBM (0x20011574)                                                     */
/*      SPRC0 (0x20011574)                                                      */
/*      SPRMCC (0x20011574)                                                     */
/*      SPRUCC (0x20011574)                                                     */
/* Register default value on SPRA0: 0x0C000001                                  */
/* Register default value on SPRB0: 0x0C000001                                  */
/* Register default value on SPRHBM: 0x0C000001                                 */
/* Register default value on SPRC0: 0x0C000001                                  */
/* Register default value on SPRMCC: 0x0C000001                                 */
/* Register default value on SPRUCC: 0x0C000001                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX retrain 0 Register
*/


#define DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_REG 0x08021574

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrt_dur : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000001*/

                            /*
                               TX retraining duration. SW must programs the
                               same TCK duration value as progammed in the
                               memory device.[br] 001: 128 x 16 TCK[br] 010:
                               256 x 16 TCK[br] 100: 512 x 16 TCK[br] others:
                               Reserved
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txrt_init : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to indicate this is the 1st retraining
                               to initialize the txrt_init_picode. HW will not
                               calculate and apply the txrt_dq_dly_adj.
                            */
    UINT32 txrt_debug_rank : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000000*/

                            /*
                               Select which rank of RO count value to capture
                               in txrt_ro_count for debug.
                            */
    UINT32 txrt_ro_count : 14;

                            /* Bits[21:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is the last TX retraining Ring Oscillator
                               count value captured by HW. HW only capture the
                               rank sepcify in txrt_debug_rank.
                            */
    UINT32 txrt_update : 1;

                            /* Bits[22:22], Access Type=RW/V, default=0x00000000*/

                            /*
                               Set to 1 to apply the calculation results to
                               txrt_init_picode or txrt_dq_dly_adj. HW will
                               clear this bit once it is applied. [br] HW does
                               not require the spid_update_req/ack handshake to
                               apply the results if this CSR is used. [br] SW
                               needs to guarantee that there are no CAP errors,
                               and there is no write traffic to the targeted
                               rank before setting this bit.
                            */
    UINT32 txrt_rank_err : 1;

                            /* Bits[23:23], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate MR46 rank did not
                               match MR47 rank when txrt_update or
                               spid_update_req/ack occurs. [br] SW must clear
                               this bit to 0 when enable HW calibration.
                            */
    UINT32 txrt_fsm_status : 2;

                            /* Bits[25:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reads back the TXRT FSM state. [br] 0: IDLE [br]
                               1: WAIT_MR [br] 2: CALC [br] 3: UPDATE
                            */
    UINT32 txrt_mrr_status : 2;

                            /* Bits[27:26], Access Type=RO/V, default=0x00000003*/

                            /*
                               Reads back the MRR that the TXRT FSM is waiting
                               on. [br] bit 0: MR46. 1 indicates that the FSM
                               is waiting for this read. [br] bit 1: MR47. 1
                               indicates that the FSM is waiting for this read.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_RETRAIN0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG supported on:                      */
/*      SPRA0 (0x20011578)                                                      */
/*      SPRB0 (0x20011578)                                                      */
/*      SPRHBM (0x20011578)                                                     */
/*      SPRC0 (0x20011578)                                                      */
/*      SPRMCC (0x20011578)                                                     */
/*      SPRUCC (0x20011578)                                                     */
/* Register default value on SPRA0: 0x03E7CF9F                                  */
/* Register default value on SPRB0: 0x03E7CF9F                                  */
/* Register default value on SPRHBM: 0x03E7CF9F                                 */
/* Register default value on SPRC0: 0x03E7CF9F                                  */
/* Register default value on SPRMCC: 0x03E7CF9F                                 */
/* Register default value on SPRUCC: 0x03E7CF9F                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX Sampler Odd offset for DQ0-3
*/


#define DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_REG 0x08021578

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sampler_odd_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Odd Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sampler_odd_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Odd Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sampler_odd_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Odd Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sampler_odd_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Odd Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_SAMPLER_ODD_OFFSET_MCIO_DDRIO_STRUCT;

/* DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG supported on:                     */
/*      SPRA0 (0x2001157c)                                                      */
/*      SPRB0 (0x2001157c)                                                      */
/*      SPRHBM (0x2001157c)                                                     */
/*      SPRC0 (0x2001157c)                                                      */
/*      SPRMCC (0x2001157c)                                                     */
/*      SPRUCC (0x2001157c)                                                     */
/* Register default value on SPRA0: 0x03E7CF9F                                  */
/* Register default value on SPRB0: 0x03E7CF9F                                  */
/* Register default value on SPRHBM: 0x03E7CF9F                                 */
/* Register default value on SPRC0: 0x03E7CF9F                                  */
/* Register default value on SPRMCC: 0x03E7CF9F                                 */
/* Register default value on SPRUCC: 0x03E7CF9F                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX Sampler Even offset for DQ0-3
*/


#define DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_REG 0x0802157C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sampler_even_offset0 : 6;

                            /* Bits[5:0], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Even Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sampler_even_offset1 : 6;

                            /* Bits[12:7], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Even Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sampler_even_offset2 : 6;

                            /* Bits[19:14], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Even Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sampler_even_offset3 : 6;

                            /* Bits[26:21], Access Type=RW/V, default=0x0000001F*/

                            /*
                               Rx Even Sampler Offset Control.[br] Normal
                               operation (dfe_coeff_training_en=0): -155mV to
                               +155mV in 5mV steps in normal operation
                               (0=-155mV, 31=no offset, 62=+155mV, 63=not
                               used)[br] Extended range
                               (dfe_coeff_training_en=1): -620mV to +620mV in
                               20mV steps (0=-620mV, 31=no offset, 62=+620mV,
                               63=not used). [br] Real range may be smaller due
                               to limited signal swing & common-mode.[br] This
                               value is added with sampler_offset_adj to allow
                               BIOS for margining sweep (Signed number. The
                               Range is -32 to 31).[br] Note: BIOS must make
                               sure the sampler_offset_adj value doesn't cause
                               an overflow or underflow.[br] This field can be
                               programmed by BIOS or HW cal FSM.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_SAMPLER_EVEN_OFFSET_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RXVREF_CTL_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x20011580)                                                      */
/*      SPRB0 (0x20011580)                                                      */
/*      SPRHBM (0x20011580)                                                     */
/*      SPRC0 (0x20011580)                                                      */
/*      SPRMCC (0x20011580)                                                     */
/*      SPRUCC (0x20011580)                                                     */
/* Register default value on SPRA0: 0x07EFDFBF                                  */
/* Register default value on SPRB0: 0x07EFDFBF                                  */
/* Register default value on SPRHBM: 0x07EFDFBF                                 */
/* Register default value on SPRC0: 0x07EFDFBF                                  */
/* Register default value on SPRMCC: 0x07EFDFBF                                 */
/* Register default value on SPRUCC: 0x07EFDFBF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX VREF Control Register
*/


#define DDRD_N0_RXVREF_CTL_MCIO_DDRIO_REG 0x08021580

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vref_ctl0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x0000003F*/

                            /*
                               Per bit Rx Vref Control for DQ0. This value is
                               added with vref_adj to allow BIOS for margining
                               sweep (Signed number. The Range is -64 to 63).
                               The final result is sent to vref CBB. [br] Note:
                               The vref CBB control input is 8-bit. The MSB
                               (Bit 8 is tie to 1). It means the default value
                               is 75% of vccddr_hv[br] Note: BIOS must make
                               sure the vref_adj value doens't cause an
                               overflow or underflow.
                            */
    UINT32 vref_ctl1 : 7;

                            /* Bits[13:7], Access Type=RW, default=0x0000003F*/

                            /*
                               Per bit Rx Vref Control for DQ1. This value is
                               added with vref_adj to allow BIOS for margining
                               sweep (Signed number. The Range is -64 to 63).
                               The final result is sent to vref CBB. [br] Note:
                               The vref CBB control input is 8-bit. The MSB
                               (Bit 8 is tie to 1). It means the default value
                               is 75% of vccddr_hv[br] Note: BIOS must make
                               sure the vref_adj value doens't cause an
                               overflow or underflow.
                            */
    UINT32 vref_ctl2 : 7;

                            /* Bits[20:14], Access Type=RW, default=0x0000003F*/

                            /*
                               Per bit Rx Vref Control for DQ2. This value is
                               added with vref_adj to allow BIOS for margining
                               sweep (Signed number. The Range is -64 to 63).
                               The final result is sent to vref CBB. [br] Note:
                               The vref CBB control input is 8-bit. The MSB
                               (Bit 8 is tie to 1). It means the default value
                               is 75% of vccddr_hv[br] Note: BIOS must make
                               sure the vref_adj value doens't cause an
                               overflow or underflow.
                            */
    UINT32 vref_ctl3 : 7;

                            /* Bits[27:21], Access Type=RW, default=0x0000003F*/

                            /*
                               Per bit Rx Vref Control for DQ3. This value is
                               added with vref_adj to allow BIOS for margining
                               sweep (Signed number. The Range is -64 to 63).
                               The final result is sent to vref CBB. [br] Note:
                               The vref CBB control input is 8-bit. The MSB
                               (Bit 8 is tie to 1). It means the default value
                               is 75% of vccddr_hv[br] Note: BIOS must make
                               sure the vref_adj value doens't cause an
                               overflow or underflow.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RXVREF_CTL_MCIO_DDRIO_STRUCT;

/* DDRD_N0_COMP_LA0_MCIO_DDRIO_REG supported on:                                */
/*      SPRA0 (0x20011584)                                                      */
/*      SPRB0 (0x20011584)                                                      */
/*      SPRHBM (0x20011584)                                                     */
/*      SPRC0 (0x20011584)                                                      */
/*      SPRMCC (0x20011584)                                                     */
/*      SPRUCC (0x20011584)                                                     */
/* Register default value on SPRA0: 0x01020408                                  */
/* Register default value on SPRB0: 0x01020408                                  */
/* Register default value on SPRHBM: 0x01020408                                 */
/* Register default value on SPRC0: 0x01020408                                  */
/* Register default value on SPRMCC: 0x01020408                                 */
/* Register default value on SPRUCC: 0x01020408                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Late Action Register for RCOMP DRV/ODT UP/DN. It is updated either by COMP training FSM or SW. The promotion from late acton register to effective register is controlled by COMP FSM after quiescent request is granted from MC.
*/


#define DDRD_N0_COMP_LA0_MCIO_DDRIO_REG 0x08021584

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rcomp_drv_up : 5;

                            /* Bits[4:0], Access Type=RW/V, default=0x00000008*/

                            /*
                               Data Rcomp Drive Up Comp Value. It is trained by
                               COMP FSM.[br] This value is added with
                               rcomp_drv_up_adj (signed number. Range is -8 to
                               7) before send it to the dqbuf and dqsbuf.[br]
                               Note: HW checks for overflow and underflow
                               condition.
                            */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_drv_dn : 5;

                            /* Bits[11:7], Access Type=RW/V, default=0x00000008*/

                            /*
                               Data Rcomp Drive Down Comp Value. It is trained
                               by COMP FSM.[br] This value is added with
                               rcomp_drv_dn_adj (signed number. Range is -8 to
                               7) before send it to the dqbuf and dqsbuf.[br]
                               Note: HW checks for overflow and underflow
                               condition.
                            */
    UINT32 rsvd_12 : 2;

                            /* Bits[13:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_odt_up : 5;

                            /* Bits[18:14], Access Type=RW/V, default=0x00000008*/

                            /*
                               Data Rcomp OdtUp Comp Value. It is trained by
                               COMP FSM.[br] This value is added with
                               rcomp_odt_up_adj (signed number. Range is -8 to
                               7) before send it to the dqbuf and dqsbuf.[br]
                               Note: HW checks for overflow and underflow
                               condition.
                            */
    UINT32 rsvd_19 : 2;

                            /* Bits[20:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_odt_dn : 5;

                            /* Bits[25:21], Access Type=RW/V, default=0x00000008*/

                            /*
                               Data Rcomp Odt-Down Comp Value. It is trained by
                               COMP FSM.[br] This value is added with
                               rcomp_odt_dn_adj (signed number. Range is -8 to
                               7) before send it to the dqbuf and dqsbuf.[br]
                               Note: HW checks for overflow and underflow
                               condition.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_COMP_LA0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_COMP_LA1_MCIO_DDRIO_REG supported on:                                */
/*      SPRA0 (0x20011588)                                                      */
/*      SPRB0 (0x20011588)                                                      */
/*      SPRHBM (0x20011588)                                                     */
/*      SPRC0 (0x20011588)                                                      */
/*      SPRMCC (0x20011588)                                                     */
/*      SPRUCC (0x20011588)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Late Action Register for SCOMP, and TCO COMP. It is updated by BIOS. The promotion from late acton register to effective register is controlled by COMP FSM after quiescent request is granted from MC.
*/


#define DDRD_N0_COMP_LA1_MCIO_DDRIO_REG 0x08021588

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tco_comp : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /*
                               Data TCO Comp Value for dqbuf and dqsbuf. It is
                               trained by BIOS.[br] 0/64: fastest rise & fall
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
                               Data Slew Rate Comp Value for dqbuf and dqsbuf.
                               It is trained by BIOS.[br] [br] 0 is fastest
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
                               overflow and underflow condition.
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxrt_dqs_dly_adj : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               Rx Retraining DQS delay adjust value for DQS N
                               and P. This field is calcuated by DDRCOMP fub
                               and broadcast to all the data fub. The adjust
                               range is -64 to 63.
                            */
    UINT32 rsvd_21 : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_COMP_LA1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG supported on:                             */
/*      SPRA0 (0x2001158c)                                                      */
/*      SPRB0 (0x2001158c)                                                      */
/*      SPRHBM (0x2001158c)                                                     */
/*      SPRC0 (0x2001158c)                                                      */
/*      SPRMCC (0x2001158c)                                                     */
/*      SPRUCC (0x2001158c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* COMP offset values add on to SCOMP, RCOMP DRV/ODT up/down
*/


#define DDRD_N0_COMP_OFFSET_MCIO_DDRIO_REG 0x0802158C

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
    UINT32 rsvd_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_odt_up_adj : 4;

                            /* Bits[13:10], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to rcomp_odt_up to generate
                               the RCOMP ODT up code. The adjust range is -8 to
                               7
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rcomp_odt_dn_adj : 4;

                            /* Bits[18:15], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number add to rcomp_odt_dn to generate
                               the RCOMP ODT down code. The adjust range is -8
                               to 7
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

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

} DDRD_N0_COMP_OFFSET_MCIO_DDRIO_STRUCT;

/* DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG supported on:                               */
/*      SPRA0 (0x20011590)                                                      */
/*      SPRB0 (0x20011590)                                                      */
/*      SPRHBM (0x20011590)                                                     */
/*      SPRC0 (0x20011590)                                                      */
/*      SPRMCC (0x20011590)                                                     */
/*      SPRUCC (0x20011590)                                                     */
/* Register default value on SPRA0: 0x00600118                                  */
/* Register default value on SPRB0: 0x00600118                                  */
/* Register default value on SPRHBM: 0x00600118                                 */
/* Register default value on SPRC0: 0x00600118                                  */
/* Register default value on SPRMCC: 0x00600118                                 */
/* Register default value on SPRUCC: 0x00600118                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* VSSHI/LDO Control Register
*/


#define DDRD_N0_VSSHI_CTL_MCIO_DDRIO_REG 0x08021590

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
    UINT32 ldo_bypass_ddra : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* LDO output is bypassed to VCCDDRA */
    UINT32 ldo_vref_sel_ddra : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Use vccddra as LDO vref */
    UINT32 ldo_bw_ctl : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /*
                               LDO driver strength based on process variation.
                               Bit 1 is unused. Bit 0 used for LDO Driver
                               Strength[br] This field should be programmed
                               according to analog_tuning field in dll status
                               register:[br] (Fast) 3'd3: 0x0[br] (Typ)
                               3'd1,3'd2: 0x0[br] (Slow) 3'd0: 0x1[br]
                            */
    UINT32 ldo_rcfilter_trim : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /* LDO RC Filter Trim */
    UINT32 ldo_codetarget : 7;

                            /* Bits[24:18], Access Type=RW, default=0x00000018*/

                            /*
                               Sets the LDO reference voltage when
                               ldo_vref_sel_ddra=0.[br] Range is (vccddr_hv/2
                               to vccddr_hv), with step size of
                               vccddr_hv/256.[br] Note: the vref_ctrl MSB (Bit
                               7) is hardtie to 1 inside LDO. Only the lower
                               7-bit is controlled by this field.
                            */
    UINT32 ldo_captrim_loop : 4;

                            /* Bits[28:25], Access Type=RW, default=0x00000000*/

                            /* LDO Cap Trim Loop */
    UINT32 ldo_close_loop : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Operate LDO in close loop mode */
    UINT32 vsshi_ldo_tmr_ext : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Double the timer for LDO/VSSHI settle time. Use
                               for failsafe in case settle time was
                               underestimated.[br] 0: 8K+1K=9K Dclk cycles[br]
                               1: 16K+2K=18K Dclk cycles[br] Doubles the
                               LDO/VSSHI settle time counter in case hard-coded
                               value is not adequate
                            */
    UINT32 vsshi_dclk_gate_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK for VSSHI
                               control logic. Default is disable clock gating.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_VSSHI_CTL_MCIO_DDRIO_STRUCT;


/* DDRD_N0_DLL_CSR_MCIO_DDRIO_REG supported on:                                 */
/*      SPRA0 (0x20011598)                                                      */
/*      SPRB0 (0x20011598)                                                      */
/*      SPRHBM (0x20011598)                                                     */
/*      SPRC0 (0x20011598)                                                      */
/*      SPRMCC (0x20011598)                                                     */
/*      SPRUCC (0x20011598)                                                     */
/* Register default value on SPRA0: 0x28000000                                  */
/* Register default value on SPRB0: 0x28000000                                  */
/* Register default value on SPRHBM: 0x28000000                                 */
/* Register default value on SPRC0: 0x28000000                                  */
/* Register default value on SPRMCC: 0x28000000                                 */
/* Register default value on SPRUCC: 0x28000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DLL Control and Status Register
*/


#define DDRD_N0_DLL_CSR_MCIO_DDRIO_REG 0x08021598

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

} DDRD_N0_DLL_CSR_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RXFIFO_PTR_LOG_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x2001159c)                                                      */
/*      SPRB0 (0x2001159c)                                                      */
/*      SPRHBM (0x2001159c)                                                     */
/*      SPRC0 (0x2001159c)                                                      */
/*      SPRMCC (0x2001159c)                                                     */
/*      SPRUCC (0x2001159c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Debug register to observe RX FIFO write and read pointer.
*/


#define DDRD_N0_RXFIFO_PTR_LOG_MCIO_DDRIO_REG 0x0802159C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxfifo_ptr_err_cnt : 3;

                            /* Bits[2:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Sticky error count. Reset by power good
                               reset.[br] Increment when data fub detect rxfifo
                               read and write pointer is out of sync.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_RXFIFO_PTR_LOG_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_OFFSET0_MCIO_DDRIO_REG supported on:                           */
/*      SPRA0 (0x200115a0)                                                      */
/*      SPRB0 (0x200115a0)                                                      */
/*      SPRHBM (0x200115a0)                                                     */
/*      SPRC0 (0x200115a0)                                                      */
/*      SPRMCC (0x200115a0)                                                     */
/*      SPRUCC (0x200115a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training offset control for VREF, TXDQ and RXDQS
*/


#define DDRD_N0_TRAIN_OFFSET0_MCIO_DDRIO_REG 0x080215A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdqs_dly_adj : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Rx DQS delay adjust value. It is added to all
                               per bit RXDQS delay. It is used by BIOS for
                               margining sweep or ACIO HW training. The adjust
                               range is -128 to 127. The expected usage model
                               is BIOS broadcast the same adjust value to all
                               the bytes.
                            */
    UINT32 txdq_dly_adj : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               Tx DQ delay adjust value. It is added to all per
                               bit TXDQ delay. It is used by BIOS for margining
                               sweep. The adjust range is -128 to 127. The
                               expected usage model is BIOS broadcast the same
                               adjust value to all the bytes.
                            */
    UINT32 vref_adj : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /*
                               Signed number with range from -64 to 63. It is
                               used by BIOS for marging sweep. This value is
                               added to the vref_ctrl before sending to the DQ
                               VREF. [br] Note: HW doesn't check for overflow
                               and underflow. BIOS must ensure the adjust value
                               doesn't overflow or underflow the final vref
                               control value.
                            */
    UINT32 sampler_offset_adj : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Rx Sampler Offset adjust value. It is added to
                               all sampler_even/odd_offset. It is used by BIOS
                               for sampler offsets margining. The adjust range
                               is -32 to 31. The expected usage model is BIOS
                               broadcast the same value to all the bytes.
                            */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_OFFSET0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG supported on:                           */
/*      SPRA0 (0x200115a4)                                                      */
/*      SPRB0 (0x200115a4)                                                      */
/*      SPRHBM (0x200115a4)                                                     */
/*      SPRC0 (0x200115a4)                                                      */
/*      SPRMCC (0x200115a4)                                                     */
/*      SPRUCC (0x200115a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PIREF offset for TXDQS, TXDQ and Receive Enable XOVER
*/


#define DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG 0x080215A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_train_pi_code : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               It is the train PI code to align the PIREF clock
                               to the failing edge of DCLK with data
                               propagation delay after xover training. [br]
                               This value is added with a signed
                               piref_setup_adj (default value is 63) to
                               generate the piref_code to DLL. The xover input
                               is driven from a failing edge of DCLK flop. [br]
                               By moving the piref odd clock to the right by 63
                               ticks, we will have 63 pi tick of setup
                               margin.[br] This field can be programed by BIOS
                               or HW cal FSM.
                            */
    UINT32 rcven_piref_offset : 7;

                            /* Bits[13:7], Access Type=RW/V, default=0x00000000*/

                            /*
                               This rcven piref offset is added with a signed
                               piref_setup_adj (default value is 63). The
                               result is added to the rcven_dly[6:0], so the
                               txdq PI odd code is relative to the piref clock.
                               This field can be programed by BIOS or HW cal
                               FSM.
                            */
    UINT32 txdq_piref_offset : 7;

                            /* Bits[20:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               This txdq piref offset is added with a signed
                               piref_setup_adj (default value is 63). The
                               result is added to the per bit txdq_dly[6:0], so
                               the txdq PI odd code is relative to the piref
                               clock. This field can be programed by BIOS or HW
                               cal FSM.
                            */
    UINT32 txdqs_piref_offset : 7;

                            /* Bits[27:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               This txdqs piref offset is added with a signed
                               piref_setup_adj (default value is 63). The
                               result is added to the txdqs_dly[6:0], so the
                               txdqs PI odd code is relative to the piref
                               clock. This field can be programed by BIOS or HW
                               cal FSM.
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_REG supported on:                           */
/*      SPRA0 (0x200115a8)                                                      */
/*      SPRB0 (0x200115a8)                                                      */
/*      SPRHBM (0x200115a8)                                                     */
/*      SPRC0 (0x200115a8)                                                      */
/*      SPRMCC (0x200115a8)                                                     */
/*      SPRUCC (0x200115a8)                                                     */
/* Register default value on SPRA0: 0x0000003F                                  */
/* Register default value on SPRB0: 0x0000003F                                  */
/* Register default value on SPRHBM: 0x0000003F                                 */
/* Register default value on SPRC0: 0x0000003F                                  */
/* Register default value on SPRMCC: 0x0000003F                                 */
/* Register default value on SPRUCC: 0x0000003F                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training offset control for all PI codes
*/


#define DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_REG 0x080215A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 piref_setup_adj : 7;

                            /* Bits[6:0], Access Type=RW/V, default=0x0000003F*/

                            /*
                               A singed number for piref setup adjust. Range is
                               -64 to 63. [br] The xover input is driven from a
                               inverted DCLK flop. This conrol allows BIOS to
                               adjust the setup time to the input of the xover
                               piref odd clock flop. [br] Default value is 63.
                               [br] It means after xover training to align the
                               piref odd clock to the dclk. The piref odd clock
                               is moved to the right by 64 ticks, and we will
                               have 64 pi tick of setup margin. [br] HW will
                               add this value to all the pi codes before send
                               to DLL.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL0_MCIO_DDRIO_REG supported on:                                 */
/*      SPRA0 (0x200115ac)                                                      */
/*      SPRB0 (0x200115ac)                                                      */
/*      SPRHBM (0x200115ac)                                                     */
/*      SPRC0 (0x200115ac)                                                      */
/*      SPRMCC (0x200115ac)                                                     */
/*      SPRUCC (0x200115ac)                                                     */
/* Register default value on SPRA0: 0x07C09003                                  */
/* Register default value on SPRB0: 0x07C09003                                  */
/* Register default value on SPRHBM: 0x07C09003                                 */
/* Register default value on SPRC0: 0x07C09003                                  */
/* Register default value on SPRMCC: 0x07C09003                                 */
/* Register default value on SPRUCC: 0x07C09003                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX control 0 Register
*/


#define DDRD_N0_RX_CTL0_MCIO_DDRIO_REG 0x080215AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 odt_seg_en : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /*
                               Control number of ODT segment enable for DQ and
                               DQS. ODT rotation is applied before sending the
                               control to CBB. The rotation is used to ensure
                               the segment is evenly burn. Rotation is happened
                               when odten is deasserted. [br] Note: Setting
                               this field to 0x3 means enable all segments and
                               rotation has no meaning.
                            */
    UINT32 rsvd : 5;

                            /* Bits[6:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pbias_stg2amp_trim : 3;

                            /* Bits[9:7], Access Type=RW, default=0x00000000*/

                            /*
                               Amplifier control. The bits in this field
                               control: [br] 2:1 - is used to adjust the bias
                               current for the 2nd stage amplifier in the DQS
                               amplifier path. 0=min, 3=max. [br] 0 - is used
                               to force local receiver bias to on. Otherwise,
                               local receiver bias is enabled through senseamp
                               enable.
                            */
    UINT32 rsvd_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
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
    UINT32 ctle_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* CTLE enable */
    UINT32 rx_postamble_len : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               RX Postamble Length.[br] 0: 0.5 TCK Postamble
                               (default case)[br] 1: 1.5 TCK Postamble (the
                               extra 1 TCK pattern is toggle)
                            */
    UINT32 rx_preamble_type : 3;

                            /* Bits[31:29], Access Type=RW, default=0x00000000*/

                            /*
                               This field is used by rxdeskew logic to
                               determine preamble pattern type.[br] 0: 1 tclk -
                               10 Pattern (MSB is the 1st UI)[br] 1: 2 tclk -
                               0010 Pattern[br] 2: 2 tclk - 1110 Pattern [br]
                               3: 3 tclk - 000010 Pattern[br] 4: 4 tclk -
                               00001010 Pattern[br] Others : Reserved
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_RX_CTL1_MCIO_DDRIO_REG supported on:                                 */
/*      SPRA0 (0x200115b0)                                                      */
/*      SPRB0 (0x200115b0)                                                      */
/*      SPRHBM (0x200115b0)                                                     */
/*      SPRC0 (0x200115b0)                                                      */
/*      SPRMCC (0x200115b0)                                                     */
/*      SPRUCC (0x200115b0)                                                     */
/* Register default value on SPRA0: 0x000001DE                                  */
/* Register default value on SPRB0: 0x000001DE                                  */
/* Register default value on SPRHBM: 0x000001DE                                 */
/* Register default value on SPRC0: 0x000001DE                                  */
/* Register default value on SPRMCC: 0x000001DE                                 */
/* Register default value on SPRUCC: 0x000001DE                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* RX control 1 Register
*/


#define DDRD_N0_RX_CTL1_MCIO_DDRIO_REG 0x080215B0

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_dqs_sa_offset : 6;

                            /* Bits[5:0], Access Type=RW, default=0x0000001E*/

                            /* strobe Rx Amp offset control in DQS RxAmp */
    UINT32 rx_offset_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               enable the offset control in DQ/DQS RxAmp and RX
                               BIAS.
                            */
    UINT32 sdll_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Controls if the slave DLL is enabled in the
                               Deskew logic.
                            */
    UINT32 rx_sampler_offset_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Rx Sampler offset enable. Also enables the RDAC
                               for the sampler offset correction. Needs to be 1
                               for DDR5/DDRT2.
                            */
    UINT32 ext_rx_pulse_width : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Extend DDR4 RxDFE serializer pulse width.[br]
                               Set to 1 to extend the pulse width of the pulsed
                               clocks to the DDR4 RxDFE serializers.[br] Value
                               of 1 seems to work across PVT. May need to set
                               to 0 for slow silicon if we see issues in post
                               silicon.
                            */
    UINT32 dfe_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Enable DFE in DQ. Also it is used in RXBIASANA. */
    UINT32 set_vcdl_fdbk_path : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               When set the feedback data is muxed into the
                               VCDL[br] When cleared the DQ data from Amp is
                               muxed into the VCDL
                            */
    UINT32 byp_vcdl_fwd_path : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the VCDL is removed from the DQ path
                               from Amp to samplers[br] When cleared, the DQ
                               passes from Amp to VCDL and then to samplers
                            */
    UINT32 rx_sampler_tailtune : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 0 by default to enable all the devices in
                               the current source legs across all skews.[br]
                               Set to 1 to reduce number of current sources for
                               any sensitivity issues during post silicon
                               testing.
                            */
    UINT32 rsvd : 10;

                            /* Bits[23:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sdl_max_picode : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               After training , this max value of picode is
                               used for enabling appropriate number of delay
                               cells in SDL for power saving . It should be
                               programmed with the max sdl picode from the
                               nibble
                            */
    UINT32 sdl_gating_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable gating of delay elements in
                               SDL. Default is disable gating. Note : gating
                               must be disabled during any training steps. BIOS
                               can enable gating for power saving after all the
                               training steps are done.
                            */
    UINT32 hw_rxfifo_ptr_rst_en : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW reset RXFIFO Pointer when
                               there is no outstanding read in DDRIO.
                            */
    UINT32 loadtrim : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rload trimming in rx decoder */
    UINT32 bwextend : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Negative cap BW extension */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_MCIO_DDRIO_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_dqs_sa_offset : 6;

                            /* Bits[5:0], Access Type=RW, default=0x0000001E*/

                            /* strobe Rx Amp offset control in DQS RxAmp */
    UINT32 rx_offset_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               enable the offset control in DQ/DQS RxAmp and RX
                               BIAS.
                            */
    UINT32 sdll_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Controls if the slave DLL is enabled in the
                               Deskew logic.
                            */
    UINT32 rx_sampler_offset_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Rx Sampler offset enable. Also enables the RDAC
                               for the sampler offset correction. Needs to be 1
                               for DDR5/DDRT2.
                            */
    UINT32 ext_rx_pulse_width : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Extend DDR4 RxDFE serializer pulse width.[br]
                               Set to 1 to extend the pulse width of the pulsed
                               clocks to the DDR4 RxDFE serializers.[br] Value
                               of 1 seems to work across PVT. May need to set
                               to 0 for slow silicon if we see issues in post
                               silicon.
                            */
    UINT32 dfe_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Enable DFE in DQ. Also it is used in RXBIASANA. */
    UINT32 set_vcdl_fdbk_path : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               When set the feedback data is muxed into the
                               VCDL[br] When cleared the DQ data from Amp is
                               muxed into the VCDL
                            */
    UINT32 byp_vcdl_fwd_path : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the VCDL is removed from the DQ path
                               from Amp to samplers[br] When cleared, the DQ
                               passes from Amp to VCDL and then to samplers
                            */
    UINT32 rx_sampler_tailtune : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 0 by default to enable all the devices in
                               the current source legs across all skews.[br]
                               Set to 1 to reduce number of current sources for
                               any sensitivity issues during post silicon
                               testing.
                            */
    UINT32 rsvd : 4;

                            /* Bits[17:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqs_ctle_en : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* DQS CTLE enable */
    UINT32 dqs_ctle_cap_en : 2;

                            /* Bits[20:19], Access Type=RW, default=0x00000000*/

                            /* DQS CTLE Cap enable. 0=1C, 1=2C, 2=3C, 3=4C. */
    UINT32 dqs_ctle_res_en : 3;

                            /* Bits[23:21], Access Type=RW, default=0x00000000*/

                            /*
                               DQS CTLE Resistor Enable (Controls DC gain). 0 =
                               lowest DC gain (max peaking)[br] Res: 0-3R,
                               1=2R, 2=3R/2, 3=R, 4=2R/3, 5=R/2, 6=R/3,
                               7=R/4[br] where R is ~500 Ohms and depends on
                               process.
                            */
    UINT32 sdl_max_picode : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               After training , this max value of picode is
                               used for enabling appropriate number of delay
                               cells in SDL for power saving . It should be
                               programmed with the max sdl picode from the
                               nibble
                            */
    UINT32 sdl_gating_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable gating of delay elements in
                               SDL. Default is disable gating. Note : gating
                               must be disabled during any training steps. BIOS
                               can enable gating for power saving after all the
                               training steps are done.
                            */
    UINT32 hw_rxfifo_ptr_rst_en : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW reset RXFIFO Pointer when
                               there is no outstanding read in DDRIO.
                            */
    UINT32 loadtrim : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rload trimming in rx decoder */
    UINT32 bwextend : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Negative cap BW extension */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_MCIO_DDRIO_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_dqs_sa_offset : 6;

                            /* Bits[5:0], Access Type=RW, default=0x0000001E*/

                            /* strobe Rx Amp offset control in DQS RxAmp */
    UINT32 rx_offset_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               enable the offset control in DQ/DQS RxAmp and RX
                               BIAS.
                            */
    UINT32 sdll_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Controls if the slave DLL is enabled in the
                               Deskew logic.
                            */
    UINT32 rx_sampler_offset_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Rx Sampler offset enable. Also enables the RDAC
                               for the sampler offset correction. Needs to be 1
                               for DDR5/DDRT2.
                            */
    UINT32 ext_rx_pulse_width : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Extend DDR4 RxDFE serializer pulse width.[br]
                               Set to 1 to extend the pulse width of the pulsed
                               clocks to the DDR4 RxDFE serializers.[br] Value
                               of 1 seems to work across PVT. May need to set
                               to 0 for slow silicon if we see issues in post
                               silicon.
                            */
    UINT32 dfe_en : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Enable DFE in DQ. Also it is used in RXBIASANA. */
    UINT32 set_vcdl_fdbk_path : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               When set the feedback data is muxed into the
                               VCDL[br] When cleared the DQ data from Amp is
                               muxed into the VCDL
                            */
    UINT32 byp_vcdl_fwd_path : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the VCDL is removed from the DQ path
                               from Amp to samplers[br] When cleared, the DQ
                               passes from Amp to VCDL and then to samplers
                            */
    UINT32 rx_sampler_tailtune : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 0 by default to enable all the devices in
                               the current source legs across all skews.[br]
                               Set to 1 to reduce number of current sources for
                               any sensitivity issues during post silicon
                               testing.
                            */
    UINT32 rsvd : 10;

                            /* Bits[23:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sdl_max_picode : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               After training , this max value of picode is
                               used for enabling appropriate number of delay
                               cells in SDL for power saving . It should be
                               programmed with the max sdl picode from the
                               nibble
                            */
    UINT32 sdl_gating_en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable gating of delay elements in
                               SDL. Default is disable gating. Note : gating
                               must be disabled during any training steps. BIOS
                               can enable gating for power saving after all the
                               training steps are done.
                            */
    UINT32 hw_rxfifo_ptr_rst_en : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW reset RXFIFO Pointer when
                               there is no outstanding read in DDRIO.
                            */
    UINT32 loadtrim : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Rload trimming in rx decoder */
    UINT32 bwextend : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000000*/

                            /* Negative cap BW extension */

  } Bits;
  UINT32 Data;

} DDRD_N0_RX_CTL1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL0_MCIO_DDRIO_REG supported on:                                 */
/*      SPRA0 (0x200115b8)                                                      */
/*      SPRB0 (0x200115b8)                                                      */
/*      SPRHBM (0x200115b8)                                                     */
/*      SPRC0 (0x200115b8)                                                      */
/*      SPRMCC (0x200115b8)                                                     */
/*      SPRUCC (0x200115b8)                                                     */
/* Register default value on SPRA0: 0x802000DF                                  */
/* Register default value on SPRB0: 0x802000DF                                  */
/* Register default value on SPRHBM: 0x802000DF                                 */
/* Register default value on SPRC0: 0x802000DF                                  */
/* Register default value on SPRMCC: 0x802000DF                                 */
/* Register default value on SPRUCC: 0x802000DF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX control 0 Register
*/


#define DDRD_N0_TX_CTL0_MCIO_DDRIO_REG 0x080215B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 drv_static_leg_cfg : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001F*/

                            /*
                               Sets number of DQ/DQS driver (Ron) static
                               legs.[br] First 3 MSB bits has weight of 4, 4th
                               MSB has weight of 2 and LSB bit has weight of
                               1.[br] RTL default of 0x1F has 15 static legs
                               enabled.[br] Static leg training starts with
                               0x1F (all 15 legs enabled)
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_seg_en : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000003*/

                            /*
                               Control number of data sement enable for DQ and
                               DQS.
                            */
    UINT32 rsvd_8 : 9;

                            /* Bits[16:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 imode_en : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Imode EQ per byte lane. [br] 0: Imode
                               completely off [br] 1: Imode enabled (Xtalk
                               cancellation OR swing boost selected by ImodeCfg
                            */
    UINT32 rsvd_18 : 1;

                            /* Bits[18:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 eq_post2_sign : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               2nd tap equalization sign bit[br] 0 +ve
                               equalization[br] 1: -ve equalization
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 eq_post2_sel : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               2nd tap selection:[br] 0 txeq_2nd_tap used for
                               2nd post cursor tap[br] 1 txeq_2nd_tap used for
                               extending range of 1st tap.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 imode_eq_code : 6;

                            /* Bits[28:23], Access Type=RW, default=0x00000000*/

                            /*
                               Imode coefficient, sets Imode current.[br] Sweep
                               values: (0-15, 40-42, 48-63) for a linear
                               increase of current from 0 to max.
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
                               0 -DQBUF and DQSBUF bypass SCOMP and TCO delay
                               in the transmitter.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TX_CTL1_MCIO_DDRIO_REG supported on:                                 */
/*      SPRA0 (0x200115bc)                                                      */
/*      SPRB0 (0x200115bc)                                                      */
/*      SPRHBM (0x200115bc)                                                     */
/*      SPRC0 (0x200115bc)                                                      */
/*      SPRMCC (0x200115bc)                                                     */
/*      SPRUCC (0x200115bc)                                                     */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* TX control 1 Register
*/


#define DDRD_N0_TX_CTL1_MCIO_DDRIO_REG 0x080215BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_preamble_len : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000001*/

                            /*
                               TX Preenable Length. [br] Legal programming
                               value for [br] DDR4: 1-2 TCK [br] DDR5: 2-4 TCK
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_preamble_pat : 8;

                            /* Bits[11:4], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Preamble pattern. Each bit
                               represents a UI of the preamble pattern starting
                               at LSB for 1st UI.[br] Note: For DDR5 write
                               leveling training, which require to mask off the
                               preamble strobe, BIOS can program the pattern to
                               all 0's.[br] [br] Legal value is listed below.
                               [br] DDR4-[br] 1 TCK: 8'bXXXX_XX01[br] 2 TCK:
                               8'bXXXX_0111 (Long Preamble)[br] DDR5-[br] 2
                               TCK: 8'XXXX_0100[br] 3 TCK: 8'bXX01_0000 or
                               8'bXX01_0100[br] 4 TCK: 8'b0101_0000
                            */
    UINT32 rsvd_12 : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_postamble_len : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               TX Postamble Length.[br] 0: 0.5 TCK Postamble
                               (default case)[br] 1: 1.5 TCK Postamble (the
                               extra 1 TCK pattern is always differentially
                               low)
                            */
    UINT32 rsvd_17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TX_CTL1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_PXC_CTL0_MCIO_DDRIO_REG supported on:                                */
/*      SPRA0 (0x200115c4)                                                      */
/*      SPRB0 (0x200115c4)                                                      */
/*      SPRHBM (0x200115c4)                                                     */
/*      SPRC0 (0x200115c4)                                                      */
/*      SPRMCC (0x200115c4)                                                     */
/*      SPRUCC (0x200115c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* PXC Control 0 Register
*/


#define DDRD_N0_PXC_CTL0_MCIO_DDRIO_REG 0x080215C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 attacker_sel0 : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select which bit within a byte is the attacker
                               for dq0 in this nibble. [br] Note: Attacker can
                               be from the other nibble inside a byte. BIOS
                               must not set the attacker to itself.
                            */
    UINT32 attacker_sel1 : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               Select which bit within a byte is the attacker
                               for dq1 in this nibble. [br] Note: Attacker can
                               be from the other nibble inside a byte. BIOS
                               must not set the attacker to itself.
                            */
    UINT32 attacker_sel2 : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               Select which bit within a byte is the attacker
                               for dq2 in this nibble. [br] Note: Attacker can
                               be from the other nibble inside a byte. BIOS
                               must not set the attacker to itself.
                            */
    UINT32 attacker_sel3 : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               Select which bit within a byte is the attacker
                               for dq3 in this nibble. [br] Note: Attacker can
                               be from the other nibble inside a byte. BIOS
                               must not set the attacker to itself.
                            */
    UINT32 rsvd_12 : 5;

                            /* Bits[16:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 12;

                            /* Bits[28:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pxc_coupling_type : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Select PXC is inductive or capacitive coupling
                               cross talk dominant type[br] 0: capacitive
                               coupling. If victim and attacker data have the
                               same transition phase, the xtalk delta is added;
                               otherwise (oppsite transition phase) the xtalk
                               delta is subtracted.[br] 1: inductive coupling.
                               If victim and attacker data have the same
                               transition phase, the xtalk delta is subtracted
                               ; otherwise (oppsite transition phase) the xtalk
                               delta is added.
                            */
    UINT32 ddjc_en : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* DDJC enable */
    UINT32 pxc_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Phase base cross talk cancellaton enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_PXC_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG supported on:                                */
/*      SPRA0 (0x200115c8)                                                      */
/*      SPRB0 (0x200115c8)                                                      */
/*      SPRHBM (0x200115c8)                                                     */
/*      SPRC0 (0x200115c8)                                                      */
/*      SPRMCC (0x200115c8)                                                     */
/*      SPRUCC (0x200115c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* DFX Control 0 Register
*/


#define DDRD_N0_DFX_CTL0_MCIO_DDRIO_REG 0x080215C8

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
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
                               0 - No Vref power saving in Self Refresh. All DQ
                               Vref generators is enabled by dq_vref_en.[br] 1
                               - DQ Vref generators are switched off while in
                               Self Refresh regardless the setting of
                               dq_vref_en.
                            */
    UINT32 ckepd_vref_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in CKE power down. All
                               DQ Vref generators is enabled by dq_vref_en.[br]
                               1 - DQ Vref generators are switched off while in
                               CKE power down regardless the setting of
                               dq_vref_en.
                            */
    UINT32 tx_rank_chg_hold_adj : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               A value of 1 add extra 1 cycle of hold time for
                               tx rank switching
                            */
    UINT32 rx_rank_chg_hold_adj : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               A value of 1 add extra 1 cycle of hold time for
                               rx rank switching
                            */
    UINT32 rsvd_8 : 5;

                            /* Bits[12:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_rcvenpost_early : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable using rcvenpost early to control the rx
                               rank switch latch.[br] If this bit is set to 1,
                               minimum Rd-Rd TR time will need to increase by
                               1.
                            */
    UINT32 rsvd_14 : 3;

                            /* Bits[16:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_17 : 14;

                            /* Bits[30:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_voc_pass_gate_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               VOC RX pass gate enable for VOC training and NTL
                               testing.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFX_CTL0_MCIO_DDRIO_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
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
                               0 - No Vref power saving in Self Refresh. All DQ
                               Vref generators is enabled by dq_vref_en.[br] 1
                               - DQ Vref generators are switched off while in
                               Self Refresh regardless the setting of
                               dq_vref_en.
                            */
    UINT32 ckepd_vref_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in CKE power down. All
                               DQ Vref generators is enabled by dq_vref_en.[br]
                               1 - DQ Vref generators are switched off while in
                               CKE power down regardless the setting of
                               dq_vref_en.
                            */
    UINT32 tx_rank_chg_hold_adj : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               A value of 1 add extra 1 cycle of hold time for
                               tx rank switching
                            */
    UINT32 rx_rank_chg_hold_adj : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               A value of 1 add extra 1 cycle of hold time for
                               rx rank switching
                            */
    UINT32 rsvd_8 : 5;

                            /* Bits[12:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_rcvenpost_early : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable using rcvenpost early to control the rx
                               rank switch latch.[br] If this bit is set to 1,
                               minimum Rd-Rd TR time will need to increase by
                               1.
                            */
    UINT32 rsvd_14 : 3;

                            /* Bits[16:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_pxc_1ui_zero : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Disable using zero for 1st UI of data when PXC
                               is enabled. It uses the calculated code instead.
                            */
    UINT32 rsvd_18 : 12;

                            /* Bits[29:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txdqs_latch_en_ovr : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Forces tx dqs rank latches to be transparent and
                               pass per rank settings directly to analog
                               endpoints.
                            */
    UINT32 rx_voc_pass_gate_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               VOC RX pass gate enable for VOC training and NTL
                               testing.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFX_CTL0_MCIO_DDRIO_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

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
                               0 - No Vref power saving in Self Refresh. All DQ
                               Vref generators is enabled by dq_vref_en.[br] 1
                               - DQ Vref generators are switched off while in
                               Self Refresh regardless the setting of
                               dq_vref_en.
                            */
    UINT32 ckepd_vref_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0 - No Vref power saving in CKE power down. All
                               DQ Vref generators is enabled by dq_vref_en.[br]
                               1 - DQ Vref generators are switched off while in
                               CKE power down regardless the setting of
                               dq_vref_en.
                            */
    UINT32 tx_rank_chg_hold_adj : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               A value of 1 add extra 1 cycle of hold time for
                               tx rank switching
                            */
    UINT32 rx_rank_chg_hold_adj : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               A value of 1 add extra 1 cycle of hold time for
                               rx rank switching
                            */
    UINT32 rsvd_8 : 5;

                            /* Bits[12:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_rcvenpost_early : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable using rcvenpost early to control the rx
                               rank switch latch.[br] If this bit is set to 1,
                               minimum Rd-Rd TR time will need to increase by
                               1.
                            */
    UINT32 rsvd_14 : 3;

                            /* Bits[16:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_17 : 14;

                            /* Bits[30:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_voc_pass_gate_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               VOC RX pass gate enable for VOC training and NTL
                               testing.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_DFX_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_VREF_TRACK_CTL_MCIO_DDRIO_REG supported on:                          */
/*      SPRA0 (0x200115cc)                                                      */
/*      SPRB0 (0x200115cc)                                                      */
/*      SPRHBM (0x200115cc)                                                     */
/*      SPRC0 (0x200115cc)                                                      */
/*      SPRMCC (0x200115cc)                                                     */
/*      SPRUCC (0x200115cc)                                                     */
/* Register default value on SPRA0: 0x800001EF                                  */
/* Register default value on SPRB0: 0x800001EF                                  */
/* Register default value on SPRHBM: 0x800001EF                                 */
/* Register default value on SPRC0: 0x800001EF                                  */
/* Register default value on SPRMCC: 0x800001EF                                 */
/* Register default value on SPRUCC: 0x800001EF                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Vref Track Control Register
*/


#define DDRD_N0_VREF_TRACK_CTL_MCIO_DDRIO_REG 0x080215CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vref_cntrl_cp : 4;

                            /* Bits[3:0], Access Type=RW, default=0x0000000F*/

                            /*
                               Code for Vref Cap to supply. 0=min, 15=max.
                               Sweep codes 0-15.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vref_cntrl_cn : 4;

                            /* Bits[8:5], Access Type=RW, default=0x0000000F*/

                            /*
                               Code for Vref Cap to gnd. 0=min, 15=max. Sweep
                               codes 0-7,11-15
                            */
    UINT32 rsvd_9 : 22;

                            /* Bits[30:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vreftrack_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Vref Track enable */

  } Bits;
  UINT32 Data;

} DDRD_N0_VREF_TRACK_CTL_MCIO_DDRIO_STRUCT;

/* DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG supported on:                           */
/*      SPRA0 (0x200115d0)                                                      */
/*      SPRB0 (0x200115d0)                                                      */
/*      SPRHBM (0x200115d0)                                                     */
/*      SPRC0 (0x200115d0)                                                      */
/*      SPRMCC (0x200115d0)                                                     */
/*      SPRUCC (0x200115d0)                                                     */
/* Register default value on SPRA0: 0x00042000                                  */
/* Register default value on SPRB0: 0x00042000                                  */
/* Register default value on SPRHBM: 0x00042000                                 */
/* Register default value on SPRC0: 0x00042000                                  */
/* Register default value on SPRMCC: 0x00042000                                 */
/* Register default value on SPRUCC: 0x00042000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW Training Control 0 Register
*/


#define DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG 0x080215D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_stage_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to enable xover training stage */
    UINT32 sampler_offset_stage_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable even/odd sampler training
                               stage for all DQ bits.[br] Note: The result is
                               only logged to the sampler even/odd offset
                               register when both even and odd training are
                               done.
                            */
    UINT32 dfe_summer_offset_stage_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable DFE summer offset training
                               stage for all DQ bits.[br] Note: the training
                               result is applied to all ranks. BIOS will adjust
                               later for all ranks setting based on eye
                               centering.
                            */
    UINT32 rsvd_3 : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 settling_time_x2 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to double the analog settling time.
                               [br] The default settling time for each stage
                               are [br] Xover : 32 Dclk for 1st, 24 Dclk for
                               subsequent [br] Sampler offset : 528 Dclk for
                               1st time; 144 Dlck for subsequent steps [br] DFE
                               summer offset : 24 Dclk [br] Rcven fine/coarse :
                               16 Dclk / 8 Dclk [br] Dqdqs fine / coarse : 16
                               Dclk / 8 Dclk
                            */
    UINT32 sample_cnt : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of Sample to evaluate for each iteration
                               step. It applied for all training stages. [br]
                               0: 16[br] 1: 32[br] 2: 64[br] 3: 2 (Should only
                               used to speed up digitial simulation)
                            */
    UINT32 sample_thresh : 6;

                            /* Bits[15:10], Access Type=RW, default=0x00000008*/

                            /* Sample threshold value for voting logic. */
    UINT32 rsvd_16 : 4;

                            /* Bits[19:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_20 : 4;

                            /* Bits[23:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xover_err : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate xover training is
                               failed. SW must clear this bit to 0 when enable
                               HW calibration.
                            */
    UINT32 even_sampler_offset_err : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate even sampler offset
                               training is failed. SW must clear this bit to 0
                               when enable HW calibration.
                            */
    UINT32 odd_sampler_offset_err : 1;

                            /* Bits[26:26], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate odd sampler offset
                               training is failed. SW must clear this bit to 0
                               when enable HW calibration.
                            */
    UINT32 dfe_summer_offset_err : 1;

                            /* Bits[27:27], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Set to 1 by HW to indicate dfe summer offset
                               training is failed. SW must clear this bit to 0
                               when enable HW calibration.
                            */
    UINT32 rsvd_28 : 2;

                            /* Bits[29:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hw_cal_en : 1;

                            /* Bits[31:31], Access Type=RW/V, default=0x00000000*/

                            /*
                               Set to 1 to enable HW calibration. They are five
                               HW calibration stages that SW can request HW to
                               perform. It is defined in bit 0 to 4 in this
                               register. [br] HW will self clear this bit when
                               all the enabled calibration stages are
                               completed. [br] For BIOS training, XOVER,
                               sampler offset, and DFE summer offset training
                               can be enabled by SW.[br] For ACIO testing, all
                               5 stages can be enabled by SW.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_HWCAL_SWEEP_RESULT0_MCIO_DDRIO_REG supported on:                     */
/*      SPRA0 (0x200115d8)                                                      */
/*      SPRB0 (0x200115d8)                                                      */
/*      SPRHBM (0x200115d8)                                                     */
/*      SPRC0 (0x200115d8)                                                      */
/*      SPRMCC (0x200115d8)                                                     */
/*      SPRUCC (0x200115d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW Calibration Sweep Result 0 Register
*/


#define DDRD_N0_HWCAL_SWEEP_RESULT0_MCIO_DDRIO_REG 0x080215D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hwcal_sweep_result0 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The sweep result 0-2 register is used for debug
                               to observe the full sweep result for rcven or
                               dqdqs fine training, so both stages can't be
                               enabled at the same time. For DQDQS training, it
                               only valid if one DQ bit is enabled.[br] Note :
                               we log every other step only. (0-143 step, only
                               even number of step is logged)
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_HWCAL_SWEEP_RESULT0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_HWCAL_SWEEP_RESULT1_MCIO_DDRIO_REG supported on:                     */
/*      SPRA0 (0x200115dc)                                                      */
/*      SPRB0 (0x200115dc)                                                      */
/*      SPRHBM (0x200115dc)                                                     */
/*      SPRC0 (0x200115dc)                                                      */
/*      SPRMCC (0x200115dc)                                                     */
/*      SPRUCC (0x200115dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW Calibration Sweep Result 1 Register
*/


#define DDRD_N0_HWCAL_SWEEP_RESULT1_MCIO_DDRIO_REG 0x080215DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hwcal_sweep_result1 : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The sweep result 0-2 register is used for debug
                               to observe the full sweep result for rcven or
                               dqdqs fine training, so both stages can't be
                               enabled at the same time. For DQDQS training, it
                               only valid if one DQ bit is enabled.[br] Note :
                               we log every other step only. (0-143 step, only
                               even number of step is logged)
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_HWCAL_SWEEP_RESULT1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_HWCAL_SWEEP_RESULT2_MCIO_DDRIO_REG supported on:                     */
/*      SPRA0 (0x200115e0)                                                      */
/*      SPRB0 (0x200115e0)                                                      */
/*      SPRHBM (0x200115e0)                                                     */
/*      SPRC0 (0x200115e0)                                                      */
/*      SPRMCC (0x200115e0)                                                     */
/*      SPRUCC (0x200115e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* HW Calibration Sweep Result 2 Register
*/


#define DDRD_N0_HWCAL_SWEEP_RESULT2_MCIO_DDRIO_REG 0x080215E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hwcal_sweep_result2 : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The sweep result 0-2 register is used for debug
                               to observe the full sweep result for rcven or
                               dqdqs fine training, so both stages can't be
                               enabled at the same time. For DQDQS training, it
                               only valid if one DQ bit is enabled.[br] Note :
                               we log every other step only. (0-143 step, only
                               even number of step is logged)
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_HWCAL_SWEEP_RESULT2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x200115e4)                                                      */
/*      SPRB0 (0x200115e4)                                                      */
/*      SPRHBM (0x200115e4)                                                     */
/*      SPRC0 (0x200115e4)                                                      */
/*      SPRMCC (0x200115e4)                                                     */
/*      SPRUCC (0x200115e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 0 Register
*/


#define DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_REG 0x080215E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dqs_odt_dly : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Logic delay to turn on DQS ODT base on receive
                               enable signal. Unit is DLCK. Range is 0-12.
                            */
    UINT32 rsvd : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqs_odt_dur : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Control how long the DQS ODT stay on after
                               receive enable signal deasserted in DCLK domain.
                               Unit is DCLK. Range is 0-31.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqs_sa_dly : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000000*/

                            /*
                               Logic delay to turn on DQS Sense Amp base on
                               receive enable signal. Unit is DCLK. Range is
                               0-12.
                            */
    UINT32 rsvd_17 : 2;

                            /* Bits[18:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dqs_sa_dur : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Control how long the DQS Sense Amp stay on after
                               receive enable signal deasserted in DCLK domain.
                               Unit is DCLK. Range is 0-31.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_CTL0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x200115e8)                                                      */
/*      SPRB0 (0x200115e8)                                                      */
/*      SPRHBM (0x200115e8)                                                     */
/*      SPRC0 (0x200115e8)                                                      */
/*      SPRMCC (0x200115e8)                                                     */
/*      SPRUCC (0x200115e8)                                                     */
/* Register default value on SPRA0: 0x98000000                                  */
/* Register default value on SPRB0: 0x98000000                                  */
/* Register default value on SPRHBM: 0x98000000                                 */
/* Register default value on SPRC0: 0x98000000                                  */
/* Register default value on SPRMCC: 0x98000000                                 */
/* Register default value on SPRUCC: 0x98000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 1 Register
*/


#define DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG 0x080215E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_train_ctr_en : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Enables the Command Training Counter mode (DQ
                               receivers, ODT needs to be enabled separately
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
    UINT32 wr_level_train_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enables Write Leveling Training Mode. Write
                               Leveling training is not the same for both DDR4
                               and DDR5. BIOS must set the ddr4 control bit to
                               indicate write leveling for DDR4 or DDR5.[br]
                               For DDR5, when set, the strobes have their
                               transmitter enable, while the DQ signals have
                               their receivers enabled. The Received value from
                               the DQ signals is sampled and stored in the
                               training result register. This is a continously
                               sampled valued.[br] Note: all clock gate enable
                               must 0 during Write Level training mode.
                            */
    UINT32 rd_level_train_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enables Read Leveling Training Mode. Read
                               Leveling training step is the same for DDR4 and
                               DDR5.[br] Note: all clock gate enable must 0
                               during Read Level training mode.
                            */
    UINT32 rd_level_train_type : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0 : Front side[br] 1: Back side (DDR5 LRDIMM /
                               3D Xpoint DIMM)[br] [br] When
                               rd_level_train_type is set to 1 and
                               rd_level_train_en is set to 1, the training
                               counter increments based on the sampling of DQ
                               with the rising edge of rcven (DB sends feedback
                               to host through DQ pins, DB samples MDQS with
                               its internal rcven signal). [br] When
                               rd_level_train_type is set to 0 and
                               rd_level_train_en is set to 1, the training
                               counter increments based on the sampling of DQS
                               with the rising edge of rcven.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xover_cal : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Setting this bit to 1 will prepare data fub in a
                               state to allow BIOS only to perform Xover
                               calibraton training. [br] - enable all the DCLK
                               And PI clocks. (may remove clock gating
                               function)
                            */
    UINT32 vref_highz : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Tri-state the vref_hv output. Set to 1 for NTL
                               and Rx Summer Offset Cal.
                            */
    UINT32 even_sampler_offset_cal_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used for even sampler offset calibration mode.
                               Shorts inputs of samplers to Vcm
                            */
    UINT32 odd_sampler_offset_cal_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used for odd sampler offset calibration mode.
                               Shorts inputs of samplers to Vcm
                            */
    UINT32 dfe_coeff_train_en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Should be set only during DFE coefficient
                               initial training (pulse response). Increases
                               sampler offset range by 2X.
                            */
    UINT32 elr_dimm : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Set this bit to 1 for DDR5 ELR DIMM type */
    UINT32 nibble_dis : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS can disable a nibble within a byte. This
                               can be used to disable 1 nibble of the ECC for
                               9x4 DIMM support.[br] The entire ECC byte can
                               also be disabled for DIMMs that doesn't support
                               ECC. In DDR4, the unused byte can be disabled as
                               well.
                            */
    UINT32 ddr4 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               For SPR, 2 Channel DDR-MC sub-system. It can
                               either operate in DDR4/DDR-T or DDR5/DDR-T2. It
                               doesn't support DDR4/DDR-T mix with
                               DDR5/DDR-T2.[br] Setting this bit to 1 indicate
                               it is operating in DDR4/DDR-T. 0 inidate it is
                               operating in DDR5/DDR-T2.[br] Fub use this bit
                               to determine the training level different
                               between DDR4 vs DDR5.
                            */
    UINT32 txrt_clk_gate_en : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate tx retraining
                               logic for power saving. Default is disable clock
                               gating.[br] Note: Clock gating must be disable
                               during any training steps. BIOS can enable clock
                               gating for power saving after all the training
                               steps are done.
                            */
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
    UINT32 tx_piclk_gate_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate TX PI CLK for
                               transmit path logic. Default is disable clock
                               gating. [br] Note: Clock gating must be disable
                               during any training steps. BIOS can enable clock
                               gating for power saving after all the training
                               steps are done.
                            */
    UINT32 rx_piclk_gate_en : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate RX PI CLK for
                               receive path logics. Default is disable clock
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
    UINT32 rank_ovr_en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, HW uses the rank_value_ovr field
                               to select rank parameters instead of rank number
                               from MC. It is used for training step that
                               require to read out rank specific infomration
                               without MC command.[br] Note: The current use
                               case is for DDR4 Write leveling training.
                            */
    UINT32 rank_value_ovr : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               When rank_ovr_en is set, this rank override
                               value is used by HW to select rank parameters
                               instead of rank number from MC. It is used for
                               training step that require to read out rank
                               specific infomration without MC command.[br]
                               Note: The current use case is for DDR4 Write
                               leveling training.
                            */
    UINT32 force_odt_on : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Force Weak DQS/DQ ODT on. It is used before ODT
                               enable window is trained and program by BIOS.
                               [br] The weak ODT is ignored when drive enable
                               is asserted. [br] Note: For ACIO, ODT is
                               provided by different DDR channel.
                            */
    UINT32 force_rxbias_on : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Force Rx bias circuit on.[br] Note: In SPR
                               setting this bit to 1 doesnt force vref on. To
                               force vref on, user must set dq_vref_en to 1 and
                               sr_vref_dis to 0.
                            */
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
                               Force Strong DQS/DQ ODT on. when set, it always
                               enable one segement for ODT regardless of the
                               state of drive enable.[br] The intend use case
                               is for ACIO with burnin.
                            */
    UINT32 dq_vref_en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Vref generator Enable. Set to 1 to enable all DQ
                               bits vref.[br] Note: The self refresh VREF
                               disable control (sr_vref_dis) can disable vref
                               during DIMM in self refresh.[br] To force VREF
                               on all the time in SPR, user must program
                               sr_vref_dis to 0 and ckepd_vref_dis to 0. [br]
                               Note: Unlike wave 1 design, ForceBiasOn doesn't
                               force verf enable anymore.
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x200115ec)                                                      */
/*      SPRB0 (0x200115ec)                                                      */
/*      SPRHBM (0x200115ec)                                                     */
/*      SPRC0 (0x200115ec)                                                      */
/*      SPRMCC (0x200115ec)                                                     */
/*      SPRUCC (0x200115ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 2 Register
*/


#define DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG 0x080215EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_strobe_mask : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Each bit in the TX strobe mask will mask out the
                               strobe pulse associated with each location in
                               the write burst (i.e. for a BL16 there are 8
                               strobe pulses (rising/falling combination)). A
                               value of 00000000b will enable all strobes. A
                               value of 1111_1110b will enable a single strobe
                               pulse at the first 2 UIs in the BL. [br] The
                               current use case is for DDR5 write leveling
                               training which only enable the 1st TCK DQS
                               strobe (0xFE)[br] Note: When drv_dqs_diff_high
                               is set, the masked Strobe is driven
                               differentially high. On the other hand, when
                               drv_dqs_diff_low is set, the masked stobe is
                               driven differentially low.
                            */
    UINT32 drv_dqs_diff_low : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the strobe is driven differentially
                               low, other than when a WRITE strobe sequence is
                               sent to the DRAM
                            */
    UINT32 drv_dqs_diff_high : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the strobe is driven differentially
                               high, other than when a WRITE strobe sequence is
                               sent to the DRAM.
                            */
    UINT32 dq_value_ovr : 4;

                            /* Bits[13:10], Access Type=RW, default=0x00000000*/

                            /*
                               When the DQ Override Enable bit is set, this
                               setting determines the value that is driven by
                               the DQ transmitter at all time. The value is
                               constant over all UIs, but can be set per DQ in
                               the nibble.
                            */
    UINT32 dq_ovr_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               This setting enables the over-ride operation
                               associated with the DQ Value Override
                               setting.[br] Note: Once this bit is set, it
                               turns on the TX driver and drives the
                               dq_value_ovr on all DQs pins
                            */
    UINT32 sample_sel : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Determines if the command training counter will
                               count 1 or 0.[br] 0: count 1[br] 1: count 0
                            */
    UINT32 dq_sample_mask : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               Mask off any DQ bits not considered in the OR
                               computation for the command training counting.
                            */
    UINT32 sampler_feedback_en : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Enables feedback from DQ SAL for any DDR5
                               training modes, requiring feedback from DQ
                               lanes.
                            */
    UINT32 unsampled_path_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               The unsampled path is automatically enabled in
                               DDR4, but not for DDR5. In DDR5, set this bit to
                               0x1 during write level, and cmd training (DQ
                               feedback is used).
                            */
    UINT32 rsvd : 6;

                            /* Bits[27:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xover_cal_mux_sel : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Select which xover phase detect output for
                               calibration. For each nibble, we have 8 xover
                               per DLL piref.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x200115f0)                                                      */
/*      SPRB0 (0x200115f0)                                                      */
/*      SPRHBM (0x200115f0)                                                     */
/*      SPRC0 (0x200115f0)                                                      */
/*      SPRMCC (0x200115f0)                                                     */
/*      SPRUCC (0x200115f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Train Control 3 Register
*/


#define DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_REG 0x080215F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dq_odt_dly : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Logic delay to turn on DQ ODT base on receive
                               enable signal. Unit is DCLK. Range is 0-12.
                            */
    UINT32 rsvd : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_odt_dur : 5;

                            /* Bits[10:6], Access Type=RW, default=0x00000000*/

                            /*
                               Control how long the DQ ODT stay on after
                               receive enable signal deasserted in DCLK domain.
                               Unit is DCLK. Range is 0-31.
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_sa_dly : 4;

                            /* Bits[16:13], Access Type=RW, default=0x00000000*/

                            /*
                               Logic delay to turn on DQ Sense Amp base on
                               receive enable signal. Unit is DCLK. Range is
                               0-12.
                            */
    UINT32 rsvd_17 : 2;

                            /* Bits[18:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dq_sa_dur : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /*
                               Control how long the DQ Sense Amp stay on after
                               receive enable signal deasserted in DCLK domain.
                               Unit is DCLK. Range is 0-31.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_CTL3_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_CNT_WINDOW0_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x200115f4)                                                      */
/*      SPRB0 (0x200115f4)                                                      */
/*      SPRHBM (0x200115f4)                                                     */
/*      SPRC0 (0x200115f4)                                                      */
/*      SPRMCC (0x200115f4)                                                     */
/*      SPRUCC (0x200115f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training Counting Window 0 Register
*/


#define DDRD_N0_TRAIN_CNT_WINDOW0_MCIO_DDRIO_REG 0x080215F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cnt_window : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               cmd_train_ctr_en:[br] value that sets the number
                               of DCLK cycles that the DQ samples will be
                               counted. Linear setting. This is broadcast to
                               all bytes one register per byte. When this field
                               is set to 0 the counter will run until it
                               saturates, or until it the CMD Training Counter
                               Mode field is cleared.[br] wr_level_train_en:
                               [br] control when to increment the coutner after
                               the trigger event (DQS OE)[br]
                               rd_level_train_en:[br] control when to increment
                               the coutner after the sampling event (rising
                               edge of receive enable)
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_CNT_WINDOW0_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_REG supported on:                        */
/*      SPRA0 (0x200115f8)                                                      */
/*      SPRB0 (0x200115f8)                                                      */
/*      SPRHBM (0x200115f8)                                                     */
/*      SPRC0 (0x200115f8)                                                      */
/*      SPRMCC (0x200115f8)                                                     */
/*      SPRUCC (0x200115f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Sample counter result for CMD Training Counter Mode
*/


#define DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_REG 0x080215F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 train_cnt : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               cmd_train_ctr_en:[br] Value that indicates the
                               number of logic 1 or 0 values sampled during the
                               Counting Window (based on sample_sel). This is
                               cleared when the CMD Training Counter mode is
                               enabled. This count is the result of the OR of
                               all DQs in the nibble, for those DQs that are
                               not masked off. Whten the cnt_window is set to
                               0, the counter will run until it saturates.[br]
                               wr_level_train_en:[br] DQ sample count[br]
                               rd_level_train_en:[br] Receive enable sample
                               count
                            */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_CNT_RESULT_MCIO_DDRIO_STRUCT;

/* DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_REG supported on:                           */
/*      SPRA0 (0x200115fc)                                                      */
/*      SPRB0 (0x200115fc)                                                      */
/*      SPRHBM (0x200115fc)                                                     */
/*      SPRC0 (0x200115fc)                                                      */
/*      SPRMCC (0x200115fc)                                                     */
/*      SPRUCC (0x200115fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrd1_type/ddrd1_n0_ch0a_0/ddrd1_n0_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Training Result 0 Register
*/


#define DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_REG 0x080215FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xover_cal_pd_result : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* Xover Calibration Phase Detected Result */
    UINT32 sa_even_sampler : 4;

                            /* Bits[4:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               Per bit even sampler value for senseamp
                               training. It is used for DDR5 sampler and DFE
                               summer VOC training stage
                            */
    UINT32 sa_odd_sampler : 4;

                            /* Bits[8:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Per bit odd sampler value for senseamp training.
                               It is used only for DDR5 sampler training stage.
                            */
    UINT32 sa_ddr4_train_result : 4;

                            /* Bits[12:9], Access Type=RO/V, default=0x00000000*/

                            /* Per bit senseamp train result for DDR4 only */
    UINT32 rsvd : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_STRUCT;

/* DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG supported on:                               */
/*      SPRA0 (0x20013f00)                                                      */
/*      SPRB0 (0x20013f00)                                                      */
/*      SPRHBM (0x20013f00)                                                     */
/*      SPRC0 (0x20013f00)                                                      */
/*      SPRMCC (0x20013f00)                                                     */
/*      SPRUCC (0x20013f00)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Sub-Channel Control 0 Register[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_CTL0_CH_0_MCIO_DDRIO_REG 0x08023F00

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
    UINT32 ch_dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Channel Disable[br] - shut off all
                               analog/DCLK/DLL
                            */
    UINT32 wr_crc_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable write crc mode.[br] DDR4: extend BL8
                               write to BL10.[br] DDR5: extend BL16 write to
                               BL18.
                            */
    UINT32 rank_ovr_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, HW uses the rank_value_ovr field
                               to select rank parameters instead of rank number
                               from MC. It is used for training step that
                               require to read out rank specific infomration
                               without MC command.[br] Note: The current use
                               case is for DDR4 Write leveling training.
                            */
    UINT32 rank_value_ovr : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               When rank_ovr_en is set, this rank override
                               value is used by HW to select rank parameters
                               instead of rank number from MC. It is used for
                               training step that require to read out rank
                               specific infomration without MC command.[br]
                               Note: The current use case is for DDR4 Write
                               leveling training.
                            */
    UINT32 wr_level_train_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               DDR5: Set to 1 to gate txfifo_wren during write
                               leveling[br] DDR4: Set to 1 to enable INTF to
                               generate a BL8 DQS output enable every 32 DCLK
                               for 1K DCLK duration, also gates txfifo_wren
                               during write leveling.[br] Note: For DDR4,
                               setting any sub-channel control bit will start
                               the write leveling logic in INTF for the entire
                               channel.
                            */
    UINT32 txclk_gate_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK for TX
                               repeaters. Default is disable clock gating.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_CTL0_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_CTL0_CH_1_MCIO_DDRIO_REG supported on:                               */
/*      SPRA0 (0x20013f04)                                                      */
/*      SPRB0 (0x20013f04)                                                      */
/*      SPRHBM (0x20013f04)                                                     */
/*      SPRC0 (0x20013f04)                                                      */
/*      SPRMCC (0x20013f04)                                                     */
/*      SPRUCC (0x20013f04)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Sub-Channel Control 0 Register[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_CTL0_CH_1_MCIO_DDRIO_REG 0x08023F04

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
    UINT32 ch_dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Channel Disable[br] - shut off all
                               analog/DCLK/DLL
                            */
    UINT32 wr_crc_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable write crc mode.[br] DDR4: extend BL8
                               write to BL10.[br] DDR5: extend BL16 write to
                               BL18.
                            */
    UINT32 rank_ovr_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, HW uses the rank_value_ovr field
                               to select rank parameters instead of rank number
                               from MC. It is used for training step that
                               require to read out rank specific infomration
                               without MC command.[br] Note: The current use
                               case is for DDR4 Write leveling training.
                            */
    UINT32 rank_value_ovr : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               When rank_ovr_en is set, this rank override
                               value is used by HW to select rank parameters
                               instead of rank number from MC. It is used for
                               training step that require to read out rank
                               specific infomration without MC command.[br]
                               Note: The current use case is for DDR4 Write
                               leveling training.
                            */
    UINT32 wr_level_train_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               DDR5: Set to 1 to gate txfifo_wren during write
                               leveling[br] DDR4: Set to 1 to enable INTF to
                               generate a BL8 DQS output enable every 32 DCLK
                               for 1K DCLK duration, also gates txfifo_wren
                               during write leveling.[br] Note: For DDR4,
                               setting any sub-channel control bit will start
                               the write leveling logic in INTF for the entire
                               channel.
                            */
    UINT32 txclk_gate_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK for TX
                               repeaters. Default is disable clock gating.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_CTL0_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_CTL0_CH_2_MCIO_DDRIO_REG supported on:                               */
/*      SPRA0 (0x20013f08)                                                      */
/*      SPRB0 (0x20013f08)                                                      */
/*      SPRHBM (0x20013f08)                                                     */
/*      SPRC0 (0x20013f08)                                                      */
/*      SPRMCC (0x20013f08)                                                     */
/*      SPRUCC (0x20013f08)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Sub-Channel Control 0 Register[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_CTL0_CH_2_MCIO_DDRIO_REG 0x08023F08

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
    UINT32 ch_dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Channel Disable[br] - shut off all
                               analog/DCLK/DLL
                            */
    UINT32 wr_crc_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable write crc mode.[br] DDR4: extend BL8
                               write to BL10.[br] DDR5: extend BL16 write to
                               BL18.
                            */
    UINT32 rank_ovr_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, HW uses the rank_value_ovr field
                               to select rank parameters instead of rank number
                               from MC. It is used for training step that
                               require to read out rank specific infomration
                               without MC command.[br] Note: The current use
                               case is for DDR4 Write leveling training.
                            */
    UINT32 rank_value_ovr : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               When rank_ovr_en is set, this rank override
                               value is used by HW to select rank parameters
                               instead of rank number from MC. It is used for
                               training step that require to read out rank
                               specific infomration without MC command.[br]
                               Note: The current use case is for DDR4 Write
                               leveling training.
                            */
    UINT32 wr_level_train_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               DDR5: Set to 1 to gate txfifo_wren during write
                               leveling[br] DDR4: Set to 1 to enable INTF to
                               generate a BL8 DQS output enable every 32 DCLK
                               for 1K DCLK duration, also gates txfifo_wren
                               during write leveling.[br] Note: For DDR4,
                               setting any sub-channel control bit will start
                               the write leveling logic in INTF for the entire
                               channel.
                            */
    UINT32 txclk_gate_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK for TX
                               repeaters. Default is disable clock gating.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_CTL0_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_CTL0_CH_3_MCIO_DDRIO_REG supported on:                               */
/*      SPRA0 (0x20013f0c)                                                      */
/*      SPRB0 (0x20013f0c)                                                      */
/*      SPRHBM (0x20013f0c)                                                     */
/*      SPRC0 (0x20013f0c)                                                      */
/*      SPRMCC (0x20013f0c)                                                     */
/*      SPRUCC (0x20013f0c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Per Sub-Channel Control 0 Register[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_CTL0_CH_3_MCIO_DDRIO_REG 0x08023F0C

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
    UINT32 ch_dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Channel Disable[br] - shut off all
                               analog/DCLK/DLL
                            */
    UINT32 wr_crc_en : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable write crc mode.[br] DDR4: extend BL8
                               write to BL10.[br] DDR5: extend BL16 write to
                               BL18.
                            */
    UINT32 rank_ovr_en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 1, HW uses the rank_value_ovr field
                               to select rank parameters instead of rank number
                               from MC. It is used for training step that
                               require to read out rank specific infomration
                               without MC command.[br] Note: The current use
                               case is for DDR4 Write leveling training.
                            */
    UINT32 rank_value_ovr : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               When rank_ovr_en is set, this rank override
                               value is used by HW to select rank parameters
                               instead of rank number from MC. It is used for
                               training step that require to read out rank
                               specific infomration without MC command.[br]
                               Note: The current use case is for DDR4 Write
                               leveling training.
                            */
    UINT32 wr_level_train_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               DDR5: Set to 1 to gate txfifo_wren during write
                               leveling[br] DDR4: Set to 1 to enable INTF to
                               generate a BL8 DQS output enable every 32 DCLK
                               for 1K DCLK duration, also gates txfifo_wren
                               during write leveling.[br] Note: For DDR4,
                               setting any sub-channel control bit will start
                               the write leveling logic in INTF for the entire
                               channel.
                            */
    UINT32 txclk_gate_en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate DCLK for TX
                               repeaters. Default is disable clock gating.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_CTL0_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f10)                                                      */
/*      SPRB0 (0x20013f10)                                                      */
/*      SPRHBM (0x20013f10)                                                     */
/*      SPRC0 (0x20013f10)                                                      */
/*      SPRMCC (0x20013f10)                                                     */
/*      SPRUCC (0x20013f10)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_REG 0x08023F10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 0 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat1 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 1 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat2 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 2 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat3 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 3 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT0_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT0_CH_1_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f14)                                                      */
/*      SPRB0 (0x20013f14)                                                      */
/*      SPRHBM (0x20013f14)                                                     */
/*      SPRC0 (0x20013f14)                                                      */
/*      SPRMCC (0x20013f14)                                                     */
/*      SPRUCC (0x20013f14)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT0_CH_1_MCIO_DDRIO_REG 0x08023F14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 0 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat1 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 1 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat2 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 2 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat3 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 3 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT0_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT0_CH_2_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f18)                                                      */
/*      SPRB0 (0x20013f18)                                                      */
/*      SPRHBM (0x20013f18)                                                     */
/*      SPRC0 (0x20013f18)                                                      */
/*      SPRMCC (0x20013f18)                                                     */
/*      SPRUCC (0x20013f18)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT0_CH_2_MCIO_DDRIO_REG 0x08023F18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 0 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat1 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 1 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat2 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 2 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat3 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 3 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT0_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT0_CH_3_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f1c)                                                      */
/*      SPRB0 (0x20013f1c)                                                      */
/*      SPRHBM (0x20013f1c)                                                     */
/*      SPRC0 (0x20013f1c)                                                      */
/*      SPRMCC (0x20013f1c)                                                     */
/*      SPRUCC (0x20013f1c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT0_CH_3_MCIO_DDRIO_REG 0x08023F1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat0 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 0 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat1 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 1 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat2 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 2 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat3 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 3 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT0_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f20)                                                      */
/*      SPRB0 (0x20013f20)                                                      */
/*      SPRHBM (0x20013f20)                                                     */
/*      SPRC0 (0x20013f20)                                                      */
/*      SPRMCC (0x20013f20)                                                     */
/*      SPRUCC (0x20013f20)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_REG 0x08023F20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat4 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 4 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat5 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 5 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat6 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 6 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat7 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 7 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT1_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT1_CH_1_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f24)                                                      */
/*      SPRB0 (0x20013f24)                                                      */
/*      SPRHBM (0x20013f24)                                                     */
/*      SPRC0 (0x20013f24)                                                      */
/*      SPRMCC (0x20013f24)                                                     */
/*      SPRUCC (0x20013f24)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT1_CH_1_MCIO_DDRIO_REG 0x08023F24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat4 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 4 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat5 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 5 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat6 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 6 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat7 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 7 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT1_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT1_CH_2_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f28)                                                      */
/*      SPRB0 (0x20013f28)                                                      */
/*      SPRHBM (0x20013f28)                                                     */
/*      SPRC0 (0x20013f28)                                                      */
/*      SPRMCC (0x20013f28)                                                     */
/*      SPRUCC (0x20013f28)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT1_CH_2_MCIO_DDRIO_REG 0x08023F28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat4 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 4 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat5 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 5 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat6 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 6 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat7 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 7 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT1_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_RT_LAT1_CH_3_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f2c)                                                      */
/*      SPRB0 (0x20013f2c)                                                      */
/*      SPRHBM (0x20013f2c)                                                     */
/*      SPRC0 (0x20013f2c)                                                      */
/*      SPRMCC (0x20013f2c)                                                     */
/*      SPRUCC (0x20013f2c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank roundtrip latency numbers[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_RT_LAT1_CH_3_MCIO_DDRIO_REG 0x08023F2C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rt_lat4 : 7;

                            /* Bits[6:0], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 4 (in DCLKs) */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat5 : 7;

                            /* Bits[14:8], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 5 (in DCLKs) */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat6 : 7;

                            /* Bits[22:16], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 6 (in DCLKs) */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rt_lat7 : 7;

                            /* Bits[30:24], Access Type=RW, default=0x00000000*/

                            /* Roundtrip Latency for rank 7 (in DCLKs) */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_RT_LAT1_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f30)                                                      */
/*      SPRB0 (0x20013f30)                                                      */
/*      SPRHBM (0x20013f30)                                                     */
/*      SPRC0 (0x20013f30)                                                      */
/*      SPRMCC (0x20013f30)                                                     */
/*      SPRUCC (0x20013f30)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank IO latency numbers. Read Command till read data on PAD.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_REG 0x08023F30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 0 (in DCLKs) */
    UINT32 io_lat1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 1 (in DCLKs) */
    UINT32 io_lat2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 2 (in DCLKs) */
    UINT32 io_lat3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 3 (in DCLKs) */
    UINT32 io_lat4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 4 (in DCLKs) */
    UINT32 io_lat5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 5 (in DCLKs) */
    UINT32 io_lat6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 6 (in DCLKs) */
    UINT32 io_lat7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 7 (in DCLKs) */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT0_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f34)                                                      */
/*      SPRB0 (0x20013f34)                                                      */
/*      SPRHBM (0x20013f34)                                                     */
/*      SPRC0 (0x20013f34)                                                      */
/*      SPRMCC (0x20013f34)                                                     */
/*      SPRUCC (0x20013f34)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank IO latency numbers. Read Command till read data on PAD.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_REG 0x08023F34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 0 (in DCLKs) */
    UINT32 io_lat1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 1 (in DCLKs) */
    UINT32 io_lat2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 2 (in DCLKs) */
    UINT32 io_lat3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 3 (in DCLKs) */
    UINT32 io_lat4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 4 (in DCLKs) */
    UINT32 io_lat5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 5 (in DCLKs) */
    UINT32 io_lat6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 6 (in DCLKs) */
    UINT32 io_lat7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 7 (in DCLKs) */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT0_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f38)                                                      */
/*      SPRB0 (0x20013f38)                                                      */
/*      SPRHBM (0x20013f38)                                                     */
/*      SPRC0 (0x20013f38)                                                      */
/*      SPRMCC (0x20013f38)                                                     */
/*      SPRUCC (0x20013f38)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank IO latency numbers. Read Command till read data on PAD.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_REG 0x08023F38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 0 (in DCLKs) */
    UINT32 io_lat1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 1 (in DCLKs) */
    UINT32 io_lat2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 2 (in DCLKs) */
    UINT32 io_lat3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 3 (in DCLKs) */
    UINT32 io_lat4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 4 (in DCLKs) */
    UINT32 io_lat5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 5 (in DCLKs) */
    UINT32 io_lat6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 6 (in DCLKs) */
    UINT32 io_lat7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 7 (in DCLKs) */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT0_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f3c)                                                      */
/*      SPRB0 (0x20013f3c)                                                      */
/*      SPRHBM (0x20013f3c)                                                     */
/*      SPRC0 (0x20013f3c)                                                      */
/*      SPRMCC (0x20013f3c)                                                     */
/*      SPRUCC (0x20013f3c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds per-rank IO latency numbers. Read Command till read data on PAD.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_REG 0x08023F3C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 0 (in DCLKs) */
    UINT32 io_lat1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 1 (in DCLKs) */
    UINT32 io_lat2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 2 (in DCLKs) */
    UINT32 io_lat3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 3 (in DCLKs) */
    UINT32 io_lat4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 4 (in DCLKs) */
    UINT32 io_lat5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 5 (in DCLKs) */
    UINT32 io_lat6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 6 (in DCLKs) */
    UINT32 io_lat7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* IO Latency for rank 7 (in DCLKs) */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT0_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f40)                                                      */
/*      SPRB0 (0x20013f40)                                                      */
/*      SPRHBM (0x20013f40)                                                     */
/*      SPRC0 (0x20013f40)                                                      */
/*      SPRMCC (0x20013f40)                                                     */
/*      SPRUCC (0x20013f40)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds overall IO Latency Compensation number.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_REG 0x08023F40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat_comp : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number to indicate additional read data IO
                               latency including inside the receiver (in
                               DCLKs)[br] Expected DDR4: 19[br] Expected DDR5:
                               20
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT1_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f44)                                                      */
/*      SPRB0 (0x20013f44)                                                      */
/*      SPRHBM (0x20013f44)                                                     */
/*      SPRC0 (0x20013f44)                                                      */
/*      SPRMCC (0x20013f44)                                                     */
/*      SPRUCC (0x20013f44)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds overall IO Latency Compensation number.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_REG 0x08023F44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat_comp : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number to indicate additional read data IO
                               latency including inside the receiver (in
                               DCLKs)[br] Expected DDR4: 19[br] Expected DDR5:
                               20
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT1_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f48)                                                      */
/*      SPRB0 (0x20013f48)                                                      */
/*      SPRHBM (0x20013f48)                                                     */
/*      SPRC0 (0x20013f48)                                                      */
/*      SPRMCC (0x20013f48)                                                     */
/*      SPRUCC (0x20013f48)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds overall IO Latency Compensation number.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_REG 0x08023F48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat_comp : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number to indicate additional read data IO
                               latency including inside the receiver (in
                               DCLKs)[br] Expected DDR4: 19[br] Expected DDR5:
                               20
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT1_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_REG supported on:                            */
/*      SPRA0 (0x20013f4c)                                                      */
/*      SPRB0 (0x20013f4c)                                                      */
/*      SPRHBM (0x20013f4c)                                                     */
/*      SPRC0 (0x20013f4c)                                                      */
/*      SPRMCC (0x20013f4c)                                                     */
/*      SPRUCC (0x20013f4c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds overall IO Latency Compensation number.[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_REG 0x08023F4C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 io_lat_comp : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Number to indicate additional read data IO
                               latency including inside the receiver (in
                               DCLKs)[br] Expected DDR4: 19[br] Expected DDR5:
                               20
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_IO_LAT1_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f50)                                                      */
/*      SPRB0 (0x20013f50)                                                      */
/*      SPRHBM (0x20013f50)                                                     */
/*      SPRC0 (0x20013f50)                                                      */
/*      SPRMCC (0x20013f50)                                                     */
/*      SPRUCC (0x20013f50)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters (t_cwl, t_cwl_adj, etc.) and control bits that need to programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_REG 0x08023F50

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddr_tcwl : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR4/DDR5 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR4 or DDR5, BIOS needs to program
                               this field for DDR4/DDR5 DIMM's t_cwl.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddr_tcwl_adj : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR4/DDR5 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number. The range is -8 to 7. The
                               total DDR4/DDR5 CAS write latency is the sum
                               this value with ddr_tcwl.
                            */
    UINT32 ddr_dqs_adj : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR4/DDR-5 DQS adjust
                               value in DCLK to deal with write leveling and
                               longer preambles length. The adjust range is -16
                               to 15.[br] DQS adjust = twcl adjust - preamble
                               length
                            */
    UINT32 ddrt_tcwl : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR-T/DDR-T2 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR-T or DDR-T2, BIOS needs to
                               program this field for DDR-T/DDR-T2 DIMM's
                               t_cwl.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrt_tcwl_adj : 4;

                            /* Bits[26:23], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR-T/DDR-T2 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number.The range is -8 to 7. The
                               total DDR-T/DDR-T2 CAS write latency is the sum
                               this value with ddrt_tcwl.
                            */
    UINT32 ddrt_dqs_adj : 5;

                            /* Bits[31:27], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR-T/DDR-T2 DQS
                               adjust value in DCLK to deal with write leveling
                               and longer preambles length. The adjust range is
                               -16 to 15.[br] DQS adjust = twcl adjust -
                               preamble length
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING0_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING0_CH_1_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f54)                                                      */
/*      SPRB0 (0x20013f54)                                                      */
/*      SPRHBM (0x20013f54)                                                     */
/*      SPRC0 (0x20013f54)                                                      */
/*      SPRMCC (0x20013f54)                                                     */
/*      SPRUCC (0x20013f54)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters (t_cwl, t_cwl_adj, etc.) and control bits that need to programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING0_CH_1_MCIO_DDRIO_REG 0x08023F54

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddr_tcwl : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR4/DDR5 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR4 or DDR5, BIOS needs to program
                               this field for DDR4/DDR5 DIMM's t_cwl.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddr_tcwl_adj : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR4/DDR5 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number. The range is -8 to 7. The
                               total DDR4/DDR5 CAS write latency is the sum
                               this value with ddr_tcwl.
                            */
    UINT32 ddr_dqs_adj : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR4/DDR-5 DQS adjust
                               value in DCLK to deal with write leveling and
                               longer preambles length. The adjust range is -16
                               to 15.[br] DQS adjust = twcl adjust - preamble
                               length
                            */
    UINT32 ddrt_tcwl : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR-T/DDR-T2 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR-T or DDR-T2, BIOS needs to
                               program this field for DDR-T/DDR-T2 DIMM's
                               t_cwl.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrt_tcwl_adj : 4;

                            /* Bits[26:23], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR-T/DDR-T2 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number.The range is -8 to 7. The
                               total DDR-T/DDR-T2 CAS write latency is the sum
                               this value with ddrt_tcwl.
                            */
    UINT32 ddrt_dqs_adj : 5;

                            /* Bits[31:27], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR-T/DDR-T2 DQS
                               adjust value in DCLK to deal with write leveling
                               and longer preambles length. The adjust range is
                               -16 to 15.[br] DQS adjust = twcl adjust -
                               preamble length
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING0_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING0_CH_2_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f58)                                                      */
/*      SPRB0 (0x20013f58)                                                      */
/*      SPRHBM (0x20013f58)                                                     */
/*      SPRC0 (0x20013f58)                                                      */
/*      SPRMCC (0x20013f58)                                                     */
/*      SPRUCC (0x20013f58)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters (t_cwl, t_cwl_adj, etc.) and control bits that need to programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING0_CH_2_MCIO_DDRIO_REG 0x08023F58

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddr_tcwl : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR4/DDR5 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR4 or DDR5, BIOS needs to program
                               this field for DDR4/DDR5 DIMM's t_cwl.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddr_tcwl_adj : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR4/DDR5 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number. The range is -8 to 7. The
                               total DDR4/DDR5 CAS write latency is the sum
                               this value with ddr_tcwl.
                            */
    UINT32 ddr_dqs_adj : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR4/DDR-5 DQS adjust
                               value in DCLK to deal with write leveling and
                               longer preambles length. The adjust range is -16
                               to 15.[br] DQS adjust = twcl adjust - preamble
                               length
                            */
    UINT32 ddrt_tcwl : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR-T/DDR-T2 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR-T or DDR-T2, BIOS needs to
                               program this field for DDR-T/DDR-T2 DIMM's
                               t_cwl.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrt_tcwl_adj : 4;

                            /* Bits[26:23], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR-T/DDR-T2 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number.The range is -8 to 7. The
                               total DDR-T/DDR-T2 CAS write latency is the sum
                               this value with ddrt_tcwl.
                            */
    UINT32 ddrt_dqs_adj : 5;

                            /* Bits[31:27], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR-T/DDR-T2 DQS
                               adjust value in DCLK to deal with write leveling
                               and longer preambles length. The adjust range is
                               -16 to 15.[br] DQS adjust = twcl adjust -
                               preamble length
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING0_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING0_CH_3_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f5c)                                                      */
/*      SPRB0 (0x20013f5c)                                                      */
/*      SPRHBM (0x20013f5c)                                                     */
/*      SPRC0 (0x20013f5c)                                                      */
/*      SPRMCC (0x20013f5c)                                                     */
/*      SPRUCC (0x20013f5c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters (t_cwl, t_cwl_adj, etc.) and control bits that need to programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING0_CH_3_MCIO_DDRIO_REG 0x08023F5C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddr_tcwl : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR4/DDR5 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR4 or DDR5, BIOS needs to program
                               this field for DDR4/DDR5 DIMM's t_cwl.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddr_tcwl_adj : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR4/DDR5 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number. The range is -8 to 7. The
                               total DDR4/DDR5 CAS write latency is the sum
                               this value with ddr_tcwl.
                            */
    UINT32 ddr_dqs_adj : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR4/DDR-5 DQS adjust
                               value in DCLK to deal with write leveling and
                               longer preambles length. The adjust range is -16
                               to 15.[br] DQS adjust = twcl adjust - preamble
                               length
                            */
    UINT32 ddrt_tcwl : 6;

                            /* Bits[21:16], Access Type=RW, default=0x00000000*/

                            /*
                               Write CAS Latency used for DDR-T/DDR-T2 DQ/DQS
                               OutputEnable generation. If any DIMM in a
                               channel is DDR-T or DDR-T2, BIOS needs to
                               program this field for DDR-T/DDR-T2 DIMM's
                               t_cwl.
                            */
    UINT32 rsvd_22 : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ddrt_tcwl_adj : 4;

                            /* Bits[26:23], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines DDR-T/DDR-T2 TCWL adjust
                               value (in DCLK) to deal with write logic delay.
                               It is a signed number.The range is -8 to 7. The
                               total DDR-T/DDR-T2 CAS write latency is the sum
                               this value with ddrt_tcwl.
                            */
    UINT32 ddrt_dqs_adj : 5;

                            /* Bits[31:27], Access Type=RW, default=0x00000000*/

                            /*
                               This register defines the DDR-T/DDR-T2 DQS
                               adjust value in DCLK to deal with write leveling
                               and longer preambles length. The adjust range is
                               -16 to 15.[br] DQS adjust = twcl adjust -
                               preamble length
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING0_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f60)                                                      */
/*      SPRB0 (0x20013f60)                                                      */
/*      SPRHBM (0x20013f60)                                                     */
/*      SPRC0 (0x20013f60)                                                      */
/*      SPRMCC (0x20013f60)                                                     */
/*      SPRUCC (0x20013f60)                                                     */
/* Register default value on SPRA0: 0x00000018                                  */
/* Register default value on SPRB0: 0x00000018                                  */
/* Register default value on SPRHBM: 0x00000018                                 */
/* Register default value on SPRC0: 0x00000018                                  */
/* Register default value on SPRMCC: 0x00000018                                 */
/* Register default value on SPRUCC: 0x00000018                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters and control bits that need to be programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_REG 0x08023F60

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rddata_valid_early : 5;

                            /* Bits[5:1], Access Type=RW, default=0x0000000C*/

                            /*
                               This field controls the number of DCLK cycles
                               that the rddata_valid is sent early to MC.
                               Expected program the same value for all sub-ch.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_analog_en_early : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that rxaen is asserted before rcven. Used to
                               enable sense amp and odt circuits early.[br]
                               Note: Expected program the same value for all
                               sub-ch. Dependent on preable and frequency.
                               Supported range is 0 to 6.[br] DDR4: 0-2
                               DCLK[br] DDR5: 0-3 DCLK[br]
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idle_cnt_valid_early : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that idle_cnt_valid is asserted before the
                               failing edge of rcven. Used by dqsdeskew to
                               account for interamble scenerios. A CR value of
                               2 is the max support for DDR4[br] 0: 1 Dclk
                               before rcven failing edge[br] 1: 2 Dclk before
                               rcven failing edge[br] 2: 3 Dclk before rcven
                               failing edge[br] 3: 4 Dclk before rcven failing
                               edge
                            */
    UINT32 rsvd_14 : 10;

                            /* Bits[23:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcwl_para_sel : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Per rank control to indicate which TCWL
                               parameter is slected[br] 0: DDR 4 or DDR 5 [br]
                               1: DDR-T or DDR-T2 [br] Used by delay logic to
                               determine which set of timing CSRs to use.
                               Unpopulated/unused ranks can be programmed
                               arbitrarliy since they will never be accessed.
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING1_CH_0_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING1_CH_1_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f64)                                                      */
/*      SPRB0 (0x20013f64)                                                      */
/*      SPRHBM (0x20013f64)                                                     */
/*      SPRC0 (0x20013f64)                                                      */
/*      SPRMCC (0x20013f64)                                                     */
/*      SPRUCC (0x20013f64)                                                     */
/* Register default value on SPRA0: 0x00000018                                  */
/* Register default value on SPRB0: 0x00000018                                  */
/* Register default value on SPRHBM: 0x00000018                                 */
/* Register default value on SPRC0: 0x00000018                                  */
/* Register default value on SPRMCC: 0x00000018                                 */
/* Register default value on SPRUCC: 0x00000018                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters and control bits that need to be programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING1_CH_1_MCIO_DDRIO_REG 0x08023F64

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rddata_valid_early : 5;

                            /* Bits[5:1], Access Type=RW, default=0x0000000C*/

                            /*
                               This field controls the number of DCLK cycles
                               that the rddata_valid is sent early to MC.
                               Expected program the same value for all sub-ch.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_analog_en_early : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that rxaen is asserted before rcven. Used to
                               enable sense amp and odt circuits early.[br]
                               Note: Expected program the same value for all
                               sub-ch. Dependent on preable and frequency.
                               Supported range is 0 to 6.[br] DDR4: 0-2
                               DCLK[br] DDR5: 0-3 DCLK[br]
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idle_cnt_valid_early : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that idle_cnt_valid is asserted before the
                               failing edge of rcven. Used by dqsdeskew to
                               account for interamble scenerios. A CR value of
                               2 is the max support for DDR4[br] 0: 1 Dclk
                               before rcven failing edge[br] 1: 2 Dclk before
                               rcven failing edge[br] 2: 3 Dclk before rcven
                               failing edge[br] 3: 4 Dclk before rcven failing
                               edge
                            */
    UINT32 rsvd_14 : 10;

                            /* Bits[23:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcwl_para_sel : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Per rank control to indicate which TCWL
                               parameter is slected[br] 0: DDR 4 or DDR 5 [br]
                               1: DDR-T or DDR-T2 [br] Used by delay logic to
                               determine which set of timing CSRs to use.
                               Unpopulated/unused ranks can be programmed
                               arbitrarliy since they will never be accessed.
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING1_CH_1_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING1_CH_2_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f68)                                                      */
/*      SPRB0 (0x20013f68)                                                      */
/*      SPRHBM (0x20013f68)                                                     */
/*      SPRC0 (0x20013f68)                                                      */
/*      SPRMCC (0x20013f68)                                                     */
/*      SPRUCC (0x20013f68)                                                     */
/* Register default value on SPRA0: 0x00000018                                  */
/* Register default value on SPRB0: 0x00000018                                  */
/* Register default value on SPRHBM: 0x00000018                                 */
/* Register default value on SPRC0: 0x00000018                                  */
/* Register default value on SPRMCC: 0x00000018                                 */
/* Register default value on SPRUCC: 0x00000018                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters and control bits that need to be programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING1_CH_2_MCIO_DDRIO_REG 0x08023F68

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rddata_valid_early : 5;

                            /* Bits[5:1], Access Type=RW, default=0x0000000C*/

                            /*
                               This field controls the number of DCLK cycles
                               that the rddata_valid is sent early to MC.
                               Expected program the same value for all sub-ch.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_analog_en_early : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that rxaen is asserted before rcven. Used to
                               enable sense amp and odt circuits early.[br]
                               Note: Expected program the same value for all
                               sub-ch. Dependent on preable and frequency.
                               Supported range is 0 to 6.[br] DDR4: 0-2
                               DCLK[br] DDR5: 0-3 DCLK[br]
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idle_cnt_valid_early : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that idle_cnt_valid is asserted before the
                               failing edge of rcven. Used by dqsdeskew to
                               account for interamble scenerios. A CR value of
                               2 is the max support for DDR4[br] 0: 1 Dclk
                               before rcven failing edge[br] 1: 2 Dclk before
                               rcven failing edge[br] 2: 3 Dclk before rcven
                               failing edge[br] 3: 4 Dclk before rcven failing
                               edge
                            */
    UINT32 rsvd_14 : 10;

                            /* Bits[23:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcwl_para_sel : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Per rank control to indicate which TCWL
                               parameter is slected[br] 0: DDR 4 or DDR 5 [br]
                               1: DDR-T or DDR-T2 [br] Used by delay logic to
                               determine which set of timing CSRs to use.
                               Unpopulated/unused ranks can be programmed
                               arbitrarliy since they will never be accessed.
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING1_CH_2_MCIO_DDRIO_STRUCT;

/* DDRINTF_DATA_TIMING1_CH_3_MCIO_DDRIO_REG supported on:                       */
/*      SPRA0 (0x20013f6c)                                                      */
/*      SPRB0 (0x20013f6c)                                                      */
/*      SPRHBM (0x20013f6c)                                                     */
/*      SPRC0 (0x20013f6c)                                                      */
/*      SPRMCC (0x20013f6c)                                                     */
/*      SPRUCC (0x20013f6c)                                                     */
/* Register default value on SPRA0: 0x00000018                                  */
/* Register default value on SPRB0: 0x00000018                                  */
/* Register default value on SPRHBM: 0x00000018                                 */
/* Register default value on SPRC0: 0x00000018                                  */
/* Register default value on SPRMCC: 0x00000018                                 */
/* Register default value on SPRUCC: 0x00000018                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds DATA related timing parameters and control bits that need to be programmed by BIOS[br]
                 reg0 : ch0a[br]
                 reg1 : ch0b[br]
                 reg2 : ch1a[br]
                 reg3 : ch1b
*/


#define DDRINTF_DATA_TIMING1_CH_3_MCIO_DDRIO_REG 0x08023F6C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rddata_valid_early : 5;

                            /* Bits[5:1], Access Type=RW, default=0x0000000C*/

                            /*
                               This field controls the number of DCLK cycles
                               that the rddata_valid is sent early to MC.
                               Expected program the same value for all sub-ch.
                            */
    UINT32 rsvd : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_analog_en_early : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that rxaen is asserted before rcven. Used to
                               enable sense amp and odt circuits early.[br]
                               Note: Expected program the same value for all
                               sub-ch. Dependent on preable and frequency.
                               Supported range is 0 to 6.[br] DDR4: 0-2
                               DCLK[br] DDR5: 0-3 DCLK[br]
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 idle_cnt_valid_early : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /*
                               This field controls the number of DCLK cycles
                               that idle_cnt_valid is asserted before the
                               failing edge of rcven. Used by dqsdeskew to
                               account for interamble scenerios. A CR value of
                               2 is the max support for DDR4[br] 0: 1 Dclk
                               before rcven failing edge[br] 1: 2 Dclk before
                               rcven failing edge[br] 2: 3 Dclk before rcven
                               failing edge[br] 3: 4 Dclk before rcven failing
                               edge
                            */
    UINT32 rsvd_14 : 10;

                            /* Bits[23:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tcwl_para_sel : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Per rank control to indicate which TCWL
                               parameter is slected[br] 0: DDR 4 or DDR 5 [br]
                               1: DDR-T or DDR-T2 [br] Used by delay logic to
                               determine which set of timing CSRs to use.
                               Unpopulated/unused ranks can be programmed
                               arbitrarliy since they will never be accessed.
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_DATA_TIMING1_CH_3_MCIO_DDRIO_STRUCT;

/* DDRINTF_CTL0_MCIO_DDRIO_REG supported on:                                    */
/*      SPRA0 (0x20013f80)                                                      */
/*      SPRB0 (0x20013f80)                                                      */
/*      SPRHBM (0x20013f80)                                                     */
/*      SPRC0 (0x20013f80)                                                      */
/*      SPRMCC (0x20013f80)                                                     */
/*      SPRUCC (0x20013f80)                                                     */
/* Register default value on SPRA0: 0x52400000                                  */
/* Register default value on SPRB0: 0x52400000                                  */
/* Register default value on SPRHBM: 0x52400000                                 */
/* Register default value on SPRC0: 0x52400000                                  */
/* Register default value on SPRMCC: 0x52400000                                 */
/* Register default value on SPRUCC: 0x52400000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* Control 0 Register
*/


#define DDRINTF_CTL0_MCIO_DDRIO_REG 0x08023F80

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sr_dis : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to dsiable SPID LPMODE self refresh. */
    UINT32 cke_pwr_dn_dis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Set to 1 to dsiable SPID LPMODE CKE power down. */
    UINT32 clk_park0_dis : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable PM FSM to park clock to 0 in
                               PM1 and PM2 state.
                            */
    UINT32 vsshi_off_dis : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable PM FSM to turn off VSSHI in
                               PM2 state.
                            */
    UINT32 ldo_off_dis : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable PM FSM to turn off LDO in
                               PM2 state.
                            */
    UINT32 dll_off_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable PM FSM to turn off DLL in
                               PM2 state.
                            */
    UINT32 csr_clk_gate_en : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable HW to gate CSR CLK. Default
                               is disable clock gating. [br] Note: Clock gating
                               must be disable during any training steps. BIOS
                               can enable clock gating for power saving after
                               all the training steps are done.
                            */
    UINT32 force_sr : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Force Channel in Self Refresh. Normally asserted
                               based on LPMODE
                            */
    UINT32 force_cke_pwr_dn : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Force Channel in CKE power down. Normally
                               asserted based on LPMODE
                            */
    UINT32 force_clk_park0 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Force the clock to park 0 (both P/N 0). Normally
                               asserted in PM1 and PM2 state.
                            */
    UINT32 rsvd : 1;

                            /* Bits[10:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ch0_train_rst : 1;

                            /* Bits[11:11], Access Type=RW/V, default=0x00000000*/

                            /*
                               Channel 0 SW set, HW clear train reset. This
                               train reset is logical ORed with MC CH0 train
                               reset (level). The intend use case is for BIOS
                               to deassert the MC train reset first and then
                               use this train reset for each training step to
                               reduce the number of SW program step
                            */
    UINT32 ch1_train_rst : 1;

                            /* Bits[12:12], Access Type=RW/V, default=0x00000000*/

                            /*
                               Channel 1 SW set, HW clear train reset. This
                               train reset is logical ORed with MC CH1 train
                               reset (level). The intend use case is for BIOS
                               to deassert the MC train reset first and then
                               use this train reset for each training step to
                               reduce the number of SW program step
                            */
    UINT32 lp_idle_dll_off_dis : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable SPID LPMODE from turning off
                               DATA FUB DLL in SLOW IDLE state.
                            */
    UINT32 lp_pd_dll_off_dis : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable SPID LPMODE from turning off
                               DATA FUB DLL in SLOW CKE POWER DOWN state.
                            */
    UINT32 lp_sr_dll_off_dis : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to disable SPID LPMODE from turning off
                               DATA FUB DLL in HIGH LATENCY SELF REFRESH state.
                            */
    UINT32 lane_reversal_en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Set to 1 to enable data lane reversal to ease
                               board routing.
                            */
    UINT32 elr_dimm : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* Set this bit to 1 for DDR5 ELR DIMM */
    UINT32 ddr4 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               For SPR, 2 Channel DDR-MC sub-system. It can
                               either operate in DDR4/DDR-T or DDR5/DDR-T2. It
                               doesn't support DDR4/DDR-T mix with
                               DDR5/DDR-T2.[br] Setting this bit to 1 indicate
                               it is operating in DDR/DDR-T. [br] 0 inidate it
                               is operating in DDR5/DDR-T2. [br] INTF use this
                               information for the MC input muxing.
                            */
    UINT32 ca7_en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               0: 14 CA bits[br] 1: 7 CA bits[br] It is used by
                               INTF for MC input muxing.
                            */
    UINT32 force_comp_update : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               DDR Initiated CompUpdate via auto-clearing CSR
                               write. Forces Comp Update pulse to be sent to
                               all FUBs bypassing the SPID Comp FSM.
                            */
    UINT32 force_comp_begin : 1;

                            /* Bits[21:21], Access Type=RW/V, default=0x00000000*/

                            /*
                               DDR Initiated CompBegin via auto-clearing
                               CompComplete, allows BIOS to poll this bit for
                               CompComplete indication. Forces Comp Begin to be
                               sent to COMP bypassing the SPID Comp FSM.
                            */
    UINT32 pm_train_rst_en : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Enable PM state machine to generate train reset
                               when in PM1 and PM2 states.
                            */
    UINT32 rsvd_23 : 2;

                            /* Bits[24:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pm_wait_comp_finish : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Enable PM FSM to wait for periodic Comp FSM to
                               finish before return a PM ACK back to MC during
                               a low power state transtion sequence.
                            */
    UINT32 same_cycle_par_en : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               0: Send parity 1 dclk cycle after command.
                               Parity for DDR4 RDIMM is always a cycle late.
                               [br] 1: Send parity same cycle as commnad. Used
                               for DDR5 RDIMM and for training. DDR4 UDIMM
                               parity needs to be in the same cycle.
                            */
    UINT32 comp_upd_quiet_time : 4;

                            /* Bits[30:27], Access Type=RW, default=0x0000000A*/

                            /*
                               Specifies the number of Dclk cycles that the
                               spid interface needs to be quiet, in order for
                               comp update to finish (see by the farthest Data
                               FUB). On the SPID interface term, it means after
                               MC asserted Quiet Time Grant to DDR, how many
                               DCLK cycles DDR needs to wait to deassert the
                               Quiet Time Request to MC.
                            */
    UINT32 tx_retrain_time : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the number of Dclk cycles required for
                               TX retraining values to be applied by the Data
                               FUBs. On the spid interface, indicates the
                               amount of time required to wait before
                               acknowledging ispid_phy_update_req.[br] 0: 16
                               Dclks[br] 1: 32 Dclks
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_CTL0_MCIO_DDRIO_STRUCT;

/* DDRINTF_PM_TIMING0_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x20013f84)                                                      */
/*      SPRB0 (0x20013f84)                                                      */
/*      SPRHBM (0x20013f84)                                                     */
/*      SPRC0 (0x20013f84)                                                      */
/*      SPRMCC (0x20013f84)                                                     */
/*      SPRUCC (0x20013f84)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds PM FSM pre-delay timings that are applied before taking a PM action
*/


#define DDRINTF_PM_TIMING0_MCIO_DDRIO_REG 0x08023F84

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pm0to1_pre_dly : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This delay controls how long to wait between
                               receiving PM1 or PM2 request and actually moving
                               PM FSM from PM0 to PM1. This delay value is in
                               units of 64 Dclks.[br] Note: Time wait before
                               parking DIMM Clock to 0.
                            */
    UINT32 pm1to2_pre_dly : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               This delay controls how long to wait between
                               receiving PM2 request and actually moving PM FSM
                               from PM1 to PM2. This delay value is in units of
                               64 Dclks.[br] Note: Time wait before turning off
                               DLL, LDO, and VSSHI
                            */
    UINT32 pm1to0_pre_dly : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               This delay controls how long to wait between
                               receiving PM0 request and actually moving PM FSM
                               from PM1 to PM0. This delay value is in units of
                               64 Dclks.[br] Note : Time to wait before un-
                               parking DIMM clcok.
                            */
    UINT32 pm2to1_pre_dly : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               This delay controls how long to wait between
                               receiving PM1 or PM0 request and actually moving
                               PM FSM from PM2 to PM1. This delay value is in
                               units of 64 Dclks.[br] Note: Time to wait before
                               turning on DLL, LDO, and VSSHI.
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_PM_TIMING0_MCIO_DDRIO_STRUCT;

/* DDRINTF_PM_TIMING1_MCIO_DDRIO_REG supported on:                              */
/*      SPRA0 (0x20013f88)                                                      */
/*      SPRB0 (0x20013f88)                                                      */
/*      SPRHBM (0x20013f88)                                                     */
/*      SPRC0 (0x20013f88)                                                      */
/*      SPRMCC (0x20013f88)                                                     */
/*      SPRUCC (0x20013f88)                                                     */
/* Register default value on SPRA0: 0x93010101                                  */
/* Register default value on SPRB0: 0x93010101                                  */
/* Register default value on SPRHBM: 0x93010101                                 */
/* Register default value on SPRC0: 0x93010101                                  */
/* Register default value on SPRMCC: 0x93010101                                 */
/* Register default value on SPRUCC: 0x93010101                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds PM FSM post-delay timings that are applied after taking a PM action (before responding with ACK or taking subsequent action)
*/


#define DDRINTF_PM_TIMING1_MCIO_DDRIO_REG 0x08023F88

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pm0to1_post_dly : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000001*/

                            /*
                               After trasiton from PM0 to PM1, this delay
                               controls how long to wait for any functions to
                               be turned off in PM1 state and responding with
                               PM ACK to MC (if request was for PM1) or moving
                               to PM2 state (if request was for PM2). This
                               delay value is in units of 64 Dclks.[br] Note:
                               Time to wait to park the DIMM clock to 0.
                            */
    UINT32 pm1to2_post_dly : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000001*/

                            /*
                               After trasiton from PM1 to PM2, this delay
                               controls how long to wait for any functions to
                               be turned off in PM2 state and responding with
                               PM ACK to MC. This delay value is in units of 64
                               Dclks.[br] Note: Analog componenets wait to turn
                               off are DLL, LDO, and VSSHI.
                            */
    UINT32 pm1to0_post_dly : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000001*/

                            /*
                               After trasiton from PM1 to PM0, this delay
                               controls how long to wait for any functions to
                               be turned on in PM0 state and responding with PM
                               ACK to MC. This delay value is in units of 64
                               Dclks.[br] Note: Time to wait to un-parking DIMM
                               clock.
                            */
    UINT32 pm2to1_post_dly : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000093*/

                            /*
                               After trasiton from PM2 to PM1, this delay
                               controls how long to wait for any functions to
                               be turned on in PM1 state and responding with PM
                               ACK to MC (if request was for PM1) or moving to
                               PM0 state (if request was for PM0). This delay
                               value is in units of 64 Dclks.[br] Note: Analog
                               componenets wait to turn on are DLL, LDO, and
                               VSSHI.
                            */

  } Bits;
  UINT32 Data;

} DDRINTF_PM_TIMING1_MCIO_DDRIO_STRUCT;

/* DDRINTF_STATUS0_MCIO_DDRIO_REG supported on:                                 */
/*      SPRA0 (0x20013f8c)                                                      */
/*      SPRB0 (0x20013f8c)                                                      */
/*      SPRHBM (0x20013f8c)                                                     */
/*      SPRC0 (0x20013f8c)                                                      */
/*      SPRMCC (0x20013f8c)                                                     */
/*      SPRUCC (0x20013f8c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRB0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRHBM Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRC0 Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRMCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* SPRUCC Register File:    sprsp_top/ddrphy_top[0]/ddrintf_type/ddrintf/ddrintf_mem*/
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRB0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRHBM Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRC0 Security PolicyGroup: MEMIO_BOOT                                       */
/* SPRMCC Security PolicyGroup: MEMIO_BOOT                                      */
/* SPRUCC Security PolicyGroup: MEMIO_BOOT                                      */
/* This CR holds read only status registers for PM FSMs, Update FSMs, and LPMODE
*/


#define DDRINTF_STATUS0_MCIO_DDRIO_REG 0x08023F8C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ch0_pm_fsm_status : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel 0 PM FSM current state:[br] 0 - PM0
                               Idle[br] 1 - PM0 Delay[br] 2 - PM1 Delay[br] 3 -
                               PM2 Delay[br] 4 - PM2 Idle
                            */
    UINT32 ch1_pm_fsm_status : 3;

                            /* Bits[5:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel 1 PM FSM current state:[br] 0 - PM0
                               Idle[br] 1 - PM0 Delay[br] 2 - PM1 Delay[br] 3 -
                               PM2 Delay[br] 4 - PM2 Idle
                            */
    UINT32 ch0_update_fsm_status : 3;

                            /* Bits[8:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel 0 Update FSM current state:[br] 0 -
                               Idle[br] 1 - COMP Begin[br] 2 - Bus Quiet Time
                               Request[br] 3 - COMP Update[br] 4 - TX
                               Retraining
                            */
    UINT32 ch1_update_fsm_status : 3;

                            /* Bits[11:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel 1 Update FSM current state:[br] 0 -
                               Idle[br] 1 - COMP Begin[br] 2 - Bus Quiet Time
                               Request[br] 3 - COMP Update[br] 4 - TX
                               Retraining
                            */
    UINT32 ch0_lpmode_status : 3;

                            /* Bits[14:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel 0 LPMODE current state:[br] 0 - Active
                               (MC sending commands), Unused/No Action[br] 1 -
                               Idle (MC not sending commands), Unused/No
                               Action[br] 2 - Idle, DLL Off (MC not sending
                               commands)[br] 3 - CKE Powerdown (all CKE are
                               low)[br] 4 - Unused/Reserved[br] 5 - CKE
                               Powerdown, DLL Off (all CKE are low)[br] 6 -
                               SelfRefresh (all ranks in SR)[br] 7 -
                               SelfRefresh, DLL Off (all ranks in SR)
                            */
    UINT32 rsvd : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ch1_lpmode_status : 3;

                            /* Bits[18:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               Channel 1 LPMODE current state:[br] 0 - Active
                               (MC sending commands), Unused/No Action[br] 1 -
                               Idle (MC not sending commands), Unused/No
                               Action[br] 2 - Idle, DLL Off (MC not sending
                               commands)[br] 3 - CKE Powerdown (all CKE are
                               low)[br] 4 - Unused/Reserved[br] 5 - CKE
                               Powerdown, DLL Off (all CKE are low)[br] 6 -
                               SelfRefresh (all ranks in SR)[br] 7 -
                               SelfRefresh, DLL Off (all ranks in SR)
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRINTF_STATUS0_MCIO_DDRIO_STRUCT;






#endif /* _MCIO_DDRIO_h */
