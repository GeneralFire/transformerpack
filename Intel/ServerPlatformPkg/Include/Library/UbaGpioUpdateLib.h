/** @file
  UBA GPIO Update Library Header File.

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

#ifndef _UBA_GPIO_UPDATE_LIB_H
#define _UBA_GPIO_UPDATE_LIB_H

#include <Base.h>
#include <Uefi.h>

#define PLATFORM_GPIO_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'G', 'P', 'O')
#define PLATFORM_GPIO_UPDATE_VERSION    01

// {E02C2982-0009-46f6-AF19-DF52BB9742BF}
#define   PLATFORM_GPIO_CONFIG_DATA_GUID \
{ 0xe02c2982, 0x9, 0x46f6, { 0xaf, 0x19, 0xdf, 0x52, 0xbb, 0x97, 0x42, 0xbf } }

#define PLATFORM_NUMBER_OF_GPIO_REGISTERS   20
#define PLATFORM_END_OF_GPIO_LIST           0xFFFFFFFF

#define GPIO_NO_OR                          0
#define GPIO_NO_AND                         0xFFFFFFFF


typedef struct {
  UINT32        Register;
  UINT32        Value;
} GPIO_DATA;

typedef struct {
  UINT32                  Signature;
  UINT32                  Version;

  GPIO_DATA               Gpios[PLATFORM_NUMBER_OF_GPIO_REGISTERS];

} PLATFORM_GPIO_UPDATE_TABLE;


EFI_STATUS
PlatformUpdateGpios (
  VOID
);

STATIC  EFI_GUID gPlatformGpioConfigDataGuid = PLATFORM_GPIO_CONFIG_DATA_GUID;

#endif //_UBA_GPIO_UPDATE_LIB_H
