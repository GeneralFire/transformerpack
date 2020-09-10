/** @file

  Header file to perform MarginTest on given signal\offset combination

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _MARGIN_TEST_H_
#define _MARGIN_TEST_H_

#include "ssabios.h"
#include "CpgcPointTest.h"
#include "MemPointTest.h"
#include "ChkSystemInfo.h"
#include "ChkRetStat.h"
#include "MarginParam.h"
#include "MemCfg.h"
#include "EvContentInternal.h"
#include <Library/BitMaskLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/DebugLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/ProcMemInitChipLib.h>
#include "EvDebugPrint.h"

#define MAX_IO_BOOT_MASK_TEST_POINTS FixedPcdGet8 (PcdMemBootSignalsToTest) * 2 // 2 points per signal
#define MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT FixedPcdGet8 (PcdMemBootSignalsToTest)

#define BOOT_RAS_RESULT_METADATA_GUID \
{0x15AEF8BF,0xACD5,0x4666,{0xA7,0xEE,0xE5,0xE4,0x28,0xC6,0x9C,0x30} }

#define BOOT_RAS_RESULT_COLUMNS_GUID \
{0xC6A98627,0x6DCE,0x41D3,{0xBA,0xA6,0x72,0xCB,0x2A,0xD7,0xBE,0x1F} }

///
/// Mask test modes
///
typedef enum {
  WarningIoBootHealthMask,                ///warning mask.
  CriticalIoBootHealthMask,               ///critical mask.
  IoBootHealthMaskDelim = MAX_INT32       ///IO_BOOT_HEALTH_MASK enumeration delimiter value.
} IO_BOOT_HEALTH_MASK;

#pragma pack(push, 1)

typedef struct BOOT_RAS_RESULT_COLUMNS {
  UINT8 Socket;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 TestPoint;
  UINT8 Status;
} BOOT_RAS_RESULT_COLUMNS;

typedef struct {
  // pointer to test configuration structure
  BOOT_RAS_CONFIG *pTestCfg;

  // pointer to test status structure
  TEST_STATUS *pTestStat;

  // system information structure
  MRC_SYSTEM_INFO SystemInfo;

  // pointer to CPGC information structure
  VOID *pCpgcInfo;

  // memory configuration structure
  MEM_CFG MemCfg;

  // memory point test configuration structure with selected devices
  MEM_POINT_TEST_CONFIG SelectMemPointTestCfg;

  // memory point test configuration structure with current devices
  MEM_POINT_TEST_CONFIG CurrentMemPointTestCfg;

  // CPGC point test configuration structure
  CPGC_POINT_TEST_CONFIG CpgcPointTestCfg;

  // pointer to CpgcPointTest test data structure
  VOID *pCpgcPointTestData;

  // per-channel array of rank combination counts
  RANK_COMBINATION_CNTS RankCombinationCnts;

  // maximum rank combination count
  UINT8 MaxRankCombinationsCnts;

  // current rank combinations index
  UINT8 CurrentRankCombinationIndex;

  IO_BOOT_HEALTH_MASK BootMask;

  // bit0 is for the warning mask result. 1: failed, 0: passed
  // bit1 is for the critical mask result. 1: failed, 0: passed
  // bit[7:2] are the test point index.
  UINT8 TestResults[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT][MAX_DIMM_CNT][MAX_RANK_CNT];
  UINT8 PointTestResult; // test result of this test point.
  UINT8 TestPointIndex; // the index of the test point of the mask
  UINT8 MarginSignalIdx;

  // Some config setting that can go to the test config file. currently just hard code.
  RANK_TEST_MODE RankTestMode;
  UINT16 BurstLength;
  UINT32 LoopCount;

} BOOT_RAS_CORE_TEST_DATA;

#pragma pack(pop)

#endif // _MARGIN_TEST_H_
