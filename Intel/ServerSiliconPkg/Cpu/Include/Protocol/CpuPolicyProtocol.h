/** @file
  Intel CPU policy protocol should be installed after CPU related setting
  are set by platform driver. CPU driver only could get CPU policy data after this
  protocol installed.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_POLICY_PROTOCOL_H_
#define _CPU_POLICY_PROTOCOL_H_

#include <CpuPolicyPeiDxeCommon.h>

#define CPU_POLICY_PROTOCOL_GUID \
  { \
    0xec7c60b4, 0xa82c, 0x42a5, { 0xbe, 0x76, 0x87, 0xfc, 0xb5, 0x81, 0xa9, 0x1b } \
  }

typedef struct {
  UINT64                PlatformCpuSocketNames;
  UINT64                PlatformCpuAssetTags;
  UINT8                 SbspSelection;
  CPU_POLICY_COMMON     Policy;
} CPU_POLICY_CONFIGURATION;

extern EFI_GUID gEfiCpuPolicyProtocolGuid;

#endif
