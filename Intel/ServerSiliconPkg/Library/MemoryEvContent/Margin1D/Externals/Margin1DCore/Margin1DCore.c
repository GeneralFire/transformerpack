/** @file
Margin1DCore.c

Common core Margin1D algorithms.

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

#pragma warning (disable : 4366)

#include "ssabios.h"

#include <Library/BitMaskLib.h>
#include "ChkRetStat.h"
#include "CommonInitPlatform.h"
#include "CreateMaxResultBuffer.h"
#include "MarginParam.h"
#include "Margin1D.h" // for BSSA_TC_MAJOR_VER, etc.
#include "Margin1DCore.h"
#include "EvContentInternal.h"
#include "Margin1DSweep.h"

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>

#include <ReferenceCodeFatalErrors.h>

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

// flag to enable results debug messages
#define ENBL_RESULTS_DEBUG_LOG_MSGS (0)

// flag to enable fixup debug messages
#define ENBL_FIXUP_DEBUG_LOG_MSGS (0)

// flag to enable CreateResultBuffer debug messages
#define ENBL_CREATE_RESULT_BUFFER_DEBUG_MSGS (0)

// flag to enable memory pool debug log messages
#define ENBL_MEM_POOL_DEBUG_LOG_MSGS (0)

// special value used for error count to indicate that an overflow occurred
#define SPCL_ERR_CNT_OVERFLOW_VAL 0xFFFFFFFF

/**
  This function is used to initialize the current memory point test
  configuration structure in the given core test data structure based on the
  selected memory point test configuration structure, current rank
  combinations index in the given core test data structure, and (when testing
  serialization is selected) the given target socket, controller, and channel.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
  @param[in]       TargetSocket       Zero-based target socket.
  @param[in]       TargetController   Zero-based target controller.
  @param[in]       TargetChannel      Zero-based target channel.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 InitCurrentMemPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData
#if SUPPORT_FOR_SERIALIZATION
  ,
  IN UINT8 TargetSocket,
  IN UINT8 TargetController,
  IN UINT8 TargetChannel
#endif // SUPPORT_FOR_SERIALIZATION
  )
{
  MEM_POINT_TEST_CONFIG *pMemPointTestCfg = \
    &pCoreTestData->CurrentMemPointTestCfg;
  MEM_POINT_TEST_CONFIG *pSelectMemPointTestCfg = \
    &pCoreTestData->SelectMemPointTestCfg;
  UINT8 Socket, Controller, Channel, Dimm, Rank, ByteGroup;
  BOOLEAN IsSocketToBeTested;
  BOOLEAN IsControllerToBeTested;
  BOOLEAN IsChannelToBeTested;
  UINT8 PairCount;
  DIMM_RANK_PAIR DimmRankPairs[2];
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // check for Gen2 backside command test support or not
  BOOLEAN IsMarginGroupSupportedInDimmA = TRUE;
  BOOLEAN IsMarginGroupSupportedInDimmB = TRUE;
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
#if SUPPORT_FOR_SERIALIZATION
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "InitMemPointTestCfg(Socket=%u Controller=%u Channel=%u CurrentRankCombinationIndex=%u)\n",
    TargetSocket, TargetController, TargetChannel, pCoreTestData->CurrentRankCombinationIndex);
#else
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "InitMemPointTestCfg(CurrentRankCombinationIndex=%u)\n",
    pCoreTestData->CurrentRankCombinationIndex);
#endif // SUPPORT_FOR_SERIALIZATION
#endif

  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    pCoreTestData->pCpgcPointTestData,
    pCoreTestData->CurrentRankCombinationIndex, 
    FALSE, // Not update HW register setting yet
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // start by setting everything to 0
  ZeroMem (pMemPointTestCfg, sizeof(*pMemPointTestCfg));

  // IF this is the frontside
  if (pCoreTestData->pTestCfg->IoLevel == DdrLevel) {
#if SUPPORT_FOR_LRDIMM
    // IF the DIMMs are LRDIMMs
    if (pCoreTestData->MemCfg.IsLrDimms) {
      // only test between the first ranks of all DIMMs
      pMemPointTestCfg->TurnaroundsTestMode = OnlyInterDimmTurnarounds;
    }
#endif // SUPPORT_FOR_LRDIMM
  }
  // ELSE this is the backside
  else {
    // only test between the ranks within each DIMM
    pMemPointTestCfg->TurnaroundsTestMode = OnlyIntraDimmTurnarounds;
  }

  // start with the socket bitmask of selected devices
  pMemPointTestCfg->SocketBitmask = \
    pSelectMemPointTestCfg->SocketBitmask;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not to be tested THEN skip it
    if ((pMemPointTestCfg->SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    IsSocketToBeTested = FALSE;

    // start with the controller bitmask of selected devices
    pMemPointTestCfg->ControllerBitmasks[Socket] = \
      pSelectMemPointTestCfg->ControllerBitmasks[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not to be tested THEN skip it
      if ((pMemPointTestCfg->ControllerBitmasks[Socket] &
        (1 << Controller)) == 0) {
         continue;
      }

      IsControllerToBeTested = FALSE;

      // start with the channel bitmask of selected devices
      pMemPointTestCfg->ChannelBitmasks[Socket][Controller] = \
        pSelectMemPointTestCfg->ChannelBitmasks[Socket][Controller];

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not to be tested THEN skip it
        if ((pMemPointTestCfg->ChannelBitmasks[Socket][Controller] &
          (1 << Channel)) == 0) {
          continue;
        }

        // IF this rank combination is unique for this channel
        // We will save(aka. validate) this channel test results. 
        if (pCoreTestData->CurrentRankCombinationIndex <
          pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT][Socket][Controller][Channel]) {
          // add this channel to the channel validation mask
          pMemPointTestCfg->ChannelValBitmasks[Socket][Controller] |= \
            1 << Channel;
        } // end if this rank combination is unique for this channel

        IsChannelToBeTested = FALSE;

        pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] = \
          pCoreTestData->pTestCfg->RankTestModes[Controller][Channel];

        PairCount = 2;
        if (GetCpgcPointTestDimmRanks(SsaServicesHandle,
          &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
          &pCoreTestData->SelectMemPointTestCfg,
          pCoreTestData->pCpgcPointTestData, Socket, Controller, Channel,
          &PairCount, &DimmRankPairs[0], pCoreTestData->pTestStat)) {
          return 1;
        }

        // IF there's a DIMM A/rank A
        if (PairCount > 0) {
          Dimm = DimmRankPairs[0].Dimm;
          Rank = DimmRankPairs[0].Rank;
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
          IsMarginGroupSupportedInDimmA = TRUE;
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

          // IF this DIMM and rank are set in the Margin1D test configuration
          // DIMM A/rank A bitmasks
          if ((pCoreTestData->pTestCfg->DimmABitmasks[Controller][Channel] & (1 << Dimm)) &&
            (pCoreTestData->pTestCfg->RankABitmasks[Controller][Channel][Dimm] & (1 << Rank))) {
            // add them to the memory point test configuration DIMM A/rank A bitmasks
            pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] |= \
              1 << Dimm;
            pMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] |= \
              1 << Rank;

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
            // CoreTestData already have the info of if a dimm supports backside cmd test or not. 
            // The "InitBasksideCmdBitMask()" of MemPointTest.c was called early in the RunMargin1DTest()
            // right after calling the InitSelectMemPointTestCfg().
            // IF this is for backside non-Cmd/Ctl margining we need to find out if it supports
            // only Gen2 register support it. 
            if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
              ((pCoreTestData->pTestCfg->MarginGroup == CmdAll) ||
              (pCoreTestData->pTestCfg->MarginGroup == CmdVref) ||
              (pCoreTestData->pTestCfg->MarginGroup == CtlAll))) {
              if (!(pCoreTestData->BacksideCmdBitmasks[Socket] & (0x1 << ((Controller * MAX_CHANNEL_CNT * MAX_DIMM_CNT) + (Channel * MAX_DIMM_CNT) + Dimm)))) {
                IsMarginGroupSupportedInDimmA = FALSE;
              }
            }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
          }
        } // end if there's a DIMM A/rank A

        // IF we're testing rank-to-rank turnarounds and there's a DIMM B/rank B
        if ((pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Turnarounds) &&
          (PairCount > 1)) {
          Dimm = DimmRankPairs[1].Dimm;
          Rank = DimmRankPairs[1].Rank;
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
          IsMarginGroupSupportedInDimmB = TRUE;
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

          // IF this DIMM and rank are set in the Margin1D test configuration
          // DIMM A/rank A bitmasks
          if ((pCoreTestData->pTestCfg->DimmBBitmasks[Controller][Channel] & (1 << Dimm)) &&
            (pCoreTestData->pTestCfg->RankBBitmasks[Controller][Channel][Dimm] & (1 << Rank))) {
            // add them to the memory point test configuration DIMM B/rank B bitmasks
            pMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] |= \
              1 << Dimm;
            pMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] |= \
              1 << Rank;
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
            // CoreTestData already have the info of if a dimm supports backside cmd test or not. 
            // The "InitBasksideCmdBitMask()" of MemPointTest.c was called early in the RunMargin1DTest()
            // right after calling the InitSelectMemPointTestCfg().
            // IF this is for backside non-Cmd/Ctl margining we need to find out if it supports
            // only Gen2 register support it. 
            if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
              ((pCoreTestData->pTestCfg->MarginGroup == CmdAll) ||
              (pCoreTestData->pTestCfg->MarginGroup == CmdVref) ||
              (pCoreTestData->pTestCfg->MarginGroup == CtlAll))) {
              if (!(pCoreTestData->BacksideCmdBitmasks[Socket] & (0x1 << ((Controller * MAX_CHANNEL_CNT * MAX_DIMM_CNT) + (Channel * MAX_DIMM_CNT) + Dimm)))) {
                IsMarginGroupSupportedInDimmB = FALSE;
              }
            }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
          }
        } // end if we're testing rank-to-rank turnarounds and there's a DIMM B/rank B

        // IF (we're testing singles AND there are DIMM A's to be tested) OR
        // (we're testing rank-to-rank turnarounds AND there are DIMM A's and DIMM B's to be tested)
        if (((pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Singles) &&
          (pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] != 0)) ||
          ((pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Turnarounds) &&
          (pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] != 0) &&
          (pMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] != 0))) {
          IsSocketToBeTested = TRUE;
          IsControllerToBeTested = TRUE;
          IsChannelToBeTested = TRUE;
        }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
        // if margin parameter was not supported for the dimm A or dimm B (if turnaround)
        // we skip validating this channel
        if ((!IsMarginGroupSupportedInDimmA) || (!IsMarginGroupSupportedInDimmB)){
          pMemPointTestCfg->ChannelValBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
        }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

        // IF it turns out that there were no ranks to test on this channel
        if (!IsChannelToBeTested) {
          // remove the channel
          pMemPointTestCfg->ChannelBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
          pMemPointTestCfg->ChannelValBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
        }
      } // end for each channel

      // IF it turns out that there were no ranks to test on this controller
      if (!IsControllerToBeTested) {
        // remove the controller
        pMemPointTestCfg->ControllerBitmasks[Socket] &= \
          ~(1 << Controller);
      }
    } // end for each controller

    // IF it turns out that there were no ranks to test on this socket
    if (!IsSocketToBeTested) {
      // remove the socket
      pMemPointTestCfg->SocketBitmask &= ~(1 << Socket);
    }
  } // end for each socket

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
#if SUPPORT_FOR_SERIALIZATION
    if (Controller != TargetController) {
      continue;
    }
#endif // SUPPORT_FOR_SERIALIZATION
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
#if SUPPORT_FOR_SERIALIZATION
      if (Channel != TargetChannel) {
        continue;
      }
#endif // SUPPORT_FOR_SERIALIZATION
#if SUPPORT_FOR_DDRT
      // IF the margin group is Early-Read-ID
      if (IsMarginGroupForErid(pCoreTestData->pTestCfg->MarginGroup)) {
#ifdef __STUB__
        ASSERT (MAX_ERID_SIGNAL_CNT <= 8);
#endif // __STUB__
        pMemPointTestCfg->LaneValBitmasks[Controller][Channel][0] = \
          ((1 << MAX_ERID_SIGNAL_CNT) - 1) & \
          pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel][0];
      }
      else
#endif // SUPPORT_FOR_DDRT
      {
        // FOR each byte group:
        for (ByteGroup = 0;
          ByteGroup < (pCoreTestData->SystemInfo.BusWidth / 8);
          ByteGroup++) {
          pMemPointTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup] = \
            0xFF & pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup];
        } // end for each byte group
      }

      pMemPointTestCfg->ChunkValBitmasks[Controller][Channel] = pCoreTestData->pTestCfg->ChunkValBitmasks[Controller][Channel]; 
    } // end for each channel
  } // end for each controller

#if ENBL_VERBOSE_LOG_MSGS
  LogMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pMemPointTestCfg);
#endif
  if (ChkMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pMemPointTestCfg, pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function InitCurrentMemPointTestCfg

/**
  This function is used to initialize the select memory point test
  configuration structure in the given core test data structure based on the
  CurrentIsDdrT element in the given core test data structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
  @param[in]       InitAsAll          Flag indicating whether to initialize as all devices

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 InitSelectMemPointTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData,
  IN BOOLEAN InitAsAll)
{
  MEM_POINT_TEST_CONFIG *pSelectMemPointTestCfg = \
    &pCoreTestData->SelectMemPointTestCfg;
  UINT8 Socket, Controller, Channel, Dimm, ByteGroup;
  BOOLEAN IsSocketToBeTested;
  BOOLEAN IsControllerToBeTested;
  BOOLEAN IsChannelToBeTested;
  BOOLEAN IsDimmAToBeTested;
  BOOLEAN IsDimmBToBeTested;
  MEMORY_TECHNOLOGY_TYPES MemoryTech;
#if SUPPORT_FOR_DDRT
  BOOLEAN HasDdrtDimmInChannel;
  BOOLEAN HasNonDdrtDimmInChannel;
#endif // SUPPORT_FOR_DDRT

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "InitSelectMemPointTestCfg(InitAsAll=%u)\n", InitAsAll);
#endif

  // start by setting everything to 0
  ZeroMem (pSelectMemPointTestCfg, sizeof(*pSelectMemPointTestCfg));

  // set default values
  pSelectMemPointTestCfg->TurnaroundsTestMode = AllRankTurnarounds;
  pSelectMemPointTestCfg->TurnaroundsPairMode = OrderIndependent;

  // IF this is the frontside
  if (pCoreTestData->pTestCfg->IoLevel == DdrLevel) {
#if SUPPORT_FOR_LRDIMM
    // IF the DIMMs are LRDIMMs
    if (pCoreTestData->MemCfg.IsLrDimms) {
      // only test between the first ranks of all DIMMs
      pSelectMemPointTestCfg->TurnaroundsTestMode = OnlyInterDimmTurnarounds;
    }
#endif // SUPPORT_FOR_LRDIMM
  }
  // ELSE this is the backside
  else {
    // only test between the ranks within each DIMM
    pSelectMemPointTestCfg->TurnaroundsTestMode = OnlyIntraDimmTurnarounds;
  }

  // use the given socket bitmask and screen out unpopulated sockets
  pSelectMemPointTestCfg->SocketBitmask = \
    pCoreTestData->pTestCfg->SocketBitmask & \
    pCoreTestData->SystemInfo.SocketBitMask;

#if SUPPORT_FOR_DDRT
  pCoreTestData->HasTestedChnlWithMixedDdrtDdr4 = FALSE;
#endif // SUPPORT_FOR_DDRT

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not to be tested THEN skip it
    if ((pSelectMemPointTestCfg->SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    IsSocketToBeTested = FALSE;

    // use the given controller bitmask and screen out unpopulated controllers
    pSelectMemPointTestCfg->ControllerBitmasks[Socket] = \
      pCoreTestData->pTestCfg->ControllerBitmask & \
      pCoreTestData->MemCfg.ControllerBitmasks[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not to be tested THEN skip it
      if ((pSelectMemPointTestCfg->ControllerBitmasks[Socket] &
        (1 << Controller)) == 0) {
        continue;
      }

      IsControllerToBeTested = FALSE;

      // use the given channel bitmask and screen out unpopulated channels
      pSelectMemPointTestCfg->ChannelBitmasks[Socket][Controller] = \
        pSelectMemPointTestCfg->ChannelValBitmasks[Socket][Controller] = \
        pCoreTestData->pTestCfg->ChannelBitmasks[Controller] & \
        pCoreTestData->MemCfg.ChannelBitmasks[Socket][Controller];

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not to be tested THEN skip it
        if ((pSelectMemPointTestCfg->ChannelBitmasks[Socket][Controller] &
          (1 << Channel)) == 0) {
          continue;
        }

        IsChannelToBeTested = FALSE;

        pSelectMemPointTestCfg->RankTestModes[Socket][Controller][Channel] = \
          pCoreTestData->pTestCfg->RankTestModes[Controller][Channel];

#if SUPPORT_FOR_DDRT
        HasDdrtDimmInChannel = FALSE;
        HasNonDdrtDimmInChannel = FALSE;
#endif // SUPPORT_FOR_DDRT

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not populated THEN skip it
          if ((pCoreTestData->MemCfg.DimmBitmasks[Socket][Controller][Channel] &
            (1 << Dimm)) == 0) {
            continue;
          }

          IsDimmAToBeTested = FALSE;
          IsDimmBToBeTested = FALSE;

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
          MemoryTech = pCoreTestData->MemCfg.MemoryTech[Socket][Controller][Channel][Dimm];

          // IF the margin group is Early-Read-ID, we need to make sure the dimm is ddrt dimm.
          if ((IsMarginGroupForErid(pCoreTestData->pTestCfg->MarginGroup)) && (MemoryTech != SsaMemoryDdrT))
          {
            continue;
          }
#endif   // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
          // IF this is for backside non-Cmd/Ctl margining and non-LRDIMM, non-DDR-T
          // devices THEN skip it
          if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
            (pCoreTestData->pTestCfg->MarginGroup != CmdAll) &&
            (pCoreTestData->pTestCfg->MarginGroup != CmdVref) &&
            (pCoreTestData->pTestCfg->MarginGroup != CtlAll) &&
            !pCoreTestData->MemCfg.IsLrDimms && 
            (MemoryTech != SsaMemoryDdrT)) {   
              continue;
          }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

          // IF this DIMM is set in the Margin1D test configuration DIMM A bitmask
          if (pCoreTestData->pTestCfg->DimmABitmasks[Controller][Channel] & (1 << Dimm)) {
            // add it to the memory point test configuration DIMM A bitmask
            pSelectMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] |= \
              1 << Dimm;

#if SUPPORT_FOR_DDRT
            if (MemoryTech != SsaMemoryDdrT) {
              HasNonDdrtDimmInChannel = TRUE;
            } else {
              HasDdrtDimmInChannel = TRUE;
            }
#endif   // SUPPORT_FOR_DDRT

            // test all ranks that are in the Margin1D test configuration
            // rank A bitmask and present
            pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] = \
              pCoreTestData->pTestCfg->RankABitmasks[Controller][Channel][Dimm] & \
              ((1 << pCoreTestData->MemCfg.RankCounts[Socket][Controller][Channel][Dimm]) - 1);

#if SUPPORT_FOR_DDRT
            // IF we're testing the frontside and the DIMM is a DDR-T
            if ((pCoreTestData->pTestCfg->IoLevel == DdrLevel) &&
              (MemoryTech == SsaMemoryDdrT)) {
              // only test the first available rank on this DIMM
              pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] = \
                1 << GetLowestBitSetInMask(
                pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm],
                MAX_RANK_CNT);
            }
#endif // SUPPORT_FOR_DDRT
            // IF there are rank A's to be tested
            if (pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] != 0) {
              IsSocketToBeTested = TRUE;
              IsControllerToBeTested = TRUE;
              IsChannelToBeTested = TRUE;
              IsDimmAToBeTested = TRUE;
            }
          } // end if this DIMM is set in the Margin1D test configuration DIMM A bitmask

          // IF we're testing rank-to-rank turnarounds
          if (pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Turnarounds) {
            // IF this DIMM is set in the Margin1D test configuration DIMM B bitmask
            if (pCoreTestData->pTestCfg->DimmBBitmasks[Controller][Channel] & (1 << Dimm)) {
              // add it to the memory point test configuration DIMM B bitmask
              pSelectMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] |= \
                1 << Dimm;

              // test all ranks that are in the Margin1D test configuration
              // rank B bitmask and present
              pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] = \
                pCoreTestData->pTestCfg->RankBBitmasks[Controller][Channel][Dimm] & \
                ((1 << pCoreTestData->MemCfg.RankCounts[Socket][Controller][Channel][Dimm]) - 1);

              // PT we can combine below two cases if we change the IsLrDimms definition to include both ddr4 and ddrt

#if SUPPORT_FOR_LRDIMM
              // IF we're testing the frontside and the DIMM is a LRDIMM
              if ((pCoreTestData->pTestCfg->IoLevel == DdrLevel) &&
                pCoreTestData->MemCfg.IsLrDimms) {
                // only test the first available rank on this DIMM
                pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] = \
                  1 << GetLowestBitSetInMask(
                  pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm],
                  MAX_RANK_CNT);
              }
#endif // SUPPORT_FOR_LRDIMM
#if SUPPORT_FOR_DDRT
              // IF we're testing the frontside and the DIMM is a DDR-T
              if ((pCoreTestData->pTestCfg->IoLevel == DdrLevel) &&
                (MemoryTech == SsaMemoryDdrT)) {
                // only test the first available rank on this DIMM
                pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] = \
                  1 << GetLowestBitSetInMask(
                  pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm],
                  MAX_RANK_CNT);
              }
#endif // SUPPORT_FOR_DDRT
              // IF there are rank B's to be tested
              if (pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] != 0) {
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
            pSelectMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] &= \
              ~(1 << Dimm);
          }

          // IF it turns out that there were no ranks to test on this DIMMB
          if (!IsDimmBToBeTested) {
            // remove the DIMM
            pSelectMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] &= \
              ~(1 << Dimm);
          }
        } // end for each DIMM

#if SUPPORT_FOR_DDRT
        if ((HasDdrtDimmInChannel) && (HasNonDdrtDimmInChannel)){
          pCoreTestData->HasTestedChnlWithMixedDdrtDdr4 = TRUE;
        }
#endif   // SUPPORT_FOR_DDRT

#if SUPPORT_FOR_TURNAROUNDS
        // IF we're doing turnarounds and this channel can't do turnarounds
        if ((pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Turnarounds) &&
          !CanChannelDoTurnarounds(SsaServicesHandle,
          &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
          pSelectMemPointTestCfg, pCoreTestData->pTestCfg->IoLevel, Socket,
          Controller, Channel, pCoreTestData->CurrentIsDdrT)) {
          // remove the channel
          IsChannelToBeTested = FALSE;
        }
#endif // SUPPORT_FOR_TURNAROUNDS

        // IF it turns out that there were no ranks to test on this channel
        if (!IsChannelToBeTested) {
          // remove the channel
          pSelectMemPointTestCfg->ChannelBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
          pSelectMemPointTestCfg->ChannelValBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
        }
      } // end for each channel

      // IF it turns out that there were no ranks to test on this controller
      if (!IsControllerToBeTested) {
        // remove the controller
        pSelectMemPointTestCfg->ControllerBitmasks[Socket] &= \
          ~(1 << Controller);
      }
    } // end for each controller

    // IF it turns out that there were no ranks to test on this socket
    if (!IsSocketToBeTested) {
      // remove the socket
      pSelectMemPointTestCfg->SocketBitmask &= ~(1 << Socket);
    }
  } // end for each socket

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
#if SUPPORT_FOR_DDRT
      // IF the margin group is Early-Read-ID
      if (IsMarginGroupForErid(pCoreTestData->pTestCfg->MarginGroup)) {
#ifdef __STUB__
        ASSERT (MAX_ERID_SIGNAL_CNT <= 8);
#endif // __STUB__
        pSelectMemPointTestCfg->LaneValBitmasks[Controller][Channel][0] = \
          ((1 << MAX_ERID_SIGNAL_CNT) - 1) & \
          pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel][0];
      }
      else
#endif // SUPPORT_FOR_DDRT
      {
        // FOR each byte group:
        for (ByteGroup = 0; ByteGroup < (pCoreTestData->SystemInfo.BusWidth / 8); ByteGroup++) {
          pSelectMemPointTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup] = \
            0xFF & pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup];
        } // end for each byte group
      }
      pSelectMemPointTestCfg->ChunkValBitmasks[Controller][Channel] = pCoreTestData->pTestCfg->ChunkValBitmasks[Controller][Channel]; 
    } // end for each channel
  } // end for each controller

#if ENBL_VERBOSE_LOG_MSGS
  LogMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pSelectMemPointTestCfg);
#endif
  if (ChkMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pSelectMemPointTestCfg, pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function InitSelectMemPointTestCfg

/**
@brief
  This function checks the test configuration for correctness/consistency.
  It also fills in the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 ChkMargin1DTestCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData)
{
  BOOLEAN InitAsAll;
#if SUPPORT_FOR_DDRT
  CHAR8 *GroupString = NULL;
#endif

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "ChkMargin1DTestCfg()\n");
#endif

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // IF we're testing backside Cmd/Ctl
  if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
    ((pCoreTestData->pTestCfg->MarginGroup == CmdAll) ||
    (pCoreTestData->pTestCfg->MarginGroup == CmdVref) ||
    (pCoreTestData->pTestCfg->MarginGroup == CtlAll))) {
    pCoreTestData->IsBacksideCmdCtl = TRUE;
  }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

#if SUPPORT_FOR_DDRT
  // IF the margin group is Early-Read-ID and the I/O level is not the frontside
  if (IsMarginGroupForErid(pCoreTestData->pTestCfg->MarginGroup) &&
    (pCoreTestData->pTestCfg->IoLevel != DdrLevel)) {
    if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->MarginGroup, &GroupString))) {
      GroupString = "Unknown group";
    }
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "ERROR: MarginGroup=%s only applies to IoLevel=%s.\n",
      GroupString,
      GetLevelStr(DdrLevel));
#endif
    ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }
#endif // SUPPORT_FOR_DDRT

  InitAsAll = TRUE;
  if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, InitAsAll)) {
    return 1;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->SelectMemPointTestCfg);
#endif
  if (ChkMemPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // initialize the CPGC test configuration to default values
  SetCpgcPointTestCfgDefaults(&pCoreTestData->CpgcPointTestCfg);

  if (UpdateCpgcPointTestCfgByMargin1DCfg (pCoreTestData->pTestCfg, &pCoreTestData->CpgcPointTestCfg)) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "ERROR: CPGC_POINT_TEST_CONFIG and MARGIN_1D_CONFIG elements do not match.\n");
#endif
    ProcError (SsaServicesHandle, Failure, __FILE__, __LINE__, pCoreTestData->pTestStat);
    return 1;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->CpgcPointTestCfg);
#endif
  if (ChkCpgcPointTestCfg(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // initialize the memory shmoo test configuration to default values
  SetMemShmooTestCfgDefaults(&pCoreTestData->MemShmooCfg);

  pCoreTestData->MemShmooCfg.IoLevel = pCoreTestData->pTestCfg->IoLevel;
  pCoreTestData->MemShmooCfg.MarginGroup = pCoreTestData->pTestCfg->MarginGroup;
  pCoreTestData->MemShmooCfg.ShmooDirection = pCoreTestData->pTestCfg->ShmooDirection;
  pCoreTestData->MemShmooCfg.StepSize = pCoreTestData->pTestCfg->StepSize;
  pCoreTestData->MemShmooCfg.GuardBandSize = pCoreTestData->pTestCfg->GuardBandSize;
  pCoreTestData->MemShmooCfg.LaneShmooStopMode = pCoreTestData->pTestCfg->LaneShmooStopMode;
  pCoreTestData->MemShmooCfg.RankShmooStopMode = pCoreTestData->pTestCfg->RankShmooStopMode;
  pCoreTestData->MemShmooCfg.ChannelShmooStopMode = pCoreTestData->pTestCfg->ChannelShmooStopMode;
  pCoreTestData->MemShmooCfg.ControllerShmooStopMode = pCoreTestData->pTestCfg->ControllerShmooStopMode;
  pCoreTestData->MemShmooCfg.SocketShmooStopMode = pCoreTestData->pTestCfg->SocketShmooStopMode;
  pCoreTestData->MemShmooCfg.StopChannelMarginOnStopCond = TRUE;

#if SUPPORT_FOR_EXTENDED_RANGE
  pCoreTestData->MemShmooCfg.EnableExtendedRange = pCoreTestData->pTestCfg->EnableExtendedRange;
#endif // SUPPORT_FOR_EXTENDED_RANGE

  pCoreTestData->MemShmooCfg.EnableIncrementalStepping = \
    IsIncrementalSteppingRequired(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup);

  if (IsJedecInitAfterErrRequired(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup)) {
    pCoreTestData->MemShmooCfg.EnableDimmInitAfterErr = TRUE;
    pCoreTestData->MemShmooCfg.DimmInitMode = Jedec;
  }
  else if (IsIoResetAfterErrRequired(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup)) {
    pCoreTestData->MemShmooCfg.EnableDimmInitAfterErr = TRUE;
    pCoreTestData->MemShmooCfg.DimmInitMode = IoReset;
  }
  else {
    pCoreTestData->MemShmooCfg.EnableDimmInitAfterErr = FALSE;
  }

#if ENBL_VERBOSE_LOG_MSGS
  LogMemShmooTestCfg(SsaServicesHandle, &pCoreTestData->MemShmooCfg);
#endif
  if (ChkMemShmooTestCfg(SsaServicesHandle, &pCoreTestData->MemShmooCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function ChkMargin1DTestCfg


/**
@brief
  This function is used to plot lane margin results.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 PlotLaneMarginResults(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData)
{
  UINT8 ByteGroup;
  UINT8 Socket, Controller, Channel, Lane;
  UINT8 DimmA, RankA;
  UINT8 DimmB = 0, RankB = 0;
  UINT32 EleIdx;
  MARGIN_1D_RESULT_COLUMNS *pElement;
  INT16 Offset;
  INT16 InnerLoSideOffset, OuterLoSideOffset;
  INT16 InnerHiSideOffset, OuterHiSideOffset;
  UINT8 BusWidth;
  BOOLEAN InitAsAll;
  BOOLEAN IsFirstCall, InitDqdbContent;
  BOOLEAN IsLastCall;
  UINT8 PairCount = 2;
  DIMM_RANK_PAIR DimmRankPairs[2];
  CHAR8 *GroupString = NULL;
  UINT32     *MetaDataSize;
  UINT32     *ElementCount;
  UINT8      SubCh;
  UINT8      NumSubCh;
  UINT8      SubChMaxBit;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "PlotLaneMarginResults()\n");
#endif

  GetSubChInfo (pCoreTestData->SystemInfo.BusWidth, &NumSubCh, &SubChMaxBit);

  //
  // Save the result element start address and total count to their core test data fields
  //
  SsaServicesHandle->SsaResultsConfig->GetMetadataSize (SsaServicesHandle, &MetaDataSize);

  pCoreTestData->pResultElements = (MARGIN_1D_RESULT_COLUMNS *)((UINTN)pCoreTestData->pMetadata + *MetaDataSize);

  SsaServicesHandle->SsaResultsConfig->GetResultElementCount (SsaServicesHandle, &ElementCount);

  pCoreTestData->ResultElementCount = *ElementCount;

#if SUPPORT_FOR_DDRT
  // IF the margin parameter is Early-Read-ID timing or voltage
  if (IsMarginGroupForErid(pCoreTestData->pTestCfg->MarginGroup)) {
    BusWidth = MAX_ERID_SIGNAL_CNT;
  }
  else
#endif // SUPPORT_FOR_DDRT
  {
    BusWidth = pCoreTestData->SystemInfo.BusWidth;
  }

  // PT will remove the for loop, also remove the array using the "pCoreTestData->CurrentIsDdrT"
  // as index.

  // FOR non-DDR-T and DDR-T devices (they're tested separately because their
  // margin parameters are different and therefore may have different ranges):
  for (pCoreTestData->CurrentIsDdrT = 0;
#if SUPPORT_FOR_DDRT
    pCoreTestData->CurrentIsDdrT <= 0;
#else
    pCoreTestData->CurrentIsDdrT <= 0;
#endif // !SUPPORT_FOR_DDRT
    pCoreTestData->CurrentIsDdrT++) {

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF this is for backside non-Cmd/Ctl margining and non-LRDIMM, non-DDRT 
    // devices THEN skip it
    if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
      (pCoreTestData->pTestCfg->MarginGroup != CmdAll) &&
      (pCoreTestData->pTestCfg->MarginGroup != CmdVref) &&
      (pCoreTestData->pTestCfg->MarginGroup != CtlAll) &&
      !pCoreTestData->MemCfg.IsLrDimms &&
      !pCoreTestData->HasDdrtDevices) {
      continue;
    }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    pCoreTestData->MemShmooCfg.IsDdrT = TRUE; //pCoreTestData->CurrentIsDdrT;

    InitAsAll = FALSE;
    if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, InitAsAll)) {
      return 1;
    }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF this is for backside Cmd/Ctl margining
    if (pCoreTestData->IsBacksideCmdCtl) {
      UINT8 MarginGroupCount = 1;
      InitBacksideCmdBitmask(SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
        &pCoreTestData->pTestCfg->MarginGroup, MarginGroupCount,
        pCoreTestData->BacksideCmdBitmasks);

      // IF there are no devices with a backside THEN skip it
      if (!IsAnyBacksideCmdBitSet(pCoreTestData->BacksideCmdBitmasks)) {
        continue;
      }
    } // end if this is for backside Cmd/Ctl margining
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    if (GetRankCombinationsCnts(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      pCoreTestData->pTestCfg->IoLevel, pCoreTestData->CurrentIsDdrT,
      &pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT],
      &pCoreTestData->MaxRankCombinationsCnts[pCoreTestData->CurrentIsDdrT],
      pCoreTestData->pTestStat)) {
      return 1;
    }

    IsFirstCall = FALSE;
    InitDqdbContent = FALSE;
    if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      NULL, pCoreTestData, pCoreTestData->pTestCfg->IoLevel,
      pCoreTestData->CurrentIsDdrT,
      pCoreTestData->pTestCfg->TestStatusLogLevel, IsFirstCall,
      InitDqdbContent, &pCoreTestData->pCpgcPointTestData,
      pCoreTestData->pTestStat)) {
      return 1;
    }

    // FOR each rank combination:
    for (pCoreTestData->CurrentRankCombinationIndex = 0;
      pCoreTestData->CurrentRankCombinationIndex <
      pCoreTestData->MaxRankCombinationsCnts[pCoreTestData->CurrentIsDdrT];
      pCoreTestData->CurrentRankCombinationIndex++) {
      // re-initialize the rank programming
      if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
        &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
        &pCoreTestData->SelectMemPointTestCfg,
        &pCoreTestData->CpgcPointTestCfg,
        pCoreTestData->pCpgcPointTestData,
        pCoreTestData->CurrentRankCombinationIndex,
        FALSE, // do NOT need to update HW register setting
        pCoreTestData->pTestStat)) {
        return 1;
      }

      // FOR each socket:
      for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
        // FOR each controller:
        for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
          // FOR each channel:
          for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
            // IF this channel is not enabled for validation THEN skip it
            if (!IsChannelValEnabled(SsaServicesHandle,
              &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
              &pCoreTestData->SelectMemPointTestCfg, Socket, Controller,
              Channel)) {
              continue;
            }

            // IF this rank combination is not unique for this channel, we will skip it. 
            if (pCoreTestData->CurrentRankCombinationIndex >=
              pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT][Socket][Controller][Channel]) {
                continue;
            } // end if this rank combination is not unique for this channel

            if (GetCpgcPointTestDimmRanks(SsaServicesHandle,
              &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
              &pCoreTestData->SelectMemPointTestCfg,
              pCoreTestData->pCpgcPointTestData, Socket, Controller, Channel,
              &PairCount, &DimmRankPairs[0],
              pCoreTestData->pTestStat)) {
              return 1;
            }

            DimmA = DimmRankPairs[0].Dimm;
            RankA = DimmRankPairs[0].Rank;

            if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->MarginGroup, &GroupString))) {
              GroupString = "Unknown group";
            }

            for (SubCh = 0; SubCh < NumSubCh; SubCh++) {

#if SUPPORT_FOR_TURNAROUNDS
              // IF rank-to-rank turnarounds is specified
              if (pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Turnarounds) {
                // IF there are not two DIMM/rank pairs then skip it
                if (PairCount != 2) {
                  continue;
                }

                DimmB = DimmRankPairs[1].Dimm;
                RankB = DimmRankPairs[1].Rank;

                if (NumSubCh > 1) {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                    "\n"
                    "Per bit margin IoLevel=%u (%s) MarginGroup=%u (%s) for N%u.C%u.D%u.SC%u.R%u.D%u.SC%u.R%u",
                    pCoreTestData->pTestCfg->IoLevel,
                    GetLevelStr (pCoreTestData->pTestCfg->IoLevel),
                    pCoreTestData->pTestCfg->MarginGroup,
                    GroupString,
                    Socket, ((Controller * MAX_CHANNEL_CNT) + Channel),
                    DimmA, SubCh, RankA, DimmB, SubCh, RankB);
                } else {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                    "\n"
                    "Per bit margin IoLevel=%u (%s) MarginGroup=%u (%s) for N%u.C%u.D%u.R%u.D%u.R%u",
                    pCoreTestData->pTestCfg->IoLevel,
                    GetLevelStr (pCoreTestData->pTestCfg->IoLevel),
                    pCoreTestData->pTestCfg->MarginGroup,
                    GroupString,
                    Socket, ((Controller * MAX_CHANNEL_CNT) + Channel),
                    DimmA, RankA, DimmB, RankB);
                }

                // For the DDRT dimms to do turnaround, both dimm must be DDRT dimms. We just need to 
                // check the DimmA.
                if (pCoreTestData->MemCfg.MemoryTech[Socket][Controller][Channel][DimmA] == SsaMemoryDdrT) {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, " (NVMDIMM)");
                }

                NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
              }
              // ELSE rank-to-rank turnarounds is not specified
              else
#endif // SUPPORT_FOR_TURNAROUNDS
              {
                // IF there is not one DIMM/rank pair then skip it
                if (PairCount != 1) {
                  continue;
                }

                if (NumSubCh > 1) {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                    "\n"
                    "Per bit margin IoLevel=%u (%s) MarginGroup=%u (%s) for N%u.C%u.D%u.SC%u.R%u",
                    pCoreTestData->pTestCfg->IoLevel,
                    GetLevelStr (pCoreTestData->pTestCfg->IoLevel),
                    pCoreTestData->pTestCfg->MarginGroup,
                    GroupString,
                    Socket, ((Controller * MAX_CHANNEL_CNT) + Channel),
                    DimmA, SubCh, RankA);
                } else {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                    "\n"
                    "Per bit margin IoLevel=%u (%s) MarginGroup=%u (%s) for N%u.C%u.D%u.R%u",
                    pCoreTestData->pTestCfg->IoLevel,
                    GetLevelStr (pCoreTestData->pTestCfg->IoLevel),
                    pCoreTestData->pTestCfg->MarginGroup,
                    GroupString,
                    Socket, ((Controller * MAX_CHANNEL_CNT) + Channel),
                    DimmA, RankA);
                }

                if (pCoreTestData->MemCfg.MemoryTech[Socket][Controller][Channel][DimmA] == SsaMemoryDdrT) {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, " (NVMDIMM)");
                }

                NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
              } // end else rank-to-rank turnarounds is not specified

              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
              // print column header
              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if SUPPORT_FOR_DDRT
              if (IsMarginGroupForErid (pCoreTestData->pTestCfg->MarginGroup)) {
#ifdef __STUB__
                ASSERT (MAX_ERID_SIGNAL_CNT == 2);
#endif // __STUB__
                NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                  "     01");
              } else
#endif // SUPPORT_FOR_DDRT
              {
                NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                  "     0------7 8-----15");

                for (ByteGroup = 2; ByteGroup < (SubChMaxBit / 8); ByteGroup++) {
                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                    " %2u----%2u", (ByteGroup * 8), (((ByteGroup + 1) * 8) - 1));
                }
              }
              NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                "\n");

              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
              // find min/max offset values
              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
              // get the range of the high and low side offsets
              InnerLoSideOffset = -512;
              OuterLoSideOffset = 0;
              InnerHiSideOffset = 512;
              OuterHiSideOffset = 0;

              // FOR each result element:
              for (EleIdx = 0; EleIdx < pCoreTestData->ResultElementCount; EleIdx++) {
                pElement = &pCoreTestData->pResultElements[EleIdx];

                // IF this is not the result element we're looking for THEN skip it
                if ((pElement->Socket != Socket) ||
                  (pElement->Controller != Controller) ||
                  (pElement->Channel != Channel) ||
                  (pElement->DimmA != DimmA) ||
                  (pElement->RankA != RankA) ||
                  (pElement->DimmB != DimmB) ||
                  (pElement->RankB != RankB)) {
                  continue;
                }

                // get the low side margin offset
                Offset = pElement->Offset;

                // IF the offset is from the high side
                if (Offset >= 0) {
                  if (Offset < InnerHiSideOffset) {
                    InnerHiSideOffset = Offset;
                  }

                  if (Offset > OuterHiSideOffset) {
                    OuterHiSideOffset = Offset;
                  }
                }
                // ELSE the offset is from the low side
                else {
                  if (Offset > InnerLoSideOffset) {
                    InnerLoSideOffset = Offset;
                  }

                  if (Offset < OuterLoSideOffset) {
                    OuterLoSideOffset = Offset;
                  }
                } // else the offset is from the low side
              } // end for each result element

              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
              // print plot
              // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
              // IF there we no failing values on the high side
              if (OuterHiSideOffset == InnerHiSideOffset) {
                // adjust the inner offset to ensure that something gets plotted
                InnerHiSideOffset -= pCoreTestData->pTestCfg->StepSize;
              }

              // FOR each offset on the high side (from outermost to innermost+1)
              for (Offset = OuterHiSideOffset; Offset >= InnerHiSideOffset; Offset--) {
                // FOR each result element:
                for (EleIdx = 0; EleIdx < pCoreTestData->ResultElementCount; EleIdx++) {
                  pElement = &pCoreTestData->pResultElements[EleIdx];

                  // IF this is not the result element we're looking for THEN skip it
                  if ((pElement->Socket != Socket) ||
                    (pElement->Controller != Controller) ||
                    (pElement->Channel != Channel) ||
                    (pElement->DimmA != DimmA) ||
                    (pElement->RankA != RankA) ||
                    (pElement->DimmB != DimmB) ||
                    (pElement->RankB != RankB) ||
                    (pElement->Offset != Offset)) {
                    continue;
                  }

                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "%4d", Offset);

                  // FOR each lane:
                  for (Lane = SubCh * SubChMaxBit; Lane < (SubCh + 1) * SubChMaxBit; Lane++) {
                    if ((Lane % 8) == 0) {
                      // insert space between byte groups
                      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, " ");
                    }

                    // IF an error was detected on this lane
                    if (pElement->LaneErrorStatus[Lane / 8] & (1 << (Lane % 8))) {
                      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "*");
                    } else {
                      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, " ");
                    }
                  } // end for each lane

                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
                  break;
                } // end for each result element
              } // end for each offset on the high side

              // IF there we no failing values on the low side
              if (OuterLoSideOffset == InnerLoSideOffset) {
                // adjust the inner offset to ensure that something gets plotted
                InnerLoSideOffset += pCoreTestData->pTestCfg->StepSize;
              }

              // FOR each offset on the low side (from innermost to outermost)
              for (Offset = InnerLoSideOffset; Offset >= OuterLoSideOffset; Offset--) {
                // FOR each result element:
                for (EleIdx = 0; EleIdx < pCoreTestData->ResultElementCount; EleIdx++) {
                  pElement = &pCoreTestData->pResultElements[EleIdx];

                  // IF this is not the result element we're looking for THEN skip it
                  if ((pElement->Socket != Socket) ||
                    (pElement->Controller != Controller) ||
                    (pElement->Channel != Channel) ||
                    (pElement->DimmA != DimmA) ||
                    (pElement->RankA != RankA) ||
                    (pElement->DimmB != DimmB) ||
                    (pElement->RankB != RankB) ||
                    (pElement->Offset != Offset)) {
                    continue;
                  }

                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "%4d", Offset);

                  // FOR each lane:
                  for (Lane = SubCh * SubChMaxBit; Lane < (SubCh + 1) * SubChMaxBit; Lane++) {
                    if ((Lane % 8) == 0) {
                      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, " ");
                    }

                    // IF an error was detected on this lane
                    if (pElement->LaneErrorStatus[Lane / 8] & (1 << (Lane % 8))) {
                      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "*");
                    } else {
                      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, " ");
                    }
                  } // end for each lane

                  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
                  break;
                } // end for each result element
              } // end for each offset on the low side
            } // end for subchannel
          } // end for each channel
        } // end for each controller
      } // end for each socket
    } // end for each rank combination


    // cleanup the CPGC point test
    IsLastCall = TRUE;
    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);
  } // end for DDR-T and DDR-T devices

  return 0;
} // end function PlotLaneMarginResults

/**
@brief
  This function initializes the given results metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
**/
static VOID InitMetadata(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData)
{
#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "InitMetadata()\n");
#endif

  pCoreTestData->pMetadata->IoLevel = pCoreTestData->pTestCfg->IoLevel;
  pCoreTestData->pMetadata->MarginGroup = pCoreTestData->pTestCfg->MarginGroup;
#ifdef __STUB__
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  ASSERT (sizeof(pCoreTestData->pMetadata->ChunkValBitmasks) ==
    sizeof(pCoreTestData->pTestCfg->ChunkValBitmasks));
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  ASSERT (sizeof(pCoreTestData->pMetadata->BurstLengths) ==
    sizeof(pCoreTestData->pTestCfg->BurstLengths));
#endif // __STUB__
#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  CopyMem (pCoreTestData->pMetadata->ChunkValBitmasks,
    pCoreTestData->pTestCfg->ChunkValBitmasks,
    sizeof(pCoreTestData->pMetadata->ChunkValBitmasks));
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  CopyMem (pCoreTestData->pMetadata->BurstLengths,
    pCoreTestData->pTestCfg->BurstLengths,
    sizeof(pCoreTestData->pMetadata->BurstLengths));

  pCoreTestData->pMetadata->ScramblerOverrideMode = \
    pCoreTestData->pTestCfg->ScramblerOverrideMode;

