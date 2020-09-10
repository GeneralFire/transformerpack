/** @file
  SsaCallbackPeim.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2014 Intel Corporation. <BR>

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

#ifndef _SSA_CALLBACK_PEIM_H_
#define _SSA_CALLBACK_PEIM_H_

#include "SysHost.h"

struct _SSA_BIOS_CALLBACKS_PPI;

extern EFI_GUID gSsaBiosCallBacksPpiGuid;
typedef int MRC_OEM_STATUS_COMMAND;

typedef
MrcStatus
(EFIAPI * SSA_MRC_CHECKPOINT) (
  EFI_PEI_SERVICES                 **PeiServices,
  struct _SSA_BIOS_CALLBACKS_PPI   *SsaBiosCallBacksPpi,
  MRC_OEM_STATUS_COMMAND           StatusCommand,
  VOID                             *CheckpointData
  );

typedef struct _SSA_BIOS_CALLBACKS_PPI {
  UINT32                      Revision;
  SSA_MRC_CHECKPOINT          MrcCheckpoint;
  VOID                        *ModuleState;
} SSA_BIOS_CALLBACKS_PPI;

#endif
