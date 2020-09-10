
/** @file
  CWV_DDRIO_DAT5_CH1.h

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


#ifndef _CWV_DDRIO_DAT5_CH1_h
#define _CWV_DDRIO_DAT5_CH1_h
#include <DataTypes.h>













































/* DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT5_CH1_REG supported on: */
/*      CWV (0x408076bc)                                                        */
/* Register default value on CWV: 0x03FFFFFF                                    */
/* CWV Register File:    ddrphy_ddrt_csr/ddrphy_ddrt_top/ddrphy_ddrdata/data5ch1/sb*/
/* Struct generated from CWV BDF: 8_0_7                                         */
/* CWV Security PolicyGroup: DDRPHY_GATED_P0                                    */
/* Timing and Voltage control for writing one data byte on RankX
*/


#define DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT5_CH1_REG 0x1BA416BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdqdelay : 11;

                            /* Bits[10:0], Access Type=RW, default=0x000007FF*/

                            /*
                               Write DQ timing control. Unsigned number from 0
                               to 15 tQCK with step size of UI/64
                            */
    UINT32 txdqsdelay : 11;

                            /* Bits[21:11], Access Type=RW, default=0x000007FF*/

                            /*
                               Write DQS timing control. Unsigned number from 0
                               to 15 tQCK with step size of UI/64
                            */
    UINT32 txequalization : 4;

                            /* Bits[25:22], Access Type=RW, default=0x0000000F*/

                            /*
                               # of static legs for de-emphasis: 10 levels. (0:
                               0 Static legs/10 DeEmph legs, 1: 1 Static/9
                               DeEmph leg, 2: 2 Static/8 DeEmph Leg, ... 10: 10
                               Static legs, 11-15: rsvd)
                            */
    UINT32 spare : 6;

                            /* Bits[31:26], Access Type=RO, default=0x00000000*/

                            /* spares */

  } Bits;
  UINT32 Data;

} DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT5_CH1_STRUCT;










































#endif /* _CWV_DDRIO_DAT5_CH1_h */
