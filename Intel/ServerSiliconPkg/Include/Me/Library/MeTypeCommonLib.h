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

#ifndef _ME_TYPE_COMMON_LIB_H_
#define _ME_TYPE_COMMON_LIB_H_

#include <MeTypeDefs.h>
#include <MkhiMsgs.h>

#define ME_DETECTION_REPEATS_MAX  200
#define ME_DETECTION_STEP_TIMEOUT 1000

typedef enum _ON_BOARD_ME_TYPE
{
   ME_TYPE_UNDEF    = METYPE_UNDEF,
   ME_TYPE_DFX      = METYPE_DFX,
   ME_TYPE_SPS      = METYPE_SPS,
   ME_TYPE_CLIENT   = METYPE_CLIENT,
   ME_TYPE_DISABLED = METYPE_DISABLED
} ON_BOARD_ME_TYPE;

ON_BOARD_ME_TYPE GetOnBoardMeType (VOID);

#pragma pack(push,1)
typedef struct
{
  UINT32              FwType    : 3;
  UINT32              FwSubType : 3;
  UINT32              Reserved  : 26;
} FW_TYPE_DATA;

typedef struct
{
  MKHI_MESSAGE_HEADER                MkhiHeader;
  FW_TYPE_DATA                       FwTypeData;
} GEN_GET_FW_TYPE_ACK;

typedef struct
{
  MKHI_MESSAGE_HEADER                MkhiHeader;
} GEN_GET_FW_TYPE;

#define BUP_GET_ME_TYPE     0x11

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

  @retval On Error:                ME_TYPE_UNDEF
  @retval On Success:              ME_TYPE_SPS
  @retval On Success:              ME_TYPE_WS
  @retval On Success:              ME_TYPE_DISABLED
**/
ON_BOARD_ME_TYPE
GetMeTypeFromHob (
  VOID
  );
#endif // _ME_TYPE_COMMON_LIB_H_
