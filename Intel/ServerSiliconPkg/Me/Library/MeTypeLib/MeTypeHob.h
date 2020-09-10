/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#ifndef _ME_TYPE_HOB_H_
#define _ME_TYPE_HOB_H_

#include <Library/MeTypeGetLib.h>

#pragma pack(push,1)
typedef struct {
  EFI_HOB_GUID_TYPE EfiHobGuidType;
  ON_BOARD_ME_TYPE  MeType;
  UINT8             FwType;
  UINT8             FwSubType;
} ME_FW_TYPE_HOB;
#pragma pack(pop)

extern EFI_GUID gMeTypeHobGuid;

/**
  Function returns ME Type read from HOB /if it is not present/
  If HOB is not present it tries to create it.

  @retval ME_TYPE_UNDEF            Can't determine ME Type
  @retval ME_TYPE_SPS              Detected SPS
  @retval ME_TYPE_CLIENT           Detected Client ME
  @retval ME_TYPE_DFX              Detected DFX ME
  @retval ME_TYPE_DISABLED         Disable all ME flows
**/
ON_BOARD_ME_TYPE
GetMeTypeFromHob (
  VOID
  );

/**
  Get detected ME FW type

  @retval ME_TYPE_UNDEF            Can't determine ME Type
  @retval ME_TYPE_SPS              Detected SPS
  @retval ME_TYPE_CLIENT           Detected Client ME
  @retval ME_TYPE_DFX              Detected DFX ME
  @retval ME_TYPE_DISABLED         Disable all ME flows
**/
ON_BOARD_ME_TYPE
GetOnBoardMeType (
  VOID
  );
#endif // _ME_TYPE_HOB_H_
