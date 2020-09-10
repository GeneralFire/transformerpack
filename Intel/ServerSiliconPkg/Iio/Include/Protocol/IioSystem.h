/** @file
  Header file for IIO driver specific Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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

#ifndef _IIO_SYSTEM_PROTOCOL_H_
#define _IIO_SYSTEM_PROTOCOL_H_

#include <IioPlatformData.h>
#include <IioSetupDefinitions.h>

//
// IIO System driver Protocol GUID
//
// {DDC3080A-2740-4ec2-9AA5-A0ADEFD6FF9C}
#define EFI_IIO_SYSTEM_GUID \
  { \
    0xDDC3080A, 0x2740, 0x4ec2, 0x9A, 0xA5, 0xA0, 0xAD, 0xEF, 0xD6, 0xFF, 0x9C \
  }

extern EFI_GUID         gEfiIioSystemProtocolGuid;

typedef struct _PORT_DESCRIPTOR {
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
} PORT_DESCRIPTOR;

typedef struct _PORT_ATTRIB {
  UINT8   PortWidth;
  UINT8   PortSpeed;
} PORT_ATTRIB;

EFI_STATUS
EFIAPI
IioGetCpuUplinkPort (
  UINT8             IioIndex,
  PORT_DESCRIPTOR   *PortDescriptor,    //Bus, Device, function
  BOOLEAN           *PortStatus,        //TRUE if enabled else disabled
  PORT_ATTRIB       *PortAttrib         //width and speed
  );

typedef
EFI_STATUS
(EFIAPI *IIO_GET_CPU_UPLINK_PORT) (
  IN  UINT8             IioIndex,
  OUT PORT_DESCRIPTOR   *PortDescriptor,
  OUT BOOLEAN           *PortStatus,
  OUT PORT_ATTRIB       *PortAttrib
  );

typedef struct _EFI_IIO_SYSTEM_PROTOCOL {
  IIO_GLOBALS               *IioGlobalData;
  IIO_GET_CPU_UPLINK_PORT   IioGetCpuUplinkPort;
} EFI_IIO_SYSTEM_PROTOCOL;

#endif //_IIO_SYSTEM_PROTOCOL_H_