#if SUPPORT_FOR_DDRT
  if (IsMarginGroupForErid(pCoreTestData->pTestCfg->MarginGroup)) {
    pCoreTestData->pMetadata->BusWidth = MAX_ERID_SIGNAL_CNT;
  }
  else
#endif // SUPPORT_FOR_DDRT
  {
    pCoreTestData->pMetadata->BusWidth = pCoreTestData->SystemInfo.BusWidth;
  }

  pCoreTestData->pMetadata->BusFreq = pCoreTestData->SystemInfo.BusFreq;


  if (!IsMarginGroupDdrTSpecific(pCoreTestData->pTestCfg->MarginGroup)) {
    pCoreTestData->pMetadata->MinOffset[0] = pCoreTestData->MinOffsets[0];
    pCoreTestData->pMetadata->MaxOffset[0] = pCoreTestData->MaxOffsets[0];
    pCoreTestData->pMetadata->StepUnit[0] = pCoreTestData->StepUnits[0];
  }

#if SUPPORT_FOR_DDRT
  if (pCoreTestData->HasDdrtDevices) {
    pCoreTestData->pMetadata->MinOffset[1] = pCoreTestData->MinOffsets[1];
    pCoreTestData->pMetadata->MaxOffset[1] = pCoreTestData->MaxOffsets[1];
    pCoreTestData->pMetadata->StepUnit[1] = pCoreTestData->StepUnits[1];
  }
