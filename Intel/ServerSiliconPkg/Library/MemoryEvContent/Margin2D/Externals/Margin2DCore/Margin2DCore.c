/** @file
Margin2DCore.c

Common core Margin2D algorithms.

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

#include "ChkRetStat.h"
#include "CreateMaxResultBuffer.h"
#include "MarginParam.h"
#include "Margin2D.h" // for BSSA_TC_MAJOR_VER, etc.
#include "Margin2DCore.h"
#include "EvContentInternal.h"
#include "Margin2DSweep.h"

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

/**
@brief
  This function checks the test configuration for correctness/consistency.
  It also fills in the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkMargin2DTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData)
{
  UINT8 TargetSocket, TargetController, TargetChannel;
  UINT8 TargetDimmA, TargetRankA, TargetDimmB, TargetRankB;
  UINT8 Socket, Controller, Channel, Dimm;
  CONST MARGIN_2D_CONFIG *pTestCfg = pCoreTestData->pTestCfg;
  MEM_POINT_TEST_CONFIG *pMemPointTestCfg = &pCoreTestData->MemPointTestCfg;
  CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg = &pCoreTestData->CpgcPointTestCfg;
  MEM_SHMOO_CONFIG *pMemShmooCfg = &pCoreTestData->MemShmooCfg;
  CHAR8 *FastGroupString = NULL;
  CHAR8 *SlowGroupString = NULL;


  TargetSocket = pTestCfg->TargetSocket;
  TargetController = pTestCfg->TargetController;
  TargetChannel = pTestCfg->TargetChannel;
  TargetDimmA = pTestCfg->TargetDimmA;
  TargetRankA = pTestCfg->TargetRankA;
  TargetDimmB = pTestCfg->TargetDimmB;
  TargetRankB = pTestCfg->TargetRankB;

  // make sure the target is populated
  if ((pCoreTestData->SystemInfo.SocketBitMask & (1UL << TargetSocket)) !=
    (1UL << TargetSocket)) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Requested TargetSocket=%u not populated SocketBitmask=0x%X\n",
      TargetSocket, pCoreTestData->SystemInfo.SocketBitMask);
    ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  if ((pCoreTestData->MemCfg.ControllerBitmasks[TargetSocket] &
    (1 << TargetController)) != (1 << TargetController)) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Requested TargetController=%u not populated ControllerBitmask=0x%X\n",
      TargetController,
      pCoreTestData->MemCfg.ControllerBitmasks[TargetSocket]);
    ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  if ((pCoreTestData->MemCfg.ChannelBitmasks[TargetSocket][TargetController] &
    (1 << TargetChannel)) != (1 << TargetChannel)) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Requested TargetChannel=%u not populated ChannelBitmask=0x%X\n",
      TargetChannel,
      pCoreTestData->MemCfg.ChannelBitmasks[TargetSocket][TargetController]);
    ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  if ((pCoreTestData->MemCfg.DimmBitmasks[TargetSocket][TargetController] \
    [TargetChannel] & (1 << TargetDimmA)) != (1 << TargetDimmA)) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Requested TargetDimmA=%u not populated DimmBitmask=0x%X\n",
      TargetDimmA, pCoreTestData->MemCfg.DimmBitmasks[TargetSocket] \
      [TargetController][TargetChannel]);
    ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  if (pCoreTestData->MemCfg.RankCounts[TargetSocket][TargetController] \
    [TargetChannel][TargetDimmA] <= TargetRankA) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Requested TargetRankA=%u not populated RankCount=%u\n",
      TargetRankA, pCoreTestData->MemCfg.RankCounts[TargetSocket] \
      [TargetController][TargetChannel][TargetDimmA]);
    ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  // IF the target channel is doing rank-to-rank turnarounds
  if (pTestCfg->RankTestModes[TargetController][TargetChannel] == Turnarounds) {
    if ((pCoreTestData->MemCfg.DimmBitmasks[TargetSocket][TargetController] \
      [TargetChannel] & (1 << TargetDimmB)) != (1 << TargetDimmB)) {
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Requested TargetDimmB=%u not populated DimmBitmask=0x%X\n",
        TargetDimmB, pCoreTestData->MemCfg.DimmBitmasks[TargetSocket] \
        [TargetController][TargetChannel]);
      ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
      return 1;
    }

    if (pCoreTestData->MemCfg.RankCounts[TargetSocket][TargetController] \
      [TargetChannel][TargetDimmB] <= TargetRankB) {
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Requested TargetRankB=%u not populated RankCount=%u\n",
        TargetRankB, pCoreTestData->MemCfg.RankCounts[TargetSocket] \
        [TargetController][TargetChannel][TargetDimmB]);
      ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
      return 1;
    }
  } // end if the target channel is doing rank-to-rank turnarounds

  // the fast margin parameter cannot be command or control timing - partially
  // because on the backside, the range can be different for the different
  // shmoo directions sides requiring a call to the InitMarginParam function
  // and resultant retraining operations by the BIOS which would be incredibly
  // inefficient - also, for the Margin2D design where MemShmoo manages the
  // fast margin parameter but MemShmoo's calling of InitMarginParam is
  // inhibited (see the DisableInitMarginParam field in the MEM_SHMOO_CONFIG
  // structure), the fast margin parameter's range changes depending on the
  // shmoo direction which would potentially require a reassessment of the
  // OffsetTestStatBfrSize field value of the MEM_SHMOO_TEST_DATA structure and
  // a reallocation of the related pOffsetTestStat field value - the current
  // design doesn't support that.
  if (EFI_ERROR (GetGroupString (pTestCfg->FastMarginGroup, &FastGroupString))) {
    FastGroupString = "Unknown group";
  }
  if (pTestCfg->IoLevel == LrbufLevel){
    if ((pTestCfg->FastMarginGroup == CmdAll) ||
      (pTestCfg->FastMarginGroup == CtlAll)) {
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: FastMarginGroup=%u (%s) not supported ",
        pTestCfg->FastMarginGroup, FastGroupString);
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "(try swapping with the SlowMarginGroup value)\n");
      ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
      return 1;
    }
  }

  if (sizeof(pCoreTestData->CoreTestCfg.LaneValBitmasks) !=
    sizeof(pTestCfg->LaneValBitmasks)) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: MARGIN2D_CORE_CONFIG and MARGIN_2D_CONFIG elements do not match.\n");
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  ZeroMem (&pCoreTestData->CoreTestCfg, sizeof(pCoreTestData->CoreTestCfg));
  pCoreTestData->CoreTestCfg.IoLevel = pTestCfg->IoLevel;
  pCoreTestData->CoreTestCfg.SlowMarginGroup = pTestCfg->SlowMarginGroup;
  pCoreTestData->CoreTestCfg.FastMarginGroup = pTestCfg->FastMarginGroup;
  pCoreTestData->CoreTestCfg.SlowStepSize = pTestCfg->SlowStepSize;
  pCoreTestData->CoreTestCfg.FastStepSize = pTestCfg->FastStepSize;
  pCoreTestData->CoreTestCfg.RankTestMode = pTestCfg->RankTestModes[TargetController][TargetChannel];
  pCoreTestData->CoreTestCfg.Socket = TargetSocket;
  pCoreTestData->CoreTestCfg.Controller = TargetController;
  pCoreTestData->CoreTestCfg.Channel = TargetChannel;
  pCoreTestData->CoreTestCfg.DimmA = TargetDimmA;
  pCoreTestData->CoreTestCfg.RankA = TargetRankA;
  pCoreTestData->CoreTestCfg.DimmB = TargetDimmB;
  pCoreTestData->CoreTestCfg.RankB = TargetRankB;
  CopyMem (&pCoreTestData->CoreTestCfg.LaneValBitmasks,
    &pTestCfg->LaneValBitmasks,
    sizeof(pCoreTestData->CoreTestCfg.LaneValBitmasks));
  pCoreTestData->CoreTestCfg.EnableMarginPlot = pTestCfg->EnableMarginPlot;
  pCoreTestData->CoreTestCfg.TestStatusLogLevel = pTestCfg->TestStatusLogLevel;

//#if SUPPORT_FOR_DDRT
  // IF the target DIMM is DDR-T
  if (pCoreTestData->MemCfg.MemoryTech[TargetSocket][TargetController] \
    [TargetChannel][TargetDimmA] == SsaMemoryDdrT) {
    pCoreTestData->CoreTestCfg.IsDdrT = TRUE;
  }

  // IF the target DIMM is not DDR-T and the margin parameter is DDR-T specific
  if (!pCoreTestData->CoreTestCfg.IsDdrT && 
    (IsMarginGroupDdrTSpecific(pCoreTestData->pTestCfg->FastMarginGroup) ||
    IsMarginGroupDdrTSpecific(pCoreTestData->pTestCfg->SlowMarginGroup))) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Margin parameter is DDR-T specific and target DIMM is not DDR-T.\n");
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  // IF either of the margin groups are for Early-Read-ID and the I/O level is
  // not the frontside
  if ((IsMarginGroupForErid(pCoreTestData->pTestCfg->FastMarginGroup) ||
    IsMarginGroupForErid(pCoreTestData->pTestCfg->SlowMarginGroup)) &&
    (pCoreTestData->pTestCfg->IoLevel != DdrLevel)) {
    
    if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->FastMarginGroup, &FastGroupString))) {
      FastGroupString = "Unknown group";
    }
    if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->SlowMarginGroup, &SlowGroupString))) {
      SlowGroupString = "Unknown group";
    }
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: MarginGroup=%s and/or %s only applies to IoLevel=%s.\n",
      FastGroupString,
      SlowGroupString,
      GetLevelStr(DdrLevel));

    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

  // IF the target channel is doing rank-to-rank turnarounds
  if (pTestCfg->RankTestModes[TargetController][TargetChannel] == Turnarounds) {
    BOOLEAN IsDdrT = FALSE;

    // IF the target device is DDR-T
    if (pCoreTestData->MemCfg.MemoryTech[TargetSocket][TargetController] \
      [TargetChannel][TargetDimmB] == SsaMemoryDdrT) {
      IsDdrT = TRUE;
    }

    // IF the DIMMs are not the same type
    if (IsDdrT != pCoreTestData->CoreTestCfg.IsDdrT) {
#if ENBL_VERBOSE_ERROR_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Target DIMMs are not the same type.\n");
#endif
      ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
      return 1;
    }
  } // end if the target channel is doing rank-to-rank turnarounds
//#endif // SUPPORT_FOR_DDRT

  // initialize the memory point test configuration to default values
  SetMemPointTestCfgDefaults(&pCoreTestData->SystemInfo,
    &pCoreTestData->MemPointTestCfg);

  // set only the target device for validation
  ZeroMem (&pCoreTestData->MemPointTestCfg.ChannelValBitmasks,
    sizeof(pCoreTestData->MemPointTestCfg.ChannelValBitmasks));
  pCoreTestData->MemPointTestCfg.ChannelValBitmasks[TargetSocket][TargetController] = \
    1 << TargetChannel;

  //initialize masks and rank combinations
  pMemPointTestCfg->SocketBitmask = pTestCfg->SocketBitmask & \
    pCoreTestData->SystemInfo.SocketBitMask;

  // FOR each socket
  for (Socket = 0; Socket < pCoreTestData->SystemInfo.MaxNumberSockets; Socket++) {
    pMemPointTestCfg->ControllerBitmasks[Socket] = \
      pTestCfg->ControllerBitmask & \
      pCoreTestData->MemCfg.ControllerBitmasks[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < pCoreTestData->SystemInfo.MaxNumberControllers; Controller++) {
      pMemPointTestCfg->ChannelBitmasks[Socket][Controller] = \
        pTestCfg->ChannelBitmasks[Controller] & \
        pCoreTestData->MemCfg.ChannelBitmasks[Socket][Controller];

      // FOR each channel:
      for (Channel = 0; Channel < pCoreTestData->SystemInfo.MaxNumberChannels; Channel++) {
        pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] = \
          pCoreTestData->MemCfg.DimmBitmasks[Socket][Controller][Channel];

        pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] = \
          pTestCfg->RankTestModes[Controller][Channel];

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
#if SUPPORT_FOR_DDRT
          {
            MEMORY_TECHNOLOGY_TYPES MemoryTech;
            BOOLEAN IsDdrT;

            // get the DIMM's memory technology
            MemoryTech = pCoreTestData->MemCfg.MemoryTech[Socket][Controller][Channel][Dimm];

            IsDdrT = (MemoryTech == SsaMemoryDdrT) ? TRUE : FALSE;

            // IF this DIMM's technology differs from the target DIMM THEN remove it
            if (IsDdrT != pCoreTestData->CoreTestCfg.IsDdrT) {
              pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] &= ~(1 << Dimm);
              continue;
            }
          }
#endif // SUPPORT_FOR_DDRT

          pMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] = \
            (1 << pCoreTestData->MemCfg.RankCounts[Socket][Controller][Channel][Dimm]) - 1;
        } // end for each DIMM

#if SUPPORT_FOR_DDRT
        // IF this channel contains no DIMMs THEN remove it
        if (pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] == 0) {
          pMemPointTestCfg->ChannelBitmasks[Socket][Controller] &= ~(1 << Channel);
        }
#endif // SUPPORT_FOR_DDRT
      } // end for each channel

#if SUPPORT_FOR_DDRT
      // IF this controller contains no channels THEN remove it
      if (pMemPointTestCfg->ChannelBitmasks[Socket][Controller] == 0) {
        pMemPointTestCfg->ControllerBitmasks[Socket] &= ~(1 << Controller);
      }
#endif // SUPPORT_FOR_DDRT
    } // end for each controller

#if SUPPORT_FOR_DDRT
    // IF this socket contains no controllers THEN remove it
    if (pMemPointTestCfg->ControllerBitmasks[Socket] == 0) {
      pMemPointTestCfg->SocketBitmask &= ~(1 << Socket);
    }
#endif // SUPPORT_FOR_DDRT
  } // end for each socket

  // preset the validation bitmasks to 0
  ZeroMem (&pMemPointTestCfg->LaneValBitmasks, sizeof(pMemPointTestCfg->LaneValBitmasks));
  ZeroMem (&pMemPointTestCfg->ChunkValBitmasks, sizeof(pMemPointTestCfg->ChunkValBitmasks));

  // for the socket, controller, and channel bitmasks, make sure the target's bits are set
  pMemPointTestCfg->SocketBitmask |= 1 << TargetSocket;
  pMemPointTestCfg->ControllerBitmasks[TargetSocket] |= \
    1 << TargetController;
  pMemPointTestCfg->ChannelBitmasks[TargetSocket][TargetController] |= \
    1 << TargetChannel;

  // for the DIMM and rank bitmasks, make sure the target's bits are the only bit set
  pMemPointTestCfg->DimmABitmasks[TargetSocket][TargetController] \
    [TargetChannel] = 1 << TargetDimmA;
  pMemPointTestCfg->DimmBBitmasks[TargetSocket][TargetController] \
    [TargetChannel] = 1 << TargetDimmB;
  pMemPointTestCfg->RankABitmasks[TargetSocket][TargetController] \
    [TargetChannel][TargetDimmA] = 1 << TargetRankA;
  pMemPointTestCfg->RankBBitmasks[TargetSocket][TargetController] \
    [TargetChannel][TargetDimmB] = 1 << TargetRankB;

  // set the validation bitmasks
#if SUPPORT_FOR_DDRT
  // IF either of the margin groups are for Early-Read-ID
  if (IsMarginGroupForErid(pCoreTestData->pTestCfg->FastMarginGroup) ||
    IsMarginGroupForErid(pCoreTestData->pTestCfg->SlowMarginGroup)) {
#ifdef __STUB__
      assert(MAX_ERID_SIGNAL_CNT <= 8);
#endif // __STUB__
      pMemPointTestCfg->LaneValBitmasks[TargetController][TargetChannel][0] = \
        ((1 << MAX_ERID_SIGNAL_CNT) - 1) & \
        pCoreTestData->pTestCfg->LaneValBitmasks[0];
  }
  // ELSE neither of the margin groups is for Early-Read-ID
  else
#endif // SUPPORT_FOR_DDRT
  {
    CopyMem (
      &pMemPointTestCfg->LaneValBitmasks[TargetController][TargetChannel],
      &pTestCfg->LaneValBitmasks,
      sizeof(pMemPointTestCfg->LaneValBitmasks[0][0]));
  }
  pMemPointTestCfg->ChunkValBitmasks[TargetController][TargetChannel] = \
    pTestCfg->ChunkValBitmask;

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // IF the margin parameter is on the backside
  if (pTestCfg->IoLevel == LrbufLevel) {
    if ((pTestCfg->FastMarginGroup == CmdAll) ||
      (pTestCfg->FastMarginGroup == CtlAll)) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "ERROR: When IoLevel is LrbufLevel, FastMarginGroup cannot be CmdAll or CtlAll.\n");
#endif
        ProcError(SsaServicesHandle, NotAvailable, __FILE__, __LINE__, pCoreTestData->pTestStat);
        return 1;
    }
  } // end if the margin parameter is on the backside

  // IF the margin parameter is a backside command THEN remove devices that do
  // not support backside command margining
  if ((pTestCfg->IoLevel == LrbufLevel) &&
    ((pTestCfg->FastMarginGroup == CmdVref) ||
    (pTestCfg->SlowMarginGroup == CmdVref) ||
    (pTestCfg->SlowMarginGroup == CmdAll) ||
    (pTestCfg->SlowMarginGroup == CtlAll))) {
    MRC_GT MarginGroups[2];
    UINT8 MarginGroupCount = 2;
    UINT16 BacksideCmdBitmasks[MAX_SOCKET_CNT];

    MarginGroups[0] = pTestCfg->FastMarginGroup;
    MarginGroups[1] = pTestCfg->SlowMarginGroup;

    InitBacksideCmdBitmask(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, pMemPointTestCfg, MarginGroups, MarginGroupCount,
      BacksideCmdBitmasks);

    RemoveNonBacksideCmdDimms(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, BacksideCmdBitmasks, pMemPointTestCfg);
  } // end if the margin parameter is a backside command
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

#if ENBL_VERBOSE_LOG_MSGS
  LogMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo, pMemPointTestCfg);
#endif
  if (ChkMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pMemPointTestCfg, pCoreTestData->pTestStat)) {
    return 1;
  }

  // initialize the CPGC test configuration to default values
  SetCpgcPointTestCfgDefaults(pCpgcPointTestCfg);

  if (UpdateCpgcPointTestCfgByMargin2DCfg (pTestCfg, pCpgcPointTestCfg)) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "ERROR: CPGC_POINT_TEST_CONFIG and MARGIN_2D_CONFIG elements do not match.\n");
#endif
    ProcError (SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

//
//  // make sure that the target channel's traffic mode is loopback
//  pCpgcPointTestCfg->TrafficModes[TargetController][TargetChannel] = LoopbackTrafficMode;
//
#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcPointTestCfg(SsaServicesHandle, pCpgcPointTestCfg);
#endif
  if (ChkCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg, pMemPointTestCfg,
    pCpgcPointTestCfg, pCoreTestData->pTestStat)) {
    return 1;
  }

  // initialize the memory shmoo test configuration to default values
  SetMemShmooTestCfgDefaults(pMemShmooCfg);

  pMemShmooCfg->IoLevel = pTestCfg->IoLevel;
  pMemShmooCfg->MarginGroup = pTestCfg->FastMarginGroup;
#if SUPPORT_FOR_DDRT
  pMemShmooCfg->IsDdrT = pCoreTestData->CoreTestCfg.IsDdrT;
#endif // SUPPORT_FOR_DDRT
  pMemShmooCfg->StepSize = pTestCfg->FastStepSize;
  pMemShmooCfg->GuardBandSize = pTestCfg->GuardBandSize;
  pMemShmooCfg->LaneShmooStopMode = AnyShmooStopMode;
  pMemShmooCfg->RankShmooStopMode = AnyShmooStopMode;
  pMemShmooCfg->ChannelShmooStopMode = AnyShmooStopMode;
  pMemShmooCfg->ControllerShmooStopMode = AnyShmooStopMode;
  pMemShmooCfg->SocketShmooStopMode = AnyShmooStopMode;
  pMemShmooCfg->EnableOptimizedShmoo = TRUE;
  pMemShmooCfg->DisableInitMarginParam = TRUE;
  pMemShmooCfg->StopChannelMarginOnStopCond = FALSE;

  pMemShmooCfg->EnableIncrementalStepping = \
    IsIncrementalSteppingRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg, pMemPointTestCfg,
    pTestCfg->IoLevel, pTestCfg->FastMarginGroup) || \
    IsIncrementalSteppingRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg, pMemPointTestCfg,
    pTestCfg->IoLevel, pTestCfg->SlowMarginGroup);

  if (IsJedecInitAfterErrRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg, pMemPointTestCfg,
    pTestCfg->IoLevel, pTestCfg->FastMarginGroup) ||
    IsJedecInitAfterErrRequired(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pMemPointTestCfg, pTestCfg->IoLevel,
    pTestCfg->SlowMarginGroup)) {
    pMemShmooCfg->EnableDimmInitAfterErr = TRUE;
    pMemShmooCfg->DimmInitMode = Jedec;
  }
  else if (IsIoResetAfterErrRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg, pMemPointTestCfg,
    pTestCfg->IoLevel, pTestCfg->FastMarginGroup) ||
    IsIoResetAfterErrRequired(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pMemPointTestCfg, pTestCfg->IoLevel,
    pTestCfg->SlowMarginGroup)) {
    pMemShmooCfg->EnableDimmInitAfterErr = TRUE;
    pMemShmooCfg->DimmInitMode = IoReset;
  }
  else {
    pMemShmooCfg->EnableDimmInitAfterErr = FALSE;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogMemShmooTestCfg(SsaServicesHandle, pMemShmooCfg);
#endif
  if (ChkMemShmooTestCfg(SsaServicesHandle, pMemShmooCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function ChkMargin2DTestCfg



/**
@brief
  This function is used to detect if there is any lane fail in the result element .

  @param[in]  pElement      Pointer to result element data structure.

  @retval  TRUE or FALSE
**/
static BOOLEAN IsAnyFailInElement( 
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN MARGIN2D_RESULT_COLUMNS *pElement)
{
  UINT8 i;
  BOOLEAN Ret = FALSE;

  for (i = 0; i < (pSystemInfo->BusWidth / 8) ; i ++)
  {
    if (pElement->LaneErrorStatus[i] > 0){
      Ret = TRUE;
      break;
    }
  } // i
  return Ret;
}


