/** @file
  Interface header file for the Mesh2Mem IP library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _MESH2MEM_REGS_H_
#define _MESH2MEM_REGS_H_

#include <RcRegs.h>

#ifdef GNRSRF_HOST
//
// CSR name aliasing as the workaround for building Spr/Mesh2MemRas.c with
// GnrSrf/M2MEM_MAIN.h file
//
#define VID_M2MEM_MAIN_REG                      VID_0_12_0_CFG_M2MEM_MAIN_REG
typedef VID_0_12_0_CFG_M2MEM_MAIN_STRUCT        VID_M2MEM_MAIN_STRUCT;
#define DID_M2MEM_MAIN_REG                      DID_0_12_0_CFG_M2MEM_MAIN_REG
typedef DID_0_12_0_CFG_M2MEM_MAIN_STRUCT        DID_M2MEM_MAIN_STRUCT;
#define M2MBIOSQUIESCE_M2MEM_MAIN_REG           B2CMIBIOSQUIESCE_M2MEM_MAIN_REG
typedef B2CMIBIOSQUIESCE_M2MEM_MAIN_STRUCT      M2MBIOSQUIESCE_M2MEM_MAIN_STRUCT;
#endif // GNRSRF_HOST

#endif // _MESH2MEM_REGS_H_