#endif // SUPPORT_FOR_DDRT

  SsaServicesHandle->SsaMemoryConfig->GetMemVoltage(SsaServicesHandle,
    &pCoreTestData->pMetadata->MemVoltage);

  pCoreTestData->pMetadata->MaxRankPerDimm = MAX_RANK_CNT;
  pCoreTestData->pMetadata->ErrCntOverflowVal = SPCL_ERR_CNT_OVERFLOW_VAL;
} // end function InitMetadata

/**
  @brief
  This function sets up the results.  It creates the results buffer and gets a
  pointer to the metadata structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
  @param[in]       ResultEleCnt       Number of result elements required.
  @param[out]      ppMetadata         Pointer to pointer to results metadata structure.
**/
UINT8
SetupMargin1DResults (
  IN OUT SSA_BIOS_SERVICES_PPI       *SsaServicesHandle,
  IN OUT CORE_TEST_DATA              *pCoreTestData,
  IN     UINT32                       ResultEleCnt,
  OUT    MARGIN_1D_RESULT_METADATA  **ppMetadata)
{
  SSA_STATUS RetStat;
  UINT32 MetadataSize = sizeof (MARGIN_1D_RESULT_METADATA);
  UINT32 ResultElementSize = sizeof (MARGIN_1D_RESULT_COLUMNS);
  EFI_GUID MetadataGuid = MARGIN_1D_RESULT_METADATA_GUID;
  EFI_GUID ColumnsGuid = MARGIN_1D_RESULT_COLUMNS_GUID;
  MARGIN_1D_RESULT_METADATA *pMetadata;
  TEST_STATUS *pTestStat;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS || ENBL_CREATE_RESULT_BUFFER_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "SetupMargin1DResults(ResultEleCnt=%u)\n", ResultEleCnt);