/**
@brief
  This function is used to plot the margin results.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.

  @retval  Nothing
**/
static VOID PlotMarginResults(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData)
{
  UINT8 i;
  UINT32 EleIdx;
  MARGIN2D_RESULT_COLUMNS *pElement = NULL;
  BOOLEAN PointFound;
  INT16 SlowOffset, MinSlowOffset, MaxSlowOffset;
  INT16 FastOffset, MinFastOffset, MaxFastOffset;
  CHAR8 *FastGroupString = NULL;
  CHAR8 *SlowGroupString = NULL;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "PlotMarginResults()\n");
#endif

  if (EFI_ERROR (GetGroupString (pCoreTestData->CoreTestCfg.FastMarginGroup, &FastGroupString))) {
    FastGroupString = "Unknown group";
  }
  if (EFI_ERROR (GetGroupString (pCoreTestData->CoreTestCfg.SlowMarginGroup, &SlowGroupString))) {
    SlowGroupString = "Unknown group";
  }
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n");
  // print the legend
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Plot Legend:\n");
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  vertical axis = %s\n", FastGroupString);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  horizontal axis = %s\n", SlowGroupString);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  \"x\" = first fail\n");
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n");

  // get the min/max offset values
  MinSlowOffset = pCoreTestData->SlowMaxOffset;
  MaxSlowOffset = pCoreTestData->SlowMinOffset;
  MinFastOffset = pCoreTestData->FastMaxOffset;
  MaxFastOffset = pCoreTestData->FastMinOffset;

  // FOR each result element:
  for (EleIdx = 0; EleIdx < pCoreTestData->ResultElementCount; EleIdx++) {
    pElement = &pCoreTestData->pResultElements[EleIdx];

    if (pElement->SlowOffset < MinSlowOffset) {
      MinSlowOffset = pElement->SlowOffset;
    }

    if (pElement->SlowOffset > MaxSlowOffset) {
      MaxSlowOffset = pElement->SlowOffset;
    }

    if (pElement->FastOffset < MinFastOffset) {
      MinFastOffset = pElement->FastOffset;
    }

    if (pElement->FastOffset > MaxFastOffset) {
      MaxFastOffset = pElement->FastOffset;
    }
  } // end for each result element

  // print the first header row
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    " ");
  for (i = 0; i < (ABS(MinSlowOffset) % 5); i++) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      " ");
  }
  for (SlowOffset = MinSlowOffset; SlowOffset <= MaxSlowOffset; SlowOffset++) {
    if ((SlowOffset % 5) == 0) {
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "%5d", SlowOffset);
    }
  }
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n");

  // print the second header row
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "     ");
  for (SlowOffset = MinSlowOffset; SlowOffset <= MaxSlowOffset; SlowOffset++) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      (SlowOffset % 5) ? "." : "|");
  }
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n");

  // FOR each fast offset from the highest to the lowest:
  for (FastOffset = MaxFastOffset; FastOffset >= MinFastOffset; FastOffset--) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "%4d ", FastOffset);

    // FOR each slow offset from the lowest to the highest:
    for (SlowOffset = MinSlowOffset; SlowOffset <= MaxSlowOffset; SlowOffset++) {
      PointFound = FALSE;
      // FOR each result element:
      for (EleIdx = 0; EleIdx < pCoreTestData->ResultElementCount; EleIdx++) {
        pElement = &pCoreTestData->pResultElements[EleIdx];

        if ((pElement->SlowOffset == SlowOffset) &&
          (pElement->FastOffset == FastOffset)) {
          PointFound = TRUE;
          break;
        }
      } // end for each result element

      // Print "x" if there was any any failed at corresponding (slow, fast) offset.
      // Otherwise print " "
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        (PointFound && pElement && IsAnyFailInElement(&pCoreTestData->SystemInfo, pElement)) ? "x" : " ");

    } // end for each slow offset

    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "\n");
  } // end for each fast offset
} // end function PlotMarginResults

