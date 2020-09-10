/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#ifndef _EV_LOADER_PEIM_H_
#define _EV_LOADER_PEIM_H_

#include "SysHost.h"
#include <Base.h>
#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>

#include <Ppi/SsaBiosServicesPpi.h>

extern EFI_GUID gSsaBiosCallBacksPpiGuid;

typedef int MRC_OEM_STATUS_COMMAND;

typedef struct SSA_BIOS_SERVICES_LOADER_STATE {
  MRC_OEM_STATUS_COMMAND      RunCheckpoint;
  BOOLEAN                     LoadFromTarget;
  VOID *                      SysConfig;
} SSA_BIOS_SERVICES_LOADER_STATE;

#define VAR_BUFFER_SIZE 2048

#define MAX_NUM_CONFIGS 20
#define MAX_NUM_CHUNKS 9999

#define SYS_CONFIG_NAME L"syscg"
#define TOOL_BINARY_NAME L"toolh"


#pragma pack(push, 1)

typedef struct SSA_BIOS_SYS_CONFIG_HEADER {
  UINT32         SystemConfigRevision;
  UINT32         ToolSize;
  UINT32         PostCode;
  UINT32         PrintLevel;
  UINT32         ConfigCount;
} SSA_BIOS_SYS_CONFIG_HEADER;

#pragma pack(pop)

#define VAR_NAME_LENGTH 5
#define VAR_NAME_SIZE (sizeof(CHAR16) * VAR_NAME_LENGTH)

#define VAR_NULL_NAME_LENGTH (VAR_NAME_LENGTH + 1)     // length as a null terminated string
#define VAR_NULL_NAME_SIZE (sizeof(CHAR16) * VAR_NULL_NAME_LENGTH)

#define NULL_STRING L""

#endif
