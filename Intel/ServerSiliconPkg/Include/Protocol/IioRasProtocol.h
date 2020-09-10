/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2012 Intel Corporation. <BR>

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

#ifndef _IIO_RAS_PROTOCOL_H_
#define _IIO_RAS_PROTOCOL_H_

#include <Guid/MemoryMapData.h>

#define EFI_IIO_RAS_PROTOCOL_GUID \
  {0x4c7e45bc, 0x8a23, 0x26cd, 0x94, 0xad, 0x5d, 0x2c, 0x26, 0x3f, 0x25, 0xfe}

#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
EFI_FORWARD_DECLARATION (EFI_IIO_RAS_PROTOCOL);

//
// Protocol Definition
//
typedef
EFI_STATUS
(EFIAPI *EFI_IIO_ONLINE) (
  IN UINT8 Socket
  );

typedef
EFI_STATUS
(EFIAPI *EFI_IIO_OFFLINE) (
  IN UINT8 Socket
  );

typedef struct _EFI_IIO_RAS_PROTOCOL {
  EFI_IIO_ONLINE  IioOnline;
  EFI_IIO_OFFLINE IioOffline;
} EFI_IIO_RAS_PROTOCOL;

extern EFI_GUID gEfiIioRasProtocolGuid;

#endif // _IIO_RAS_PROTOCOL_H_