/**
@brief
  This is the definition for the point test result handler function.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.
  @param[in]       FastOffset         Fast margin parameter offset value.
  @param[in]       SlowOffset         Slow margin parameter offset value.
  @param[in]       pLaneErrorStatus   Pointer to array of lane error status masks.  Number of array elements is SystemInfo.BusWidth/8

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 AppendResultElement(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData,
  IN INT16 FastOffset,
  IN INT16 SlowOffset,
  IN UINT8 *pLaneErrorStatus)
{
  SSA_STATUS RetStat;
  MARGIN2D_RESULT_COLUMNS *pElement;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "AppendResultElement(FastOffset=%d SlowOffset=%d LaneErrorStatus=0x",
    FastOffset, SlowOffset);
  LogByteArrayAsHex(pLaneErrorStatus,
    (pCoreTestData->SystemInfo.BusWidth / 8));
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR, ")\n");
#endif
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  GetNextResultElement()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement(
    SsaServicesHandle, &pElement);
  if (RetStat  != Success) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: Insufficient result buffer elements available to complete test.\n");
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "Try increasing the StepSize.\n");
    ProcError(SsaServicesHandle, RetStat, __FILE__, __LINE__,
      pCoreTestData->pTestStat);
    return 1;
  }

  pElement->FastOffset  = FastOffset;
  pElement->SlowOffset  = SlowOffset;
  CopyMem (pElement->LaneErrorStatus, pLaneErrorStatus, (pCoreTestData->SystemInfo.BusWidth / 8));

  // IF this is the first element
  if (pCoreTestData->ResultElementCount == 0) {
    pCoreTestData->pResultElements = pElement;
  }

  pCoreTestData->ResultElementCount++;

  return 0;
} // end function AppendResultElement

/**
@brief
  This function is used to set the margin parameter to the given offset.  This
  function also waits the given settling delay amount before returning.  The
  given current offset variable is updated to the new offset.

  Note that the margin parameter is only applied to logical ranks that are
  enabled for validation.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.
  @param[in]       MarginGroup        Margin group.
  @param[in, out]  pCurrentOffset     Pointer to current offset.
  @param[in]       NewOffset          New offset.
  @param[in]       EnableIncrementalStepping  Flag to enable incremental stepping.
  @param[in]       SettleDelay        Delay required (in microseconds) for the margin parameter to settle.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetMarginParam(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData,
  IN MRC_GT MarginGroup,
  IN INT16 *pCurrentOffset,
  IN INT16 NewOffset,
  IN UINT16 SettleDelay,
  IN BOOLEAN EnableIncrementalStepping)
{
  SSA_STATUS RetStat;
  INT16 StepSize;
  UINT8 StepNumber, StepCount;
  CHAR8 *GroupString = NULL;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetMarginParam(CurrentOffset=%d NewOffset=%d SettleDelay=%d EnableIncrementalStepping=%u)\n",
    *pCurrentOffset, NewOffset, SettleDelay, EnableIncrementalStepping);
#else
  if (EFI_ERROR (GetGroupString (MarginGroup, &GroupString))) {
    GroupString = "Unknown group";
  }
  // IF the new offset is different from the current offset
  if (*pCurrentOffset != NewOffset) {
    if (pCoreTestData->CoreTestCfg.TestStatusLogLevel >= 4) {
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "\nMoving IoLevel=%s MarginGroup=%s from %d to %d\n",
        GetLevelStr(pCoreTestData->CoreTestCfg.IoLevel),
        GroupString, *pCurrentOffset, NewOffset);
    }
  } // end if the new offset is different from the current offset
#endif

  // IF the new offset is different from the current offset
  if (*pCurrentOffset != NewOffset) {
    if (EnableIncrementalStepping) {
      // for command/address parameters make multiple steps in increments of one
      if (NewOffset > *pCurrentOffset) {
        StepSize = 1;
      }
      else {
        StepSize = -1;
      }

      StepCount = (UINT8) ABS(NewOffset - *pCurrentOffset);
    }
    else {
      // for other parameters, move the full distance in one step
      StepSize = NewOffset - *pCurrentOffset;
      StepCount = 1;
    }

    // FOR each step:
    for (StepNumber = 0; StepNumber < StepCount; StepNumber++) {
      NewOffset = *pCurrentOffset + StepSize;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  SetMarginParamOffset(Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n",
        pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.Controller,
        pCoreTestData->CoreTestCfg.Channel, pCoreTestData->CoreTestCfg.DimmA,
        pCoreTestData->CoreTestCfg.RankA);
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "    LaneMasks=0x");
      LogByteArrayAsHex(
        pCoreTestData->CoreTestCfg.LaneValBitmasks,
        (pCoreTestData->SystemInfo.BusWidth / 8));
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        " IoLevel=%u MarginGroup=%u CurrentOffset=%d NewOffset=%d)\n",
        pCoreTestData->CoreTestCfg.IoLevel, MarginGroup, *pCurrentOffset, NewOffset);
#endif
      RetStat = SsaServicesHandle->SsaMemoryConfig->SetMarginParamOffset(
        SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
        pCoreTestData->CoreTestCfg.Controller, pCoreTestData->CoreTestCfg.Channel,
        pCoreTestData->CoreTestCfg.DimmA, pCoreTestData->CoreTestCfg.RankA,
        pCoreTestData->CoreTestCfg.LaneValBitmasks,
        pCoreTestData->CoreTestCfg.IoLevel, MarginGroup, *pCurrentOffset, NewOffset);
      if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
        pCoreTestData->pTestStat)) {
        return 1;
      }

      if (pCoreTestData->CoreTestCfg.RankTestMode == Turnarounds) {
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  SetMarginParamOffset(Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n",
          pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.Controller,
          pCoreTestData->CoreTestCfg.Channel, pCoreTestData->CoreTestCfg.DimmB,
          pCoreTestData->CoreTestCfg.RankB);
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "    LaneMasks=0x");
        LogByteArrayAsHex(
          pCoreTestData->CoreTestCfg.LaneValBitmasks,
          (pCoreTestData->SystemInfo.BusWidth / 8));
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          " IoLevel=%u MarginGroup=%u CurrentOffset=%d NewOffset=%d)\n",
          pCoreTestData->CoreTestCfg.IoLevel, MarginGroup, *pCurrentOffset, NewOffset);
#endif
        RetStat = SsaServicesHandle->SsaMemoryConfig->SetMarginParamOffset(
          SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
          pCoreTestData->CoreTestCfg.Controller,
          pCoreTestData->CoreTestCfg.Channel, pCoreTestData->CoreTestCfg.DimmB,
          pCoreTestData->CoreTestCfg.RankB,
          pCoreTestData->CoreTestCfg.LaneValBitmasks,
          pCoreTestData->CoreTestCfg.IoLevel, MarginGroup, *pCurrentOffset,
          NewOffset);
        if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
          pCoreTestData->pTestStat)) {
          return 1;
        }
      }

      // wait for the margin parameter to settle
      if (SettleDelay != 0) {
        SsaServicesHandle->SsaCommonConfig->Wait(SsaServicesHandle, SettleDelay);
      }

      *pCurrentOffset = NewOffset;
    } // end for each step
  } // end if the new offset is different from the current offset

  return 0;
} // end function SetMarginParam

/**
@brief
  This function initializes the given results metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to Margin2D core test data structure.
**/
static VOID InitCommonMetadata(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData)
{
  pCoreTestData->pMetadata->IoLevel = pCoreTestData->CoreTestCfg.IoLevel;
  pCoreTestData->pMetadata->FastMarginGroup = pCoreTestData->CoreTestCfg.FastMarginGroup;
  pCoreTestData->pMetadata->SlowMarginGroup = pCoreTestData->CoreTestCfg.SlowMarginGroup;
  pCoreTestData->pMetadata->TargetSocket = pCoreTestData->CoreTestCfg.Socket;
  pCoreTestData->pMetadata->TargetController = pCoreTestData->CoreTestCfg.Controller;
  pCoreTestData->pMetadata->TargetChannel = pCoreTestData->CoreTestCfg.Channel;
  pCoreTestData->pMetadata->TargetDimmA = pCoreTestData->CoreTestCfg.DimmA;
  pCoreTestData->pMetadata->TargetRankA = pCoreTestData->CoreTestCfg.RankA;
  pCoreTestData->pMetadata->TargetDimmB = pCoreTestData->CoreTestCfg.DimmB;
  pCoreTestData->pMetadata->TargetRankB = pCoreTestData->CoreTestCfg.RankB;
  pCoreTestData->pMetadata->ScramblerOverrideMode = pCoreTestData->pTestCfg->ScramblerOverrideMode;
  pCoreTestData->pMetadata->MinFastOffset = pCoreTestData->FastMinOffset;
  pCoreTestData->pMetadata->MaxFastOffset = pCoreTestData->FastMaxOffset;
  pCoreTestData->pMetadata->MinSlowOffset = pCoreTestData->SlowMinOffset;
  pCoreTestData->pMetadata->MaxSlowOffset = pCoreTestData->SlowMaxOffset;
  pCoreTestData->pMetadata->FastStepUnit = pCoreTestData->FastStepUnit;
  pCoreTestData->pMetadata->SlowStepUnit = pCoreTestData->SlowStepUnit;

#if SUPPORT_FOR_DDRT
  if (IsMarginGroupForErid(pCoreTestData->pTestCfg->FastMarginGroup) ||
    IsMarginGroupForErid(pCoreTestData->pTestCfg->SlowMarginGroup)) {
    pCoreTestData->pMetadata->BusWidth = MAX_ERID_SIGNAL_CNT;
  }
  else
#endif // SUPPORT_FOR_DDRT
  {
    pCoreTestData->pMetadata->BusWidth = pCoreTestData->SystemInfo.BusWidth;
  }

  pCoreTestData->pMetadata->BusFreq = pCoreTestData->SystemInfo.BusFreq;

  SsaServicesHandle->SsaMemoryConfig->GetMemVoltage(SsaServicesHandle,
    &pCoreTestData->pMetadata->MemVoltage);
} // end function InitCommonMetadata

