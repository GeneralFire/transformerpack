/** @file
  Guid of a NV Variable which store the information about the
  FD/HD/CD/NET/BEV order.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 Intel Corporation. <BR>

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

#ifndef __LEGACY_DEV_ORDER_VARIABLE_GUID_H__
#define __LEGACY_DEV_ORDER_VARIABLE_GUID_H__

///
/// Name and Guid of a NV Variable which stores the information about the
/// FD/HD/CD/NET/BEV order
///
#define EFI_LEGACY_DEV_ORDER_VARIABLE_GUID \
  { \
  0xa56074db, 0x65fe, 0x45f7, {0xbd, 0x21, 0x2d, 0x2b, 0xdd, 0x8e, 0x96, 0x52} \
  }

typedef UINT8 BBS_TYPE;

#pragma pack(1)
typedef struct {
  BBS_TYPE  BbsType;
  ///
  /// Length = sizeof (UINT16) + sizeof (Data)
  ///
  UINT16    Length;
  UINT16    Data[1];
} LEGACY_DEV_ORDER_ENTRY;
#pragma pack()

#define VAR_LEGACY_DEV_ORDER L"LegacyDevOrder"

extern EFI_GUID gEfiLegacyDevOrderVariableGuid;

#endif
