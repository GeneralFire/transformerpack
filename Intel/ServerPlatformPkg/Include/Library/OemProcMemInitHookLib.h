/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef  _OEM_PROC_MEM_INIT_HOOK_LIB_H_
#define  _OEM_PROC_MEM_INIT_HOOK_LIB_H_

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"

#define   PLATFORM_SPD_RAW_DATA_GUID { 0xaa7e60ee, 0xa309, 0x40a5, { 0x8f, 0xfa, 0x36, 0x6b, 0xc1, 0x3c, 0xb8, 0xf0 } }

#pragma pack(1)

typedef struct _PLATFORM_SPD_RAW_DATA_HOB {
  UINT32                     Size;
  UINT8                      Socket;
  UINT8                      Channel;
  UINT8                      Dimm;
  // UINT8                      Data[]
} PLATFORM_SPD_RAW_DATA_HOB;

#pragma pack()

VOID
PlatformCheckPORCompatHook (
  PSYSHOST Host
  );

MRC_STATUS
EFIAPI
PlatformFatalErrorHook (
  IN UINT8        majorCode,
  IN UINT8        minorCode,
  IN OUT UINT8    *haltOnError
  );


UINT32
PlatformMemInitWarningHook (
  IN PSYSHOST Host
  );
  
#endif
