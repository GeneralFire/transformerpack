/** @file
  MarginTest.c

  File helps in performing margin test on given parameters

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

#include "MarginTest.h"

#pragma warning(disable :4366) //remove this when EV Library is completely obsorbed

/**
  This function is used to initialize the select memory point test
  configuration structure in the given core test data structure based on the
  CurrentIsDdrT element in the given core test data structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
InitMemPointTestCfg (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT BOOT_RAS_CORE_TEST_DATA *pCoreTestData
  )
{
  MEM_POINT_TEST_CONFIG *pSelectMemPointTestCfg = NULL;
  UINT8 Socket;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 ByteGroup;
  BOOLEAN IsSocketToBeTested;
  BOOLEAN IsControllerToBeTested;
  BOOLEAN IsChannelToBeTested;

  pSelectMemPointTestCfg = &pCoreTestData->SelectMemPointTestCfg;

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "InitMemPointTestCfg ()\n");

  // start by setting everything to 0
  ZeroMem (pSelectMemPointTestCfg, sizeof(*pSelectMemPointTestCfg));

  // set default values
  pSelectMemPointTestCfg->TurnaroundsTestMode = AllRankTurnarounds;
  pSelectMemPointTestCfg->TurnaroundsPairMode = OrderIndependent;

  // IF the DIMMs are LRDIMMs
  if (pCoreTestData->MemCfg.IsLrDimms) {
    // only test between the first ranks of all DIMMs
    pSelectMemPointTestCfg->TurnaroundsTestMode = OnlyInterDimmTurnarounds;
  }

  // test on all populated sockets
  pSelectMemPointTestCfg->SocketBitmask =  \
    pCoreTestData->SystemInfo.SocketBitMask;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not to be tested THEN skip it
    if ((pSelectMemPointTestCfg->SocketBitmask & (1 << Socket)) == 0) {
      continue;
    }

    IsSocketToBeTested = FALSE;

    // test on all populated controllers
    pSelectMemPointTestCfg->ControllerBitmasks[Socket] =  \
      pCoreTestData->MemCfg.ControllerBitmasks[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not to be tested THEN skip it
      if ((pSelectMemPointTestCfg->ControllerBitmasks[Socket] &
        (1 << Controller)) == 0) {
        continue;
      }

      IsControllerToBeTested = FALSE;

      // tets on all populated channels
      pSelectMemPointTestCfg->ChannelBitmasks[Socket][Controller] = \
        pSelectMemPointTestCfg->ChannelValBitmasks[Socket][Controller] = \
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
          pCoreTestData->RankTestMode;

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not populated THEN skip it
          if ((pCoreTestData->MemCfg.DimmBitmasks[Socket][Controller][Channel] &
            (1 << Dimm)) == 0) {
            continue;
          }

          // add it to the memory point test configuration DIMM A bitmask
          pSelectMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] |= \
            1 << Dimm;

          // rank A bitmask and present
          pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] = \
            ((1 << pCoreTestData->MemCfg.RankCounts[Socket][Controller][Channel][Dimm]) - 1);

#ifdef LRDIMM_SUPPORT
          // IF we're testing the frontside and the DIMM is a LRDIMM
          // PT for POC we only test the fronside and not cmd/ctl, thus for the LRDIMM, we only test the first rank of the dimm
          if (pCoreTestData->MemCfg.IsLrDimms) {
            // only test the first available rank on this DIMM
            pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] = \
              1 << GetLowestBitSetInMask(
              pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm],
              MAX_RANK_CNT);
          }
#endif // LRDIMM_SUPPORT

#if SUPPORT_FOR_TURNAROUNDS
          if (pCoreTestData->RankTestMode == Turnarounds) {
            pSelectMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] |= 1 << Dimm;
            pSelectMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] = \
              pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm];
          }
#endif // SUPPORT_FOR_TURNAROUNDS

          if (pSelectMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] != 0) {
            IsSocketToBeTested = TRUE;
            IsControllerToBeTested = TRUE;
            IsChannelToBeTested = TRUE;
          }
         } // end for each DIMM

#if SUPPORT_FOR_TURNAROUNDS
          // IF we're doing turnarounds and this channel can't do turnarounds
          if ((pCoreTestData->RankTestMode == Turnarounds) &&
            !CanChannelDoTurnarounds(SsaServicesHandle,
            &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg, pSelectMemPointTestCfg,
            DdrLevel, Socket, Controller, Channel, TRUE)) {      // the IoLevel and IsDdrT parameters are not used in the CanChannelDoTurnarounds() anymore.
              // remove the channel
              IsChannelToBeTested = FALSE;
          }
#endif // SUPPORT_FOR_TURNAROUNDS

        if (!IsChannelToBeTested) {
          // remove the channel
          pSelectMemPointTestCfg->ChannelBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
          pSelectMemPointTestCfg->ChannelValBitmasks[Socket][Controller] &= \
            ~(1 << Channel);
        }
      } // end for each channel

      if (!IsControllerToBeTested) {
        // remove the controller
        pSelectMemPointTestCfg->ControllerBitmasks[Socket] &= \
          ~(1 << Controller);
      }
    } // end for each controller

    if (!IsSocketToBeTested) {
      // remove the socket
      pSelectMemPointTestCfg->SocketBitmask &= ~(1 << Socket);
    }
  } // end for each socket

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // FOR each byte group:
      for (ByteGroup = 0;
        ByteGroup < (pCoreTestData->SystemInfo.BusWidth / 8);
        ByteGroup++) {
          pSelectMemPointTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup] = 0xFF;
      } // end for each byte group
      pSelectMemPointTestCfg->ChunkValBitmasks[Controller][Channel] = ALL_CHUNKS_BITMASK;
    } // end for each channel
  } // end for each controller

  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogMemPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
      pSelectMemPointTestCfg);
  }

  if (ChkMemPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pSelectMemPointTestCfg, pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function InitMemPointTestCfg

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
UINT8
InitCurrentMemPointTestCfg (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT BOOT_RAS_CORE_TEST_DATA *pCoreTestData
  )
{
  MEM_POINT_TEST_CONFIG *pMemPointTestCfg = \
    &pCoreTestData->CurrentMemPointTestCfg;
  MEM_POINT_TEST_CONFIG *pSelectMemPointTestCfg = \
    &pCoreTestData->SelectMemPointTestCfg;
  UINT8 Socket;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 ByteGroup;
  BOOLEAN IsSocketToBeTested;
  BOOLEAN IsControllerToBeTested;
  BOOLEAN IsChannelToBeTested;
  UINT8 PairCount;
  DIMM_RANK_PAIR DimmRankPairs[2];

  EvDebugPrint (SDBG_INFO, FUNCTION_DEBUG_MSGS,
    "InitMemPointTestCfg(CurrentRankCombinationIndex=%u)\n",
    pCoreTestData->CurrentRankCombinationIndex);

  if (SetCpgcPointTestRankCombinationIndex (SsaServicesHandle,
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

#ifdef LRDIMM_SUPPORT
    // if the DIMMs are LRDIMMs
    if (pCoreTestData->MemCfg.IsLrDimms) {
      // only test between the first ranks of all DIMMs
      pMemPointTestCfg->TurnaroundsTestMode = OnlyInterDimmTurnarounds;
    }
#endif // LRDIMM_SUPPORT


  // start with the socket bitmask of selected devices
  pMemPointTestCfg->SocketBitmask = \
    pSelectMemPointTestCfg->SocketBitmask;

  // for each socket:
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
          pCoreTestData->RankCombinationCnts[Socket][Controller][Channel]) {
          // add this channel to the channel validation mask
          pMemPointTestCfg->ChannelValBitmasks[Socket][Controller] |= \
            1 << Channel;
        } // end if this rank combination is unique for this channel

        IsChannelToBeTested = FALSE;

        pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] = \
          pCoreTestData->RankTestMode;

        PairCount = 2;
        if (GetCpgcPointTestDimmRanks (SsaServicesHandle,
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

          // add them to the memory point test configuration DIMM A/rank A bitmasks
          pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] |= \
            1 << Dimm;
          pMemPointTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm] |= \
            1 << Rank;
        } // end if there's a DIMM A/rank A

        // IF we're testing rank-to-rank turnarounds and there's a DIMM B/rank B
        if ((pCoreTestData->RankTestMode == Turnarounds) &&
          (PairCount > 1)) {
          Dimm = DimmRankPairs[1].Dimm;
          Rank = DimmRankPairs[1].Rank;

          // add them to the memory point test configuration DIMM B/rank B bitmasks
          pMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] |= \
            1 << Dimm;
          pMemPointTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm] |= \
            1 << Rank;
        } // end if we're testing rank-to-rank turnarounds and there's a DIMM B/rank B

        // IF (we're testing singles AND there are DIMM A's to be tested) OR
        // (we're testing rank-to-rank turnarounds AND there are DIMM A's and DIMM B's to be tested)
        if (((pCoreTestData->RankTestMode == Singles) &&
          (pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] != 0)) ||
          ((pCoreTestData->RankTestMode == Turnarounds) &&
          (pMemPointTestCfg->DimmABitmasks[Socket][Controller][Channel] != 0) &&
          (pMemPointTestCfg->DimmBBitmasks[Socket][Controller][Channel] != 0))) {
          IsSocketToBeTested = TRUE;
          IsControllerToBeTested = TRUE;
          IsChannelToBeTested = TRUE;
        }

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
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // FOR each byte group:
      for (ByteGroup = 0;
        ByteGroup < (pCoreTestData->SystemInfo.BusWidth / 8);
        ByteGroup++) {
        pMemPointTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup] = 0xFF;
      } // end for each byte group

      pMemPointTestCfg->ChunkValBitmasks[Controller][Channel] = ALL_CHUNKS;
    } // end for each channel
  } // end for each controller

  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogMemPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
      pMemPointTestCfg);
  }
  if (ChkMemPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, pMemPointTestCfg, pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function InitCurrentMemPointTestCfg

/**
@brief
  This function checks the test configuration for correctness/consistency.
  It also fills in the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
ChkTestCfg (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT BOOT_RAS_CORE_TEST_DATA *pCoreTestData
  )
{
  UINT8 Controller;
  UINT8 Channel;

  EvDebugPrint (SDBG_INFO, FUNCTION_DEBUG_MSGS, "ChkTestCfg ()\n");

  if (InitMemPointTestCfg (SsaServicesHandle, pCoreTestData)) {
    return 1;
  }

  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogMemPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
      &pCoreTestData->SelectMemPointTestCfg);
  }
  if (ChkMemPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  // initialize the CPGC test configuration to default values
  SetCpgcPointTestCfgDefaults (&pCoreTestData->CpgcPointTestCfg);

  // update the loopcounts and burstlengths
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      pCoreTestData->CpgcPointTestCfg.BurstLengths[Controller][Channel] = pCoreTestData->BurstLength;
      pCoreTestData->CpgcPointTestCfg.LoopCounts[Controller][Channel] = pCoreTestData->LoopCount;
    }
  }

  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogCpgcPointTestCfg (SsaServicesHandle, &pCoreTestData->CpgcPointTestCfg);
  }
  if (ChkCpgcPointTestCfg (SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  return 0;
} // end function ChkTestCfg

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
PointTestResultHandler (
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
  OUT TEST_STATUS *pTestStat
  )
{
  BOOT_RAS_CORE_TEST_DATA* pCoreTestData = (BOOT_RAS_CORE_TEST_DATA*)pVoidTestData;
  UINT8 ByteGroup;
  BOOLEAN ChannelFailed = FALSE;
  UINT8 FailStatus = 0;
  EWL_SEVERITY Severity;
  EWL_ENTRY_MEMORY_LOCATION MemoryLocation;
  MRC_GT MarginType;
  INT16 Offset;

  // FOR each byte group:
  for (ByteGroup = 0; ByteGroup < (pSystemInfo->BusWidth / 8); ByteGroup++) {
    if (pLaneErrorStatus[ByteGroup] != 0) {
      ChannelFailed = TRUE;
      break;
    }
  } // end for each byte group

  if (ChannelFailed) {
    RcDebugPrint (SDBG_ERROR, "IO boot ");
    // Update test status and display message
    if (pCoreTestData->BootMask == WarningIoBootHealthMask) {
      FailStatus = 0x1; // bit 0 is for warning mask
      RcDebugPrint (SDBG_ERROR, "warning ");
    } else{
      FailStatus = 0x2; // bit 1 is for citical mask
      RcDebugPrint (SDBG_ERROR, "critical ");
    }
    // Update the test status
    pCoreTestData->TestResults[Socket][Controller][Channel][DimmRankA.Dimm][DimmRankA.Rank] |= FailStatus;
    // Update the test point index which are the bits[7:2]. We only save the last failing test point.
    pCoreTestData->TestResults[Socket][Controller][Channel][DimmRankA.Dimm][DimmRankA.Rank] &= 0x3;
    pCoreTestData->TestResults[Socket][Controller][Channel][DimmRankA.Dimm][DimmRankA.Rank] |= (pCoreTestData->TestPointIndex << 0x2);
    pCoreTestData->PointTestResult |= FailStatus;
    RcDebugPrint (SDBG_ERROR, "mask failed: ");
    RcDebugPrint (SDBG_ERROR, "S%d:C%d:D%d:R%d failed\n",
      Socket, (Controller * MAX_CHANNEL_CNT + Channel),DimmRankA.Dimm, DimmRankA.Rank);
    //
    // Log Enhanced warning
    //
    MemoryLocation.Socket = Socket;
    MemoryLocation.Channel =  (Controller * MAX_CHANNEL_CNT + Channel);
    MemoryLocation.Dimm = DimmRankA.Dimm;
    MemoryLocation.Rank = DimmRankA.Rank;
    Severity = EwlSeverityWarning;

    if (pCoreTestData->BootMask == WarningIoBootHealthMask) {
      Offset = pCoreTestData->pTestCfg->WarningMask[pCoreTestData->TestPointIndex][pCoreTestData->MarginSignalIdx];
    } else {
      Offset = pCoreTestData->pTestCfg->CriticalMask[pCoreTestData->TestPointIndex][pCoreTestData->MarginSignalIdx];
    }
    MarginType = pCoreTestData->pTestCfg->MarginGroups[pCoreTestData->MarginSignalIdx];
    RcDebugPrint (SDBG_INFO, "About to log warning, Severity = %d, MarginType = %d, Offset = %d\n", Severity, MarginType, Offset);
    EwlOutputType25 (WARN_MEMORY_BOOT_HEALTH_CHECK, WARN_MEMORY_BOOT_HEALTH_CHECK_MASK_FAIL, MemoryLocation, Severity, MarginType, Offset);
  }

  EvDebugPrint (SDBG_INFO, VERBOSE_ERROR_MSGS,
    "point test result:\n"
    "  Socket=%u Controller=%u Channel=%u DimmA=%u RankA=%u DimmB=%u RankB=%u\n",
    Socket, Controller, Channel, DimmRankA.Dimm, DimmRankA.Rank, DimmRankB.Dimm, DimmRankB.Rank);

  EvDebugPrint (SDBG_INFO, VERBOSE_ERROR_MSGS,
    "  ErrorCount=%d Overflow=%u LaneErrorStatus=0x",
    ErrorCount.Bits.Count, ErrorCount.Bits.Overflow);

  if (IsCheckMsgTypeEnabled (VERBOSE_ERROR_MSGS)) {
    LogByteArrayAsHex (pLaneErrorStatus, (pSystemInfo->BusWidth / 8));
  }
  EvDebugPrint (SDBG_INFO, VERBOSE_ERROR_MSGS, "\n");

  return 0;
} // end function PointTestResultHandler



/**
@brief
  This function sets the given margin parameter to the given offset

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       Offset             Margin parameter offset.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8
SetMarginParam (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN INT16 Offset,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  OUT TEST_STATUS *pTestStat
  )
{
  UINT8 Socket;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 RankCount;
  UINT8 LaneBitmasks[MAX_BUS_WIDTH / 8];
  MARGIN_PARAM_SPECIFICITY Specificity;
  UINT16 CurrentOffset = 0;
  SSA_STATUS RetStat;

  EvDebugPrint (SDBG_INFO, FUNCTION_DEBUG_MSGS,
    "SetMarginParam(Offset=%d IoLevel=%u MarginGroup=%u)\n",
     Offset, IoLevel, MarginGroup);

  if (GetMarginParamSpecificity (SsaServicesHandle, IoLevel, MarginGroup,
    __LINE__, &Specificity, pTestStat)) {
    return 1;
  }

  EvDebugPrint (SDBG_INFO, VERBOSE_LOG_MSGS,
    "Setting IoLevel=%u MarginGroup=%u to Offset=%d.\n",
    IoLevel, MarginGroup, Offset);

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel has no lanes enabled for validation THEN skip it
        if (!IsChannelValEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not enabled for testing THEN skip it
          if (!IsDimmATestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel, Dimm)) {
            continue;
          }

          RankCount = pMemCfg->RankCounts[Socket][Controller][Channel][Dimm];

          // FOR each rank:
          for (Rank = 0; Rank < RankCount; Rank++) {
            // IF this rank is not enabled for testing THEN skip it
            if (!IsRankATestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
              pMemPointTestCfg, Socket, Controller, Channel, Dimm, Rank)) {
              continue;
            }

            ZeroMem (LaneBitmasks, sizeof(LaneBitmasks));
            CopyMem (LaneBitmasks,
              pMemPointTestCfg->LaneValBitmasks[Controller][Channel],
              (pSystemInfo->BusWidth / 8));

            EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS,
              "  SetMarginParamOffset(\n"
              "    Socket=%u Controller=%u Channel=%u Dimm=%u Rank=%u\n"
              "    LaneBitmasks=0x",
              Socket, Controller, Channel, Dimm, Rank);
            if (IsCheckMsgTypeEnabled (BSSA_API_DEBUG_MSGS)) {
              LogByteArrayAsHex (pMemPointTestCfg->LaneValBitmasks[Controller][Channel],
                (MAX_BUS_WIDTH / 8));
            }
            EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS,
              " IoLevel=%u MarginGroup=%u\n"
              "    CurrentOffset=%d NewOffset=%d)\n",
              IoLevel, MarginGroup, CurrentOffset, Offset);

            RetStat = SsaServicesHandle->SsaMemoryConfig->SetMarginParamOffset (
              SsaServicesHandle, Socket, Controller, Channel, Dimm, Rank, LaneBitmasks,
              IoLevel, MarginGroup, CurrentOffset, Offset);
            if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, pTestStat)) {
              return 1;
            }

            // IF the margin parameter is not rank specific THEN exit the rank for loop
            if ((Specificity & RankSpecific) == 0) {
              break;
            }
          } // end for each rank

          // IF the margin parameter is not rank specific THEN exit the DIMM for loop
          if ((Specificity & RankSpecific) == 0) {
            break;
          }
        } // end for each DIMM

        // IF the margin parameter is not channel specific THEN exit the channel for loop
        if ((Specificity & ChannelSpecific) == 0) {
          break;
        }
      } // end for each channel

      // IF the margin parameter is not controller specific THEN exit the controller for loop
      if ((Specificity > ControllerSpecific) == 0) {
        break;
      }
    } // end for each controller

    // IF the margin parameter is not socket specific THEN exit the socket for loop
    if ((Specificity & SocketSpecific) == 0) {
      break;
    }
  } // end for each socket

  return 0;
} // end function SetMarginParam


/**
@brief
  This function test the boot mask

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       Offset             Margin parameter offset.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
TestMask (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT BOOT_RAS_CORE_TEST_DATA *pCoreTestData,
  IN OUT BOOLEAN *MaskFailed,
  OUT TEST_STATUS *pTestStat
  )
{
  LANE_FAIL_STATS LaneFailStats;
  UINT8 TestPointIdx;
  UINT8 MarginSignalIdx;
  INT16 MarginSignalCurrentOffsets[MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT];
  INT16 Mask[MAX_IO_BOOT_MASK_TEST_POINTS][MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT];

  // All margin signals are in the nominal state.
  ZeroMem (&MarginSignalCurrentOffsets, sizeof (MarginSignalCurrentOffsets));

  // Select the mask
  if (pCoreTestData->BootMask == WarningIoBootHealthMask) {
    CopyMem (Mask, pCoreTestData->pTestCfg->WarningMask, sizeof(Mask));
  } else {
    CopyMem (Mask, pCoreTestData->pTestCfg->CriticalMask, sizeof(Mask));
  }

  // Iterate all the test points in the mask
  for (TestPointIdx = 0; TestPointIdx < MAX_IO_BOOT_MASK_TEST_POINTS; TestPointIdx++) {
    pCoreTestData->TestPointIndex = TestPointIdx;
    // Iterate all the margin signal offsets
    for (MarginSignalIdx = 0; MarginSignalIdx < MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT; MarginSignalIdx++) {
      // Save the current margin signal idx so that we can log the signal name and offset later when test fails.
      // We only offset one of 4 signals each test point, just need to save the none-zero margin index.
      if (Mask[TestPointIdx][MarginSignalIdx] != 0) {
        pCoreTestData->MarginSignalIdx = MarginSignalIdx;
      }
      if (Mask[TestPointIdx][MarginSignalIdx] != MarginSignalCurrentOffsets[MarginSignalIdx]) {
        if (SetMarginParam (SsaServicesHandle,
          &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
          &pCoreTestData->CurrentMemPointTestCfg,
          Mask[TestPointIdx][MarginSignalIdx],
          DdrLevel,
          pCoreTestData->pTestCfg->MarginGroups[MarginSignalIdx],
          pCoreTestData->pTestStat)) {
            return 1;
        }
        MarginSignalCurrentOffsets[MarginSignalIdx] = Mask[TestPointIdx][MarginSignalIdx];
      }
    }

    ZeroMem (LaneFailStats, sizeof(LaneFailStats)) ;

    pCoreTestData->PointTestResult = 0;

    RcDebugPrint (SDBG_INFO, "Performing CPGC Test on below Signal and Offsets\n");

    for (MarginSignalIdx = 0; MarginSignalIdx < MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT; MarginSignalIdx++) {
        RcDebugPrint (SDBG_INFO, "signal:%d offset:%d ",
          pCoreTestData->pTestCfg->MarginGroups[MarginSignalIdx],
          Mask[TestPointIdx][MarginSignalIdx]);
    }

    if (RunCpgcPointTest (SsaServicesHandle, &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->CurrentMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg, pCoreTestData->pCpgcPointTestData, &LaneFailStats,
      pCoreTestData->pTestStat)) {
        return 1;
    }
    RcDebugPrint (SDBG_INFO, "End CPGC Test\n");

    if (pCoreTestData->PointTestResult) {
      *MaskFailed = TRUE;
      RcDebugPrint (SDBG_INFO, "at ");
      for (MarginSignalIdx = 0; MarginSignalIdx < MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT; MarginSignalIdx++) {
        RcDebugPrint (SDBG_INFO, "signal:%d offset:%d ",
          pCoreTestData->pTestCfg->MarginGroups[MarginSignalIdx],
          Mask[TestPointIdx][MarginSignalIdx]);
      }
      RcDebugPrint (SDBG_INFO, "\n ");
    }
  }  // TestPointIdx

  // reset to nominal condition
  // Iterate all the margin signal offsets
  for (MarginSignalIdx = 0; MarginSignalIdx < MAX_IO_BOOT_MASK_TEST_SIGNAL_CNT; MarginSignalIdx++) {

    if (MarginSignalCurrentOffsets[MarginSignalIdx] != 0) {
      if (SetMarginParam (SsaServicesHandle,
        &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
        &pCoreTestData->CurrentMemPointTestCfg,
        0,
        DdrLevel,
        pCoreTestData->pTestCfg->MarginGroups[MarginSignalIdx],
        pCoreTestData->pTestStat)) {
          return 1;
      }
      MarginSignalCurrentOffsets[MarginSignalIdx] = 0;
    }
  }
  return 0;
} //end of TestMask

/*
@brief
  This function is used to run a Margin1D test.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in, out]  pCoreTestData         Pointer to core test data structure.
  @param[in, out]  pWarningMaskFailed    Pointer to warning mask failure status.
  @param[in, out]  pCriticalMaskFailed   Pointer to critical mask failure status.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
RunTest (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT BOOT_RAS_CORE_TEST_DATA *pCoreTestData,
  IN OUT BOOLEAN *pWarningMaskFailed,
  IN OUT BOOLEAN *pCriticalMaskFailed
  )
{
  BOOLEAN InitDqdbContent;
  BOOLEAN IsFirstCall;
  BOOLEAN IsLastCall;

  *pWarningMaskFailed = FALSE;
  *pCriticalMaskFailed = FALSE;

  EvDebugPrint (SDBG_INFO, FUNCTION_DEBUG_MSGS, "RunTest()\n");

  ZeroMem (pCoreTestData->TestResults, sizeof(pCoreTestData->TestResults));

  // initialize the DQDB content for all devices that will be participating
  InitDqdbContent = TRUE;
  if (InitDqdb (SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    InitDqdbContent, pCoreTestData->pTestStat)) {
    return 1;
  }
  InitDqdbContent = FALSE;

  if (InitMemPointTestCfg (SsaServicesHandle, pCoreTestData)) {
    return 1;
  }

  if (GetRankCombinationsCnts (SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    DdrLevel/*pCoreTestData->pTestCfg->IoLevel*/, TRUE/*pCoreTestData->CurrentIsDdrT*/,
    &pCoreTestData->RankCombinationCnts,
    &pCoreTestData->MaxRankCombinationsCnts,
    pCoreTestData->pTestStat)) {
    return 1;
  }

  IsFirstCall = TRUE;
  if (SetupCpgcPointTest (SsaServicesHandle, &pCoreTestData->SystemInfo,
    pCoreTestData->pCpgcInfo, &pCoreTestData->MemCfg,
    &pCoreTestData->SelectMemPointTestCfg, &pCoreTestData->CpgcPointTestCfg,
    &PointTestResultHandler, pCoreTestData, DdrLevel/*pCoreTestData->pTestCfg->IoLevel*/,
    TRUE/*pCoreTestData->CurrentIsDdrT*/,
    pCoreTestData->pTestCfg->TestStatusLogLevel, IsFirstCall,
    InitDqdbContent, &pCoreTestData->pCpgcPointTestData,
    pCoreTestData->pTestStat)) {
    return 1;
  }
  IsFirstCall = FALSE;

  // Test the warning mask
  pCoreTestData->BootMask = WarningIoBootHealthMask;

  // FOR each rank combination:
  for (pCoreTestData->CurrentRankCombinationIndex = 0;
    pCoreTestData->CurrentRankCombinationIndex <
    pCoreTestData->MaxRankCombinationsCnts;
    pCoreTestData->CurrentRankCombinationIndex++) {
    RcDebugPrint (SDBG_INFO, "\nTesting rank combination %u of %u.\n",
        (pCoreTestData->CurrentRankCombinationIndex + 1),
        pCoreTestData->MaxRankCombinationsCnts);

    if (InitCurrentMemPointTestCfg (SsaServicesHandle, pCoreTestData)) {
      return 1;
    }

    // re-initialize the rank programming
    if (SetCpgcPointTestRankCombinationIndex (SsaServicesHandle,
      &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
      &pCoreTestData->CurrentMemPointTestCfg,
      &pCoreTestData->CpgcPointTestCfg, pCoreTestData->pCpgcPointTestData,
      pCoreTestData->CurrentRankCombinationIndex,
      TRUE, // need to update HW register setting
      pCoreTestData->pTestStat)) {
        return 1;
    }

    // Test the mask for this rank
    if (TestMask (SsaServicesHandle, pCoreTestData, pWarningMaskFailed, pCoreTestData->pTestStat)) {
      return 1;
    }
  } // end for each rank combination

  if (*pWarningMaskFailed) {
    // Test the critical mask
    pCoreTestData->BootMask = CriticalIoBootHealthMask;

    // FOR each rank combination:
    for (pCoreTestData->CurrentRankCombinationIndex = 0;
      pCoreTestData->CurrentRankCombinationIndex <
      pCoreTestData->MaxRankCombinationsCnts;
      pCoreTestData->CurrentRankCombinationIndex++) {
      RcDebugPrint (SDBG_INFO,"\nTesting rank combination %u of %u.\n",
      (pCoreTestData->CurrentRankCombinationIndex + 1),
      pCoreTestData->MaxRankCombinationsCnts);

      if (InitCurrentMemPointTestCfg (SsaServicesHandle, pCoreTestData)) {
          return 1;
      }

    // re-initialize the rank programming
      if (SetCpgcPointTestRankCombinationIndex (SsaServicesHandle,
        &pCoreTestData->SystemInfo, &pCoreTestData->MemCfg,
        &pCoreTestData->CurrentMemPointTestCfg,
        &pCoreTestData->CpgcPointTestCfg, pCoreTestData->pCpgcPointTestData,
        pCoreTestData->CurrentRankCombinationIndex,
        TRUE, // need to update HW register setting
        pCoreTestData->pTestStat)) {
          return 1;
      }
      if (TestMask (SsaServicesHandle, pCoreTestData, pCriticalMaskFailed, pCoreTestData->pTestStat)) {
        return 1;
      }
   } // end for each rank combination
  } // if WarningMaskFailed

  IsLastCall = TRUE;
  CleanupCpgcPointTest (SsaServicesHandle, &pCoreTestData->SystemInfo,
    &pCoreTestData->MemCfg, &pCoreTestData->SelectMemPointTestCfg,
    IsLastCall, &pCoreTestData->pCpgcPointTestData);

  return 0;
} // end function RunTest