#endif

#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
  RcDumpHeapUsage ();
#endif
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_CREATE_RESULT_BUFFER_DEBUG_MSGS || ENBL_MEM_POOL_DEBUG_LOG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  CreateResultBuffer(MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u)\n",
    MetadataSize, ResultElementSize, ResultEleCnt);
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer (
    SsaServicesHandle, MetadataSize, ResultElementSize, ResultEleCnt);
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
  RcDumpHeapUsage ();
#endif

  // IF we couldn't create the result buffer with the requested number of
  // result elements plus the additional elements to cover the reserved size
  if (RetStat != Success) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  Failed to CreateResultBuffer(MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u)\n",
      MetadataSize, ResultElementSize, ResultEleCnt);
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  GetMetadata()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->GetMetadata (
    SsaServicesHandle, ppMetadata);
  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  pMetadata = *ppMetadata;

  // zero the metadata
  ZeroMem (pMetadata, sizeof (*pMetadata));

  // set the version information
  pMetadata->MajorVer = BSSA_TC_MAJOR_VER;
  pMetadata->MinorVer = BSSA_TC_MINOR_VER;
  pMetadata->RevVer = BSSA_TC_REV_VER;
  pMetadata->BuildVer = BSSA_TC_BUILD_VER;

  pTestStat = (TEST_STATUS*)&pMetadata->StatusCode;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  SetMetadataType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetMetadataType (
    SsaServicesHandle, &MetadataGuid);
  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  SetResultElementType()\n");
#endif
  RetStat = SsaServicesHandle->SsaResultsConfig->SetResultElementType (
    SsaServicesHandle, &ColumnsGuid);
  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
    return 1;
  }

  return 0;
} // end function SetupMargin1DResults

