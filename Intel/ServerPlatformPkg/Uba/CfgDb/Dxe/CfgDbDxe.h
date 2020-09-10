/** @file
  UbaConfigDatabase Dxe Driver head file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2017 Intel Corporation. <BR>

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

#ifndef _UBA_CONFIG_DATABASE_DXE_H_
#define _UBA_CONFIG_DATABASE_DXE_H_

#include <Base.h>
#include <Uefi.h>
#include <Protocol/UbaCfgDb.h>
#include <Guid/UbaCfgHob.h>

typedef struct _UBA_DXE_PRIVATE_DATA {
  UINT32                          Signature;
  UINT32                          Version;

  UINTN                           ConfigDataCount;              //for AllConfigDataSize
  UINTN                           HandleCount;
  UBA_BOARD_NODE                  *CurrentSku;

  UBA_CONFIG_DATABASE_PROTOCOL   UbaCfgDbProtocol;
} UBA_DXE_PRIVATE_DATA;

#define PRIVATE_DATA_FROM_PROTOCOL(p)    CR(p, UBA_DXE_PRIVATE_DATA, UbaCfgDbProtocol, UBA_BOARD_SIGNATURE)

#endif // _UBA_CONFIG_DATABASE_DXE_H_

