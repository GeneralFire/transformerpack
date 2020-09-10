/** @file
  RunCpgcPointTest.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#include "ssabios.h"


#include <Library/BitMaskLib.h>
#include "ChkSystemInfo.h"
#include "ChkRetStat.h"
#include "CommonInitPlatform.h"
#include "CpgcPointTest.h"
#include "InitPlatform.h"
#include "Platform.h"
#include "EvContentInternal.h"
#include "RunCpgcPointTestResultColumns.h"
#include "RunCpgcPointTestResultMetadata.h"
#include "RunCpgcPointTest.h"

#include <Library/BaseMemoryLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

#define SPCL_ERR_CNT_OVERFLOW_VAL 0xFFFFFFFF

#pragma pack(push, 1)

typedef struct {
  // previous platform settings
  BOOLEAN PrevScramblerEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevSelfRefreshEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevPageOpenEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevZQCalEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
  BOOLEAN PrevRCompUpdateEnables[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT];
} TEST_DATA;

#pragma pack(pop)

/**
@brief
  This function sets up the results.  It creates the results buffer and gets a
  pointer to the metadata structure.

  @param[in, out]  SsaServicesHandle    Pointer to SSA services.
  @param[in]       ResultEleCnt         Number of result elements required.
  @param[out]      ppMetadata           Pointer to pointer to results metadata structure.
**/
UINT8 SetupRunCpgcTestResults(
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN UINT32 ResultEleCnt,
  OUT RUN_CPGC_POINT_TEST_RESULT_METADATA **ppMetadata)
{
  SSA_STATUS RetStat;
  UINT32 MetadataSize = sizeof(RUN_CPGC_POINT_TEST_RESULT_METADATA);
  UINT32 ResultElementSize = sizeof(RUN_CPGC_POINT_TEST_RESULT_COLUMNS);
  EFI_GUID MetadataGuid = RUN_CPGC_POINT_TEST_RESULT_METADATA_GUID;
  EFI_GUID ColumnsGuid = RUN_CPGC_POINT_TEST_RESULT_COLUMNS_GUID;
  RUN_CPGC_POINT_TEST_RESULT_METADATA *pMetadata;
  TEST_STATUS *pTestStat;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "CreateResultBuffer(ResultEleCnt=%u)\n"
    "  MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u\n",
    ResultEleCnt, MetadataSize, ResultElementSize, ResultEleCnt);
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer(
    SsaServicesHandle, MetadataSize, ResultElementSize,
    ResultEleCnt);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetMetadata()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetMetadata(
    SsaServicesHandle, ppMetadata);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  pMetadata = *ppMetadata;

  // zero the metadata
  ZeroMem (pMetadata, sizeof(*pMetadata));

  pMetadata->ErrCntOverflowVal = SPCL_ERR_CNT_OVERFLOW_VAL;

  // set the version information
  pMetadata->MajorVer = BSSA_TC_MAJOR_VER;
  pMetadata->MinorVer = BSSA_TC_MINOR_VER;
  pMetadata->RevVer = BSSA_TC_REV_VER;
  pMetadata->BuildVer = BSSA_TC_BUILD_VER;

  pTestStat = (TEST_STATUS*) &pMetadata->StatusCode;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetMetadataType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetMetadataType(
    SsaServicesHandle, &MetadataGuid);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetResultElementType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetResultElementType(
    SsaServicesHandle, &ColumnsGuid);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

  return 0;
} // end function SetupRunCpgcTestResults