/**
@brief
  This function gets the number of result elements required.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pTestData          Pointer to core test data structure.
  @param[out]      pResultEleCnt      Pointer to result element count value.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 GetResultEleCnt(
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData,
  OUT UINT32 *pResultEleCnt)
{
  UINT8 Socket, Controller, Channel;
  INT16 MinOffset = 0, MaxOffset = 0;
  BOOLEAN InitAsAll;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetResultEleCnt()\n");
#endif

  // maximum number of elements is one for each margin parameter offset * one
  // for each channel that's not idle * one for each rank iteration
  *pResultEleCnt = 0;

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  // IF we need to test the shmoo directions separately
  if (pCoreTestData->IsBacksideCmdCtl &&
    ((pCoreTestData->pTestCfg->ShmooDirection == LoThenHiDirection) ||
    (pCoreTestData->pTestCfg->ShmooDirection == HiThenLoDirection))) {
    pCoreTestData->ShmooDirCount = 2;

    if (pCoreTestData->pTestCfg->ShmooDirection == LoThenHiDirection) {
      pCoreTestData->ShmooDirs[0] = LoDirection;
      pCoreTestData->ShmooDirs[1] = HiDirection;
    }
    else {
      pCoreTestData->ShmooDirs[0] = HiDirection;
      pCoreTestData->ShmooDirs[1] = LoDirection;
    }
  }
  // ELSE we can test the shmoo directions together
  else
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
  {
    pCoreTestData->ShmooDirCount = 1;
    pCoreTestData->ShmooDirs[0] = pCoreTestData->pTestCfg->ShmooDirection;
  }

  // FOR non-DDR-T and DDR-T devices (they're tested separately because their
  // margin parameters are different and therefore may have different ranges):
  for (pCoreTestData->CurrentIsDdrT = 0;
#if SUPPORT_FOR_DDRT
    pCoreTestData->CurrentIsDdrT <= 0;
#else
    pCoreTestData->CurrentIsDdrT <= 0;
#endif // !SUPPORT_FOR_DDRT
    pCoreTestData->CurrentIsDdrT++) {

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF this is for backside non-Cmd/Ctl margining and non-LRDIMM, non-DDRT
    // devices THEN skip it
    if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
      (pCoreTestData->pTestCfg->MarginGroup != CmdAll) &&
      (pCoreTestData->pTestCfg->MarginGroup != CmdVref) &&
      (pCoreTestData->pTestCfg->MarginGroup != CtlAll) &&
      !pCoreTestData->MemCfg.IsLrDimms &&
      !pCoreTestData->HasDdrtDevices) {
      continue;
    }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    pCoreTestData->MemShmooCfg.IsDdrT = TRUE; // It needs to be TRUE, otherwise the SetupMemShmoo() would not init the DDRT device training mode. 

    InitAsAll = FALSE;
    if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, InitAsAll)) {
      return 1;
    }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF this is for backside Cmd/Ctl margining
    if (pCoreTestData->IsBacksideCmdCtl) {
      UINT8 MarginGroupCount = 1;
      InitBacksideCmdBitmask(SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
        &pCoreTestData->pTestCfg->MarginGroup, MarginGroupCount,
        pCoreTestData->BacksideCmdBitmasks);

      // IF there are no devices with a backside THEN skip it
      if (!IsAnyBacksideCmdBitSet(pCoreTestData->BacksideCmdBitmasks)) {
        continue;
      }
    } // end if this is for backside Cmd/Ctl margining
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    if (GetRankCombinationsCnts(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      pCoreTestData->pTestCfg->IoLevel, pCoreTestData->CurrentIsDdrT,
      &pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT],
      &pCoreTestData->MaxRankCombinationsCnts[pCoreTestData->CurrentIsDdrT],
      pCoreTestData->pTestStat)) {
      return 1;
    }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // FOR each shmoo direction (backside command margin parameters must have
    // the calls to InitMarginParam performed in a per shmoo direction basis):
    for (pCoreTestData->CurrentShmooDirIdx = 0;
      pCoreTestData->CurrentShmooDirIdx < pCoreTestData->ShmooDirCount;
      pCoreTestData->CurrentShmooDirIdx++) {
#else
    {
      pCoreTestData->CurrentShmooDirIdx = 0;
#endif // !(SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT)

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
      // IF we're testing the shmoo directions separately
      if (pCoreTestData->ShmooDirCount != 1) {
        if (pCoreTestData->CurrentShmooDirIdx != 0) {
          // cleanup the shmoo test
          if (CleanupMemShmoo(SsaServicesHandle, &pCoreTestData->SystemInfo,
            &pCoreTestData->MemCfg, &pCoreTestData->MemShmooCfg,
            &pCoreTestData->SelectMemPointTestCfg,
            &pCoreTestData->pMemShmooData, pCoreTestData->pTestStat)) {
            return 1;
          }
        }

        pCoreTestData->MemShmooCfg.ShmooDirection = \
          pCoreTestData->ShmooDirs[pCoreTestData->CurrentShmooDirIdx];
      } // end if we're testing the shmoo directions separately
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

      if (SetupMemShmoo(SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg, &pCoreTestData->MemShmooCfg,
        &pCoreTestData->SelectMemPointTestCfg, &GetCpgcPointTestDimmRanks,
        &SetCpgcPointTestInBinarySearchState, &RunCpgcPointTest, NULL, NULL,
        NULL, NULL, pCoreTestData->pTestCfg, pCoreTestData,
        pCoreTestData->pTestCfg->TestStatusLogLevel,
        &pCoreTestData->pMemShmooData, pCoreTestData->pTestStat)) {
        return 1;
      }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
      // IF we're testing the shmoo directions separately
      if (pCoreTestData->ShmooDirCount != 1) {
        if (pCoreTestData->MemShmooCfg.ShmooDirection == LoDirection) {
          MinOffset = GetMemShmooMinOffset(pCoreTestData->pMemShmooData);
        }
        else {
          MaxOffset = GetMemShmooMaxOffset(pCoreTestData->pMemShmooData);
        }
      }
      // ELSE we're testing the shmoo directions together
      else
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
      {
        MinOffset = GetMemShmooMinOffset(pCoreTestData->pMemShmooData);
        MaxOffset = GetMemShmooMaxOffset(pCoreTestData->pMemShmooData);
      } // end else we're testing the shmoo directions separately
    } // end for each shmoo direction

    // Save the Min/Max offset per memory technology to CoreTestData, and fill up the 
    // metadata later. The pMemShmooData would be invalid once CleanupMemShmoo() is called.
    if (!IsMarginGroupDdrTSpecific(pCoreTestData->pTestCfg->MarginGroup)) {
      pCoreTestData->MinOffsets[0] = GetMemShmooMinOffsetByMemoryTech(FALSE, pCoreTestData->pMemShmooData);
      pCoreTestData->MaxOffsets[0] = GetMemShmooMaxOffsetByMemoryTech(FALSE, pCoreTestData->pMemShmooData);
      pCoreTestData->StepUnits[0] = GetMemShmooStepUnitByMemoryTech(FALSE, pCoreTestData->pMemShmooData);
    }

#if SUPPORT_FOR_DDRT
    if (pCoreTestData->HasDdrtDevices) {
      pCoreTestData->MinOffsets[1] = GetMemShmooMinOffsetByMemoryTech(TRUE, pCoreTestData->pMemShmooData);
      pCoreTestData->MaxOffsets[1] = GetMemShmooMaxOffsetByMemoryTech(TRUE, pCoreTestData->pMemShmooData);
      pCoreTestData->StepUnits[1]= GetMemShmooStepUnitByMemoryTech(TRUE, pCoreTestData->pMemShmooData);
    }
#endif // SUPPORT_FOR_DDRT

    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          // IF this channel is not enabled for validation THEN skip it
          if (!IsChannelValEnabled(SsaServicesHandle,
            &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
            &pCoreTestData->SelectMemPointTestCfg, Socket, Controller,
            Channel)) {
            continue;
          }

          // and an element for each rank combination at each possible offset
          *pResultEleCnt += \
            pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT] \
            [Socket][Controller][Channel] * \
            (((MaxOffset - MinOffset) / pCoreTestData->pTestCfg->StepSize) + 1);
        } // end for each channel
      } // end for each controller
    } // end for each socket

    // cleanup the shmoo test
    if (CleanupMemShmoo(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->MemShmooCfg,
      &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->pMemShmooData,
      pCoreTestData->pTestStat)) {
      return 1;
    }
  } // end for non-DDR-T and DDR-T devices

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  (GetResultEleCnt ResultEleCnt=%u)\n", *pResultEleCnt);
#endif

  return 0;
} // end function GetResultEleCnt

/**
@brief
  This is the margin parameter change handler function.

  @param[in, out]  pClientTestData   Pointer to client test data.
  @param[in]       IoLevel           I/O level.
  @param[in]       MarginGroup       Margin group.
  @param[in]       TestingOffset     Margin parameter offset being tested.
**/
static VOID MarginParamChgHandler(
  IN OUT VOID *pClientTestData,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN INT16 TestingOffset)
{
  CORE_TEST_DATA* pCoreTestData = (CORE_TEST_DATA*) pClientTestData;

  pCoreTestData->TestingOffset = TestingOffset;
} // end function MarginParamChgHandler

