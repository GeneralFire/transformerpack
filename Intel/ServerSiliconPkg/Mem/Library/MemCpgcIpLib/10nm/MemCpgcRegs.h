/** @file
  Interface header file for the CPGC IP library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _MEM_CPGC_REGS_H_
#define _MEM_CPGC_REGS_H_

//
// for wave1 the CPGC is contained by MC register group, TODO: SPR will update the Cpgc related as single register group
//
#include "Chip10nm/Include/RcRegs10nm.h"
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>

// CPGC CSR Aliases
// A group of CPGC self-defines was removed from this section, e.g: #define CPGC_ERR_ADDR1_MCDDC_CTL_STRUCT   CPGC_ERR_ADDR1_MCDDC_CTL_STRUCT
// TODO: Should this file be removed? Keeping for now in case SPR has use for it

#endif // _MEM_CPGC_REGS_H_
