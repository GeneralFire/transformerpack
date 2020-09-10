/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __RMT_RESULT_COLUMNS_H__
#define __RMT_RESULT_COLUMNS_H__

#include "ssabios.h"

#define RMT_RESULT_COLUMNS_GUID \
{0xD98145F2,0x62F4,0x47CD,{0xAA,0xD1,0xDA,0x77,0x91,0xB2,0x77,0xF1} }

#pragma pack (push, 1)

struct RMT_RESULT_ROW_HEADER;

enum RMT_RESULT_TYPE{
  Rank0RmtResultType = 0,
  Rank1RmtResultType = 1,
  LaneRmtResultType = 2,
  TurnaroundRmtResultType = 3,
  ParamLimits0ResultType = 4,
  ParamLimits1ResultType = 5,
  ParamLimits2ResultType = 6,
  Rank2RmtResultType = 7,
  Rank3RmtResultType = 8,
  RmtResultTypeMax = 9,
  RMT_RESULT_TYPE_DELIM = MAX_INT32
};


typedef enum RMT_RESULT_TYPE RMT_RESULT_TYPE;


struct RMT_RESULT_ROW_HEADER{
  UINT32 ResultType :4;
  UINT32 Socket :3;
  UINT32 Controller :3;
  UINT32 Channel :3;
  UINT32 DimmA :2;
  UINT32 RankA :3;
  UINT32 DimmB :2;
  UINT32 RankB :3;
  UINT32 Lane :7;
  UINT32 IoLevel :1;
  UINT32 IsDdrT :1;
};

typedef struct RMT_RESULT_ROW_HEADER RMT_RESULT_ROW_HEADER;

typedef struct RMT_RESULT_COLUMNS{
  struct RMT_RESULT_ROW_HEADER Header;
  UINT8 Margin[4][2];
}RMT_RESULT_COLUMNS;


#pragma pack (pop)

#endif //__RMT_RESULT_COLUMNS_H__