/**
@brief
  This function save the results.  It creates the results buffer, update the
  metadata and copy channel test status to the result structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in, out]  pCoreTestData      Pointer to core test data structure.
**/
UINT8 SaveResults (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT BOOT_RAS_CORE_TEST_DATA *pCoreTestData
  )
{
  SSA_STATUS RetStat;
  UINT32 MetadataSize = sizeof(BOOT_RAS_CONFIG);
  UINT32 ResultElementSize = sizeof(BOOT_RAS_RESULT_COLUMNS);
  EFI_GUID MetadataGuid = BOOT_RAS_RESULT_METADATA_GUID;
  EFI_GUID ColumnsGuid = BOOT_RAS_RESULT_COLUMNS_GUID;
  BOOT_RAS_CONFIG *pMetadata;
  BOOT_RAS_RESULT_COLUMNS *pElement;
  UINT32 FailResultCnt = 0;
  UINT32 ResultEleCnt;
  UINT8 Socket, Controller, Channel, Dimm, Rank;
  UINT32 FailingChannelBitMask;
  UINT8  NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  EvDebugPrint (SDBG_INFO, FUNCTION_DEBUG_MSGS, "SaveResults()\n");

  // find the result element count. We intend to only save the test entry(socket,
  // controller, channel, dimm, rank, test point) that failed in either warning
  // or critical masks. However, if there is no any test point failed, then we
  // will use one entry to save the passed status.
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    FailingChannelBitMask = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          for (Rank = 0; Rank < MAX_RANK_CNT; Rank++) {
            if (pCoreTestData->TestResults[Socket][Controller][Channel][Dimm][Rank] & 0x3) {  // bit0 and 1 are the warning and critical mask test status
              FailResultCnt += 1;
              // save the critical failing channel bit mask
              if (pCoreTestData->TestResults[Socket][Controller][Channel][Dimm][Rank] & 0x2) {
                FailingChannelBitMask |= 1 << (NumChPerMc * Controller + Channel);
              }
            }
          } // Rank
        } // Dimm
      } // Channel
    } // Controller
    pCoreTestData->pTestCfg->ChannelsFailedCriticalMask[Socket] = FailingChannelBitMask;
  } // Socket

  ResultEleCnt = FailResultCnt;
  if (FailResultCnt == 0) {
    ResultEleCnt = 1;
  }

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS,
    "  CreateResultBuffer(MetadataSize=%u ResultElementSize=%u ResultsCapacity=%u)\n",
    MetadataSize, ResultElementSize, ResultEleCnt);

  RetStat = SsaServicesHandle->SsaResultsConfig->CreateResultBuffer (
    SsaServicesHandle, MetadataSize, ResultElementSize, ResultEleCnt);

  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "  GetMetadata()\n");

  RetStat = SsaServicesHandle->SsaResultsConfig->GetMetadata (
    SsaServicesHandle, &pMetadata);
  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  // save the metadata
  CopyMem (pMetadata->MarginGroups, pCoreTestData->pTestCfg->MarginGroups, sizeof(pMetadata->MarginGroups));
  CopyMem (pMetadata->WarningMask, pCoreTestData->pTestCfg->WarningMask, sizeof(pMetadata->WarningMask));
  CopyMem (pMetadata->CriticalMask, pCoreTestData->pTestCfg->CriticalMask, sizeof(pMetadata->CriticalMask));


  // save the test results
  if (FailResultCnt) {
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
            for (Rank = 0; Rank < MAX_RANK_CNT; Rank++) {
              if (pCoreTestData->TestResults[Socket][Controller][Channel][Dimm][Rank]) {
                EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "  GetNextResultElement()\n");
                RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement (
                  SsaServicesHandle, &pElement);
                if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
                  return 1;
                }
                pElement->Socket = Socket;
                pElement->Controller = Controller;
                pElement->Channel = Channel;
                pElement->Dimm = Dimm;
                pElement->Rank = Rank;
                pElement->TestPoint = pCoreTestData->TestResults[Socket][Controller][Channel][Dimm][Rank] >> 2 ;   // upper 6 bits are the test point index
                pElement->Status = pCoreTestData->TestResults[Socket][Controller][Channel][Dimm][Rank] & 0x3 ;   // lower 6 bits are the test status
              }
            } // Rank
          } // Dimm
        } // Channel
      } // Controller
    } // Socket
  }
  else {
    // no failure in both mask. set to 0s.
    EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "  GetNextResultElement()\n");

    RetStat = SsaServicesHandle->SsaResultsConfig->GetNextResultElement (
      SsaServicesHandle, &pElement);
    if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
      return 1;
    }
    ZeroMem (pElement, sizeof(pElement));
  }

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "  SetMetadataType()\n");

  RetStat = SsaServicesHandle->SsaResultsConfig->SetMetadataType (
    SsaServicesHandle, &MetadataGuid);
  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "  SetResultElementType()\n");

  RetStat = SsaServicesHandle->SsaResultsConfig->SetResultElementType (
    SsaServicesHandle, &ColumnsGuid);
  if (ChkRetStat (SsaServicesHandle, RetStat, __FILE__, __LINE__, NULL)) {
    return 1;
  }

  return 0;
} // end function SaveResults

