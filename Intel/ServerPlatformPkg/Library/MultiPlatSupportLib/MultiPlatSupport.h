/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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

#ifndef _MULTI_PLATFORM_SUPPORT_H_
#define _MULTI_PLATFORM_SUPPORT_H_

#include <Uefi.h>

#include <Library/PcdLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/DefaultDataFile.h>

extern EFI_GUID gDefaultDataFileGuid;
extern EFI_GUID gEfiVariableGuid;
extern EFI_GUID gEfiAuthenticatedVariableGuid;

typedef struct {
  UINT16 DefaultId;
  UINT8  BoardId;
} DEFAULT_INFO;

typedef struct {
  //
  // HeaderSize includes HeaderSize fields and DefaultInfo arrays
  //
  UINT16 HeaderSize;
  //
  // DefaultInfo arrays those have the same default setting.
  //
  DEFAULT_INFO DefaultInfo[1];
  //
  // Default data is stored as variable storage.
  // VARIABLE_STORE_HEADER  VarStorageHeader;
  //
} DEFAULT_DATA;

typedef struct {
  UINT16 Offset;
  UINT8  Value;
} DATA_DELTA;

#endif
