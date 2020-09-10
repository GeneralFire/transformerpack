/** @file
  UBA PCD Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2015 Intel Corporation. <BR>

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

#ifndef _UBA_PCD_UPDATE_LIB_H
#define _UBA_PCD_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_PCD_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'P', 'C', 'D')
#define PLATFORM_PCD_UPDATE_VERSION    01

// {D5081573-B3B6-4a1f-9FBC-C3DEDA04CD49}
#define   PLATFORM_PCD_CONFIG_DATA_GUID \
{ 0xd5081573, 0xb3b6, 0x4a1f, { 0x9f, 0xbc, 0xc3, 0xde, 0xda, 0x4, 0xcd, 0x49 } }

typedef
EFI_STATUS
(*PCD_UPDATE_CALLBACK) (
  VOID
);

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  PCD_UPDATE_CALLBACK     CallUpdate;

} PLATFORM_PCD_UPDATE_TABLE;

EFI_STATUS
PlatformUpdatePcds (
  VOID
);

STATIC  EFI_GUID gPlatformPcdConfigDataGuid = PLATFORM_PCD_CONFIG_DATA_GUID;

#endif //_UBA_PCD_UPDATE_LIB_H