/**
@brief
  This function gets the number of result elements that will be required.

  @param[in]   MinOffset            Minimum offset value.
  @param[in]   MaxOffset            Maximum offset value.
  @param[in]   StepSize             Step size.

  @retval  The number of result elements.
**/
static UINT32 GetResultEleCnt(
  INT16 MaxSlowOffset,
  INT16 MinSlowOffset,
  UINT8 SlowStepSize)
{
  UINT32 ResultEleCnt;

  ResultEleCnt = (MaxSlowOffset - MinSlowOffset) + 1;

  // reduce by step size
  ResultEleCnt /= SlowStepSize;

  // one element for each side (hi/lo) of the fast margin parameter shmoo
  ResultEleCnt *= 2;

  return ResultEleCnt;
} // end function GetResultEleCnt

/**
@brief
  This is the pre-DIMM-initialization handler function.  The function will be
  called by the MemShmoo code immediately before a DIMM initialization is
  performed.

  This function conforms to the PRE_DIMM_INIT_HANDLER function typedef
  published in MemShmoo.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pClientTestCfg     Pointer to client test configuration structure.
  @param[in, out]  pClientTestData    Pointer to client test data structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 PreDimmInitHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  OUT TEST_STATUS *pTestStat)
{
  MARGIN2D_CORE_CONFIG* pCoreTestCfg = (MARGIN2D_CORE_CONFIG*) pClientTestCfg;
  MARGIN2D_CORE_DATA* pCoreTestData = (MARGIN2D_CORE_DATA*) pClientTestData;
  UINT16 NewOffset;
  BOOLEAN EnableIncrementalStepping = FALSE;

  // set the slow margin parameter back to nominal
  NewOffset = 0;
  if (SetMarginParam(SsaServicesHandle, pCoreTestData,
    pCoreTestCfg->SlowMarginGroup, &pCoreTestData->CurrentSlowOffset,
    NewOffset, pCoreTestData->SlowSettleDelay,
    EnableIncrementalStepping)) {
    return 1;
  }

  return 0;
} // end function PreDimmInitHandler

/**
@brief
  This is the post-DIMM-initialization handler function.  The function will be
  called by the MemShmoo code immediately after a DIMM initialization is
  performed.

  This function conforms to the POST_DIMM_INIT_HANDLER function typedef
  published in MemShmoo.h.

  @param[in, out]  SsaServicesHandle      Pointer to SSA services.
  @param[in]       pSystemInfo            Pointer to system information structure.
  @param[in]       pMemCfg                Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg       Pointer to generic memory point test configuration structure.
  @param[in]       pClientTestCfg         Pointer to client test configuration structure.
  @param[in, out]  pClientTestData        Pointer to client test data structure.
  @param[in]       IsLastRankCombination  Flag indicating that this is the last rank combination.
  @param[out]      pTestStat              Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 Margin2DPostDimmInitHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  IN BOOLEAN IsLastRankCombination,
  OUT TEST_STATUS *pTestStat)
{
  MARGIN2D_CORE_DATA* pCoreTestData = (MARGIN2D_CORE_DATA*) pClientTestData;
  BOOLEAN IsFirstCall, InitDqdbContent, IsLastCall;
  UINT8 RankCombinationIndex;

  // cleanup the CPGC point test
  IsLastCall = FALSE;
  CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->MemPointTestCfg, IsLastCall,
    &pCoreTestData->pCpgcPointTestData);

  // re-initialize the CPGC engine
  IsFirstCall = FALSE;
  InitDqdbContent = FALSE;
  if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    &Margin2DPointTestResultHandler, pCoreTestData, pCoreTestData->CoreTestCfg.IoLevel,
    pCoreTestData->CoreTestCfg.IsDdrT,
    pCoreTestData->CoreTestCfg.TestStatusLogLevel, IsFirstCall,
    InitDqdbContent, &pCoreTestData->pCpgcPointTestData, pTestStat)) {
    return 1;
  }

  // re-initialize the rank programming
  RankCombinationIndex = 0;
  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    pCoreTestData->pCpgcPointTestData, RankCombinationIndex,
    TRUE, // update HW register setting
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // IF we're currently in the binary search state
  if (GetInBinarySearchState(pCoreTestData->pMemShmooData)) {
    // restore binary search settings
    if (SetCpgcPointTestInBinarySearchState(SsaServicesHandle,
      &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      pCoreTestData->pCpgcPointTestData, TRUE, pTestStat)) {
      return 1;
    }
  }

  return 0;
} // end function Margin2DPostDimmInitHandler

/**
@brief
  This is the pre-RunPointTest handler function.  The function will be called
  by the MemShmoo code immediately before a PointTest is performed.

  This function conforms to the PRE_RUN_POINT_TEST_HANDLER function typedef
  published in MemShmoo.h.

  @param[in, out]  SsaServicesHandle      Pointer to SSA services.
  @param[in]       pSystemInfo            Pointer to system information structure.
  @param[in]       pMemCfg                Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg       Pointer to generic memory point test configuration structure.
  @param[in]       pClientTestCfg         Pointer to client test configuration structure.
  @param[in, out]  pClientTestData        Pointer to client test data structure.
  @param[out]      pTestStat              Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 PreRunPointTestHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  OUT TEST_STATUS *pTestStat)
{
  MARGIN2D_CORE_CONFIG* pCoreTestCfg = (MARGIN2D_CORE_CONFIG*) pClientTestCfg;
  MARGIN2D_CORE_DATA* pCoreTestData = (MARGIN2D_CORE_DATA*) pClientTestData;

  // ensure that the slow margin parameter is at the testing offset
  if (SetMarginParam(SsaServicesHandle, pCoreTestData,
    pCoreTestCfg->SlowMarginGroup, &pCoreTestData->CurrentSlowOffset,
    pCoreTestData->TestingSlowOffset, pCoreTestData->SlowSettleDelay,
    pCoreTestData->SlowEnableIncrementalStepping)) {
    return 1;
  }

  return 0;
} // end function PreRunPointTestHandler

/**
@brief
  This is the definition for the point test result handler function.

  This function conforms to the MEM_POINT_TEST_RESULT_HANDLER function typedef
  published in MemPointTest.h.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in]       pSystemInfo           Pointer to system information structure.
  @param[in]       pClientTestData       Pointer to client test data.
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
UINT8 Margin2DPointTestResultHandler(
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN VOID *pClientTestData,
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
  MARGIN2D_CORE_DATA* pCoreTestData = (MARGIN2D_CORE_DATA*) pClientTestData;

  // IF errors were detected
  if ((ErrorCount.Bits.Count != 0) || ErrorCount.Bits.Overflow) {
    // IF we're currently testing the high side
    if (pCoreTestData->FastShmooDirection == HiDirection) {
      // IF the testing offset is inside the current first fail offset THEN
      if (pCoreTestData->TestingFastOffset < pCoreTestData->FirstFailOffset) {
        pCoreTestData->FirstFailOffset = pCoreTestData->TestingFastOffset;
        CopyMem (pCoreTestData->FirstFailLaneErrorStatusHi,
          pLaneErrorStatus, (pSystemInfo->BusWidth / 8));
      }
    }
    // ELSE we're currently testing the low side
    else {
      // IF the testing offset is inside the current first fail offset THEN
      if (pCoreTestData->TestingFastOffset > pCoreTestData->FirstFailOffset) {
        pCoreTestData->FirstFailOffset = pCoreTestData->TestingFastOffset;
        CopyMem (pCoreTestData->FirstFailLaneErrorStatusLo,
          pLaneErrorStatus, (pSystemInfo->BusWidth / 8));
      }
    } // end else we're currently testing the low side

  } // end if errors were detected

  return 0;
} // end function Margin2DPointTestResultHandler

/*
@brief
  This function is used to run a margin 2D test.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in, out]  pCoreTestData       Pointer to Margin2D core test data structure.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunMargin2D(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData,
  IN INIT_SPECIFIC_METADATA InitSpecificMetadata)
{
  INT16 InnerLimitOffset;
  BOOLEAN HiSideLastPassFound = FALSE;
  INT16 HiSideLastPassOffset;
  BOOLEAN LoSideLastPassFound = FALSE;
  INT16 LoSideLastPassOffset;
  BOOLEAN HiSideFirstFailFound = FALSE;
  INT16 HiSideFirstFailOffset;
  BOOLEAN LoSideFirstFailFound = FALSE;
  INT16 LoSideFirstFailOffset;
  UINT8 i;
  INT16 SavedHiSideLastPassOffset = 0;
  INT16 SavedLoSideLastPassOffset = 0;
  SSA_STATUS RetStat;
  INT16 MinOffset, MaxOffset;
  UINT32 ResultEleCnt;
  UINT8 ShmooDirIdx;
  SHMOO_DIRECTION ShmooDirs[2];
  UINT8 ShmooDirCount;
  SETUP_CLEANUP SetupCleanup;
  BOOLEAN InitDqdbContent, IsFirstCall, IsLastCall;
  UINT8 RankCombinationIndex;
  CHAR8 *FastGroupString = NULL;
  CHAR8 *SlowGroupString = NULL;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RunMargin2D()\n");
#endif

  pCoreTestData->FastMinOffset = GetMemShmooMinOffset(pCoreTestData->pMemShmooData);
  pCoreTestData->FastMaxOffset = GetMemShmooMaxOffset(pCoreTestData->pMemShmooData);
  pCoreTestData->FastSettleDelay = GetMemShmooSettleDelay(pCoreTestData->pMemShmooData);
  pCoreTestData->FastStepUnit = GetMemShmooStepUnit(pCoreTestData->pMemShmooData);

#if ENBL_VERBOSE_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "FastMinOffset=%d FastMaxOffset=%d\n",
    pCoreTestData->FastMinOffset, pCoreTestData->FastMaxOffset);
#endif

  if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->FastMarginGroup, &FastGroupString))) {
    FastGroupString = "Unknown group";
  }
  if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->SlowMarginGroup, &SlowGroupString))) {
    SlowGroupString = "Unknown group";
  }
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "IoLevel=%u (%s)\n",
    pCoreTestData->pTestCfg->IoLevel,
    GetLevelStr(pCoreTestData->pTestCfg->IoLevel));
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "FastMarginGroup=%u (%s)\n",
    pCoreTestData->pTestCfg->FastMarginGroup,
    FastGroupString);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SlowMarginGroup=%u (%s)\n",
    pCoreTestData->pTestCfg->SlowMarginGroup,
    SlowGroupString);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Target: Socket=%u Controller=%u Channel=%u ",
    pCoreTestData->pTestCfg->TargetSocket,
    pCoreTestData->pTestCfg->TargetController,
    pCoreTestData->pTestCfg->TargetChannel);
  if (pCoreTestData->pTestCfg->RankTestModes\
    [pCoreTestData->pTestCfg->TargetController]\
    [pCoreTestData->pTestCfg->TargetChannel] == Singles) {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "Dimm=%u Rank=%u\n",
      pCoreTestData->pTestCfg->TargetDimmA,
      pCoreTestData->pTestCfg->TargetRankA);
  }
  else {
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "DimmA=%u RankA=%u DimmB=%u RankB=%u\n",
      pCoreTestData->pTestCfg->TargetDimmA,
      pCoreTestData->pTestCfg->TargetRankA,
      pCoreTestData->pTestCfg->TargetDimmB,
      pCoreTestData->pTestCfg->TargetRankB);
  }

  pCoreTestData->FastEnableIncrementalStepping = IsIncrementalSteppingRequired(
    SsaServicesHandle, &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->MemPointTestCfg, pCoreTestData->CoreTestCfg.IoLevel,
    pCoreTestData->CoreTestCfg.FastMarginGroup);

  pCoreTestData->SlowEnableIncrementalStepping = IsIncrementalSteppingRequired(
    SsaServicesHandle, &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->MemPointTestCfg, pCoreTestData->CoreTestCfg.IoLevel,
    pCoreTestData->CoreTestCfg.SlowMarginGroup);

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // IF we need to test the high side and low side separately
  if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
    ((pCoreTestData->pTestCfg->SlowMarginGroup == CmdAll) ||
    (pCoreTestData->pTestCfg->SlowMarginGroup == CmdVref) ||
    (pCoreTestData->pTestCfg->SlowMarginGroup == CtlAll))) {
    ShmooDirCount = 2;

    ShmooDirs[0] = LoDirection;
    ShmooDirs[1] = HiDirection;
  }
  // ELSE we can test the high and low sides in one go
  else
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  {
    ShmooDirCount = 1;
    ShmooDirs[0] = HiThenLoDirection;
  }

  // FOR each shmoo direction (backside command margin parameters must have
  // the calls to InitMarginParam performed in a per shmoo direction basis):
  for (ShmooDirIdx = 0; ShmooDirIdx < ShmooDirCount; ShmooDirIdx++) {
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF the shmoo directions are to be tested separately and this is not the
    // first direction
    if (ShmooDirIdx != 0) {
      // return the slow parameter to nominal
      pCoreTestData->TestingSlowOffset = 0;
      if (SetMarginParam(SsaServicesHandle, pCoreTestData,
        pCoreTestData->CoreTestCfg.SlowMarginGroup,
        &pCoreTestData->CurrentSlowOffset, pCoreTestData->TestingSlowOffset,
        pCoreTestData->SlowSettleDelay,
        pCoreTestData->SlowEnableIncrementalStepping)) {
          return 1;
      }

      // cleanup the slow margin parameter
      SetupCleanup = Cleanup;
      // NOTE: Backside command margining setup/cleanup is different for
      // high and low sides so we're piggybacking the current shmoo direction
      // on the Setup/Cleanup parameter.
      if (ShmooDirCount > 1) {
        SetupCleanup |= ShmooDirs[ShmooDirIdx - 1] << 1;
      }
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=%u)\n",
        pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.IoLevel,
        pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
#endif
      RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
        SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
        pCoreTestData->CoreTestCfg.IoLevel,
        pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
      if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
        pCoreTestData->pTestStat)) {
        return 1;
      }
    } // end if the shmoo directions are to be tested separately and ...
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    // setup the slow margin parameter
    SetupCleanup = Setup;
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // NOTE: Backside command margining setup/cleanup is different for
    // high and low sides so we're piggybacking the shmoo direction on
    // the Setup/Cleanup parameter.
    if (ShmooDirCount > 1) {
      SetupCleanup |= ShmooDirs[ShmooDirIdx] << 1;
    }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=%u)\n",
      pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.IoLevel,
      pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
#endif
    RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
      SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
      pCoreTestData->CoreTestCfg.IoLevel,
      pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
    if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
      pCoreTestData->pTestStat)) {
      return 1;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // The call to the InitMarginParam function may use the CPGC engine for
    // retraining so we need to reinitialize the CPGC engine with our settings.
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // cleanup the CPGC point test
    IsLastCall = FALSE;
    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->MemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);

    // re-initialize the CPGC engine
    IsFirstCall = FALSE;
    InitDqdbContent = TRUE;
    if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      &Margin2DPointTestResultHandler, pCoreTestData,
      pCoreTestData->pTestCfg->IoLevel, pCoreTestData->CoreTestCfg.IsDdrT,
      pCoreTestData->pTestCfg->TestStatusLogLevel, IsFirstCall,
      InitDqdbContent, &pCoreTestData->pCpgcPointTestData,
      pCoreTestData->pTestStat)) {
      return 1;
    }

    // re-initialize the rank programming
    RankCombinationIndex = 0;
    if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
      &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      pCoreTestData->pCpgcPointTestData, RankCombinationIndex,
      TRUE, //update HW register setting
      pCoreTestData->pTestStat)) {
      return 1;
    }

    RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamLimits(
      SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
      pCoreTestData->CoreTestCfg.Controller, pCoreTestData->CoreTestCfg.Channel,
      pCoreTestData->CoreTestCfg.DimmA, pCoreTestData->CoreTestCfg.RankA,
      pCoreTestData->CoreTestCfg.LaneValBitmasks,
      pCoreTestData->CoreTestCfg.IoLevel,
      pCoreTestData->CoreTestCfg.SlowMarginGroup, &MinOffset, &MaxOffset,
      &pCoreTestData->SlowSettleDelay, &pCoreTestData->SlowStepUnit);
    if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
      pCoreTestData->pTestStat)) {
      return 1;
    }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF we're testing the high and low sides separately
    if (ShmooDirCount > 1) {
      if (ShmooDirs[ShmooDirIdx] == LoDirection) {
        pCoreTestData->SlowMinOffset = MinOffset;
      }
      else {
        pCoreTestData->SlowMaxOffset = MaxOffset;
      }
    }
    // ELSE we're testing the high and low sides in one go
    else
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    {
      pCoreTestData->SlowMinOffset = MinOffset;
      pCoreTestData->SlowMaxOffset = MaxOffset;
    } // end else we're testing the high and low sides in one go

    // IF we're doing rank-to-rank turnarounds THEN factor in the limits for the second rank
    if (pCoreTestData->CoreTestCfg.RankTestMode == Turnarounds) {
      RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamLimits(
        SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
        pCoreTestData->CoreTestCfg.Controller,
        pCoreTestData->CoreTestCfg.Channel,
        pCoreTestData->CoreTestCfg.DimmB, pCoreTestData->CoreTestCfg.RankB,
        pCoreTestData->CoreTestCfg.LaneValBitmasks,
        pCoreTestData->CoreTestCfg.IoLevel,
        pCoreTestData->CoreTestCfg.SlowMarginGroup, &MinOffset, &MaxOffset,
        &pCoreTestData->SlowSettleDelay, &pCoreTestData->SlowStepUnit);
      if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
        pCoreTestData->pTestStat)) {
        return 1;
      }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
      // IF we're testing the high and low sides separately
      if (ShmooDirCount > 1) {
        if (ShmooDirs[ShmooDirIdx] == LoDirection) {
          if (MinOffset > pCoreTestData->SlowMinOffset) {
            pCoreTestData->SlowMinOffset = MinOffset;
          }
        }
        else {
          if (MaxOffset < pCoreTestData->SlowMaxOffset) {
            pCoreTestData->SlowMaxOffset = MaxOffset;
          }
        }
      }
      // ELSE we're testing the high and low sides in one go
      else
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
      {
        if (MinOffset > pCoreTestData->SlowMinOffset) {
          pCoreTestData->SlowMinOffset = MinOffset;
        }

        if (MaxOffset < pCoreTestData->SlowMaxOffset) {
          pCoreTestData->SlowMaxOffset = MaxOffset;
        }
      } // end else we're testing the high and low sides in one go
    } // end if we're doing rank-to-rank turnarounds
  } // end for each shmoo direction

#if ENBL_VERBOSE_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SlowMinOffset=%d SlowMaxOffset=%d\n",
    pCoreTestData->SlowMinOffset, pCoreTestData->SlowMaxOffset);
#endif

  // get the number of results elements required
  ResultEleCnt = GetResultEleCnt(pCoreTestData->SlowMaxOffset,
    pCoreTestData->SlowMinOffset, pCoreTestData->CoreTestCfg.SlowStepSize);

  // setup the real results
  if (SetupMargin2DResults(SsaServicesHandle, ResultEleCnt, &pCoreTestData->pMetadata)) {
    return 1;
  }
  pCoreTestData->pTestStat = (TEST_STATUS*) &pCoreTestData->pMetadata->StatusCode;

  InitCommonMetadata(SsaServicesHandle, pCoreTestData);

  if (InitSpecificMetadata != NULL) {
    InitSpecificMetadata(SsaServicesHandle, pCoreTestData);
  }

  pCoreTestData->SlowShmooDirection = HiDirection;
  pCoreTestData->TestingFastOffset = 0;
  pCoreTestData->TestingSlowOffset = 0;
  pCoreTestData->CurrentFastOffset = 0;
  pCoreTestData->CurrentSlowOffset = 0;

  HiSideLastPassOffset = HiSideFirstFailOffset = SavedHiSideLastPassOffset = 0;
  LoSideLastPassOffset = LoSideFirstFailOffset = SavedLoSideLastPassOffset = \
    -1 * pCoreTestData->CoreTestCfg.FastStepSize;

  pCoreTestData->MemShmooCfg.EnableOptimizedShmoo = TRUE;

  // turn off binary search for DDR-T CmdAll/CmdVref/CtlAll test
  // Since we test DDR4 and DDRT together, if a system has DDRT
  // we have to turn off binary search for every one. 
  if (HasDdrtDevicesValEnabled(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->MemPointTestCfg) && 
    (IsCmdMarginGroup(pCoreTestData->pTestCfg->FastMarginGroup)||
    IsCtlMarginGroup(pCoreTestData->pTestCfg->FastMarginGroup))){
      pCoreTestData->MemShmooCfg.EnableOptimizedShmoo = FALSE;
  }

  // main loop
  for (i = 0; ; i++) {
    if (pCoreTestData->CoreTestCfg.TestStatusLogLevel >= 1) {
      if (EFI_ERROR (GetGroupString (pCoreTestData->CoreTestCfg.SlowMarginGroup, &SlowGroupString))) {
        SlowGroupString = "Unknown group";
      }
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "\nTesting at slow margin parameter (%s) offset %d\n",
          SlowGroupString,
          pCoreTestData->TestingSlowOffset);
    }

    // Reset the FirstFailLaneErrorStatusHi and FirstFailLaneErrorStatusLo before finding the fast parameter edge.
    // Because the Margin2DPointTestResultHandler only update these two variables when detected error, if we don't reset 
    // them, it would caused the subsequent passing points (like those points not fail up to rail, like CmdVref n ot
    // fail up to rail for the CmdAll range from -10 to 0.) got the leftover value of last fail.
    ZeroMem (pCoreTestData->FirstFailLaneErrorStatusHi, sizeof(pCoreTestData->FirstFailLaneErrorStatusHi));
    ZeroMem (pCoreTestData->FirstFailLaneErrorStatusLo, sizeof(pCoreTestData->FirstFailLaneErrorStatusLo));

    // IF the previous shmoo found a last pass offset THEN start this shmoo there
    if (HiSideLastPassFound) {
      pCoreTestData->TestingFastOffset = HiSideLastPassOffset;
    }
    // ELSE start this shmoo at the first fail found during the previous shmoo
    else {
      pCoreTestData->TestingFastOffset = HiSideFirstFailOffset;
    }

    // don't test below one step above the previous fast shmoo's low side last pass offset
    InnerLimitOffset = LoSideLastPassOffset + pCoreTestData->CoreTestCfg.FastStepSize;

    // test the high side of the fast margin parameter
    pCoreTestData->FastShmooDirection = HiDirection;
    pCoreTestData->FirstFailOffset = pCoreTestData->FastMaxOffset;

    if (FindEdge(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->MemShmooCfg,
      &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      pCoreTestData->pMemShmooData, pCoreTestData->pCpgcPointTestData,
      pCoreTestData->FastShmooDirection, InnerLimitOffset,
      &pCoreTestData->TestingFastOffset, &pCoreTestData->CurrentFastOffset,
      &HiSideLastPassFound, &HiSideLastPassOffset, &HiSideFirstFailFound,
      &HiSideFirstFailOffset, pCoreTestData->pTestStat)) {
      return 1;
    }

    // IF the previous shmoo found a last pass offset THEN start this shmoo there
    if (LoSideLastPassFound) {
      pCoreTestData->TestingFastOffset = LoSideLastPassOffset;
    }
    // ELSE start this shmoo at the first fail found during the previous shmoo
    else {
      pCoreTestData->TestingFastOffset = LoSideFirstFailOffset;
    }

    // don't test above one step below this fast shmoo's high side last pass offset
    InnerLimitOffset = HiSideLastPassOffset - \
      pCoreTestData->CoreTestCfg.FastStepSize;

    // test the low side of the fast margin parameter
    pCoreTestData->FastShmooDirection = LoDirection;
    pCoreTestData->FirstFailOffset = pCoreTestData->FastMinOffset;

    if (FindEdge(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->MemShmooCfg,
      &pCoreTestData->MemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      pCoreTestData->pMemShmooData, pCoreTestData->pCpgcPointTestData,
      pCoreTestData->FastShmooDirection, InnerLimitOffset,
      &pCoreTestData->TestingFastOffset, &pCoreTestData->CurrentFastOffset,
      &LoSideLastPassFound, &LoSideLastPassOffset, &LoSideFirstFailFound,
      &LoSideFirstFailOffset, pCoreTestData->pTestStat)) {
      return 1;
    }

    // save the high side results
    if (HiSideLastPassFound) {
      if (AppendResultElement(SsaServicesHandle, pCoreTestData,
        HiSideLastPassOffset, pCoreTestData->TestingSlowOffset,
        pCoreTestData->FirstFailLaneErrorStatusHi)) {
        return 1;
      }
    }

    // IF a low side last pass offset was found and either no high side last
    // pass offset was found or the high side and low side last pass offsets
    // are not the same (we don't need to double report a single point)
    if (LoSideLastPassFound &&
      (!HiSideLastPassFound ||
      (HiSideLastPassOffset != LoSideLastPassOffset))) {
      if (AppendResultElement(SsaServicesHandle, pCoreTestData,
        LoSideLastPassOffset, pCoreTestData->TestingSlowOffset,
        pCoreTestData->FirstFailLaneErrorStatusLo)) {
        return 1;
      }
    }

    // IF this was the first fast shmoo
    if (i == 0) {
      // save fast margin parameter's last pass values to use when we change
      // shmoo directions for the slow margin parameter
      if (HiSideLastPassFound) {
        SavedHiSideLastPassOffset = HiSideLastPassOffset;
      }
      else {
        SavedHiSideLastPassOffset = HiSideFirstFailOffset;
      }

      if (LoSideLastPassFound) {
        SavedLoSideLastPassOffset = LoSideLastPassOffset;
      }
      else {
        SavedLoSideLastPassOffset = LoSideFirstFailOffset;
      }

      // switch to unoptimized shmooing
      pCoreTestData->MemShmooCfg.EnableOptimizedShmoo = FALSE;
    } // end if this was the first fast shmoo

    if (pCoreTestData->CoreTestCfg.TestStatusLogLevel >= 2) {
      if (EFI_ERROR (GetGroupString (pCoreTestData->CoreTestCfg.FastMarginGroup, &FastGroupString))) {
        FastGroupString = "Unknown group";
      }

      if (HiSideLastPassFound) {
        if (HiSideFirstFailFound) {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "Fast margin parameter (%s) hi side last pass offset = %d\n",
            FastGroupString, HiSideLastPassOffset);
        }
        else {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "Fast margin parameter (%s) hi side last pass offset = %d (first fail not found)\n",
            FastGroupString, HiSideLastPassOffset);
        }
      }
      else {
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "Fast margin parameter (%s) hi side last pass offset = not found\n",
          FastGroupString);
      }

      if (LoSideLastPassFound) {
        if (LoSideFirstFailFound) {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "Fast margin parameter (%s) lo side last pass offset = %d\n",
            FastGroupString, LoSideLastPassOffset);
        }
        else {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "Fast margin parameter (%s) lo side last pass offset = %d (first fail not found)\n",
            FastGroupString, LoSideLastPassOffset);
        }
      }
      else {
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "Fast margin parameter (%s) lo side last pass offset = not found\n",
          FastGroupString);
      }
    }

    // IF we failed to find a last pass in either direction for the fast margin
    // parameter THEN we're done in this shmoo direction for the slow margin
    // parameter
    if ((HiSideLastPassFound == FALSE) && (LoSideLastPassFound == FALSE)) {
      if (pCoreTestData->SlowShmooDirection == HiDirection) {
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
        // IF we're testing the high and low sides separately
        if (ShmooDirCount > 1) {
          // return the slow parameter to nominal
          pCoreTestData->TestingSlowOffset = 0;
          if (SetMarginParam(SsaServicesHandle, pCoreTestData,
            pCoreTestData->CoreTestCfg.SlowMarginGroup,
            &pCoreTestData->CurrentSlowOffset, pCoreTestData->TestingSlowOffset,
            pCoreTestData->SlowSettleDelay,
            pCoreTestData->SlowEnableIncrementalStepping)) {
            return 1;
          }

          // cleanup the slow margin parameter
          SetupCleanup = Cleanup;
          // NOTE: Backside command margining setup/cleanup is different for
          // high and low sides so we're piggybacking the shmoo direction on
          // the Setup/Cleanup parameter.
          if (ShmooDirCount > 1) {
            SetupCleanup |= pCoreTestData->SlowShmooDirection << 1;
          }
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=%u)\n",
            pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.IoLevel,
            pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
#endif
          RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
            SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
            pCoreTestData->CoreTestCfg.IoLevel,
            pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
          if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
            pCoreTestData->pTestStat)) {
            return 1;
          }
        } // end if we're testing the high and low sides separately
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

        pCoreTestData->SlowShmooDirection = LoDirection;

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
        // IF we're testing the high and low sides separately
        if (ShmooDirCount > 1) {
          // setup the slow margin parameter
          SetupCleanup = Setup;
          // NOTE: Backside command margining setup/cleanup is different for
          // high and low sides so we're piggybacking the shmoo direction on
          // the Setup/Cleanup parameter.
          if (ShmooDirCount > 1) {
            SetupCleanup |= pCoreTestData->SlowShmooDirection << 1;
          }
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=%u)\n",
            pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.IoLevel,
            pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
#endif
          RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
            SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
            pCoreTestData->CoreTestCfg.IoLevel,
            pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
          if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
            pCoreTestData->pTestStat)) {
            return 1;
          }
        } // end if we're testing the high and low sides separately
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

        // use the offset values saved from the initial shmoo
        HiSideLastPassOffset = SavedHiSideLastPassOffset;
        LoSideLastPassOffset = SavedLoSideLastPassOffset;

        // set the flags to ensure the last-pass values are used
        HiSideLastPassFound = TRUE;
        LoSideLastPassFound = TRUE;

        // pre-set the testing slow offset value (it will get moved one step in the negative direction and then set below)
        pCoreTestData->TestingSlowOffset = 0;
      }
      else
        // the test is done
        break;
    }

    if (pCoreTestData->SlowShmooDirection == HiDirection) {
      pCoreTestData->TestingSlowOffset = pCoreTestData->TestingSlowOffset + \
        pCoreTestData->CoreTestCfg.SlowStepSize;

      if (pCoreTestData->TestingSlowOffset > pCoreTestData->SlowMaxOffset) {
        pCoreTestData->SlowShmooDirection = LoDirection;

        // use the saved values
        HiSideLastPassOffset = SavedHiSideLastPassOffset;
        LoSideLastPassOffset = SavedLoSideLastPassOffset;

        // set the flags to ensure the last-pass values are used
        HiSideLastPassFound = TRUE;
        LoSideLastPassFound = TRUE;

        pCoreTestData->TestingSlowOffset = -1;
      }
    }
    else {
      pCoreTestData->TestingSlowOffset = pCoreTestData->TestingSlowOffset - \
        pCoreTestData->CoreTestCfg.SlowStepSize;
      if (pCoreTestData->TestingSlowOffset < pCoreTestData->SlowMinOffset) {
        // the test is done
        break;
      }
    }
  } // end main loop

  // set the margin parameter offsets back to nominal
  pCoreTestData->TestingFastOffset = 0;
  if (SetMarginParam(SsaServicesHandle, pCoreTestData,
    pCoreTestData->CoreTestCfg.FastMarginGroup,
    &pCoreTestData->CurrentFastOffset, pCoreTestData->TestingFastOffset,
    pCoreTestData->FastSettleDelay,
    pCoreTestData->FastEnableIncrementalStepping)) {
    return 1;
  }

  pCoreTestData->TestingSlowOffset = 0;
  if (SetMarginParam(SsaServicesHandle, pCoreTestData,
    pCoreTestData->CoreTestCfg.SlowMarginGroup,
    &pCoreTestData->CurrentSlowOffset, pCoreTestData->TestingSlowOffset,
    pCoreTestData->SlowSettleDelay,
    pCoreTestData->SlowEnableIncrementalStepping)) {
    return 1;
  }

  // cleanup the slow margin parameter
  SetupCleanup = Cleanup;
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // NOTE: Backside command margining setup/cleanup is different for
  // high and low sides so we're piggybacking the shmoo direction on
  // the Setup/Cleanup parameter.
  if (ShmooDirCount > 1) {
    SetupCleanup |= pCoreTestData->SlowShmooDirection << 1;
  }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  InitMarginParam(Socket=%u IoLevel=%u MarginGroup=%u SetupCleanup=%u)\n",
    pCoreTestData->CoreTestCfg.Socket, pCoreTestData->CoreTestCfg.IoLevel,
    pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
#endif
  RetStat = SsaServicesHandle->SsaMemoryConfig->InitMarginParam(
    SsaServicesHandle, pCoreTestData->CoreTestCfg.Socket,
    pCoreTestData->CoreTestCfg.IoLevel,
    pCoreTestData->CoreTestCfg.SlowMarginGroup, SetupCleanup);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  if (pCoreTestData->CoreTestCfg.EnableMarginPlot) {
    PlotMarginResults(SsaServicesHandle, pCoreTestData);
  }

  return 0;
} // end function RunMargin2D

/**
  This function is used to initialize the outer 1D Sweep parameter

  It is called at each outer loop step

  @param[in, out]  CommonParamaters     Pointer to the 2D margin context information
  @param[in]       Socket               Socket number
  @param[in]       ShmooDir             Margin direction

**/
STATIC
VOID
InitializeOuter1DSweepParameter (
  OUT    COMMON_1D_PARAMETERS   *CommonParameters,
  IN     UINT8                   Socket,
  IN     SHMOO_DIRECTION         ShmooDir
)
{
  MARGIN_2D_PARAMETER     *Margin2DSweep;
  COMMON_1D_PARAMETERS    *OuterCommonParameters;
  MARGIN2D_CORE_DATA      *CoreTestData;
  UINT32                  ChannelBitMask;
  MRC_RT                  DimmRank[MAX_CH];
  MRC_LT                  Level;
  MRC_GT                  Group;
  UINT8                   StepSize;
  UINT8                   DitherValue;
  INT16                   OffsetLeft;
  INT16                   OffsetRight;
  INT16                   LimitOffset;
  FLAGS_CATEGORY_OFFSETS  FlagsOffsets;
  FLAGS_CATEGORY_RESULTS  FlagsResults;
  FLAGS_CATEGORY_FSM      FlagsFsm;
  MRC_MST                 MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  Margin2DSweep = (MARGIN_2D_PARAMETER *)CommonParameters;
  OuterCommonParameters = &Margin2DSweep->OuterCommonParameters;
  CoreTestData = Margin2DSweep->CoreTestData;

  //
  // Get the Channel mask. Margin2D only target ONE channel.
  //
  ChannelBitMask = 0x1 << (CoreTestData->CoreTestCfg.Controller * MAX_MC_CH + CoreTestData->CoreTestCfg.Channel);

  //
  // Get the list of DIMM/RANK
  //
  GetMargin2DTargetedDimmRank (Margin2DSweep->SsaServicesHandle, CoreTestData, Socket, &DimmRank);

  //
  // Get margin parameter
  //
  Level = CoreTestData->CoreTestCfg.IoLevel;
  Group = CoreTestData->CoreTestCfg.SlowMarginGroup;
  StepSize = CoreTestData->CoreTestCfg.SlowStepSize;
  DitherValue = 1;

  OffsetLeft = -1;
  OffsetRight = 0;
  LimitOffset = 0;

  //
  // Flags
  //
  FlagsOffsets = DefaultStartOffset;
  FlagsResults = ResultsRelative;

  if (ShmooDir == LoDirection) {
    FlagsFsm = LeftSideOnly;
  } else {
    FlagsFsm = RightSideOnly;
  }

  CreateContextParameters (OuterCommonParameters, MemSsType, Socket, ~ChannelBitMask, 0, ALL_SUBCH, DimmRank, Level, Group, gsmCsnDelim,
    StepSize, DitherValue, OffsetLeft, OffsetRight, LimitOffset, FlagsOffsets, FlagsResults, FlagsFsm);

}
/*
  @brief
  This function is used to run a margin 2D test with 1D sweep

  @param[in ]  SsaServicesHandle   Pointer to SSA services.
  @param[in, out]  pCoreTestData       Pointer to Margin2D core test data structure.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
RunMargin2DWith1DSweep (
  IN     SSA_BIOS_SERVICES_PPI  *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA     *pCoreTestData,
  IN     INIT_SPECIFIC_METADATA InitSpecificMetadata
  )
{

  UINT32 ResultEleCnt;
  CHAR8 *FastGroupString = NULL;
  CHAR8 *SlowGroupString = NULL;

  MARGIN_2D_PARAMETER Margin2DSweep;
  UINT8               Socket;
  RESULTS_1D          Results;
  Callback1D          Pre1DSweepHandler;
  Callback1D          Post1DSweepHandler;
  Callback1D          ResultProcessor;
  SHMOO_DIRECTION     ShmooDir;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "RunMargin2DWith1DSweep()\n");
#endif

  if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->FastMarginGroup, &FastGroupString))) {
    FastGroupString = "Unknown group";
  }
  if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->SlowMarginGroup, &SlowGroupString))) {
    SlowGroupString = "Unknown group";
  }
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "IoLevel=%u (%s)\n",
    pCoreTestData->pTestCfg->IoLevel,
    GetLevelStr (pCoreTestData->pTestCfg->IoLevel));
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "FastMarginGroup=%u (%s)\n",
    pCoreTestData->pTestCfg->FastMarginGroup,
    FastGroupString);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "SlowMarginGroup=%u (%s)\n",
    pCoreTestData->pTestCfg->SlowMarginGroup,
    SlowGroupString);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "Target: Socket=%u Controller=%u Channel=%u ",
    pCoreTestData->pTestCfg->TargetSocket,
    pCoreTestData->pTestCfg->TargetController,
    pCoreTestData->pTestCfg->TargetChannel);
  if (pCoreTestData->pTestCfg->RankTestModes\
    [pCoreTestData->pTestCfg->TargetController]\
    [pCoreTestData->pTestCfg->TargetChannel] == Singles) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "Dimm=%u Rank=%u\n",
      pCoreTestData->pTestCfg->TargetDimmA,
      pCoreTestData->pTestCfg->TargetRankA);
  } else {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "DimmA=%u RankA=%u DimmB=%u RankB=%u\n",
      pCoreTestData->pTestCfg->TargetDimmA,
      pCoreTestData->pTestCfg->TargetRankA,
      pCoreTestData->pTestCfg->TargetDimmB,
      pCoreTestData->pTestCfg->TargetRankB);
  }

  // get the number of results elements required
  ResultEleCnt = 64;

  // setup the real results
  if (SetupMargin2DResults (SsaServicesHandle, ResultEleCnt, &pCoreTestData->pMetadata)) {
    return 1;
  }
  pCoreTestData->pTestStat = (TEST_STATUS*)&pCoreTestData->pMetadata->StatusCode;

  InitCommonMetadata (SsaServicesHandle, pCoreTestData);

  if (InitSpecificMetadata != NULL) {
    InitSpecificMetadata (SsaServicesHandle, pCoreTestData);
  }

  //
  // Initialize private data
  //
  Margin2DSweep.SsaServicesHandle = SsaServicesHandle;
  Margin2DSweep.CoreTestData = pCoreTestData;
  Margin2DSweep.InnerLeft = -1;
  Margin2DSweep.InnerRight = 0;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not configured to be tested THEN skip it
    if ((pCoreTestData->MemPointTestCfg.SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    //
    // Separate two directions becaue the backside Cmd/Ctl margin need DeNoramlization only on the low side
    //
    for (ShmooDir = LoDirection; ShmooDir <= HiDirection; ShmooDir++) {

      InitializeOuter1DSweepParameter (&Margin2DSweep.OuterCommonParameters, Socket, ShmooDir);

      // allocate result
      AllocatePoolForResults1D (&Margin2DSweep.OuterCommonParameters, &Results);

      //
      // Update callback function
      //
      Pre1DSweepHandler = Margin2DPre1DSweep;
      Post1DSweepHandler = Margin2DPost1DSweep;
      ResultProcessor = Margin2DResultHandler;

      ProcessSweep1D (&Margin2DSweep.OuterCommonParameters, Pre1DSweepHandler, NULL, NULL, NULL, Post1DSweepHandler, ResultProcessor, &Results);

      // Release outer 1D sweep result
      ReleaseResources1D (&Margin2DSweep.OuterCommonParameters, &Results);
    } // ShmooDir
  }// Socket

  if (pCoreTestData->CoreTestCfg.EnableMarginPlot) {
    PlotMarginResults (SsaServicesHandle, pCoreTestData);
  }

  return 0;
} // end function RunMargin2DWith1DSweep

/**
@brief
  This function performs JEDEC initializations at the end of the test (if
  required) to ensure the DIMMs are left in a functional state.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
**/
VOID Margin2DCleanupJedecInit(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT MARGIN2D_CORE_DATA *pCoreTestData)
{
  UINT8 Socket, Controller;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "Margin2DCleanupJedecInit()\n");
