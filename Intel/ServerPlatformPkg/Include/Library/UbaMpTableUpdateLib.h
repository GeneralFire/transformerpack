/** @file
  UBA Mp table Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2015 Intel Corporation. <BR>

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

#ifndef _UBA_MP_TABLE_UPDATE_LIB_H
#define _UBA_MP_TABLE_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#include <PlatDevData.h>

#define PLATFORM_MP_TABLE_UPDATE_SIGNATURE  SIGNATURE_32 ('U', 'M', 'P', 'T')
#define PLATFORM_MP_TABLE_UPDATE_VERSION    0x01

// {0FF8A1CF-A0AB-4ac0-BFC9-34A78F68DD8A}
#define   PLATFORM_MP_TABLE_CONFIG_DATA_GUID \
{ 0xff8a1cf, 0xa0ab, 0x4ac0, { 0xbf, 0xc9, 0x34, 0xa7, 0x8f, 0x68, 0xdd, 0x8a } };


typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  DEVICE_DATA             *DeviceDataPtr;

} PLATFORM_MP_UPDATE_TABLE;

EFI_STATUS
PlatformGetMpTableDataPointer (
  IN  DEVICE_DATA                       **DeviceData
);

STATIC  EFI_GUID gPlatformMpTableConfigDataGuid =PLATFORM_MP_TABLE_CONFIG_DATA_GUID;

#endif //_UBA_MP_TABLE_UPDATE_LIB_H
