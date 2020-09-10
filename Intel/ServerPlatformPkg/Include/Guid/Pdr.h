/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _PDR_GUID_H_
#define _PDR_GUID_H_

#include <PiPei.h>

// {27E753F6-7CCF-46fd-AF01-75489D88D338}
#define PLATFORM_PDR_GUID \
{ 0x27e753f6, 0x7ccf, 0x46fd, { 0xaf, 0x1, 0x75, 0x48, 0x9d, 0x88, 0xd3, 0x38 } }

#pragma pack(1)

//
// PDR definition from Platform EAS
//
typedef struct _PLATFORM_PDR_TABLE {
  UINT8                       SystemUuid[16];     // 16 bytes
  UINT32                      Signature;          // "$PIT" 0x54495024
  UINT32                      Size;               // Size of the table
  UINT16                      Revision;           // Revision of the table
  UINT16                      Flavor;             // 00- None, 01- Blade, 02- Telco
  UINT16                      Type;               // Platform Type
  UINT16                      PlatformRevision;   // Revision of the platform, Add in Grantley PDR definition

  } PLATFORM_PDR_TABLE;

#pragma pack()

STATIC EFI_GUID gPlatforPdrGuid = PLATFORM_PDR_GUID;

#endif