#endif

  // IF JEDEC initialization is required
  if (IsJedecInitAfterErrRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->MemPointTestCfg, pCoreTestData->pTestCfg->IoLevel,
    pCoreTestData->pTestCfg->FastMarginGroup) ||
    IsJedecInitAfterErrRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->MemPointTestCfg, pCoreTestData->pTestCfg->IoLevel,
    pCoreTestData->pTestCfg->SlowMarginGroup)) {
    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // IF the controller is not enabled for testing THEN skip it
        if (!IsControllerTestingEnabled(SsaServicesHandle,
          &pCoreTestData->SystemInfo,
          &pCoreTestData->MemCfg, &pCoreTestData->MemPointTestCfg, Socket,
          Controller)) {
          continue;
        }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  IoReset(Socket=%u Controller=%u)\n",
          Socket, Controller);
#endif
        SsaServicesHandle->SsaMemoryConfig->IoReset(SsaServicesHandle,
          Socket, Controller);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  JedecReset(Socket=%u Controller=%u)\n",
          Socket, Controller);
#endif
        SsaServicesHandle->SsaMemoryConfig->JedecReset(
          SsaServicesHandle, Socket, Controller);
      } // end for each controller
    } // end for each socket
  } // end if JEDEC initialization is required
} // end function Margin2DCleanupJedecInit

