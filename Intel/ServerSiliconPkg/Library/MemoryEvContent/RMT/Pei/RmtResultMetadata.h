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

#ifndef __RMT_RESULT_METADATA_H__
#define __RMT_RESULT_METADATA_H__

#include "ssabios.h"

#define RMT_RESULT_METADATA_GUID \
{0x477AEB0D,0xDE07,0x4277,{0xA4,0xE7,0x87,0x81,0x0B,0x10,0x00,0x31} }

#pragma pack (push, 1)







typedef struct RMT_RESULT_METADATA{
  BOOLEAN EnableCtlAllMargin;
  UINT16 SinglesBurstLength;
  UINT32 SinglesLoopCount;
  UINT16 TurnaroundsBurstLength;
  UINT32 TurnaroundsLoopCount;
  SCRAMBLER_OVERRIDE_MODE ScramblerOverrideMode;
  BOOLEAN EnableExtendedRange;
  UINT8 PiStepUnit[2];
  UINT16 RxVrefStepUnit[2];
  UINT16 TxVrefStepUnit[2][2];
  UINT16 CmdVrefStepUnit[2][2];
  UINT16 EridVrefStepUnit;
  UINT8 BusWidth;
  UINT32 BusFreq;
  UINT32 MemVoltage;
  UINT8 MajorVer;
  UINT8 MinorVer;
  UINT8 RevVer;
  UINT32 BuildVer;
  UINT16 ResultEleCount;
  UINT8 StatusCode;
  UINT16 LineNumber;
  CHAR8 FileName[13];
}RMT_RESULT_METADATA;


#pragma pack (pop)

#endif  //__RMT_RESULT_METADATA_H__