/**
@brief
  This is the definition for the point test result handler function.

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
  @param[out]      pTestStat             Pointer to return status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 PointTestResultHandler(
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
  MARGIN_1D_RESULT_COLUMNS *pElement;
  CORE_TEST_DATA* pCoreTestData = (CORE_TEST_DATA*) pVoidTestData;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "PointTestResultHandler(Socket=%u Controller=%u Channel=%u DimmA=%u RankA=%u\n",
    Socket, Controller, Channel, DimmRankA.Dimm, DimmRankA.Rank);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  DimmB=%u RankB=%u LaneErrorStatus=0x",
    DimmRankB.Dimm, DimmRankB.Rank);
  LogByteArrayAsHex (pLaneErrorStatus,
    (pCoreTestData->SystemInfo.BusWidth / 8));
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    " ErrorCount=%u Overflow=%u\n",
    ErrorCount.Bits.Count, ErrorCount.Bits.Overflow);
#endif

  // IF this channel isn't actually enabled for validation (can occur if this
  // channel has less than the maximum number of rank combinations and the
  // ExtendedRange feature is enabled )THEN skip it
  if (!IsChannelValEnabled(SsaServicesHandle, pSystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->CurrentMemPointTestCfg, Socket, Controller, Channel)) {
    return 0;
  }

  // IF this channel has fewer rank combinations than the one we're
  // currently on THEN this is repeat data so skip it
  if (pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT] \
    [Socket][Controller][Channel] <= RankCombinationIndex) {
    return 0;
  }

  // IF this result doesn't contain error information
  if (ErrorCount.Data == 0) {
#if SUPPORT_FOR_EXTENDED_RANGE
    // IF extended range testing is enabled
    if (pCoreTestData->pTestCfg->EnableExtendedRange) {
      // IF the margin parameter offset being tested is at or beyond one of the
      // limits
      if ((ErrorCount.Data == 0) &&
        (pCoreTestData->TestingOffset !=
        GetMemShmooMaxOfMinOffset(&pCoreTestData->SystemInfo,
        pCoreTestData->pMemShmooData, Socket, Controller, Channel)) &&
        (pCoreTestData->TestingOffset !=
        GetMemShmooMinOfMaxOffset(&pCoreTestData->SystemInfo,
        pCoreTestData->pMemShmooData, Socket, Controller, Channel))) {
        // we don't need to produce a result for it (always report when we're
        // at or beyond one of the limits)
        return 0;
      }
    }
    else
#endif // SUPPORT_FOR_EXTENDED_RANGE
    {
      // IF the margin parameter offset being tested is not one of the limits
      if ((pCoreTestData->TestingOffset !=
        GetMemShmooMinOffset(pCoreTestData->pMemShmooData)) &&
        (pCoreTestData->TestingOffset !=
        GetMemShmooMaxOffset(pCoreTestData->pMemShmooData))) {
        // we don't need to produce a result for it (always report when we're
        // at one of the limits)
        return 0;
      }
    } // end else  extended range testing is not enabled
  } // end if this result doesn't contain error information

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
      "Try increasing the StepSize and/or reducing the number of channels/DIMMs/ranks to test.\n");
    ProcError(SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat);
    return 1;
  }

  // IF this is the first result element:
  if (pCoreTestData->ResultElementCount == 0) {
    pCoreTestData->pResultElements = pElement;
  }
  pCoreTestData->ResultElementCount++;

  pElement->Socket               = Socket;
  pElement->Controller           = Controller;
  pElement->Channel              = Channel;
  pElement->DimmA                = DimmRankA.Dimm;
  pElement->RankA                = DimmRankA.Rank;
  pElement->DimmB                = DimmRankB.Dimm;
  pElement->RankB                = DimmRankB.Rank;
  pElement->RankCombinationIndex = RankCombinationIndex;
  pElement->Offset               = pCoreTestData->TestingOffset;

  ZeroMem (pElement->LaneErrorStatus, sizeof(pElement->LaneErrorStatus));
  CopyMem (pElement->LaneErrorStatus, pLaneErrorStatus, (pSystemInfo->BusWidth / 8));

  if (ErrorCount.Bits.Overflow) {
    pElement->ErrorCount = SPCL_ERR_CNT_OVERFLOW_VAL;
  }
  else {
    pElement->ErrorCount = ErrorCount.Bits.Count;
  }

#if ENBL_RESULTS_DEBUG_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "ResultElement=%d Socket=%u Controller=%u Channel=%u ",
    pCoreTestData->ResultElementCount, Socket, Controller, Channel);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "DimmA=%u RankA=%u DimmB=%u RankB=%u\n",
    DimmRankA.Dimm, DimmRankA.Rank, DimmRankB.Dimm, DimmRankB.Rank);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  RankCombinationIndex=%u Offset=%d LaneErrorStatus=0x",
    RankCombinationIndex, pElement->Offset);
  LogByteArrayAsHex (pElement->LaneErrorStatus,
    (pSystemInfo->BusWidth / 8));
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    " ErrorCount=%d\n", pElement->ErrorCount);
#endif // ENBL_RESULTS_DEBUG_LOG_MSGS

  return 0;
} // end function PointTestResultHandler

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
static UINT8 PostDimmInitHandler(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pClientTestCfg,
  IN OUT VOID *pClientTestData,
  IN BOOLEAN IsLastRankCombination,
  OUT TEST_STATUS *pTestStat)
{
  MARGIN_1D_CONFIG* pTestCfg = (MARGIN_1D_CONFIG*) pClientTestCfg;
  CORE_TEST_DATA* pCoreTestData = (CORE_TEST_DATA*) pClientTestData;
  BOOLEAN IsFirstCall, InitDqdbContent, IsLastCall;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "PostDimmInitHandler()\n");
#endif

  // cleanup the CPGC point test
  IsLastCall = FALSE;
  CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->CurrentMemPointTestCfg, IsLastCall,
    &pCoreTestData->pCpgcPointTestData);

  // re-initialize the CPGC engine
  IsFirstCall = FALSE;
  InitDqdbContent = FALSE;
  if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->CurrentMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    &PointTestResultHandler, pCoreTestData, pTestCfg->IoLevel,
    pCoreTestData->CurrentIsDdrT, pTestCfg->TestStatusLogLevel,
    IsFirstCall, InitDqdbContent, &pCoreTestData->pCpgcPointTestData,
    pTestStat)) {
    return 1;
  }

  // re-initialize the rank programming
  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->CurrentMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    pCoreTestData->pCpgcPointTestData,
    pCoreTestData->CurrentRankCombinationIndex,
    TRUE, // need to update HW register setting
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // IF we're currently in the binary search state
  if (GetInBinarySearchState(pCoreTestData->pMemShmooData)) {
    // restore binary search settings
    if (SetCpgcPointTestInBinarySearchState(SsaServicesHandle,
      &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      pCoreTestData->pCpgcPointTestData, TRUE, pTestStat)) {
      return 1;
    }
  }

  return 0;
} // end function PostDimmInitHandler


#if SUPPORT_FOR_DDRT
/**
@brief
  The function reset the CADB on-deselect setting and start a point test. It is a
  workaround to an issue that the on_deselect setting of the DDR4 could cause margin
  variation(drop) of the sub-sequentially DDRT test.

  @param[in, out]  SsaServicesHandle      Pointer to SSA services.
  @param[in]       pSystemInfo            Pointer to system information structure.
  @param[in]       pMemCfg                Pointer to memory configuration structure.
  @param[in]       pMemShmooCfg           Pointer to memory shmoo configuration structure.
  @param[in]       pMemPointTestCfg       Pointer to generic memory point test configuration structure.
  @param[in]       pCoreTestData          Pointer to MARGIN_1D_CORE_TEST_DATA structure
  @param[out]      pTestStat              Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 CleanUpWithPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_SHMOO_CONFIG *pMemShmooCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CORE_TEST_DATA* pCoreTestData,
  OUT TEST_STATUS *pTestStat)
{
  UINT8 Controller, Channel;
  CADB_MODE CadbModes_Saved[MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT];
  BOOLEAN HasAnyOnDeselectEnabled = FALSE;
  //BOOLEAN IsFirstCall, InitDqdbContent, IsLastCall;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "CleanUpWithPointTest()\n");
#endif

  // Skip if not a front side Cmd/Ctl test
  if (!((pMemShmooCfg->IoLevel == DdrLevel) &&
    (IsCmdMarginGroup(pMemShmooCfg->MarginGroup) ||
    IsCtlMarginGroup(pMemShmooCfg->MarginGroup)))) return 0;

  // Skip if there is no DDRT device involved.
  if (!pCoreTestData->HasTestedChnlWithMixedDdrtDdr4) return 0;

  // Save current CADB mode setting
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++){
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++){
      CadbModes_Saved[Controller][Channel] = pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel];
      if (pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel] == OnDeselectCadbMode) {
        HasAnyOnDeselectEnabled = TRUE;
      }
    }
  }

  // Skip if 
  if (!HasAnyOnDeselectEnabled) return 0;

  // Override the cadb on-deselect
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++){
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++){
      if (pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel]!= NormalCadbMode) {
        pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel] = NormalCadbMode;
      }
    }
  }

  // Re-initialize the rank programming which will re-program the CADB control registers.
  if (SetCpgcPointTestRankCombinationIndex(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->CurrentMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    pCoreTestData->pCpgcPointTestData,
    pCoreTestData->CurrentRankCombinationIndex,
    TRUE, // need to update HW register setting
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // Do not save the test results. Change the result handler to NULL
  if (ChangeCpgcPointTestResultHandler(SsaServicesHandle, NULL ,pCoreTestData->pCpgcPointTestData)) {
    return 1;
  }
  // Run a point test at the current offset which should be nominal
  if (RunCpgcPointTest(
    SsaServicesHandle, pSystemInfo, pMemCfg, pMemPointTestCfg,
    &pCoreTestData->CpgcPointTestCfg, pCoreTestData->pCpgcPointTestData, NULL,
    pTestStat)) {
      return 1;
  }

  // Restore current CADB mode setting. It will take effect when set up the next test rank.
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++){
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++){
      pCoreTestData->CpgcPointTestCfg.CadbModes[Controller][Channel] = CadbModes_Saved[Controller][Channel];
    }
  }

  // Restore the result handler
  if (ChangeCpgcPointTestResultHandler(SsaServicesHandle, &PointTestResultHandler ,pCoreTestData->pCpgcPointTestData)) {
    return 1;
  }

  return 0;
} // end function CleanUpWithPointTest
#endif SUPPORT_FOR_DDRT

#if SUPPORT_FOR_EXTENDED_RANGE
/*
@brief
  This function is used to fixup extended range results.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in, out]  pCoreTestData       Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 FixupResults(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData)
{
  SSA_STATUS RetStat;
  MARGIN_PARAM_SPECIFICITY Specificity;
  UINT8 Socket, Controller, Channel, DimmA, RankA, DimmB, RankB;
  UINT8 LaneBitmasks[MAX_BUS_WIDTH / 8];
  UINT16 SettleDelay;
  INT16 MinOffset, MaxOffset;
  UINT16 StepUnit;
  UINT32 Idx;
  MRC_DIMM_INFO DimmInfo;
  UINT8 DeviceWidth = 8;
  INT16 Offset;
  UINT8 ByteGroup, Strobe, Lane;
  UINT8 ByteGroupBitmask;
  BOOLEAN IsDoingTurnarounds;
  MARGIN_1D_RESULT_COLUMNS *pResultElement;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "FixupResults(StartIndex=%u ElementCount=%u)\n",
    pCoreTestData->FixupStartResultElementIndex,
    pCoreTestData->ResultElementCount);
#endif

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  GetMarginParamSpecificity(IoLevel=%u MarginGroup=%u)\n",
    pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup);
#endif
  RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamSpecificity(
    SsaServicesHandle, pCoreTestData->pTestCfg->IoLevel,
    pCoreTestData->pTestCfg->MarginGroup, &Specificity);
  if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
    pCoreTestData->pTestStat)) {
    return 1;
  }
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "    Specificity=0x%X\n", Specificity);
#endif

  // FOR each relevant result element:
  for (Idx = pCoreTestData->FixupStartResultElementIndex;
    Idx < pCoreTestData->ResultElementCount; Idx++) {
    pResultElement = &(pCoreTestData->pResultElements[Idx]);

    Socket = pResultElement->Socket;
    Controller = pResultElement->Controller;
    Channel = pResultElement->Channel;
    DimmA = pResultElement->DimmA;
    RankA = pResultElement->RankA;
    DimmB = pResultElement->DimmB;
    RankB = pResultElement->RankB;
    Offset = pResultElement->Offset;

#if ENBL_FIXUP_DEBUG_LOG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "\n"
      "  Idx=%u Socket=%u Controller=%u Channel=%u ",
      Idx, Socket, Controller, Channel);
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "DimmA=%u RankA=%u DimmB=%u RankB=%u Offset=%d\n",
      DimmA, RankA, DimmB, RankB, Offset);
#endif // ENBL_FIXUP_DEBUG_LOG_MSGS

#if SUPPORT_FOR_TURNAROUNDS
    if (pCoreTestData->pTestCfg->RankTestModes[Controller][Channel] == Turnarounds) {
      IsDoingTurnarounds = CanChannelDoTurnarounds(SsaServicesHandle,
        &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
        &pCoreTestData->CurrentMemPointTestCfg, pCoreTestData->pTestCfg->IoLevel,
        Socket, Controller, Channel, pCoreTestData->CurrentIsDdrT);
    }
    else
#endif  // SUPPORT_FOR_TURNAROUNDS
    {
      IsDoingTurnarounds = FALSE;
    }

    // IF the margin parameter is strobe specific
    if (Specificity & StrobeSpecific) {
      // IF the device is an LRDIMM and the margin parameter is frontside TxVref or backside RxVref
      if ((pCoreTestData->MemCfg.IsLrDimms) &&
        (!pCoreTestData->CurrentIsDdrT) &&
        (((pCoreTestData->pTestCfg->IoLevel == DdrLevel) &&
        (pCoreTestData->pTestCfg->MarginGroup == TxVref)) ||
        ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
        (pCoreTestData->pTestCfg->MarginGroup == RxVref)))) {
        // the strobe width is 8
        DeviceWidth = 8;
      }
      else {
        // use the actual DIMM device width
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  GetDimmInfo(Socket=%u Controller=%u Channel=%u Dimm=%u)\n",
          Socket, Controller, Channel, DimmA);
#endif
        SsaServicesHandle->SsaMemoryConfig->GetDimmInfo(SsaServicesHandle,
          Socket, Controller, Channel, DimmA, &DimmInfo);

        DeviceWidth = \
          (DimmInfo.DeviceWidth == 0) ? 4 : DimmInfo.DeviceWidth;

        // Override x16 to x8 to calculate the lane mask. Otherwise would cause divided by 0 error.
        if (DeviceWidth > 8) {
          DeviceWidth = 8;
        }

#if SUPPORT_FOR_TURNAROUNDS
        // IF this channel is doing rank-to-rank turnarounds and the DIMMs are
        // different
        if (IsDoingTurnarounds && (DimmB != DimmA)) {
          UINT8 DeviceWidthB;
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  GetDimmInfo(Socket=%u Controller=%u Channel=%u Dimm=%u)\n",
            Socket, Controller, Channel, DimmB);
#endif
          SsaServicesHandle->SsaMemoryConfig->GetDimmInfo(SsaServicesHandle,
            Socket, Controller, Channel, DimmB, &DimmInfo);

          DeviceWidthB = \
            (DimmInfo.DeviceWidth == 0) ? 4 : DimmInfo.DeviceWidth;

          if (DeviceWidthB < DeviceWidth) {
            DeviceWidth = DeviceWidthB;
          }
        } // end if this channel is doing rank-to-rank turnarounds and ...
#endif // SUPPORT_FOR_TURNAROUNDS
      }
    } // end if the margin parameter is strobe specific

    // FOR each lane:
    for (Lane = 0; Lane < pCoreTestData->SystemInfo.BusWidth; Lane++) {
      // IF the margin parameter is lane specific
      if (Specificity & LaneSpecific) {
        // set the lane bitmasks to just this lane
        ZeroMem (LaneBitmasks, sizeof(LaneBitmasks));

        ByteGroup = Lane / 8;

        LaneBitmasks[ByteGroup] = (1 << (Lane % 8)) & \
          pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup];

        // IF this lane isn't being validated THEN skip it
        if (LaneBitmasks[Lane / 8] == 0) {
          continue;
        }
      }
      // ELSE IF the margin parameter is strobe specific
      else if (Specificity & StrobeSpecific) {
        // IF this lane is not the first of a strobe group THEN skip it
        if ((Lane % DeviceWidth) != 0) {
          continue;
        }

        // set the lane bitmasks to just this strobe group
        ZeroMem (LaneBitmasks, sizeof(LaneBitmasks));

        Strobe = Lane / DeviceWidth;

        ByteGroup = Strobe / (8 / DeviceWidth);

        ByteGroupBitmask = (1 << DeviceWidth) - 1;

        ByteGroupBitmask <<= (Strobe % (8 / DeviceWidth)) * DeviceWidth;

        LaneBitmasks[ByteGroup] = ByteGroupBitmask & \
          pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup];

        // IF no lane in the strobe group is being validated THEN skip it
        if (LaneBitmasks[ByteGroup] == 0) {
          continue;
        }
      }
      // ELSE the margin parameter is neither lane nor strobe specific
      else {
        if (Lane != 0) {
          continue;
        }

        CopyMem (LaneBitmasks,
          pCoreTestData->pTestCfg->LaneValBitmasks[Controller][Channel],
          sizeof(LaneBitmasks));
      } // end else the margin parameter is neither lane nor strobe specific

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  GetMarginParamLimits(\n"
        "    Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n"
        "    LaneBitmasks=0x",
        Socket, Controller, Channel, DimmA, RankA);
      LogByteArrayAsHex (LaneBitmasks,
        (pCoreTestData->SystemInfo.BusWidth / 8));
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        " IoLevel=%u MarginGroup=%u)\n",
        pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup);
#endif
      RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamLimits(
        SsaServicesHandle, Socket, Controller, Channel, DimmA, RankA,
        LaneBitmasks, pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup,
        &MinOffset, &MaxOffset, &SettleDelay, &StepUnit);
      // IF the margin parameter isn't supported for this device THEN skip it
      if (RetStat == UnsupportedValue) {
        continue;
      }
      if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
        pCoreTestData->pTestStat)) {
        return 1;
      }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "    MinOffset=%d MaxOffset=%d SettleDelay=%u\n",
        MinOffset, MaxOffset, SettleDelay);
#endif
      // IF this channel is doing rank-to-rank turnarounds and the ranks are
      // different
      if (IsDoingTurnarounds && ((DimmA != DimmB) || (RankA != RankB))) {
        INT16 MinOffsetB, MaxOffsetB;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  GetMarginParamLimits(\n"
          "    Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n"
          "    LaneBitmasks=0x",
          Socket, Controller, Channel, DimmB, RankB);
        LogByteArrayAsHex (LaneBitmasks,
          (pCoreTestData->SystemInfo.BusWidth / 8));
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          " IoLevel=%u MarginGroup=%u)\n",
          pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup);
#endif
        RetStat = SsaServicesHandle->SsaMemoryConfig->GetMarginParamLimits(
          SsaServicesHandle, Socket, Controller, Channel, DimmB, RankB,
          LaneBitmasks, pCoreTestData->pTestCfg->IoLevel,
          pCoreTestData->pTestCfg->MarginGroup,
          &MinOffsetB, &MaxOffsetB, &SettleDelay, &StepUnit);
        if (ChkRetStat(SsaServicesHandle, RetStat, __FILE__, __LINE__,
          pCoreTestData->pTestStat)) {
          return 1;
        }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_FIXUP_DEBUG_LOG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "    MinOffset=%d MaxOffset=%d SettleDelay=%u\n",
          MinOffsetB, MaxOffsetB, SettleDelay);
#endif
        // IF rank B's minimum offset is inside rank A's minimum offset THEN
        if (MinOffsetB > MinOffset) {
          // use rank B's value
          MinOffset = MinOffsetB;
        }

        // IF rank B's maximum offset is inside rank A's maximum offset THEN
        if (MaxOffsetB < MaxOffset) {
          // use rank B's value
          MaxOffset = MaxOffsetB;
        }
      } // end if this channel is doing rank-to-rank turnarounds and ...

      // IF the result element's offset is outside the valid range
      if ((Offset < MinOffset) || (Offset > MaxOffset)) {
#if ENBL_FIXUP_DEBUG_LOG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "Fixing up result Index=%u Socket=%u Controller=%u Channel=%u ",
          Idx, Socket, Controller, Channel);
        if (IsDoingTurnarounds) {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "DimmA=%u RankA=%u DimmB=%u RankB=%u:\n",
            DimmA, RankA, DimmB, RankB);
        }
        else {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "Dimm=%u Rank=%u:\n",
            DimmA, RankA);
        }
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  LaneBitmask=0x");
        LogByteArrayAsHex (LaneBitmasks,
          (pCoreTestData->SystemInfo.BusWidth / 8));
        if (Offset < MinOffset) {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            " Offset=%d MinOffset=%d\n",
            Offset, MinOffset);
        }
        else {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            " Offset=%d MaxOffset=%d\n",
            Offset, MaxOffset);
        }
#endif // ENBL_FIXUP_DEBUG_LOG_MSGS
        // set the lane bit(s) to indicate a failure occurred
        for (ByteGroup = 0;
          ByteGroup < (pCoreTestData->SystemInfo.BusWidth / 8); ByteGroup++) {
          pResultElement->LaneErrorStatus[ByteGroup] |= LaneBitmasks[ByteGroup];
        }
      } // end if the result element's offset is outside the valid range
    } // end for each lane
  } // end for each relevant result element

  return 0;
} // end function FixupResults
#endif // SUPPORT_FOR_EXTENDED_RANGE

/**
@brief
  This function performs JEDEC initializations at the end of the test (if
  required) to ensure the DIMMs are left in a functional state.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 CleanupJedecInit(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData)
{
  UINT8 Socket, Controller;
#if SUPPORT_FOR_DDRT
  PSYSHOST Host = GetSysHostPointer ();
  UINT8 Channel, Dimm;
#endif // SUPPORT_FOR_DDRT

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "CleanupJedecInit()\n");
#endif

  // IF JEDEC initialization is required
  if (IsJedecInitAfterErrRequired(SsaServicesHandle,
    &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, pCoreTestData->pTestCfg->IoLevel,
    pCoreTestData->pTestCfg->MarginGroup)) {
    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // IF the controller is not enabled for testing THEN skip it
        if (!IsControllerTestingEnabled(SsaServicesHandle,
          &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
          &pCoreTestData->SelectMemPointTestCfg, Socket, Controller)) {
          continue;
        }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  IoReset(Socket=%u Controller=%u)\n",
          Socket, Controller);
#endif
        SsaServicesHandle->SsaMemoryConfig->IoReset(SsaServicesHandle,
          Socket, Controller);
      } // end for each controller
    } // end for each socket

    // do an AEP IO reset if the dimm is AEP dimm.
#if SUPPORT_FOR_DDRT
    // IF the system has DDR-T devices
    if (SSAIsDdrtDimmPresent (Host)) {
      // FOR each socket:
      for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
        // IF the socket is not enabled for testing THEN skip it
        if (!IsSocketTestingEnabled(SsaServicesHandle, &pCoreTestData->SystemInfo,
          &pCoreTestData->SelectMemPointTestCfg, Socket)) {
          continue;
        }

        // FOR each controller:
        for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
          // FOR each channel:
          for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
            // IF the channel is not enabled for testing THEN skip it
            if (!IsChannelTestingEnabled(SsaServicesHandle,
              &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
              &pCoreTestData->SelectMemPointTestCfg, Socket, Controller,
              Channel)) {
              continue;
            }

            for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
              // IF this DIMM isn't DDR-T THEN skip it
              if (((pCoreTestData->MemCfg.DimmBitmasks[Socket][Controller][Channel] & (1 << Dimm)) == 0) ||
                (pCoreTestData->MemCfg.MemoryTech[Socket][Controller][Channel][Dimm] != SsaMemoryDdrT)) {
                continue;
              }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
              NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
                "  AepIoReset(Socket=%u Controller=%u Channel=%u Dimm=%u)\n",
                Socket, Controller, Channel, Dimm);
#endif
              SsaServicesHandle->SsaMemoryDdrtConfig->AepIoReset(
                SsaServicesHandle, Socket, Controller, Channel, Dimm);
            } // end for each DIMM
          } // end for each channel
        } // end for each controller
        // do FIFO train reset
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  ResetAepFifoTrain(Socket=%u)\n", Socket);
#endif
        SsaServicesHandle->SsaMemoryDdrtConfig->ResetAepFifoTrain(
          SsaServicesHandle, Socket);
      } // end for each socket
    } // end if we're testing DDR-T devices
#endif // SUPPORT_FOR_DDRT

    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // IF the controller is not enabled for testing THEN skip it
        if (!IsControllerTestingEnabled(SsaServicesHandle,
          &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
          &pCoreTestData->SelectMemPointTestCfg, Socket, Controller)) {
          continue;
        }
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

  return 0;
} // end function CleanupJedecInit

/**
  This function is used to get the current targted DIMM/RANK from the core test data.
  The info is needed to report test results and set up the 1D sweep

  @param[in ]  SsaServicesHandle  Pointer to SSA services.
  @param[in ]  pCoreTestData      Pointer to RMT core test data.
  @param[in ]  Socket             Socket number
  @param[out]  DimmRank           Pointer to the Dimm/rank array

**/
VOID
GetTargetedDimmRank (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CORE_TEST_DATA        *pCoreTestData,
  IN UINT8                  Socket,
  OUT MRC_RT               (*DimmRank)[MAX_CH]
)
{
  UINT8 Controller;
  UINT8 Channel;
  UINT8 ChannelInSocket;
  UINT8 PairCount = 1;
  DIMM_RANK_PAIR DimmRankPair;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "Margin1DCore.GetTargetedDimmRank()\n");
