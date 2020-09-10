/** @file
  MemPcx.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef  __MEM_PXC_H__
#define  __MEM_PXC_H__

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"

//
// Function prototypes that are private to this lib
//

/**
  Read or write PXC Enable or Disable on all channels

  @param[in]      Host      - Pointer to SYSHOST
  @param[in]      Socket    - Socket number
  @param[in]      Channel   - Channel number (ALL_CH for all channels)
  @param[in]      Mode      - Bit-field of different modes
  @param[in,out]  State     - Pointer to value to get or set - PXC_ENABLE or PXC_DISABLE

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
GetSetPxcControl (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             Channel,
  IN      UINT8             Mode,
  IN      INT16             *State
  );

/**
  Early initialization required when TX Phase-based Xtalk Cancellation (PXC)
  or Data Dependent Jitter Cancellation (DDJC) is to be enabled.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket

  @retval SUCCESS

**/
UINT32
PxcDdjcInitialize (
  IN     PSYSHOST               Host,
  IN     UINT8                  Socket
  );

#endif   // __MEM_PXC_H__
