/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2012 Intel Corporation. <BR>

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

#ifndef _PLATFORM_PCH_POLICY_UPDATE_LIB_H
#define _PLATFORM_PCH_POLICY_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#include <Ppi/PchPolicy.h>


#define PLATFORM_PCH_POLICY_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'C', 'H', 'P')
#define PLATFORM_PCH_POLICY_UPDATE_VERSION    01

// {DA33A8DD-9C7C-4af9-BF21-D70909F05575}
#define   PLATFORM_PCH_POLICY_DATA_GUID \
{ 0xda33a8dd, 0x9c7c, 0x4af9, { 0xbf, 0x21, 0xd7, 0x9, 0x9, 0xf0, 0x55, 0x75 } }


typedef
EFI_STATUS
(*PCH_POLICY_UPDATE_CALLBACK) (
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *DxePchPolicy
);

typedef struct
{
  UINT32                          Signature;
  UINT32                          Version;

  PCH_POLICY_UPDATE_CALLBACK      CallPchPolicyUpdate;

} PLATFORM_PCH_POLICY_UPDATE_TABLE;

EFI_STATUS
PlatformUpdatePchPolicy(
  IN  DXE_PCH_PLATFORM_POLICY_PROTOCOL  *DxePchPolicy
);


STATIC  EFI_GUID gPlatformPchPolicygDataGuid = PLATFORM_PCH_POLICY_DATA_GUID;

#endif //_PLATFORM_PCH_POLICY_UPDATE_LIB_H
