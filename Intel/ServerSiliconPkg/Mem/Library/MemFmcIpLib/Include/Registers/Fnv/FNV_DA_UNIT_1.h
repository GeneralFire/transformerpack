
/** @file
  FNV_DA_UNIT_1.h

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


#ifndef _FNV_DA_UNIT_1_h
#define _FNV_DA_UNIT_1_h
#include "DataTypes.h"

/* DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG supported on:                      */
/*      BWV (0x40003800)                                                        */
/* Register default value on BWV: 0x00000000                                    */
/* BWV Register File:    0_0_3_BWV                                              */
/* Struct generated from BWV BDF: 0_0_3                                         */
/* BWV Security PolicyGroup:                                                    */
/* Resets CX FIFOs pointers in core during training 
*/


#define DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG 0x1B340800

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cxfifo_trainreset : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* This resets CX FIFO pointers in training */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_STRUCT;


































#endif /* _FNV_DA_UNIT_1_h */