/**
  This is the memory point test result handler function.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in]       pSystemInfo           Pointer to system information structure.
  @param[in]       pVoidTestData         Pointer to client test data.
  @param[in]       Socket                Socket tested.
  @param[in]       Controller            Controller tested.
  @param[in]       Channel               Channel tested.
  @param[in]       DimmRankA             First DIMM+Rank tested.
  @param[in]       DimmRankB             Second DIMM+Rank tested.
  @param[in]       RankCombinationIndex  Rank combination index tested.
  @param[in]       pLaneErrorStatus      Pointer to array of lane error status masks.  Number of array elements is SystemInfo.BusWidth/8
  @param[in]       ErrorCount            Channel error count.
  @param[out]      pTestStat             Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 
PointTestResultHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN VOID *pVoidTestData,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN DIMM_RANK_PAIR DimmRankA,
  IN DIMM_RANK_PAIR DimmRankB,
  IN UINT8 RankCombinationIndex,
  IN CONST UINT8 *pLaneErrorStatus,
  IN ERROR_COUNT_32BITS ErrorCount,
  OUT TEST_STATUS *pTestStat)
{
  SSA_STATUS RetStat;
  RUN_CPGC_POINT_TEST_RESULT_COLUMNS *pElement;
  RUN_CPGC_POINT_TEST_CONFIG *pTestCfg = (RUN_CPGC_POINT_TEST_CONFIG*) pVoidTestData;

  if (pTestCfg->TestStatusLogLevel < 5) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "point test result:\n"
      "  Socket=%u Controller=%u Channel=%u DimmA=%u RankA=%u DimmB=%u RankB=%u\n",
      Socket, Controller, Channel, DimmRankA.Dimm, DimmRankA.Rank, DimmRankB.Dimm, DimmRankB.Rank);
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  ErrorCount=%d Overflow=%u LaneErrorStatus=0x",
      ErrorCount.Bits.Count, ErrorCount.Bits.Overflow);
    LogByteArrayAsHex (pLaneErrorStatus, (pSystemInfo->BusWidth / 8));
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR, "\n");
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetNextResultElement()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement(
    SsaServicesHandle, &pElement);
  if (RetStat  != Success) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Insufficient result buffer elements available to complete test.\n");
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "Try increasing the StepSize and/or reducing the number of channels/DIMMs/ranks to test.\n");
    ProcError(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat);
    return 1;
  }

  pElement->Socket               = Socket;
  pElement->Controller           = Controller;
  pElement->Channel              = Channel;
  pElement->DimmA                = DimmRankA.Dimm;
  pElement->RankA                = DimmRankA.Rank;
  pElement->DimmB                = DimmRankB.Dimm;
  pElement->RankB                = DimmRankB.Rank;

  ZeroMem (pElement->LaneErrorStatus, sizeof(pElement->LaneErrorStatus));
  CopyMem (pElement->LaneErrorStatus, pLaneErrorStatus, (pSystemInfo->BusWidth / 8));

  if (ErrorCount.Bits.Overflow) {
    pElement->ErrorCount = SPCL_ERR_CNT_OVERFLOW_VAL;
  }
  else {
    pElement->ErrorCount = ErrorCount.Bits.Count;
  }

  return 0;
} // end function PointTestResultHandler

/**
@brief
  This function is the Run CPGC Point Test test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
RunCpgcPointTestEntryPoint (
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN RUN_CPGC_POINT_TEST_CONFIG *Config
  )
{
  RUN_CPGC_POINT_TEST_CONFIG *pTestCfg = Config;
  RUN_CPGC_POINT_TEST_RESULT_METADATA *pMetadata = NULL;
  MRC_SYSTEM_INFO SystemInfo;
  MRC_SERVER_CPGC_INFO CpgcInfo;
  MEM_CFG MemCfg;
  MEM_POINT_TEST_CONFIG MemPointTestCfg;
  CPGC_POINT_TEST_CONFIG CpgcPointTestCfg;
  TEST_DATA TestData;
  UINT32 ResultEleCnt;
  VOID *pPointTestData;
  LANE_FAIL_STATS LaneFailStats;
  TEST_STATUS *pTestStat;
  MRC_LT IoLevel = DdrLevel;
  UINT8 Socket, Controller, Channel, Dimm;
  MEMORY_TECHNOLOGY_TYPES MemoryTech;
  BOOLEAN IsSocketToBeTested;
  BOOLEAN IsControllerToBeTested;
  BOOLEAN IsChannelToBeTested;
  BOOLEAN IsDimmAToBeTested;
  BOOLEAN IsDimmBToBeTested;
  BOOLEAN IsFirstCall, InitDqdbContent, IsLastCall;
  UINT32 ExpectedRevision;

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Starting RunCpgcPointTest test v%u.%u.%u.%u\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  ZeroMem  (&TestData, sizeof(TestData));

  // get metadata with no columnar results just in case we have an error
  ResultEleCnt = 1;
  if (SetupRunCpgcTestResults(SsaServicesHandle, ResultEleCnt, &pMetadata)) {
    return EFI_UNSUPPORTED;
  }
  pTestStat = (TEST_STATUS*) &pMetadata->StatusCode;

  if (Config == NULL) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Config is NULL!\n");
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return EFI_UNSUPPORTED;
  }

#ifdef SSA_REVISION_BIOS
  ExpectedRevision = SSA_REVISION_BIOS;
#else // must be rev 1.0
  ExpectedRevision = 0x01000000;
#endif
  if (SsaServicesHandle->Revision != ExpectedRevision) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: BIOS-SSA API revision (0x%08X) not expected value (0x%08X)\n",
      SsaServicesHandle->Revision, ExpectedRevision);
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetSystemInfo()\n");
#endif
  SsaServicesHandle->SsaMemoryConfig->GetSystemInfo(SsaServicesHandle,
    &SystemInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogSystemInfo(SsaServicesHandle, &SystemInfo);
#endif
  if (ChkSystemInfo(SsaServicesHandle, &SystemInfo, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetCpgcInfo()\n");
#endif
  SsaServicesHandle->SsaMemoryServerConfig->GetCpgcInfo(SsaServicesHandle,
    &CpgcInfo);
#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcInfo(SsaServicesHandle, &CpgcInfo);
#endif
  if (ChkCpgcInfo(SsaServicesHandle, &CpgcInfo, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  if (GetMemCfg(SsaServicesHandle, &SystemInfo, &MemCfg, pTestStat)) {
    return EFI_UNSUPPORTED;
  }
#if ENBL_VERBOSE_LOG_MSGS
  LogMemCfg(SsaServicesHandle, &SystemInfo, &MemCfg);
#endif

#if ENBL_VERBOSE_LOG_MSGS
  LogRunCpgcTestCfg(SsaServicesHandle, pTestCfg);
#endif


  // start by setting everything to 0
  ZeroMem (&MemPointTestCfg, sizeof(MemPointTestCfg));

  // set default value
  MemPointTestCfg.TurnaroundsTestMode = AllRankTurnarounds;
  MemPointTestCfg.TurnaroundsPairMode = OrderIndependent;

  // IF this is the frontside
  if (pTestCfg->IoLevel == DdrLevel) {
#if SUPPORT_FOR_LRDIMM
    // IF the DIMMs are LRDIMMs
    if (MemCfg.IsLrDimms) {
      // only test between the first ranks of all DIMMs
      MemPointTestCfg.TurnaroundsTestMode = OnlyInterDimmTurnarounds;
    }
#endif // SUPPORT_FOR_LRDIMM
  }
  // ELSE this is the backside
  else {
    // only test between the ranks within each DIMM
    MemPointTestCfg.TurnaroundsTestMode = OnlyIntraDimmTurnarounds;
  }

  // use the given socket bitmask and screen out unpopulated sockets
  MemPointTestCfg.SocketBitmask = \
    pTestCfg->SocketBitmask & SystemInfo.SocketBitMask;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not to be tested THEN skip it
    if ((MemPointTestCfg.SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    IsSocketToBeTested = FALSE;

    // use the given controller bitmask and screen out unpopulated controllers
    MemPointTestCfg.ControllerBitmasks[Socket] = \
      pTestCfg->ControllerBitmasks[Socket] & MemCfg.ControllerBitmasks[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not to be tested THEN skip it
      if ((MemPointTestCfg.ControllerBitmasks[Socket] &
        (1 << Controller)) == 0) {
        continue;
      }

      IsControllerToBeTested = FALSE;

      // use the given channel bitmask and screen out unpopulated channels
      MemPointTestCfg.ChannelBitmasks[Socket][Controller] = \
        MemPointTestCfg.ChannelValBitmasks[Socket][Controller] = \
        pTestCfg->ChannelBitmasks[Socket][Controller] & \
        MemCfg.ChannelBitmasks[Socket][Controller];

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not to be tested THEN skip it
        if ((MemPointTestCfg.ChannelBitmasks[Socket][Controller] &
          (1 << Channel)) == 0) {
          continue;
        }

        IsChannelToBeTested = FALSE;

        MemPointTestCfg.RankTestModes[Socket][Controller][Channel] = \
          pTestCfg->RankTestModes[Socket][Controller][Channel];

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not populated THEN skip it
          if ((MemCfg.DimmBitmasks[Socket][Controller][Channel] &
            (1 << Dimm)) == 0) {
            continue;
          }

          IsDimmAToBeTested = FALSE;
          IsDimmBToBeTested = FALSE;

          MemoryTech = MemCfg.MemoryTech[Socket][Controller][Channel][Dimm];

          // IF this DIMM is set in the Margin1D test configuration DIMM A bitmask
          if (pTestCfg->DimmABitmasks[Socket][Controller][Channel] & (1 << Dimm)) {
            // add it to the memory point test configuration DIMM A bitmask
            MemPointTestCfg.DimmABitmasks[Socket][Controller][Channel] |= 1 << Dimm;

            // test all ranks that are in the Margin1D test configuration
            // rank A bitmask and present
            MemPointTestCfg.RankABitmasks[Socket][Controller][Channel][Dimm] = \
              pTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] & \
              ((1 << MemCfg.RankCounts[Socket][Controller][Channel][Dimm]) - 1);

#if SUPPORT_FOR_LRDIMM
            // IF we're testing the frontside and the DIMM is a LRDIMM
            if ((pTestCfg->IoLevel == DdrLevel) && MemCfg.IsLrDimms) {
              // only test the first available rank on this DIMM
              MemPointTestCfg.RankABitmasks[Socket][Controller][Channel][Dimm] = \
                1 << GetLowestBitSetInMask(
                MemPointTestCfg.RankABitmasks[Socket][Controller][Channel][Dimm],
                MAX_RANK_CNT);
            }
#endif // SUPPORT_FOR_LRDIMM
#if SUPPORT_FOR_DDRT
            // IF we're testing the frontside and the DIMM is a DDR-T
            if ((pTestCfg->IoLevel == DdrLevel) &&
              (MemoryTech == SsaMemoryDdrT)) {
              // only test the first available rank on this DIMM
              MemPointTestCfg.RankABitmasks[Socket][Controller][Channel][Dimm] = \
                1 << GetLowestBitSetInMask(
                MemPointTestCfg.RankABitmasks[Socket][Controller][Channel][Dimm],
                MAX_RANK_CNT);
            }
#endif // SUPPORT_FOR_DDRT
            // IF there are rank A's to be tested
            if (MemPointTestCfg.RankABitmasks[Socket][Controller][Channel][Dimm] != 0) {
              IsSocketToBeTested = TRUE;
              IsControllerToBeTested = TRUE;
              IsChannelToBeTested = TRUE;
              IsDimmAToBeTested = TRUE;
            }
          } // end if this DIMM is set in the Margin1D test configuration DIMM A bitmask

          // IF we're testing rank-to-rank turnarounds
          if (pTestCfg->RankTestModes[Socket][Controller][Channel] == Turnarounds) {
            // IF this DIMM is set in the Margin1D test configuration DIMM B bitmask
            if (pTestCfg->DimmBBitmasks[Socket][Controller][Channel] & (1 << Dimm)) {
              // add it to the memory point test configuration DIMM B bitmask
              MemPointTestCfg.DimmBBitmasks[Socket][Controller][Channel] |= 1 << Dimm;

              // test all ranks that are in the Margin1D test configuration
              // rank B bitmask and present
              MemPointTestCfg.RankBBitmasks[Socket][Controller][Channel][Dimm] = \
                pTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] & \
                ((1 << MemCfg.RankCounts[Socket][Controller][Channel][Dimm]) - 1);

#if SUPPORT_FOR_LRDIMM
              // IF we're testing the frontside and the DIMM is a LRDIMM
              if ((pTestCfg->IoLevel == DdrLevel) && MemCfg.IsLrDimms) {
                // only test the first available rank on this DIMM
                MemPointTestCfg.RankBBitmasks[Socket][Controller][Channel][Dimm] = \
                  1 << GetLowestBitSetInMask(
                  MemPointTestCfg.RankBBitmasks[Socket][Controller][Channel][Dimm],
                  MAX_RANK_CNT);
              }
#endif // SUPPORT_FOR_LRDIMM
#if SUPPORT_FOR_DDRT
              // IF we're testing the frontside and the DIMM is a DDR-T
              if ((pTestCfg->IoLevel == DdrLevel) &&
                (MemoryTech == SsaMemoryDdrT)) {
                // only test the first available rank on this DIMM
                MemPointTestCfg.RankBBitmasks[Socket][Controller][Channel][Dimm] = \
                  1 << GetLowestBitSetInMask(
                  MemPointTestCfg.RankBBitmasks[Socket][Controller][Channel][Dimm],
                  MAX_RANK_CNT);
              }
#endif // SUPPORT_FOR_DDRT
              // IF there are rank B's to be tested
              if (MemPointTestCfg.RankBBitmasks[Socket][Controller][Channel][Dimm] != 0) {
                IsSocketToBeTested = TRUE;
                IsControllerToBeTested = TRUE;
                IsChannelToBeTested = TRUE;
                IsDimmBToBeTested = TRUE;
              }
            } // end if this DIMM is set in the Margin1D test configuration DIMM B bitmask
          } // end if we're testing rank-to-rank turnarounds

          // IF it turns out that there were no ranks to test on this DIMMA
          if (!IsDimmAToBeTested) {
            // remove the DIMM
            MemPointTestCfg.DimmABitmasks[Socket][Controller][Channel] &= \
              ~(1 << Dimm);
          }

          // IF it turns out that there were no ranks to test on this DIMMB
          if (!IsDimmBToBeTested) {
            // remove the DIMM
            MemPointTestCfg.DimmBBitmasks[Socket][Controller][Channel] &= \
              ~(1 << Dimm);
          }
        } // end for each DIMM

#if SUPPORT_FOR_TURNAROUNDS
        // IF we're doing turnarounds and this channel can't do turnarounds
        if ((pTestCfg->RankTestModes[Socket][Controller][Channel] == Turnarounds) &&
          !CanChannelDoTurnarounds(SsaServicesHandle, &SystemInfo, &MemCfg,
          &MemPointTestCfg, pTestCfg->IoLevel, Socket, Controller, Channel,
          FALSE)) {
          // remove the channel
          IsChannelToBeTested = FALSE;
        }
#endif // SUPPORT_FOR_TURNAROUNDS

        // IF it turns out that there were no ranks to test on this channel
        if (!IsChannelToBeTested) {
          // remove the channel
          MemPointTestCfg.ChannelBitmasks[Socket][Controller] &= ~(1 << Channel);
          MemPointTestCfg.ChannelValBitmasks[Socket][Controller] &= ~(1 << Channel);
        }
      } // end for each channel

      // IF it turns out that there were no ranks to test on this controller
      if (!IsControllerToBeTested) {
        // remove the controller
        MemPointTestCfg.ControllerBitmasks[Socket] &= ~(1 << Controller);
      }
    } // end for each controller

    // IF it turns out that there were no ranks to test on this socket
    if (!IsSocketToBeTested) {
      // remove the socket
      MemPointTestCfg.SocketBitmask &= ~(1 << Socket);
    }
  } // end for each socket


  if ((sizeof(MemPointTestCfg.LaneValBitmasks) != sizeof(pTestCfg->LaneValBitmasks)) ||
    (sizeof(MemPointTestCfg.ChunkValBitmasks) != sizeof(pTestCfg->ChunkValBitmasks))) {
#if ENBL_VERBOSE_ERROR_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: MEM_POINT_TEST_CONFIG and RUN_CPGC_POINT_TEST_CONFIG elements do not match.\n");
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return EFI_UNSUPPORTED;
  }

  CopyMem (&MemPointTestCfg.LaneValBitmasks, &pTestCfg->LaneValBitmasks, sizeof(MemPointTestCfg.LaneValBitmasks));
  CopyMem (&MemPointTestCfg.ChunkValBitmasks, &pTestCfg->ChunkValBitmasks, sizeof(MemPointTestCfg.ChunkValBitmasks));

  if (ChkMemPointTestCfg(SsaServicesHandle, &SystemInfo, &MemCfg,
    &MemPointTestCfg, pTestStat)) {
      return EFI_UNSUPPORTED;
  }

#if ENBL_VERBOSE_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n\nMemPointTestCfg:\n");
  LogMemPointTestCfg(SsaServicesHandle, &SystemInfo, &MemPointTestCfg);
#endif

  // initialize the CPGC point test configuration to default values
  SetCpgcPointTestCfgDefaults(&CpgcPointTestCfg);

  if (UpdateCpgcPointTestCfgByRunCpgcPoinTestCfg (pTestCfg, &CpgcPointTestCfg)) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "ERROR: CPGC_POINT_TEST_CONFIG and RUN_CPGC_POINT_TEST_CONFIG elements do not match.\n");
#endif
    ProcError (SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return EFI_UNSUPPORTED;
  }

  if (ChkCpgcPointTestCfg(SsaServicesHandle, &SystemInfo, &CpgcInfo, &MemCfg,
    &MemPointTestCfg, &CpgcPointTestCfg, pTestStat)) {
    return EFI_UNSUPPORTED;
  }
#if ENBL_VERBOSE_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n\nCpgcPointTestCfg: \n");
  LogCpgcPointTestCfg(SsaServicesHandle, &CpgcPointTestCfg);
#endif

#if SUPPORT_FOR_DDRT
  // Set up the ERID test mode
  if (GetCpgcPointTestCfgErid (&CpgcPointTestCfg)){
    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // IF the socket is not enabled for testing THEN skip it
      if (!IsSocketTestingEnabled(SsaServicesHandle, &SystemInfo,
        &MemPointTestCfg, Socket)) {
        continue;
      }
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  SetAepDimmTrainingMode(Socket=%u Mode=%u)\n",
        Socket, FnvTrainModeEarlyRidCoarse);
#endif
      SsaServicesHandle->SsaMemoryDdrtConfig->SetAepDimmTrainingMode(
        SsaServicesHandle, Socket, FnvTrainModeEarlyRidCoarse);

      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        if (!IsControllerTestingEnabled(SsaServicesHandle, &SystemInfo, &MemCfg,
          &MemPointTestCfg, Socket, Controller)) {
            continue;
        }
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          if (!IsChannelTestingEnabled(SsaServicesHandle, &SystemInfo, &MemCfg,
            &MemPointTestCfg, Socket, Controller, Channel)) {
              continue;
          }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  SetEridTrainingConfig(Socket=%u Controller=%u Channel=%u )\n",
            Socket, Controller, Channel);
#endif
          SsaServicesHandle->SsaMemoryDdrtConfig->SetEridTrainingConfig(
            SsaServicesHandle, Socket, Controller, Channel, TRUE);
        } // end for each channel
      } // end for each controller
    } // Socket
  }
#endif // SUPPORT_FOR_DDRT

  // set up the platform
  InitPlatform(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Setup, pTestCfg->ScramblerOverrideMode, TestData.PrevScramblerEnables,
    TestData.PrevSelfRefreshEnables);

  InitRCompUpdate(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Setup, TestData.PrevRCompUpdateEnables);

  InitZQCal(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Setup, TestData.PrevZQCalEnables);

  InitPageOpen(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Setup, pTestCfg->PageOpenOverrideMode, TestData.PrevPageOpenEnables);

  IsFirstCall = TRUE;
  InitDqdbContent = TRUE;
  if (SetupCpgcPointTest(SsaServicesHandle, &SystemInfo, &CpgcInfo, &MemCfg,
    &MemPointTestCfg, &CpgcPointTestCfg, PointTestResultHandler, pTestCfg,
    IoLevel, FALSE, pTestCfg->TestStatusLogLevel, IsFirstCall,
    InitDqdbContent, &pPointTestData, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  ResultEleCnt = 0;
  // FOR each socket:
  for (Socket = 0; Socket < SystemInfo.MaxNumberSockets; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < SystemInfo.MaxNumberControllers; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < SystemInfo.MaxNumberChannels; Channel++) {
        if (!IsChannelValEnabled(SsaServicesHandle, &SystemInfo, &MemCfg,
          &MemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        ResultEleCnt++;
      } // end for each channel
    } // end for each controller
  } // end for each socket
  if (SetupRunCpgcTestResults(SsaServicesHandle, ResultEleCnt, &pMetadata)) {
    return EFI_UNSUPPORTED;
  }
  pTestStat = (TEST_STATUS*) &pMetadata->StatusCode;

  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle, &SystemInfo,
    &MemCfg, &MemPointTestCfg, &CpgcPointTestCfg, pPointTestData,
    pTestCfg->RankIterationIndex, FALSE, pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle, &SystemInfo,
    &MemCfg, &MemPointTestCfg, &CpgcPointTestCfg, pPointTestData,
    pTestCfg->RankIterationIndex, TRUE, pTestStat)) {
      return EFI_UNSUPPORTED;
  }

  if (RunCpgcPointTest(SsaServicesHandle, &SystemInfo, &MemCfg,
    &MemPointTestCfg, &CpgcPointTestCfg, pPointTestData, &LaneFailStats,
    pTestStat)) {
    return EFI_UNSUPPORTED;
  }

  // clean up the CPGC point test
  IsLastCall = TRUE;
  CleanupCpgcPointTest(SsaServicesHandle, &SystemInfo, &MemCfg,
    &MemPointTestCfg, IsLastCall, &pPointTestData);

  // clean up the platform
  InitPageOpen(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Cleanup, pTestCfg->PageOpenOverrideMode, TestData.PrevPageOpenEnables);

  InitZQCal(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Cleanup, TestData.PrevZQCalEnables);

  InitRCompUpdate(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Cleanup, TestData.PrevRCompUpdateEnables);

  InitPlatform(SsaServicesHandle, &SystemInfo, &MemCfg, &MemPointTestCfg,
    Cleanup, pTestCfg->ScramblerOverrideMode, TestData.PrevScramblerEnables,
    TestData.PrevSelfRefreshEnables);

#if SUPPORT_FOR_DDRT
  // Clean up the ERID test mode
  if (GetCpgcPointTestCfgErid (&CpgcPointTestCfg)){
    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // IF the socket is not enabled for testing THEN skip it
      if (!IsSocketTestingEnabled(SsaServicesHandle, &SystemInfo,
        &MemPointTestCfg, Socket)) {
        continue;
      }
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  SetAepDimmTrainingMode(Socket=%u Mode=%u)\n",
        Socket, FnvTrainModeLateCmdCk);
#endif
      SsaServicesHandle->SsaMemoryDdrtConfig->SetAepDimmTrainingMode(
        SsaServicesHandle, Socket, FnvTrainModeLateCmdCk);

      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        if (!IsControllerTestingEnabled(SsaServicesHandle, &SystemInfo, &MemCfg,
          &MemPointTestCfg, Socket, Controller)) {
            continue;
        }
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          if (!IsChannelTestingEnabled(SsaServicesHandle, &SystemInfo, &MemCfg,
            &MemPointTestCfg, Socket, Controller, Channel)) {
              continue;
          }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  SetEridTrainingConfig(Socket=%u Controller=%u Channel=%u )\n",
            Socket, Controller, Channel);
#endif
          SsaServicesHandle->SsaMemoryDdrtConfig->SetEridTrainingConfig(
            SsaServicesHandle, Socket, Controller, Channel, FALSE);
        } // end for each channel
      } // end for each controller
    } // Socket
  }
#endif // SUPPORT_FOR_DDRT

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RunCpgcPointTest test v%u.%u.%u.%u complete\n",
    BSSA_TC_MAJOR_VER, BSSA_TC_MINOR_VER, BSSA_TC_REV_VER, BSSA_TC_BUILD_VER);

  return EFI_SUCCESS;
} // end function RunCpgcTestEntryPoint


// end file RunCpgcPointTest.c
