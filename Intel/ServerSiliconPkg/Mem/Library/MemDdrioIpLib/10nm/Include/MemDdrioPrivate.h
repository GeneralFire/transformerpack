
/** @file
  MemDdrioPrivate.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _DDRIO_PRIVATE_H_
#define _DDRIO_PRIVATE_H_

#include <ReferenceCodeDataTypes.h>
#include <Library/MemDdrioIpLib.h>

//
// Structure used to associate power training hook identifiers with
// power training hook functions
//
typedef struct {
  POWER_TRAIN_HOOK  PowerTrainHook;
  MRC_STATUS        (*HookFunction) (UINT8 Socket);
} POWER_TRAINING_HOOK_FUNCTIONS;

/**

  Restore roundtrip timings for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval EFI_SUCCESS   Round trip value was successfully restored
  @retval !EFI_SUCCESS  Round trip value was not successfully restored

**/
EFI_STATUS
RestoreRoundTrip (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  );

/**

  Return cached RCVEN Xover offset for a strobe

  @param[in]  Host        - Ptr to SYSHOST, system Host (root) structure
  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  Strobe      - Strobe number

  @retval Rcven xover offset

**/
UINT32
GetCachedRcvenXoverOffsetDdrio (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Strobe
  );

#endif //_DDRIO_PRIVATE_H_
