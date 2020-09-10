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
#ifndef __MARGIN2D_RESULT_COLUMNS_H__
#define __MARGIN2D_RESULT_COLUMNS_H__

#include "ssabios.h"

#define MARGIN2D_RESULT_COLUMNS_GUID \
{0x050C8CF2,0xDD1F,0x4048,{0x90,0x3B,0xA2,0x77,0x75,0x29,0x08,0x77} }

#pragma pack (push, 1)

typedef struct MARGIN2D_RESULT_COLUMNS{
  INT16 FastOffset;
  INT16 SlowOffset;
  UINT8 LaneErrorStatus[MAX_BITS / 8];
}MARGIN2D_RESULT_COLUMNS;


#pragma pack (pop)

#endif //__MARGIN2D_RESULT_COLUMNS_H__
