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

#ifndef _PLATFORM_PCH_EARLY_UPDATE_LIB_H
#define _PLATFORM_PCH_EARLY_UPDATE_LIB_H


#include <Base.h>
#include <PiPei.h>

#include <Guid/SetupVariable.h>

#define PLATFORM_PCH_EARLY_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'P', 'C', 'H')
#define PLATFORM_PCH_EARLY_UPDATE_VERSION    01


// {1763F1D2-6A47-43f8-8279-3765A6929060}
#define   PLATFORM_PCH_EARLY_CONFIG_DATA_GUID \
{ 0x1763f1d2, 0x6a47, 0x43f8, { 0x82, 0x79, 0x37, 0x65, 0xa6, 0x92, 0x90, 0x60 } }


typedef
EFI_STATUS
(*PLATFORM_PCH_LAN_CONFIG) (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);

typedef
EFI_STATUS
(*PLATFORM_EARLY_INIT_HOOK) (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);

typedef struct
{
  UINT32                          Signature;
  UINT32                          Version;

  PLATFORM_PCH_LAN_CONFIG         ConfigLan;
  PLATFORM_EARLY_INIT_HOOK        InitLateHook;

} PLATFORM_PCH_EARLY_UPDATE_TABLE;


EFI_STATUS
PlatformPchLanConfig (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);

EFI_STATUS
PlatformInitLateHook (
  IN SYSTEM_CONFIGURATION         *SystemConfig
);


STATIC  EFI_GUID gPlatformPchEarlyConfigDataGuid = PLATFORM_PCH_EARLY_CONFIG_DATA_GUID;

#endif //_PLATFORM_PCH_EARLY_UPDATE_LIB_H
