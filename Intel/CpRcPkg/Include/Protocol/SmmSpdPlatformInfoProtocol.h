/** @file
  Contains protocol information for SPD Consumption

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

#ifndef __SMM_SPD_PLATFORM_INFO_PROTOCOL_H__
#define __SMM_SPD_PLATFORM_INFO_PROTOCOL_H__

extern EFI_GUID gSmmSpdPlatformInfoProtocolGuid;

#include <Guid/PlatformInfo.h>

#pragma pack(1)

typedef struct _SMM_SPD_PLATFORM_INFO_PROTOCOL {
  UINT32                      TypeRevisionId;                   // Board Revision ID
  EFI_PLATFORM_TYPE           BoardId;                          // Board ID
  INTERPOSER_TYPE             InterposerType[MAX_SOCKET];       // 0 - Unknown, 1 - InterposerA, 2 - InterposerB
} SMM_SPD_PLATFORM_INFO_PROTOCOL;

#pragma pack()

#endif //#ifndef __SMM_SPD_PLATFORM_INFO_PROTOCOL_H__
