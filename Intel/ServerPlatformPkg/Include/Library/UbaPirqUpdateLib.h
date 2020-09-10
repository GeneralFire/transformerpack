/** @file
  UBA PirqData Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2014 Intel Corporation. <BR>

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

#ifndef _UBA_PIRQ_UPDATE_LIB_H
#define _UBA_PIRQ_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#include <PlatPirqData.h>

#define PLATFORM_PIRQ_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'I', 'R', 'Q')
#define PLATFORM_PIRQ_UPDATE_VERSION    0x01


// {4C1F48A5-C976-4d90-9F03-8E9B1C327FCF}
#define   PLATFORM_PIRQ_CONFIG_DATA_GUID \
{ 0x4c1f48a5, 0xc976, 0x4d90, { 0x9f, 0x3, 0x8e, 0x9b, 0x1c, 0x32, 0x7f, 0xcf } }


typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  PLATFORM_PIRQ_DATA      *PirqDataPtr;

} PLATFORM_PIRQ_UPDATE_TABLE;

EFI_STATUS
PlatformGetPirqDataPointer (
  IN  PLATFORM_PIRQ_DATA                **PirqData
);

STATIC  EFI_GUID gPlatformPirqConfigDataGuid = PLATFORM_PIRQ_CONFIG_DATA_GUID;

#endif //_UBA_PIRQ_UPDATE_LIB_H
