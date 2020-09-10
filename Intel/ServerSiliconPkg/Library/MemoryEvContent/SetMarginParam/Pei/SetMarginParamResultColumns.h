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

#ifndef __SET_MARGIN_PARAM_RESULT_COLUMNS_H__
#define __SET_MARGIN_PARAM_RESULT_COLUMNS_H__

#include "ssabios.h"

#define SET_MARGIN_PARAM_RESULT_COLUMNS_GUID \
{0x3E66E751,0xD143,0x456D,{0xB0,0x68,0xE2,0x34,0x10,0x5B,0x70,0x3D} }

#pragma pack (push, 1)

typedef struct SET_MARGIN_PARAM_RESULT_COLUMNS{
  UINT8 DummyParameter;
}SET_MARGIN_PARAM_RESULT_COLUMNS;


#pragma pack (pop)

#endif //__SET_MARGIN_PARAM_RESULT_COLUMNS_H__