#endif

  ZeroMem (DimmRank, sizeof (*DimmRank));

  // FOR each controller:
  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {

      // IF this channel is not enabled for validation THEN skip it
      if (!IsChannelTestingEnabled (SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg,
        &pCoreTestData->SelectMemPointTestCfg,
        Socket, Controller, Channel)) {
        continue;
      }

      GetCpgcPointTestDimmRanks (SsaServicesHandle,
        &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
        &pCoreTestData->SelectMemPointTestCfg, pCoreTestData->pCpgcPointTestData, Socket,
        Controller, Channel, &PairCount, &DimmRankPair,
        pCoreTestData->pTestStat);

      ChannelInSocket = MAX_MC_CH * Controller + Channel;

      (*DimmRank)[ChannelInSocket].Dimm = DimmRankPair.Dimm;
      (*DimmRank)[ChannelInSocket].Rank = DimmRankPair.Rank;

    } // end for each channel
  } // end for each controller

} // end function  GetTargetedDimmRank

/**
  This function is used to get the channel mask for the 1D Sweep parameter

  @param[in ]  SsaServicesHandle    Pointer to SSA services.
  @param[in ]  pCoreTestData        Pointer to RMT core test data.
  @param[in ]  Socket               Socket number

  Retval Channel mask
**/
STATIC
UINT32
Get1DSweepChannelMask (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     UINT8                  Socket
  )
{
  UINT32 ChannelMask;
  UINT8  Controller;
  UINT8  Channel;

  ChannelMask = 0;

  for (Controller = 0; Controller < MAX_IMC; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_MC_CH; Channel++) {
      // IF this channel is not enabled for validation THEN skip it
      if (!IsChannelValEnabled (SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg,
        &pCoreTestData->CurrentMemPointTestCfg,
        Socket, Controller, Channel)) {
        continue;
      }

      ChannelMask |= 1 << (MAX_MC_CH * Controller + Channel);
    } // Channel
  } // Controller

  return ChannelMask;
}

/**
  This function is used to initialize the 1D Sweep parameter

  @param[in, out]  SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       Socket               Socket number.
  @param[in]       Direction            Margin direction.
  @param[out]      CommonParamaters     Pointer to the 1D sweep context information

**/
STATIC
VOID
InitializeMargin1DSweepParameter (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     UINT8                  Socket,
  IN     SHMOO_DIRECTION        Direction,
  OUT    COMMON_1D_PARAMETERS   *CommonParameters
  )
{
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

  MRC_MST MemSsType;

  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  //
  // Get the Channel mask
  //
  ChannelBitMask = Get1DSweepChannelMask (SsaServicesHandle, pCoreTestData, Socket);

  //
  // Get the list of DIMM/RANK
  //
  GetTargetedDimmRank (SsaServicesHandle, pCoreTestData, Socket, &DimmRank);

  //
  // Get margin parameter
  //
  Level = pCoreTestData->pTestCfg->IoLevel;
  Group = pCoreTestData->pTestCfg->MarginGroup;
  StepSize = pCoreTestData->pTestCfg->StepSize;
  DitherValue = pCoreTestData->pTestCfg->GuardBandSize;

  OffsetLeft = -1;
  OffsetRight = 0;
  LimitOffset = 0;

  //
  // Flags
  //
  FlagsOffsets = DefaultStartOffset;
  FlagsResults = ResultsPerBit | ResultsRelative;

  //
  // Margin to the first error for Cmd/Ctl
  //
  if (CheckCMDGroup (Group) || CheckCTLGroup (Group)) {
    FlagsResults |= ResultsPerSubChannel;
  }

  if (Direction == LoDirection) {
    FlagsFsm = LeftSideOnly;
  } else if (Direction == HiDirection) {
    FlagsFsm = RightSideOnly;
  } else {
    FlagsFsm = DefaultFsm; // both side.
  }
  
  FlagsFsm |= AvoidBoundaryCross;
  //
  // Because the Margin1D tracks the error count and the count value responds to the offset from the nominal
  // setting. Keep incrementing the offset even the strobe already fails.
  //
  FlagsFsm |= ContinousMode;

  CreateContextParameters (CommonParameters, MemSsType, Socket, ~ChannelBitMask, 0, ALL_SUBCH, DimmRank, Level, Group, gsmCsnDelim,
    StepSize, DitherValue, OffsetLeft, OffsetRight, LimitOffset, FlagsOffsets, FlagsResults, FlagsFsm);
}

/**
  This function is used to run the Margin1D test with 1D sweep in CA/CS training mode.

  @param[in]       SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       ShmooDir             Margin direction.

  @retval  N/A
**/
VOID
RunMargin1DWith1DSweepInCaCsTrainingMode (
  IN     SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     SHMOO_DIRECTION        ShmooDir
  )
{
  // find the channel list, dimm/rank
  // based  on the margin group, call CA/CS worker

  // save the result to margin1D structure
  // may need to update the lane count
  UINT8             Socket;
  UINT32            ChannelBitMask;
  MRC_RT            DimmRank[MAX_CH];
  MRC_LT            Level;
  MRC_GT            Group;
  UINT8             StepSize;
  struct baseMargin DcaSweepResults[MAX_CH][SUB_CH][RCD_CA_SIGNALS_TO_SWEEP];
  struct baseMargin DcsSweepResults[MAX_CH][SUB_CH];
  MRC_STATUS        Status;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "RmtCore.RunMargin1DWith1DSweepInCaCsTrainingMode()\n");
