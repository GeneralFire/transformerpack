/** @file
  UBA System Config Update Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#ifndef _UBA_SYSTEM_CONFIG_UPDATE_LIB_H_
#define _UBA_SYSTEM_CONFIG_UPDATE_LIB_H_

#include <Base.h>
#include <Uefi.h>

#include <Guid/SetupVariable.h>

#define SYSTEM_CONFIG_UPDATE_SIGNATURE  SIGNATURE_32 ('S', 'C', 'O', 'N')
#define SYSTEM_CONFIG_UPDATE_VERSION    01

STATIC EFI_GUID  gSystemConfigUpdateDataGuid =
{ 0x41037136, 0x8834, 0x4F35, { 0xBB, 0x10, 0x28, 0x0, 0x87, 0xAD, 0xB2, 0x22 } };

typedef
VOID
(*IIO_DEFAULT_CONFIG_UPDATE_CALLBACK) (
  IN  SYSTEM_CONFIGURATION       *Default
  );

typedef struct
{
  UINT32                                    Signature;
  UINT32                                    Version;
  IIO_DEFAULT_CONFIG_UPDATE_CALLBACK        CallUpdateIioConfig;
} SYSTEM_CONFIG_UPDATE_DATA;

EFI_STATUS
UpdateIioDefaultConfig (
  IN  SYSTEM_CONFIGURATION       *Default
  );

#endif  //_UBA_SYSTEM_CONFIG_UPDATE_LIB_H_