/**
@brief
  This function is the Margin1D test entry point.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       Config             Pointer to test configuration structure.

  @retval  EFI_SUCCESS  - Successfully executed function
  @retval  !EFI_SUCCESS - Error in Function completion
**/
EFI_STATUS
EFIAPI
MemHealthCheckTestEntry (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN BOOT_RAS_CONFIG *Config
  )
{
  BOOT_RAS_CORE_TEST_DATA CoreTestData;
  MRC_SERVER_CPGC_INFO CpgcInfo;
  BOOLEAN WarningMaskFailed;
  BOOLEAN CriticalMaskFailed;

  ZeroMem (&CoreTestData, sizeof (CoreTestData));

  CoreTestData.pTestCfg = Config;
  CoreTestData.pCpgcInfo = &CpgcInfo;
  CoreTestData.RankTestMode = Singles;  // only do single rank test for now
  CoreTestData.BurstLength = 32;
  CoreTestData.LoopCount = 16;

  if (Config == NULL) {
    EvDebugPrint (SDBG_INFO, VERBOSE_ERROR_MSGS, "ERROR: Config is NULL!\n");
    ProcError (SsaServicesHandle, Failure, __FILE__, __LINE__, CoreTestData.pTestStat);
    return 1;
  }

  EvDebugPrint (SDBG_INFO, FUNCTION_DEBUG_MSGS, "  GetSystemInfo()\n");

  SsaServicesHandle->SsaMemoryConfig->GetSystemInfo (SsaServicesHandle,
    &CoreTestData.SystemInfo);

  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogSystemInfo (&CoreTestData.SystemInfo);
  }

  if (ChkSystemInfo (SsaServicesHandle, &CoreTestData.SystemInfo,
    CoreTestData.pTestStat)) {
    return 1;
  }
  EvDebugPrint (SDBG_INFO, BSSA_API_DEBUG_MSGS, "  GetCpgcInfo()\n");

  SsaServicesHandle->SsaMemoryServerConfig->GetCpgcInfo (SsaServicesHandle,
    CoreTestData.pCpgcInfo);
  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogCpgcInfo (SsaServicesHandle, CoreTestData.pCpgcInfo);
  }

  if (ChkCpgcInfo (SsaServicesHandle, CoreTestData.pCpgcInfo,
    CoreTestData.pTestStat)) {
    return 1;
  }

  if (GetMemCfg (SsaServicesHandle, &CoreTestData.SystemInfo, &CoreTestData.MemCfg,
    CoreTestData.pTestStat)) {
    return 1;
  }
  if (IsCheckMsgTypeEnabled (VERBOSE_LOG_MSGS)) {
    LogMemCfg (SsaServicesHandle, &CoreTestData.SystemInfo, &CoreTestData.MemCfg);
  }

  if (ChkTestCfg (SsaServicesHandle, &CoreTestData)) {
    return 1;
  }

  if (RunTest (SsaServicesHandle, &CoreTestData, &WarningMaskFailed, &CriticalMaskFailed)) {
    return 1;
  }

  // Save to test results
  if (SaveResults (SsaServicesHandle, &CoreTestData)) {
    return 1;
  }

  if (CriticalMaskFailed) {
    RcDebugPrint (SDBG_ERROR, "ERROR: Critical mask failed!\n");
  } else if (WarningMaskFailed) {
    RcDebugPrint (SDBG_ERROR, "WARNING: Warning mask failed\n");
  } else {
    RcDebugPrint (SDBG_ERROR, "IO Boot RAS Passed\n");
  }

  if (CriticalMaskFailed) {
    Config->CriticalTestStatus = TRUE;
  }
  return 0;
} // end function TestEntryPoint
// end file BootRAS.c
