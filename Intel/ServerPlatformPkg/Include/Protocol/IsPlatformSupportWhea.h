/** @file
  IsPlatformSupportWhea Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2017 Intel Corporation. <BR>

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

#ifndef _IS_PLATFORM_SUPPORT_WHEA_H_
#define _IS_PLATFORM_SUPPORT_WHEA_H_

typedef enum {
  WheaNotSupported = 0x00,
  WheaHestSupport = 0x01,
  WheaBertSupport = 0x02,
  WheaEinjSupport = 0x04,
  WheaErstSupport = 0x08,
  WheaSupported = 0x0F  // WheaHestSupport|WheaBertFeature|WheaEinjFeature|WheaErstFeature
  } WHEA_SUPPORT_LEVEL;


typedef struct _EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL;

typedef
BOOLEAN
(EFIAPI *IS_PLATFORM_SUPPORT_WHEA)  (
  IN EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL          *This
  );
/*++


Whea core calls this function to see if the specific platform support WHEA

--*/

typedef struct _EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL {
  WHEA_SUPPORT_LEVEL            WheaSupport;
  UINT8                         WheaAcpi5Support;
  IS_PLATFORM_SUPPORT_WHEA      IsPlatformSupportWhea;
} EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL;

extern EFI_GUID     gEfiIsPlatformSupportWheaProtocolGuid;

#endif
/* _IS_PLATFORM_SUPPORT_WHEA_H_ */

