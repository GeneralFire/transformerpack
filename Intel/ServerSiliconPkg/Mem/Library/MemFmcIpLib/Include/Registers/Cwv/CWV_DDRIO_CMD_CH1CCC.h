
/** @file
  CWV_DDRIO_CMD_CH1CCC.h

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


#ifndef _CWV_DDRIO_CMD_CH1CCC_h
#define _CWV_DDRIO_CMD_CH1CCC_h
#include <DataTypes.h>








/* DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG supported on:*/
/*      CWV (0x40807718)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch1ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* CCC Pi Settings
*/


#define DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG 0x1BA43718

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

} DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_STRUCT;

































/* DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG supported on:  */
/*      CWV (0x40807798)                                                        */
/* Register default value on CWV: 0x20202020                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_command/ch1ccc/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* RX vref controls.
*/


#define DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_REG 0x1BA43798

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

} DDRCCC_CR_DDRCRCCCVREF2_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_STRUCT;




















#endif /* _CWV_DDRIO_CMD_CH1CCC_h */