/**
@brief
  This function sets up the results.  It creates the results buffer and gets a
  pointer to the metadata structure.

  @param[in]   SsaServicesHandle    Pointer to SSA services.
  @param[in]   ResultEleCnt         The number of result elements required.
  @param[out]  ppMetadata           Pointer to pointer to results metadata structure.
**/
UINT8 SetupMargin2DResults(
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN UINT32 ResultEleCnt,
  OUT MARGIN2D_RESULT_METADATA **ppMetadata)
{
  SSA_STATUS RetStat;
  UINT32 MetadataSize = sizeof(MARGIN2D_RESULT_METADATA);
  UINT32 ResultElementSize = sizeof(MARGIN2D_RESULT_COLUMNS);
  EFI_GUID MetadataGuid = MARGIN2D_RESULT_METADATA_GUID;
  EFI_GUID ColumnsGuid = MARGIN2D_RESULT_COLUMNS_GUID;
  MARGIN2D_RESULT_METADATA *pMetadata;
  TEST_STATUS *pTestStat;
  UINT32 ReservedSize = 0; // the CpgcPointTest and MemShmoo memory allocations are already made at this point

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetupMargin2DResults(ResultEleCnt=%u)\n", ResultEleCnt);
#endif

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  CreateResultBuffer(MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u)\n",
    MetadataSize, ResultElementSize, ResultEleCnt);
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer(
    SsaServicesHandle, MetadataSize, ResultElementSize,
    ResultEleCnt);

  // IF we couldn't create the result buffer with the requested number of
  // result elements THEN create the maximum possible
  if (RetStat != Success) {
    if (CreateMaxResultBuffer(SsaServicesHandle, MetadataSize,
      ResultElementSize, ReservedSize, NULL)) return 1;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  GetMetadata()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetMetadata(
    SsaServicesHandle, ppMetadata);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) return 1;

  pMetadata = *ppMetadata;

  // zero the metadata
  ZeroMem (pMetadata, sizeof(*pMetadata));

  // set the version information
  pMetadata->MajorVer = BSSA_TC_MAJOR_VER;
  pMetadata->MinorVer = BSSA_TC_MINOR_VER;
  pMetadata->RevVer = BSSA_TC_REV_VER;
  pMetadata->BuildVer = BSSA_TC_BUILD_VER;

  pTestStat = (TEST_STATUS*) &pMetadata->StatusCode;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  SetMetadataType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetMetadataType(
    SsaServicesHandle, &MetadataGuid);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) return 1;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  SetResultElementType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetResultElementType(
    SsaServicesHandle, &ColumnsGuid);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) return 1;

  return 0;
} // end function SetupMargin2DResults

