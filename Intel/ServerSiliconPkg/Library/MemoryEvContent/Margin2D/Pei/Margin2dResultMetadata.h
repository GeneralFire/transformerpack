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

#ifndef __MARGIN2D_RESULT_METADATA_H__
#define __MARGIN2D_RESULT_METADATA_H__

#include "ssabios.h"

#define MARGIN2D_RESULT_METADATA_GUID \
{0xEC5890F3,0xFA4C,0x40AB,{0x9C,0xA7,0xC2,0xFA,0xCF,0x38,0xE9,0xEF} }

#pragma pack (push, 1)

typedef struct MARGIN2D_RESULT_METADATA{
  MRC_LT IoLevel;
  MRC_GT SlowMarginGroup;
  MRC_GT FastMarginGroup;
  UINT8 TargetSocket;
  UINT8 TargetController;
  UINT8 TargetChannel;
  UINT8 TargetDimmA;
  UINT8 TargetDimmB;
  UINT8 TargetRankA;
  UINT8 TargetRankB;
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  INT16 MinFastOffset;
  INT16 MaxFastOffset;
  INT16 MinSlowOffset;
  INT16 MaxSlowOffset;
  UINT16 FastStepUnit;
  UINT16 SlowStepUnit;
  UINT8 BusWidth;
  UINT32 BusFreq;
  UINT32 MemVoltage;
  UINT8 MajorVer;
  UINT8 MinorVer;
  UINT8 RevVer;
  UINT32 BuildVer;
  UINT8 StatusCode;
  UINT16 LineNumber;
  CHAR8 FileName[13];
}MARGIN2D_RESULT_METADATA;


#pragma pack (pop)

#endif //__MARGIN2D_RESULT_METADATA_H__
