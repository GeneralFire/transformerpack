/** @file
  Internal header file for PCH IEH RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation.

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

#ifndef _PCHS_IEH_H_
#define _PCHS_IEH_H_

#define V_CDF_PCH_IEH_CFG_DEVICE_ID     0x0998

#define PCI_DEVICE_NUMBER_PCH_IEH       0x1D
#define PCI_FUNCTION_NUMBER_PCH_IEH     0x0

//
//update register according head files later
//
#define R_PCH_IEH_IEHTYPE       0x26C
#define   B_PCH_IEH_IEHTYPE     0x0f
#define   V_PCH_IEH_GLOBAL      0x00
#define   V_PCH_IEH_NORTH       0x01
#define   V_PCH_IEH_SOUTH       0x02
#define   B_PCH_IEH_BUSNUM      0xff00

#define R_PCH_IEH_GCOERRSTS   0x200
#define R_PCH_IEH_GCOFERRSTS  0x204
#define R_PCH_IEH_GCONERRSTS  0x208
#define R_PCH_IEH_GNFERRSTS   0x210
#define R_PCH_IEH_GNFFERRSTS  0x214
#define R_PCH_IEH_GNFNERRSTS  0x218
#define R_PCH_IEH_GFAERRSTS   0x220
#define R_PCH_IEH_GFAFERRSTS  0x224
#define R_PCH_IEH_GFANERRSTS  0x228
#define R_PCH_IEH_GCOERRMSK   0x230
#define R_PCH_IEH_GNFERRMSK   0x234
#define R_PCH_IEH_GFAERRMSK   0x238
#define R_PCH_IEH_GTIME       0x240
#define R_PCH_IEH_GCOFERRTIME 0x248
#define R_PCH_IEH_GNFFERRTIME 0x250
#define R_PCH_IEH_GFAFERRTIME 0x258
#define R_PCH_IEH_GSYSEVTSTS  0x260
#define   IEH_GSYSEVTSTS_MASK   7
#define   IEH_CORRECT_ERROR     BIT0
#define   IEH_NON_FATAL_ERROR   BIT1
#define   IEH_FATAL_ERROR       BIT2
#define R_PCH_IEH_GSYSEVTCTL  0x264
#define   CORR_SYS_ENABLE       BIT0
#define   NON_FATAL_SYS_ENABLE  BIT1
#define   FATAL_SYS_ENABLE      BIT2
#define R_PCH_IEH_GSYSEVTMAP  0x268
#define   GENERATE_SMI          0x01

#define R_PCH_IEH_ERRPINCTRL  0x270
#define   ASSERT_ERROR_PIN    0x02
#define R_PCH_IEH_ERRPINSTS   0x274
#define   IEH_ERRPINSTS_MASK      0x07
#define R_PCH_IEH_ERRPINDATA  0x278
#define R_PCH_IEH_BITMAP      0x27c
#define R_PCH_IEH_DEVCOUNT    0x2fc
#define    IEH_DEVCOUNT_MASK      0x1f

#define R_PCH_IEH_LERRUNCSTS    0x280
#define R_PCH_IEH_LFERRUNCSTS   0x284
#define R_PCH_IEH_LNERRUNCSTS   0x288
#define R_PCH_IEH_LERRUNCMSK    0x28c
#define R_PCH_IEH_LERRUNCSEV    0x290

#define R_PCH_IEH_LERRCORSTS    0x294
#define R_PCH_IEH_LFERRCORSTS   0x298
#define R_PCH_IEH_LNERRCORSTS   0x29c
#define R_PCH_IEH_LERRCORMSK    0x2A0
#define R_PCH_IEH_LCORERRCNTSEL 0x2A4

#define R_PCH_IEH_DEVFUN_BASE 0x300

#endif //_PCHS_IEH_H_
