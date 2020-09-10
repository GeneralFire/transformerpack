/** @file
  UBA FPK configuration library header

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _UBA_FPK_CONFIG_LIB_H
#define _UBA_FPK_CONFIG_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <Library/GpioLib.h>

#define PLATFORM_FPK_CONFIG_SIGNATURE  SIGNATURE_32 ('P', 'F', 'P', 'K')
#define PLATFORM_FPK_CONFIG_VERSION    01

// {38F0930C-E7FB-49CC-B88E-D9909EB65D77}
#define PLATFORM_FPK_CONFIG_DATA_GUID \
{ 0x38f0930c, 0xe7fb, 0x49cc, { 0xb8, 0x8e, 0xd9, 0x90, 0x9e, 0xb6, 0x5d, 0x77 } }

typedef enum {
  PortMappedToFunc0,
  PortMappedToFunc1,
  PortMappedToFunc2,
  PortMappedToFunc3,
  PortNotMapped
} PLATFORM_FPK_PORT_MAP;

typedef struct _PLATFORM_FPK_CONFIG_STRUCT {
  UINT32                            Signature;
  UINT32                            Version;
  PLATFORM_FPK_PORT_MAP             *PortToFuncMapPtr;
  UINTN                             PortToFuncMapSize;
  GPIO_PAD                          PciDisNGpioPad;
  GPIO_PAD                          LanDisNGpioPad;
} PLATFORM_FPK_CONFIG_STRUCT;

/**
  Retrieves FPK config struct from UBA database

  @retval EFI_SUCCESS           Config struct is retrieved.
  @retval EFI_NOT_FOUND         UBA protocol, platform or data not found.
  @retval EFI_INVALID_PARAMETER If PlatformFpkConfigStruct is NULL.
**/
EFI_STATUS
FpkConfigGetConfigStruct (
  OUT PLATFORM_FPK_CONFIG_STRUCT *PlatformFpkConfigStruct
  );

STATIC EFI_GUID gPlatformFpkConfigDataGuid = PLATFORM_FPK_CONFIG_DATA_GUID;

#endif // !_UBA_FPK_CONFIG_LIB_H
