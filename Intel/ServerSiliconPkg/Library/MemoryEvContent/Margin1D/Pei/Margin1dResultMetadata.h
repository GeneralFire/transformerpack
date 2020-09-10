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

#ifndef __MARGIN1D_RESULT_METADATA_H__
#define __MARGIN1D_RESULT_METADATA_H__

#include "ssabios.h"

#define MARGIN_1D_RESULT_METADATA_GUID \
{0xEE897CC8,0xDDE2,0x4A40,{0xBF,0x2D,0x01,0x78,0x7A,0x90,0xE6,0xC1} }

#pragma pack (push, 1)

typedef struct MARGIN_1D_RESULT_METADATA{
  MRC_LT IoLevel;
  MRC_GT MarginGroup;
  UINT16 ChunkValBitmasks[MAX_IMC][MAX_MC_CH];
  UINT16 BurstLengths[MAX_IMC][MAX_MC_CH];
  UINT32 LoopCounts[MAX_IMC][MAX_MC_CH];
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN EnableExtendedRange;
  INT16 MinOffset[2];
  INT16 MaxOffset[2];
  INT16 StepUnit[2];
  UINT8 BusWidth;
  UINT32 BusFreq;
  UINT32 MemVoltage;
  UINT8 MaxRankPerDimm;
  UINT32 ErrCntOverflowVal;
  UINT8 MajorVer;
  UINT8 MinorVer;
  UINT8 RevVer;
  UINT32 BuildVer;
  UINT8 StatusCode;
  UINT16 LineNumber;
  CHAR8 FileName[13];
}MARGIN_1D_RESULT_METADATA;


#pragma pack (pop)

#endif //__MARGIN1D_RESULT_METADATA_H__
