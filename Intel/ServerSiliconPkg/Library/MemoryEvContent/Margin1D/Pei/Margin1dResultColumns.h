/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef __MARGIN1D_RESULT_COLUMNS_H__
#define __MARGIN1D_RESULT_COLUMNS_H__

#include "ssabios.h"

#define MARGIN_1D_RESULT_COLUMNS_GUID \
{0xEBD5C65F,0x7DAE,0x4B82,{0x81,0x3B,0x01,0x2D,0x24,0x4F,0x16,0xB6} }

#pragma pack (push, 1)

typedef struct MARGIN_1D_RESULT_COLUMNS{
  UINT8 Socket;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 DimmA;
  UINT8 RankA;
  UINT8 DimmB;
  UINT8 RankB;
  UINT8 RankCombinationIndex;
  INT16 Offset;
  UINT8 LaneErrorStatus[MAX_BITS / 8];
  UINT32 ErrorCount;
}MARGIN_1D_RESULT_COLUMNS;


#pragma pack (pop)

#endif //__MARGIN1D_RESULT_COLUMNS_H__