/**
  This function is used to get the current targted DIMM/RANK from the core test data.
  The info is needed to report test results and set up the 1D sweep

  @param[in ]  SsaServicesHandle  Pointer to SSA services.
  @param[in ]  pCoreTestData      Pointer to RMT core test data.
  @param[in ]  Socket             Socket number
  @param[out]  DimmRank           Pointer to the Dimm/rank array

**/
VOID
GetMargin2DTargetedDimmRank (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN2D_CORE_DATA    *pCoreTestData,
  IN UINT8                  Socket,
  OUT MRC_RT              (*DimmRank)[MAX_CH]
  )
{
  UINT8 Controller;
  UINT8 Channel;
  UINT8 ChannelInSocket;
  UINT8 PairCount = 1;
  DIMM_RANK_PAIR DimmRankPair;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "Margin2DCore.GetMargin2DTargetedDimmRank()\n");
#endif

  ZeroMem (DimmRank, sizeof (*DimmRank));

  // FOR each controller:
  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {

      // IF this channel is not enabled for validation THEN skip it
      if (!IsChannelTestingEnabled (SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg,
        &pCoreTestData->MemPointTestCfg,
        Socket, Controller, Channel)) {
        continue;
      }

      GetCpgcPointTestDimmRanks (SsaServicesHandle,
        &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
        &pCoreTestData->MemPointTestCfg, pCoreTestData->pCpgcPointTestData, Socket,
        Controller, Channel, &PairCount, &DimmRankPair,
        pCoreTestData->pTestStat);

      ChannelInSocket = MAX_MC_CH * Controller + Channel;

      (*DimmRank)[ChannelInSocket].Dimm = DimmRankPair.Dimm;
      (*DimmRank)[ChannelInSocket].Rank = DimmRankPair.Rank;

    } // end for each channel
  } // end for each controller

} // end function  GetMargin2DTargetedDimmRank

// end file Margin2DCore.c
