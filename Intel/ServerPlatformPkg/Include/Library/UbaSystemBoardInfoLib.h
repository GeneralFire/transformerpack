/** @file
  UBA System Board Info Library Header File.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef _UBA_SYSTEM_BOARD_INFO_LIB_H
#define _UBA_SYSTEM_BOARD_INFO_LIB_H

#include <Base.h>
#include <Uefi.h>
#include <SystemBoard.h>

#define SYSTEM_SYSTEM_BOARD_INFO_SIGNATURE  SIGNATURE_32 ('I', 'N', 'F', 'O')
#define SYSTEM_SYSTEM_BOARD_INFO_VERSION    01

typedef DXE_SYSTEM_BOARD_INFO *(*SYSTEM_BOARD_INFO_CALLBACK) ();

typedef struct
{
  UINT32                                    Signature;
  UINT32                                    Version;
  SYSTEM_BOARD_INFO_CALLBACK                CallUpdate;
} SYSTEM_BOARD_INFO_DATA;

EFI_STATUS
GetSystemBoardInfo (
  IN OUT   DXE_SYSTEM_BOARD_INFO       **SystemboardInfoTableBuffer
  );

extern EFI_GUID gSystemBoardInfoConfigDataGuid;

#endif  //_UBA_SYSTEM_BOARD_INFO_LIB_H