#endif

  ZeroMem (DcaSweepResults, sizeof (DcaSweepResults));
  ZeroMem (DcsSweepResults, sizeof (DcsSweepResults));

  //
  // Get margin parameter
  //
  Level = pCoreTestData->pTestCfg->IoLevel;
  Group = pCoreTestData->pTestCfg->MarginGroup;
  StepSize = pCoreTestData->pTestCfg->StepSize;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not configured to be tested THEN skip it
    if ((pCoreTestData->CurrentMemPointTestCfg.SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }
    //
    // Get the Channel mask
    //
    ChannelBitMask = Get1DSweepChannelMask (SsaServicesHandle, pCoreTestData, Socket);

    //
    // Get the list of DIMM/RANK
    //
    GetTargetedDimmRank (SsaServicesHandle, pCoreTestData, Socket, &DimmRank);

    if ((Group == DcaDelay) || (Group == RcdDcaVref) || (Group == DcaDfeVref)) {

      Status = CaMargins1DWorker (&pCoreTestData->CpgcPointTestCfg, Socket, ~ChannelBitMask, DimmRank, Level, Group, (UINT8)ShmooDir,
        StepSize, DCA_DUTY_CYCLE_COUNTING_WINDOW_VALUE, &DcaSweepResults);

      //
      // Save to Margin1D result.
      //
      if (Status == MRC_STATUS_SUCCESS) {
        SaveMargin1DResultInCaTrainingMode (SsaServicesHandle, pCoreTestData, ShmooDir, Socket, ~ChannelBitMask, DimmRank, &DcaSweepResults);
      } // Status = MRC_STATUS_SUCCESS
    } else if (Group == DcsDelay) {
      Status = CsMargins1DWorker (&pCoreTestData->CpgcPointTestCfg, Socket, ~ChannelBitMask, DimmRank, Level, Group, (UINT8)ShmooDir,
        StepSize, CSCA_TF_COUNTING_WINDOW_VALUE, &DcsSweepResults);

      if (Status == MRC_STATUS_SUCCESS) {
       SaveMargin1DResultInCsTrainingMode (SsaServicesHandle, pCoreTestData, ShmooDir, Socket, ~ChannelBitMask, DimmRank, &DcsSweepResults);
      } // Status = MRC_STATUS_SUCCESS
    }
  }// Socket
}

/**
  This function is used to run the Margin1D test with 1D sweep.

  @param[in, out]  SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       LaneShmooStopMode    Lane shmoo stop mode.
  @param[in]       ShmooDirCount        Number of entries in pShmooDirs array.
  @param[in]       pShmooDirs           Pointer to array of shmoo direction values.

  @retval  0  success
  @retval  1  error occurred
**/
STATIC
UINT8 
RunMargin1DWith1DSweep (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CORE_TEST_DATA        *pCoreTestData,
  IN SHMOO_DIRECTION        ShmooDir
  )
{
  MARGIN_1D_PARAMETER Margin1DSweep;
  UINT8       Socket;
  RESULTS_1D  Results;
  Callback1D  Pre1DSweepHandler;
  Callback1D  Post1DSweepHandler;
  Callback1D  ResultProcessor;
  Callback1D  ErrorHandler;
  MRC_LT      MarginGroup;
#ifdef SPR_PO_FLAG
  UINT32      PrevDebugLevel;
#endif
#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "MarginCore.RunMargin1DWith1DSweep()\n");
#endif

  //
  // If margin group requires being margined in CA/CS training mode, call the CA/CS training mode version.
  //
  if (UseCaCsTrainingMode (pCoreTestData->pTestCfg->IoLevel, pCoreTestData->pTestCfg->MarginGroup)) {
    RunMargin1DWith1DSweepInCaCsTrainingMode (SsaServicesHandle, pCoreTestData, ShmooDir);
    return 0;
  }

  //
  // Initialize private data
  //
  Margin1DSweep.SsaServicesHandle = SsaServicesHandle;
  Margin1DSweep.CoreTestData = pCoreTestData;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not configured to be tested THEN skip it
    if ((pCoreTestData->CurrentMemPointTestCfg.SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    InitializeMargin1DSweepParameter (SsaServicesHandle, pCoreTestData, Socket, ShmooDir, &Margin1DSweep.CommonParameters);

    //
    // Allocate result
    //
    AllocatePoolForResults1D (&Margin1DSweep.CommonParameters, &Results);

#ifdef SPR_PO_FLAG
      //
      // Enable 1D sweep debug log
      //
      PrevDebugLevel = GetDebugLevel ();
      (&Margin1DSweep.CommonParameters)->DebugLevel |= SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0;
      SetDebugLevel (SDBG_SWEEP_L3 | SDBG_SWEEP_L2 | SDBG_SWEEP_L1 |SDBG_SWEEP_L0 |PrevDebugLevel);
#endif
    //
    // update callback function
    //
    ResultProcessor = Margin1DResultHandler;
    ErrorHandler = NULL;
    Pre1DSweepHandler = Margin1DPre1DSweep;
    Post1DSweepHandler = Margin1DPost1DSweep;

    MarginGroup = (&Margin1DSweep.CommonParameters)->Group;

    if (CheckCMDGroup (MarginGroup) || CheckCTLGroup (MarginGroup)) {
      ErrorHandler = ErrorHandlerAdvanceCmdCtl;
    } else if (MarginGroup == TxVref) {
#ifndef DDR5_SUPPORT
    ErrorHandler = ErrorHandlerAdvanceTxVref;
#endif
    }

    ProcessSweep1D (&Margin1DSweep.CommonParameters, Pre1DSweepHandler, Margin1DSetupTest, Margin1DExecuteTest, ErrorHandler, Post1DSweepHandler, ResultProcessor, &Results);
#ifdef SPR_PO_FLAG
      //
      // Restore debug level
      //
      SetDebugLevel (PrevDebugLevel);
#endif
    //
    // Release 1D sweep result
    //
    ReleaseResources1D (&Margin1DSweep.CommonParameters, &Results);

  }// Socket

  return 0;
} // end function RunMargin1DWith1DSweep

/*
@brief
  This function is used to run a Margin1D test.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in, out]  pCoreTestData         Pointer to core test data structure.
  @param[in]       InitSpecificMetadata  Pointer to function to initialize product specific metadata.  A NULL value is acceptable.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunMargin1DTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA *pCoreTestData,
  IN INIT_SPECIFIC_METADATA InitSpecificMetadata)
{
  BOOLEAN InitDqdbContent, IsFirstCall, IsLastCall;
  BOOLEAN InitAsAll;
  CHAR8 *GroupString = NULL;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RunMargin1DTest()\n");
#endif
  
  if (EFI_ERROR (GetGroupString (pCoreTestData->pTestCfg->MarginGroup, &GroupString))) {
    GroupString = "Unknown group";
  }
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "IoLevel=%u (%s) MarginGroup=%u (%s)\n",
    pCoreTestData->pTestCfg->IoLevel,
    GetLevelStr(pCoreTestData->pTestCfg->IoLevel),
    pCoreTestData->pTestCfg->MarginGroup,
    GroupString);

  InitRCompUpdate(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg, Setup,
    pCoreTestData->PrevRCompUpdateEnables);

  InitZQCal(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg, Setup,
    pCoreTestData->PrevZQCalEnables);

  pCoreTestData->ShmooDirCount = 2;
  pCoreTestData->ShmooDirs[0] = LoDirection;
  pCoreTestData->ShmooDirs[1] = HiDirection;

  InitMetadata(SsaServicesHandle, pCoreTestData);
  if (InitSpecificMetadata) {
    InitSpecificMetadata(SsaServicesHandle, pCoreTestData);
  }

  IsFirstCall = TRUE;

  // FOR non-DDR-T and DDR-T devices (they're tested separately because their
  // margin parameters are different and therefore may have different ranges):
  for (pCoreTestData->CurrentIsDdrT = 0;
#if SUPPORT_FOR_DDRT
    pCoreTestData->CurrentIsDdrT <= 0;
#else
    pCoreTestData->CurrentIsDdrT <= 0;
#endif // !SUPPORT_FOR_DDRT
    pCoreTestData->CurrentIsDdrT++) {

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF this is for backside non-Cmd/Ctl margining and non-LRDIMM, non-DDR-T
    // devices THEN skip it
    if ((pCoreTestData->pTestCfg->IoLevel == LrbufLevel) &&
      (pCoreTestData->pTestCfg->MarginGroup != CmdAll) &&
      (pCoreTestData->pTestCfg->MarginGroup != CmdVref) &&
      (pCoreTestData->pTestCfg->MarginGroup != CtlAll) &&
      !pCoreTestData->MemCfg.IsLrDimms &&
      !pCoreTestData->HasDdrtDevices) {    // PT ideally, we should change the MemCfg.IsLrDimms to cover both ddr4 lrdimm and ddrt
      continue;
    }
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    pCoreTestData->MemShmooCfg.IsDdrT = TRUE; // It needs to be TRUE, otherwise the SetupMemShmoo() would not init the DDRT device training mode. 

    InitAsAll = FALSE;  // TODOD remove it!
    if (InitSelectMemPointTestCfg(SsaServicesHandle, pCoreTestData, InitAsAll)) {
      return 1;
    }

#if SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT
    // IF this is for backside Cmd/Ctl margining
    if (pCoreTestData->IsBacksideCmdCtl) {
      UINT8 MarginGroupCount = 1;
      InitBacksideCmdBitmask(SsaServicesHandle, &pCoreTestData->SystemInfo,
        &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
        &pCoreTestData->pTestCfg->MarginGroup, MarginGroupCount,
        pCoreTestData->BacksideCmdBitmasks);

      // IF there are no devices with a backside THEN skip it
      if (!IsAnyBacksideCmdBitSet(pCoreTestData->BacksideCmdBitmasks)) {
        continue;
      }
    } // end if this is for backside Cmd/Ctl margining
#endif // SUPPORT_FOR_RDIMM || SUPPORT_FOR_LRDIMM || SUPPORT_FOR_DDRT

    if (GetRankCombinationsCnts(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      pCoreTestData->pTestCfg->IoLevel, pCoreTestData->CurrentIsDdrT,
      &pCoreTestData->RankCombinationCnts[pCoreTestData->CurrentIsDdrT],
      &pCoreTestData->MaxRankCombinationsCnts[pCoreTestData->CurrentIsDdrT],
      pCoreTestData->pTestStat)) {
      return 1;
    }

    //
    // This function allocates the point test data which stores the 
    // per chanel test data which contains the current test DIMM/RANK pair.
    //
    InitDqdbContent = FALSE;
    if (SetupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
      &PointTestResultHandler, pCoreTestData, pCoreTestData->pTestCfg->IoLevel,
      pCoreTestData->CurrentIsDdrT,
      pCoreTestData->pTestCfg->TestStatusLogLevel, IsFirstCall,
      InitDqdbContent, &pCoreTestData->pCpgcPointTestData,
      pCoreTestData->pTestStat)) {
      return 1;
    }

    IsFirstCall = FALSE;

    // FOR each rank combination:
    for (pCoreTestData->CurrentRankCombinationIndex = 0;
      pCoreTestData->CurrentRankCombinationIndex <
      pCoreTestData->MaxRankCombinationsCnts[pCoreTestData->CurrentIsDdrT];
      pCoreTestData->CurrentRankCombinationIndex++) {
      if (pCoreTestData->pTestCfg->TestStatusLogLevel >= 1) {
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "\n"
          "Testing rank combination %u of %u.",
          (pCoreTestData->CurrentRankCombinationIndex + 1),
          pCoreTestData->MaxRankCombinationsCnts[pCoreTestData->CurrentIsDdrT]);
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR, "\n");
      }

      //
      // This function updates the point test data with the current Dimm/Rank pair!
      //
      if (InitCurrentMemPointTestCfg(SsaServicesHandle, pCoreTestData)) {
        return 1;
      }

      // FOR each shmoo direction (backside command margin parameters must have
      // the calls to InitMarginParam performed in a per shmoo direction basis):
      for (pCoreTestData->CurrentShmooDirIdx = 0;
        pCoreTestData->CurrentShmooDirIdx < pCoreTestData->ShmooDirCount;
        pCoreTestData->CurrentShmooDirIdx++) {

        pCoreTestData->MemShmooCfg.ShmooDirection = \
          pCoreTestData->ShmooDirs[pCoreTestData->CurrentShmooDirIdx];

        // IF we're testing the shmoo directions separately
        if (pCoreTestData->ShmooDirCount != 1) {
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "\n"
            "Testing %s side.\n",
            (pCoreTestData->MemShmooCfg.ShmooDirection == LoDirection) ? "Low":"High");
        } // end if we're testing the shmoo directions separately

        //
        // Call 1D sweep
        //
        RunMargin1DWith1DSweep (SsaServicesHandle, pCoreTestData, pCoreTestData->ShmooDirs[pCoreTestData->CurrentShmooDirIdx]);

      } // end for each shmoo direction

    } // end for each rank combination


    // cleanup the CPGC point test
    if (pCoreTestData->pTestCfg->EnableMarginPlot) {
      // this is not the last call to CleanupCpgcPointTest
      // PlotLaneMarginResults will clean up the Cpgc poin test data
      IsLastCall = FALSE;
    }
    else {
      IsLastCall = TRUE;
    }

    CleanupCpgcPointTest(SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
      IsLastCall, &pCoreTestData->pCpgcPointTestData);

  } // end for FOR non-DDR-T and DDR-T devices

  if (pCoreTestData->pTestCfg->EnableMarginPlot) {
    if (PlotLaneMarginResults(SsaServicesHandle, pCoreTestData)) {
      return 1;
    }
  }

  // clean up the platform
  InitZQCal(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg, Cleanup,
    pCoreTestData->PrevZQCalEnables);

  InitRCompUpdate(SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg, Cleanup,
    pCoreTestData->PrevRCompUpdateEnables);

  return 0;
} // end function RunMargin1DTest


// end file Margin1DCore.c
