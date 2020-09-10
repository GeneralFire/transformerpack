/** @file
  CpgcPointTest.c

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
#include <Library/RcMemAllocLib.h>
#include "MemPointTest.h"
#include "CpgcPointTest.h"
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/BitMaskLib.h>


// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

// flag to enable function trace debug messages
#define ENBL_FUNCTION_TRACE_DEBUG_MSGS (0)

// flag to enable verbose log messages
#define ENBL_VERBOSE_LOG_MSGS (0)

// flag to enable verbose error messages
#define ENBL_VERBOSE_ERROR_MSGS (0)

// flag to enable memory pool debug log messages
#define ENBL_MEM_POOL_DEBUG_LOG_MSGS (0)

// flag to enable DDR-T debug messages
#define ENBL_DDRT_DEBUG_LOG_MSGS (0)

// flag to enable DqDB debug messages
#define ENBL_DQDB_DEBUG_LOG_MSGS (0)

// flag to enable timeout debug messages
#define ENBL_TIMEOUT_DEBUG_LOG_MSGS (0)

//
// shorthand CPGC Subsequence types
//
#ifndef BRd
#define BRd    (BaseReadSubseqType)
#endif // BRd
#ifndef BWr
#define BWr    (BaseWriteSubseqType)
#endif // BWr
#ifndef BRdWr
#define BRdWr  (BaseReadWriteSubseqType)
#endif // BRdWr
#ifndef BWrRd
#define BWrRd  (BaseWriteReadSubseqType)
#endif // BWrRd
#ifndef ORd
#define ORd    (OffsetReadSubseqType)
#endif // ORd
#ifndef OWr
#define OWr    (OffsetWriteSubseqType)
#endif // OWr


/**
@brief
  This function is used to initialize the DqDB pattern and unisequencers.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
  @param[in]       InitDqdbContent    Flag indicating whether to initialize the DQDB contents.
  @param[out]      pTestStat          Pointer to return status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 InitDqdb(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MRC_SERVER_CPGC_INFO *pCpgcInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN BOOLEAN InitDqdbContent,
  OUT TEST_STATUS *pTestStat)
{
  //
  // Gen2 does NOT use this function to program the WDB.
  //
  return 0;
} // end function InitDqdb

/**
@brief
  This function is used to set the logical ranks based on the given test data.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in]       pSystemInfo         Pointer to system information structure.
  @param[in]       pMemCfg             Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg    Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg   Pointer to CPGC memory point test configuration structure.
  @param[in]       pCpgcPointTestData  Pointer to CPGC point test data structure.
  @param[out]      pTestStat           Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 SetCrntRanks(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN CPGC_POINT_TEST_DATA *pCpgcPointTestData,
  OUT TEST_STATUS *pTestStat)
{
  UINT8 Socket, Controller, Channel;
  DIMM_RANK_PAIR CrntDimmRankA, CrntDimmRankB;
  UINT8 TestedChnlIdx;
  MRC_RT                  MrcRankList;
  UINT8                   MemSsType;
  UINT32                  ChBitMask;
  PSYSHOST                Host = NULL;

  Host = GetSysHostPointer ();

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "CpgcPointTest.SetCrntRanks()\n");
#endif

  //
  // Update the error checker configuration fields of the CPGC_POINT_TEST_CONFIG structre.
  // These fields were added lately and they were part of the  MEM_POINT_TEST_CONFIG.
  //
  CopyMem (pCpgcPointTestCfg->LaneValBitmasks, pMemPointTestCfg->LaneValBitmasks, sizeof (pCpgcPointTestCfg->LaneValBitmasks));
  CopyMem (pCpgcPointTestCfg->ChunkValBitmasks, pMemPointTestCfg->ChunkValBitmasks, sizeof (pCpgcPointTestCfg->ChunkValBitmasks));

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not enabled for testing THEN skip it
        if (!IsChannelTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }
        // get the index of tested channel.
        TestedChnlIdx = GetTestChnlIndex (SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel);

        CrntDimmRankA = pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankA;
        CrntDimmRankB = pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankB;

        // Turnaround test case are not supported!
        MrcRankList.Dimm = CrntDimmRankA.Dimm;
        MrcRankList.Rank = CrntDimmRankA.Rank;
        ChBitMask = (0x1 << Channel) << (Controller * MAX_CHANNEL_CNT);

        //
        // Set up the test sequence and address
        //
        SetupBssaCpgcTest (Host, Socket, MemSsType, ChBitMask, 1, &MrcRankList, (CPGC_CONFIGURE_HEADER*)pCpgcPointTestCfg, FALSE, TRUE);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "  SetupBssaCpgcTest(Socket=%u MemSsType=%u ChBitMask=0x%x RankListSize=%u Dimm=%u Rank=%u UpdatePatternAndChecker=%u UpdateSequenceAndAddress=%u)\n",
          Socket, MemSsType, ChBitMask, 1, MrcRankList.Dimm, MrcRankList.Rank, FALSE, TRUE);
#endif
      } // end for each channel
    } // end for each controller
  } // end for each socket
  return 0;
} // end function SetCrntRanks

  /**
  @brief
  This function is a wrapper to the MRC SetupBssaCpgcTest() which is used to initialize the CPGC.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
  **/
static VOID SetupTestWrapper (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg)
{
  UINT8 Socket, Controller, Channel;
  MRC_RT                  MrcRankList;
  UINT8                   MemSsType;
  UINT32                  ChBitMask;
  PSYSHOST                Host = NULL;

  Host = GetSysHostPointer ();

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "CpgcPointTest.SetupTestWrapper()\n");
#endif

  //
  // Update the error checker configuration fields of the CPGC_POINT_TEST_CONFIG structre.
  // These fields were added lately and they were part of the  MEM_POINT_TEST_CONFIG.
  //
  CopyMem (pCpgcPointTestCfg->LaneValBitmasks, pMemPointTestCfg->LaneValBitmasks, sizeof (pCpgcPointTestCfg->LaneValBitmasks));
  CopyMem (pCpgcPointTestCfg->ChunkValBitmasks, pMemPointTestCfg->ChunkValBitmasks, sizeof (pCpgcPointTestCfg->ChunkValBitmasks));

  MrcRankList.Dimm = 0;
  MrcRankList.Rank = 0;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    ChBitMask = 0x0;
    MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      if (!IsControllerTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
        continue;
      }

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not enabled for testing THEN skip it
        if (!IsChannelTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        //
        // Call SetupBssaCpgcTest() per channel
        //
        ChBitMask |= (0x1 << Channel) << (Controller * MAX_CHANNEL_CNT);
      } // end for each channel
    } // end for each controller

    // PT need to remove all the "CONST" in BSSA code to avoid the casting so that we can directly pass &(pCpgcPointTestCfg->Header)
    SetupBssaCpgcTest (Host, Socket, MemSsType, ChBitMask, 1, &MrcRankList, (CPGC_CONFIGURE_HEADER*)pCpgcPointTestCfg, TRUE, FALSE);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  SetupBssaCpgcTest(Socket=%u MemSsType=%u ChBitMask=0x%x RankListSize=%u Dimm=%u Rank=%u UpdatePatternAndChecker=%u UpdateSequenceAndAddress=%u)\n",
      Socket, MemSsType, ChBitMask, 1, MrcRankList.Dimm, MrcRankList.Rank, TRUE, FALSE);
#endif
  } // end for each socket
} // end function SetupTestWrapper



/**
@brief
  This function is used to run a point test.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in]       pSystemInfo         Pointer to system information structure.
  @param[in]       pMemCfg             Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg    Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg   Pointer to CPGC point test configuration structure.
  @param[in]       pCpgcPointTestData  Pointer to CPGC point test data structure.
  @param[out]      pTestStat           Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
static UINT8 RunPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN CPGC_POINT_TEST_DATA *pCpgcPointTestData,
  OUT TEST_STATUS *pTestStat)
{
  UINT8                Socket, Controller, Channel;
  BOOLEAN              ClearErrors;
  BOOLEAN              StartTest;
  BOOLEAN              StopTest;
  BOOLEAN              SocketAnyLaneFailed, ControllerAnyLaneFailed;
  UINT8                LaneErrorStatus[MAX_BUS_WIDTH / 8];
  INT8                 ByteGroup;
  UINT8                Counter = 0;
  ERROR_COUNT_32BITS   ErrCnt;
  UINT8                TestedChnlIdx;
  DIMM_RANK_PAIR       DimmRankB;
  DIMM_RANK_PAIR       DimmRankA;
  PSYSHOST             Host;
  UINT32               ChannelMaskInSocket = 0;
  UINT8                NumChPerMc;
  UINT8                MemSsType;
  MRC_TT               TestType;
  UINT16               ChunkMask = ALL_CHUNKS;
  MRC_CT               ChunkErrSel;
  UINT32               StatusArraySize;
  UINT32               ResultStatus[MAX_CONTROLLER_CNT * MAX_CHANNEL_CNT] [3];
  UINT8                SubChannel;
  CPGC_ERROR_COUNT_STATUS CpgcErrorCounterStatus;


#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "CpgcPointTest.RunPointTest()\n");
#endif

#if ENBL_VERBOSE_LOG_MSGS || ENBL_TIMEOUT_DEBUG_LOG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  Clearing errors and starting the DFx engine(s).\n");
#endif

  NumChPerMc = GetNumChannelPerMc ();
  Host = GetSysHostPointer ();

  //
  // DDR5/CPGC2 uses the subchannel1. DDR4/CPGC1 uses no subchannel
  //
#ifdef DDR5_SUPPORT
  SubChannel = SUB_CH_B;
#else
  SubChannel = NO_SUBCH;
#endif

  // IF we're testing an early read ID margin parameter THEN clear the early
  // read ID status
  if (GetCpgcPointTestCfgErid (pCpgcPointTestCfg)) {
    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          // IF this channel is not enabled for testing THEN skip it
          if (!IsChannelTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel)) {
            continue;
          }

          // PT need to check if AEP DIMM is available
#if ENBL_BIOS_SSA_API_DEBUG_MSGS || ENBL_DDRT_DEBUG_LOG_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "  ClearEridErrorStatus(Socket=%u Controller=%u Channel=%u)\n",
            Socket, Controller, Channel);
#endif
          SsaServicesHandle->SsaMemoryDdrtConfig->ClearEridErrorStatus (
            SsaServicesHandle, Socket, Controller, Channel);
        } // end for each channel
      } // end for each controller
    } // end for each socket
  } // end if we're testing an early read ID margin parameter

    // ------------------------------------------------------------------------
    // Start the test.
    // ------------------------------------------------------------------------

    // PT before start test, always stop test first. Because in certain test case that the previous test was a forever loop 
    // point test.
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    ChannelMaskInSocket = 0;
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      if (!IsControllerTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
        continue;
      }

      ChannelMaskInSocket |= 0x1 << (Controller * NumChPerMc);
    } // end for each controller

    MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "   TestControl(Socket=%u MemSsType=%u ChBitMask=0x%x TestType=%u CleanError=%u StartTest=%u StopTest=%u )\n",
      Socket, MemSsType, ChannelMaskInSocket, RmtDqTest, TRUE, NOT_START_TEST, STOP_TEST);
#endif
    TestControl (Host, Socket, MemSsType, ChannelMaskInSocket, SubChannel, RmtDqTest, TRUE, NOT_START_TEST, STOP_TEST);
  } // end for each socket


  ClearErrors = TRUE;
  StartTest = TRUE;
  StopTest = FALSE;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    ChannelMaskInSocket = 0;

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not enabled for testing THEN skip it
      if (!IsControllerTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
        continue;
      }

      ChannelMaskInSocket |= 0x1 << (Controller * NumChPerMc);
    } // end for each controller
    MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "   TestControl(Socket=%u MemSsType=%u ChBitMask=0x%x TestType=%u CleanError=%u StartTest=%u StopTest=%u )\n",
      Socket, MemSsType, ChannelMaskInSocket, RmtDqTest, TRUE, START_TEST, NOT_STOP_TEST);
#endif
    TestControl (Host, Socket, MemSsType, ChannelMaskInSocket, SubChannel, RmtDqTest, TRUE, START_TEST, NOT_STOP_TEST);

  } // end for each socket

  // ------------------------------------------------------------------------
  // Get the error status.
  // ------------------------------------------------------------------------
  // Note that we only get error status and counts and write results for
  // controllers that are enabled for validation.
#if ENBL_VERBOSE_LOG_MSGS || ENBL_TIMEOUT_DEBUG_LOG_MSGS
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  Checking error status.\n");
#endif
  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    SocketAnyLaneFailed = FALSE;

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      if (!IsControllerTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller)) {
          continue;
        }

      // set default values
      ControllerAnyLaneFailed = FALSE;

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not enabled for testing THEN skip it
        if (!IsChannelTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        // IF this channel is idle THEN skip it
        if (pCpgcPointTestCfg->TrafficModes[Controller][Channel] ==
          IdleTrafficMode) {
          continue;
        }

#if SUPPORT_FOR_TURNAROUNDS
        // IF this channel is configured to do rank-to-rank turnaround testing
        // AND this channel can't do rank-to-rank turnarounds (this can happen
        // as a result of the fact that the RankTestMode is per-socket whereas
        // the TrafficMode is only per-controller) THEN skip it
        if ((pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] == Turnarounds) &&
          !CanChannelDoTurnarounds (SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, pCpgcPointTestData->IoLevel, Socket, Controller,
            Channel, pCpgcPointTestData->IsDdrT)) {
          continue;
        }
#endif // SUPPORT_FOR_TURNAROUNDS

        // get test channel index
        TestedChnlIdx = GetTestChnlIndex (SsaServicesHandle, pSystemInfo,
          pMemCfg, pMemPointTestCfg, Socket, Controller, Channel);

        // IF this channel has lanes enabled for validation:
        if (IsChannelValEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {

          // preset the lane fail status
          pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AnyLaneFailed = FALSE;
          pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AllLanesFailed = TRUE;

          // PT currently I assume that there is no need to check ERID and DQ error status simultaneously
          // the error status either DQ status or EIRD status, NOT both.
          // This allows us to reuse the Lane failing status structure for ERID which occupy the first
          // two lanes.

          //
          // Call GetLaneErrorStatus ()
          //
          ChannelMaskInSocket = (0x1 << Channel) << (Controller * NumChPerMc);
          MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
          ChunkMask = pMemPointTestCfg->ChunkValBitmasks[Controller][Channel];
          TestType = RmtDqTest;
          if (GetCpgcPointTestCfgErid (pCpgcPointTestCfg)) {
            TestType = EarlyEridTest;
          }
          if ((ChunkMask & EVEN_CHUNKS) && (ChunkMask & ODD_CHUNKS)) {
            ChunkErrSel = All;
          } else if (ChunkMask & EVEN_CHUNKS) {
            ChunkErrSel = Even;
          } else if (ChunkMask & ODD_CHUNKS) {
            ChunkErrSel = Odd;
          } else {
            ChunkErrSel = All;
          }

          StatusArraySize = sizeof(ResultStatus);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "  GetLaneErrorStatus(Socket=%u MemSsType=%u ChBitMask=0x%x TestType=%u )\n",
            Socket, MemSsType, ChannelMaskInSocket, TestType);
#endif

          GetCpgcLaneErrorStatus (Host, Socket, MemSsType, (Controller * NumChPerMc + Channel), TestType, ChunkErrSel, sizeof (LaneErrorStatus),  LaneErrorStatus);



#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "    LaneErrorStatus=0x");
          for (ByteGroup = ((pSystemInfo->BusWidth / 8) - 1); ByteGroup >= 0; ByteGroup--) {
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "%02X", LaneErrorStatus[ByteGroup]);
          }
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "\n");
#endif
          // FOR each byte group:
          for (ByteGroup = 0; ByteGroup < (pSystemInfo->BusWidth / 8); ByteGroup++) {
            if (LaneErrorStatus[ByteGroup] != 0) {
              pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AnyLaneFailed = TRUE;
              break;
            }
          } // end for each byte group

            // IF we're testing Early Read ID
          if (GetCpgcPointTestCfgErid (pCpgcPointTestCfg)) {
#if __STUB__
            ASSERT (MAX_ERID_SIGNAL_CNT <= 8);
#endif // __STUB__
            if ((LaneErrorStatus[0] &
              pMemPointTestCfg->LaneValBitmasks[Controller][Channel][0] &
              ((1 << MAX_ERID_SIGNAL_CNT) - 1)) !=
              (pMemPointTestCfg->LaneValBitmasks[Controller][Channel][0] &
              ((1 << MAX_ERID_SIGNAL_CNT) - 1))) {
              pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AllLanesFailed = FALSE;
            }
          }
          // ELSE we're not testing Early Read ID
          else {
            // FOR each byte group:
            for (ByteGroup = 0; ByteGroup < (pSystemInfo->BusWidth / 8); ByteGroup++) {
              if ((LaneErrorStatus[ByteGroup] &
                pMemPointTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup]) !=
                pMemPointTestCfg->LaneValBitmasks[Controller][Channel][ByteGroup]) {
                pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AllLanesFailed = FALSE;
                break;
              }
            } // end for each byte group
          } // end else we're not testing Early Read ID

          //
          // Call MRC GetErroCount () which already takes care of the RPQ timeout
          //
          Counter = 0;
          GetErrorCount (Host, Socket, MemSsType, (Controller * NumChPerMc + Channel), TestType, Counter, &CpgcErrorCounterStatus);

          ErrCnt.Bits.Count = CpgcErrorCounterStatus.Count;
          ErrCnt.Bits.Overflow = CpgcErrorCounterStatus.Overflow;


          if (pCpgcPointTestData->TestStatusLogLevel >= 5) {

            DimmRankA = pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankA;
            DimmRankB = pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankB;

            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "\n"
              "  Socket=%u Controller=%u Channel=%u DimmA=%u RankA=%u\n",
              Socket, Controller, Channel, DimmRankA.Dimm, DimmRankA.Rank);

            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "  DimmB=%u RankB=%u RankCombinationsIndex=%u\n"
              "  LaneErrStat=0x",
              DimmRankB.Dimm, DimmRankB.Rank, pCpgcPointTestData->RankCombinationsIndex);

            for (ByteGroup = ((pSystemInfo->BusWidth / 8) - 1); ByteGroup >= 0; ByteGroup--) {
              NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
                "%02X", LaneErrorStatus[ByteGroup]);
            }

            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              " ErrCnt=%u Overflow=%u\n",
              ErrCnt.Bits.Count, ErrCnt.Bits.Overflow);
          }

          if ((((ErrCnt.Bits.Count != 0) || (ErrCnt.Bits.Overflow == TRUE)) &&
            (pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AnyLaneFailed == FALSE)) ||
            (((ErrCnt.Bits.Count == 0) && (ErrCnt.Bits.Overflow == FALSE)) &&
            (pCpgcPointTestData->LaneFailStats[Socket][Controller][Channel].AnyLaneFailed == TRUE))) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: Error count and status don't agree.\n");
#endif
            // PT Don't return error for now. The Simics and RcSim don't have the right
            // CPGC model which can cause error cnt and status not agree.
            // There is a run time variable can be used to determine whether is in Simics,
            // however, the variable is part of the MRC Host structure which the EV content
            // can not access unless we create an API for it.
            // The RcSim can be determined at the compiling.
            //            ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
            //            return 1;
          }

          if ((ErrCnt.Bits.Count != 0) || ErrCnt.Bits.Overflow) {
            ControllerAnyLaneFailed = TRUE;
          }

          // IF we're not in the binary search state and there is a result
          // handler
          if (!pCpgcPointTestData->InBinarySearchState &&
            pCpgcPointTestData->ResultHandler) {
            // pass the results back upstream
            if (pCpgcPointTestData->ResultHandler (
              SsaServicesHandle, pSystemInfo,
              pCpgcPointTestData->pClientTestData, Socket, Controller, Channel,
              pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankA,
              pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankB,
              pCpgcPointTestData->RankCombinationsIndex, LaneErrorStatus,
              ErrCnt, pTestStat)) {
              return 1;
            }
          } // end if we're not in the binary search state and ...
        }
        // ELSE this channel is active but not validated
        else {
          // PT where will below status go?
          // we still need to record a result to capture it's rank info
          ZeroMem (LaneErrorStatus, sizeof (LaneErrorStatus));
          ErrCnt.Bits.Count = 0;
          ErrCnt.Bits.Overflow = FALSE;
        }
      } // end for each channel

      if (ControllerAnyLaneFailed) {
        SocketAnyLaneFailed = TRUE;
      }
    } // end for each controller
  } // end for each socket

  return 0;
} // end function RunPointTest

/**
@brief
  This function is used to change the CPGC point test result handler

  @param[in, out]  SsaServicesHandle        Pointer to SSA services.
  @param[in]       ResultHandler            Pointer to result handler function.
  @param[in, out]  pVoidPointTestData       pointer to CPCG point test data

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
ChangeCpgcPointTestResultHandler (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MEM_POINT_TEST_RESULT_HANDLER ResultHandler,
  IN OUT VOID *pVoidPointTestData
  )
{
  CPGC_POINT_TEST_DATA *pPointTestData;

  if (pVoidPointTestData != NULL) {
    pPointTestData = (CPGC_POINT_TEST_DATA *)(pVoidPointTestData);
    pPointTestData->ResultHandler = ResultHandler;
    return 0;
  } else {
    return 1;
  }

}

/**
@brief
  This function is used to set up the CPGC point test.

  This function conforms to the SETUP_MEM_POINT_TEST function typedef in
  MemPointTest.h.

  If the IsFirstCall input parameter is set to TRUE then this function will
  allocate memory for the CpgcPointTest's private test status data structure,
  otherwise it will treat the ppPointTestData parameter as an input.

  If the InitDqdbContent input parameter is set to TRUE then this function will
  initialize the contents of the WDB, otherwise, it won't.

  @param[in, out]  SsaServicesHandle   Pointer to SSA services.
  @param[in]       pSystemInfo         Pointer to system information structure.
  @param[in]       pCpgcInfo           Pointer to CPGC information structure.
  @param[in]       pMemCfg             Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg    Pointer to generic memory point test configuration structure.
  @param[in]       pCpgcPointTestCfg   Pointer to CPGC point test configuration structure.
  @param[in]       ResultHandler       Pointer to result handler function.
  @param[in]       pClientTestData     Pointer to client test data structure.
  @param[in]       IoLevel             I/O level.
  @param[in]       IsDdrT              Flag indicating whether testing is for DDR-T devices.
  @param[in]       TestStatusLogLevel  Test status logging level.
  @param[in]       IsFirstCall         Flag indicating whether the call is the first.
  @param[in]       InitDqdbContent     Flag indicating whether to initialize the DQDB contents.
  @param[in, out]  ppPointTestData     Pointer to pointer to specific memory point test data
  @param[out]      pTestStat           Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetupCpgcPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MRC_SERVER_CPGC_INFO *pCpgcInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN MEM_POINT_TEST_RESULT_HANDLER ResultHandler,
  IN VOID *pClientTestData,
  IN MRC_LT IoLevel,
  IN BOOLEAN IsDdrT,
  IN UINT8 TestStatusLogLevel,
  IN BOOLEAN IsFirstCall,
  IN BOOLEAN InitDqdbContent,
  IN OUT VOID **ppVoidPointTestData,
  OUT TEST_STATUS *pTestStat)
{
  CPGC_POINT_TEST_DATA *pPointTestData;
  UINT8 TestedChnlCnt;

#if SUPPORT_FOR_TURNAROUNDS
  CPGC_ADDRESS TurnaroundCpgcAddr0 = { // rank-to-rank turnaround test
    //                 Rank,              Bank,              Row,               Col
    /* Start       */ {0,                 0,                 0,                 0},
    /* Wrap        */ {1,                 0,                 0,                 0x7F}, // the row wrap address will be runtime updated based on the DIMM info
    /* Order       */ {0,                 0,                 0,                 0},
    /* IncMode     */ {ExponentialAddrIncMode, LinearAddrIncMode, LinearAddrIncMode, LinearAddrIncMode},
    /* IncRate     */ {0,                 0,                 0,                 1},   // R0:A0, R1:A0, R0:A1, R1:A1, R0:A2, R1:A2, ..
    /* IncVal      */ {1,                 0,                 0,                 1},
    /* CarryEnable */ {FALSE,             FALSE,             FALSE,             FALSE}
  };
  CPGC_ADDRESS TurnaroundCpgcAddr1 = { // rank-to-rank turnaround test
    //                 Rank,              Bank,              Row,               Col
    /* Start       */ {0,                 0,                 0,                 0},
    /* Wrap        */ {1,                 0,                 0,                 0x7F}, // the row wrap address will be runtime updated based on the DIMM info
    /* Order       */ {0,                 0,                 0,                 0},
    /* IncMode     */ {LinearAddrIncMode, LinearAddrIncMode, LinearAddrIncMode, LinearAddrIncMode},
    /* IncRate     */ {1,                 0,                 0,                 3},   // R0:A0, R1:A0, R0:A1, R1:A1, R0:A2, R1:A2, ..
    /* IncVal      */ {1,                 0,                 0,                 1},
    /* CarryEnable */ {FALSE,             FALSE,             FALSE,             FALSE}
  };
#endif // SUPPORT_FOR_TURNAROUNDS

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetupCpgcPointTest(IoLevel=%u IsDdrT=%u TestStatusLogLevel=%u ",
    IoLevel, IsDdrT, TestStatusLogLevel);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "IsFirstCall=%u InitDqdbContent=%u)\n",
    IsFirstCall, InitDqdbContent);
#endif

  // IF this is the first call to this function
  if (IsFirstCall) {
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    RcDumpHeapUsage ();
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  Malloc(Size=%u) for CPGC point test data\n", sizeof(*pPointTestData));
#endif
    pPointTestData = (CPGC_POINT_TEST_DATA *)SsaServicesHandle->SsaCommonConfig->Malloc(
      SsaServicesHandle, sizeof(*pPointTestData));
    if (pPointTestData == NULL) {
#if ENBL_VERBOSE_ERROR_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Could not allocate memory for point test data.\n");
#endif
      ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
      return 1;
    }
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "    pBfr=%p\n", pPointTestData);
    RcDumpHeapUsage ();
#endif

    *ppVoidPointTestData = pPointTestData;

    ZeroMem (pPointTestData, sizeof(*pPointTestData));
  }
  // ELSE this is not the first call to this function
  else {
    // use the previously allocated buffer
    pPointTestData = (CPGC_POINT_TEST_DATA *)(*ppVoidPointTestData);
  }

  pPointTestData->TestStatusLogLevel = TestStatusLogLevel;
  pPointTestData->ResultHandler = ResultHandler;
  pPointTestData->pClientTestData = pClientTestData;
  pPointTestData->IoLevel = IoLevel;
  pPointTestData->IsDdrT = IsDdrT;
#if SUPPORT_FOR_TURNAROUNDS
  CopyMem (&pPointTestData->TurnaroundCpgcAddr0, &TurnaroundCpgcAddr0, sizeof (TurnaroundCpgcAddr0));
  CopyMem (&pPointTestData->TurnaroundCpgcAddr1, &TurnaroundCpgcAddr1, sizeof (TurnaroundCpgcAddr1));
#endif // SUPPORT_FOR_TURNAROUNDS

  // get the count of enabled channels (add one because index is zero base)
  TestedChnlCnt = GetTestChnlIndex(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, (MAX_SOCKET_CNT - 1), (MAX_CONTROLLER_CNT - 1),
    (MAX_CHANNEL_CNT - 1)) + 1;

  // IF this is not the first call to this function and the number of tested
  // channels has increased
  if (!IsFirstCall && (pPointTestData->pChannelTestData != NULL) &&
    (TestedChnlCnt > pPointTestData->TestedChnlCnt)) {
    // release the existing buffer
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
      RcDumpHeapUsage ();
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  Free(pBfr=%p)\n", pPointTestData->pChannelTestData);
#endif
    SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle,
      pPointTestData->pChannelTestData);
    pPointTestData->pChannelTestData = NULL;
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    RcDumpHeapUsage ();
#endif
  } // end if this is not the first call to this function and ...

  // save the number of tested channels value
  pPointTestData->TestedChnlCnt = TestedChnlCnt;

  // IF we need to allocate a buffer for the channel test data
  if ((pPointTestData->TestedChnlCnt != 0) &&
    (pPointTestData->pChannelTestData == NULL)) {
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    RcDumpHeapUsage ();
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  Malloc(Size=%u) for CPGC per channel test data array\n",
      pPointTestData->TestedChnlCnt * sizeof(CPGC_PER_CHNL_POINT_TEST_DATA));
#endif
    pPointTestData->pChannelTestData = (CPGC_PER_CHNL_POINT_TEST_DATA *) \
      SsaServicesHandle->SsaCommonConfig->Malloc(SsaServicesHandle,
      (pPointTestData->TestedChnlCnt * sizeof(CPGC_PER_CHNL_POINT_TEST_DATA)));
    if (pPointTestData->pChannelTestData == NULL) {
#if ENBL_VERBOSE_ERROR_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Could not allocate memory for per channel test data\n");
#endif
      ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
      return 1;
    }
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "    pBfr=%p\n", pPointTestData->pChannelTestData);
    RcDumpHeapUsage ();
#endif

    ZeroMem (pPointTestData->pChannelTestData, pPointTestData->TestedChnlCnt * sizeof(CPGC_PER_CHNL_POINT_TEST_DATA));

  } // end if we need to allocate a buffer for the channel test data

  if (GetRankCombinationsCnts(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, IoLevel, IsDdrT, &pPointTestData->RankCombinationsCnts,
    &pPointTestData->MaxRankCombinationsCnt, pTestStat)) {
    return 1;
  }

  //
  // Call converged MRC API
  //
  SetupTestWrapper (SsaServicesHandle, pSystemInfo, pMemCfg, pMemPointTestCfg, (CPGC_POINT_TEST_CONFIG *) pCpgcPointTestCfg);

  return 0;
} // end function SetupCpgcPointTest

/**
@brief
  This function is used to get the DIMM/rank pairs for a given channel.

  This function conforms to the GET_MEM_POINT_TEST_DIMM_RANKS typedef in
  MemPointTest.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pPointTestCfg      Pointer to specific memory point test configuration structure.
  @param[in]       pPointTestData     Pointer to specific memory point test data structure.
  @param[in]       Socket             Zero based CPU socket number.
  @param[in]       Controller         Zero based controller number.
  @param[in]       Channel            Zero based channel number.
  @param[in, out]  pPairCount         Number of entries in the DimmRankPairs array.  Input=number of elements in pDimmRankPairs.  Output=number of populated elements in pDimmRankPairs.
  @param[out]      pDimmRankPairs     Array of DIMM and physical rank pair values.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 GetCpgcPointTestDimmRanks(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN VOID *pVoidPointTestData,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN OUT UINT8 *pPairCount,
  OUT DIMM_RANK_PAIR *pDimmRankPairs,
  OUT TEST_STATUS *pTestStat)
{
  CPGC_POINT_TEST_DATA *pPointTestData = (CPGC_POINT_TEST_DATA*) pVoidPointTestData;
  UINT8 PairCountInput = *pPairCount;
  UINT8 TestedChnlIdx;

  UNREFERENCED_PARAMETER(SsaServicesHandle);

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "GetCpgcPointTestDimmRanks(Socket=%u Controller=%u Channel=%u)\n",
    Socket, Controller, Channel);
#endif

  // IF the channel is not enabled for testing
  if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, Socket, Controller, Channel)) {
    *pPairCount = 0;
  }
  // ELSE the channel is enabled for testing
  else {
    TestedChnlIdx = GetTestChnlIndex(SsaServicesHandle, pSystemInfo, pMemCfg,
      pMemPointTestCfg, Socket, Controller, Channel);

    // IF there are sufficient DIMM/rank pairs
    if (PairCountInput >= 1) {
      *pPairCount = 1;
      CopyMem (&pDimmRankPairs[0],
        &pPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankA,
        sizeof (DIMM_RANK_PAIR));

#if SUPPORT_FOR_TURNAROUNDS
      if (pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] == Turnarounds) {
        // IF there are sufficient DIMM/rank pairs
        if (PairCountInput >= 2) {
          *pPairCount = 2;
          CopyMem (&pDimmRankPairs[1],
            &pPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankB,
            sizeof (DIMM_RANK_PAIR));
        }
        // ELSE there are insufficient DIMM/rank pairs
        else {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "ERROR: Insufficient DIMM/rank pairs\n");
#endif
          ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      } // end if this channel is doing rank-to-rank turnarounds
#endif // SUPPORT_FOR_TURNAROUNDS
    }
    // ELSE there are insufficient DIMM/rank pairs
    else {
#if ENBL_VERBOSE_ERROR_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "ERROR: Insufficient DIMM/rank pairs\n");
#endif
      ProcError(SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
      return 1;
    }
  } // end else the channel is enabled for testing

  return 0;
} // end function GetCpgcPointTestDimmRanks

/**
@brief
  This function is used to update the pointtest data with the "current" dimm/rank pair
  based on the the rank iteration index.
  This function configures that hardware to run the point test for the given
  rank index.

  This function conforms to the SET_MEM_POINT_TEST_RANK_COMBINATION_INDEX
  typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle     Pointer to SSA services.
  @param[in]       pSystemInfo           Pointer to system information structure.
  @param[in]       pMemCfg               Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg      Pointer to generic memory point test configuration structure.
  @param[in]       pPointTestCfg         Pointer to specific memory point test configuration structure.
  @param[in]       pPointTestData        Pointer to specific memory point test data structure.
  @param[in]       RankCombinationIndex  Rank combination index.
  @param[in]       UpdateHwSetting       Flag to indicate if need to apply the setting to register or not
                                         FALSE: only update on the rankCombinartionIndex update the
                                         "Current" dimm/rank pairs of the point test data.
                                         TRUE: will retrieve the "current" dimm/rank paris and program them
                                         to HW registers.
                                         NOTE: the function need to be called at least twice. The first
                                         with this parameter as FALSE, then later called with this parameter
                                         as TRUE.
  @param[out]      pTestStat             Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetCpgcPointTestRankCombinationIndex(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pVoidPointTestCfg,
  IN VOID *pVoidPointTestData,
  IN UINT8 RankCombinationIndex,
  IN BOOLEAN UpdateHwSetting,
  OUT TEST_STATUS *pTestStat)
{
  CPGC_POINT_TEST_CONFIG* pCpgcPointTestCfg = (CPGC_POINT_TEST_CONFIG*) pVoidPointTestCfg;
  CPGC_POINT_TEST_DATA* pCpgcPointTestData = (CPGC_POINT_TEST_DATA*) pVoidPointTestData;
  UINT8 Socket, Controller, Channel;
  UINT8 ChnlRankCombinationIndex;
  UINT8 TestedChnlIdx;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetCpgcPointTestRankCombinationIndex(RankCombinationIndex=%u, UpdateHwSetting=%u)\n",
    RankCombinationIndex, UpdateHwSetting);
#endif

  if (UpdateHwSetting){
    //
    // program the rank setting which include program the sequencer, subsequencers and address generator
    //
    if (SetCrntRanks(SsaServicesHandle, pSystemInfo, pMemCfg,
      pMemPointTestCfg, pCpgcPointTestCfg, pCpgcPointTestData, pTestStat)) {
        return 1;
    }
  }// UpdateHwSetting
  else {
    if (GetRankCombinationsCnts(SsaServicesHandle, pSystemInfo, pMemCfg,
      pMemPointTestCfg, pCpgcPointTestData->IoLevel, pCpgcPointTestData->IsDdrT,
      &pCpgcPointTestData->RankCombinationsCnts,
      &pCpgcPointTestData->MaxRankCombinationsCnt, pTestStat)) {
      return 1;
    }

    pCpgcPointTestData->RankCombinationsIndex = RankCombinationIndex;

    // FOR each socket
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          // IF this channel is not enabled for testing THEN skip it
          if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel)) {
            continue;
          }

          TestedChnlIdx = GetTestChnlIndex(SsaServicesHandle, pSystemInfo,
            pMemCfg, pMemPointTestCfg, Socket, Controller, Channel);

          //NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          //  "CpgcPointTest.SetCpgcPointTestRankCombinationIndex(Socket:%u controller:%u channel: %u TestChnlIdx=%u)\n", Socket, Controller, Channel, TestedChnlIdx);

          // IF this channel has more than one rank combination
          if (pCpgcPointTestData->RankCombinationsCnts[Socket][Controller][Channel] > 0) {
            // limit the rank iteration index to the maximum number supported by
            // the given channel
            ChnlRankCombinationIndex = RankCombinationIndex % \
              pCpgcPointTestData->RankCombinationsCnts[Socket][Controller][Channel];

            GetDimmRankPerCombinationIndex(SsaServicesHandle,pSystemInfo, pMemCfg,pMemPointTestCfg,
              Socket, Controller, Channel, ChnlRankCombinationIndex,
              &pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankA,
              &pCpgcPointTestData->pChannelTestData[TestedChnlIdx].CrntDimmRankB);

          } // end if this channel has more than one rank combination
        } // end for each channel
      } // end for each controller
    } // end for each socket
  } // else not update HW, but update the CrntDimmRankA/B of the point test data.
  return 0;
} // end function SetCpgcPointTestRankCombinationIndex

/**
@brief
  This function is used to set the flag indicating whether we're in the binary
  search state.  It is intended that the point test runs for a shorter period
  of time in the binary search state.

  This function conforms to the SET_MEM_POINT_TEST_IN_BINARY_SEARCH_STATE
  typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle    Pointer to SSA services.
  @param[in]       pSystemInfo          Pointer to system information structure.
  @param[in]       pMemCfg              Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg     Pointer to generic memory point test configuration structure.
  @param[in]       pVoidPointTestCfg    Pointer to specific memory point test configuration structure.
  @param[in]       pVoidPointTestData   Pointer to specific memory point test data structure.
  @param[in]       InBinarySearchState  Flag indicating whether we're in the binary search state.
  @param[out]      pTestStat            Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 SetCpgcPointTestInBinarySearchState(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pVoidPointTestCfg,
  IN VOID *pVoidPointTestData,
  IN BOOLEAN InBinarySearchState,
  OUT TEST_STATUS *pTestStat)
{
  CONST CPGC_POINT_TEST_CONFIG* pCpgcPointTestCfg = (const CPGC_POINT_TEST_CONFIG*) pVoidPointTestCfg;
  CPGC_POINT_TEST_DATA *pPointTestData = (CPGC_POINT_TEST_DATA*) pVoidPointTestData;
  UINT8 Socket, Controller, Channel;
  UINT32 LoopCount;
  UINT8  MemSsType;
  UINT32 ChBitMask;
  PSYSHOST Host = NULL;

  Host = GetSysHostPointer ();

  UNREFERENCED_PARAMETER(pTestStat);

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "SetCpgcPointTestInBinarySearchState(InBinarySearchState=%u)\n", InBinarySearchState);
#endif
  pPointTestData->InBinarySearchState = InBinarySearchState;

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not enabled for testing THEN skip it
        if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        // IF this channel is idle THEN skip it
        if (pCpgcPointTestCfg->TrafficModes[Controller][Channel] ==
          IdleTrafficMode) {
          continue;
        }

        LoopCount = (InBinarySearchState) ? \
          pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel] : \
          pCpgcPointTestCfg->LoopCounts[Controller][Channel];

        //
        // Call SetupTestLoopCount () per channel
        //
        MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];
        ChBitMask = (0x1 << Channel) << (Controller * MAX_CHANNEL_CNT);

        SetupTestLoopCount (Host, Socket, MemSsType, ChBitMask, LoopCount);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "  SetupTestLoopCount(Socket=%u MeSsType=%u ChBitMask=%u LoopCount=%u)\n",
          Socket, MemSsType, ChBitMask, LoopCount);
#endif

      } // end for each channel
    } // end for each controller
  } // end for each socket

  return 0;
} // end function SetCpgcPointTestInBinarySearchState

/**
@brief
  This function is used to run a CPGC point test.

  This function conforms to the RUN_MEM_POINT_TEST typedef in MemPointTest.h.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       pVoidPointTestCfg  Pointer to specific memory point test configuration structure.
  @param[in]       pVoidPointTestData Pointer to specific memory point test data structure.
  @param[out]      pLaneFailStats     Pointer to lane failure status array.  Indexed as [Socket][Controller][Channel].  (Value may be NULL.)
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 RunCpgcPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST VOID *pVoidPointTestCfg,
  IN VOID *pVoidPointTestData,
  OUT LANE_FAIL_STATS *pLaneFailStats,
  OUT TEST_STATUS *pTestStat)
{
  CONST CPGC_POINT_TEST_CONFIG* pCpgcPointTestCfg = (const CPGC_POINT_TEST_CONFIG*) pVoidPointTestCfg;
  CPGC_POINT_TEST_DATA *pPointTestData = (CPGC_POINT_TEST_DATA*) pVoidPointTestData;

#if SUPPORT_FOR_TURNAROUNDS | ENBL_FUNCTION_TRACE_DEBUG_MSGS
  UINT8 Socket;
  UINT8 Controller, Channel;
#endif

#if SUPPORT_FOR_TURNAROUNDS
  UINT8 Uniseq;
  UINT8 SubseqStartIndex, SubseqEndIndex;
  UINT32 DqdbUniseqSeed;
  UINT32 LoopCount;
  BOOLEAN EnableGlobalControl;
  TRAFFIC_MODE TrafficMode;
  BOOLEAN IsAnyWr2Rd = FALSE;
  MEM_POINT_TEST_RESULT_HANDLER SavedResultHandler;
#endif // #if SUPPORT_FOR_TURNAROUNDS

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "RunCpgcPointTest()\n");
#endif


#if SUPPORT_FOR_TURNAROUNDS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if any channel's traffic mode is write-to-read rank-to-rank turnarounds
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not enabled for testing THEN skip it
        if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        // IF this channel's traffic mode is write-to-read rank-to-rank turnarounds
        if (pCpgcPointTestCfg->TrafficModes[Controller][Channel] == Wr2RdTrafficMode) {
          IsAnyWr2Rd = TRUE;
          break;
        }
      } // end for each channel

      if (IsAnyWr2Rd) {
        break;
      }
    } // end for each controller

    if (IsAnyWr2Rd) {
      break;
    }
  } // end for each socket

  // IF at least one channel is configured for write-to-read rank-to-rank
  // turnarounds
  if (IsAnyWr2Rd) {
    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          // IF this channel is not enabled for testing THEN skip it
          if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel)) {
            continue;
          }

          TrafficMode = pCpgcPointTestCfg->TrafficModes[Controller][Channel];

          // IF this channel's traffic mode is write-to-read rank-to-rank turnarounds
          if (TrafficMode == Wr2RdTrafficMode) {
            // setup to perform the initial write operation
            SubseqStartIndex = 0;
            SubseqEndIndex = 0;
            EnableGlobalControl = TRUE;

            // set the address schema for the first part
            SetSeqAddr(SsaServicesHandle, Socket, Controller, Channel,
              &pPointTestData->TurnaroundCpgcAddr0);

            // restore the write seed
            // FOR each DqDB unisequencer:
            for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
              NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
                "SetDqdbUniseqWrSeed(Socket=%u Controller=%u Channel=%u Uniseq=%u Seed=0x%06X)\n",
                Socket, Controller, Channel, Uniseq,
                pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);
#endif
              SsaServicesHandle->SsaMemoryServerConfig->SetDqdbUniseqWrSeed(
                SsaServicesHandle, Socket, Controller, Channel, Uniseq,
                pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);
            } // end for each DqQD unisequencer
          }
          // ELSE IF the traffic mode is Idle THEN skip it
          else if (TrafficMode == IdleTrafficMode) {
            continue;
          }
          else {
            // temporarily disable global control
            SubseqStartIndex = 0;
            SubseqEndIndex = SequencersByTrafficMode[TrafficMode].SubSequenceEnd;
            EnableGlobalControl = FALSE;
          }

          // only one loop for the first part
          LoopCount = 1;

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "SetSeqConfig(Socket=%u Controller=%u Channel=%u StartDelay=%u\n",
            Socket, Controller, Channel, pCpgcPointTestCfg->StartDelays[Controller][Channel]);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  SubseqStartIndex=%u SubseqEndIndex=%u LoopCount=0x%X InitMode=%u\n",
            SubseqStartIndex, SubseqEndIndex, LoopCount, ReutCpgcInitMode);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  EnableGlobalControl=%u EnableConstantWriteStrobe=%u EnableDummyReads=%u\n",
            EnableGlobalControl, FALSE, FALSE);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  AddrUpdateRateMode=%u)\n",
            CachelineAddrUpdateRate);
#endif
          SsaServicesHandle->SsaMemoryServerConfig->SetSeqConfig(
            SsaServicesHandle, Socket, Controller, Channel,
            pCpgcPointTestCfg->StartDelays[Controller][Channel],
            SubseqStartIndex, SubseqEndIndex, LoopCount, ReutCpgcInitMode,
            EnableGlobalControl, FALSE, FALSE, CachelineAddrUpdateRate);
        } // end for each channel
      } // end for each controller
    } // end for each socket

    // for write-to-read turnaround, we need to first run a test to prime the
    // memory, but we should check and save the test result of the first run.
    // we need to set the pointtest result handler to NULL to let the RunPointTest()
    // to skip saving the result.
    SavedResultHandler = pPointTestData->ResultHandler;
    pPointTestData->ResultHandler = NULL;

    // perform the initial write operation
    if (RunPointTest(SsaServicesHandle, pSystemInfo, pMemCfg,
      pMemPointTestCfg, pCpgcPointTestCfg, pPointTestData, pTestStat)) {
      return 1;
    }

    // restore the results handler
    pPointTestData->ResultHandler = SavedResultHandler;

    EnableGlobalControl = TRUE;

    // FOR each socket:
    for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
      // FOR each controller:
      for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
        // FOR each channel:
        for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
          // IF this channel is not enabled for testing THEN skip it
          if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
            pMemPointTestCfg, Socket, Controller, Channel)) {
            continue;
          }

          TrafficMode = pCpgcPointTestCfg->TrafficModes[Controller][Channel];

          // IF this channel's traffic mode is write-to-read rank-to-rank turnarounds
          if (TrafficMode == Wr2RdTrafficMode) {
            // setup to perform the base-read-write operations
            SubseqStartIndex = 1;
            SubseqEndIndex = 1;

            // set the address schema for the second part
            SetSeqAddr(SsaServicesHandle, Socket, Controller, Channel,
              &pPointTestData->TurnaroundCpgcAddr1);

            // FOR each DqDB unisequencer:
            for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
              // read the current DQDB write seed
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
              NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
                "GetDqdbUniseqWrSeed(Socket=%u Controller=%u Channel=%u Uniseq=%u)\n",
                Socket, Controller, Channel, Uniseq,
                pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);
#endif
              SsaServicesHandle->SsaMemoryServerConfig->GetDqdbUniseqWrSeed(
                SsaServicesHandle, Socket, Controller, Channel, Uniseq,
                &DqdbUniseqSeed);
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
              NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
                "  Seed=0x%06X\n", DqdbUniseqSeed);
#endif

              // set the current DQDB write seed
#if ENBL_BIOS_SSA_API_DEBUG_MSGS
              NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
                "SetDqdbUniseqWrSeed(Socket=%u Controller=%u Channel=%u Uniseq=%u Seed=0x%06X)\n",
                Socket, Controller, Channel, Uniseq, DqdbUniseqSeed);
#endif
              SsaServicesHandle->SsaMemoryServerConfig->SetDqdbUniseqWrSeed(
                SsaServicesHandle, Socket, Controller, Channel, Uniseq,
                DqdbUniseqSeed);
            } // end for each DqQD unisequencer
          }
          // ELSE IF the traffic mode is Idle THEN skip it
          else if (TrafficMode == IdleTrafficMode) {
            continue;
          }
          else {
            // re-enable global control
            SubseqStartIndex = 0;
            SubseqEndIndex = SequencersByTrafficMode[TrafficMode].SubSequenceEnd;
          }

          // full loop count for the second part
          LoopCount = pCpgcPointTestCfg->LoopCounts[Controller][Channel];

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "SetSeqConfig(Socket=%u Controller=%u Channel=%u StartDelay=%u\n",
            Socket, Controller, Channel, pCpgcPointTestCfg->StartDelays[Controller][Channel]);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  SubseqStartIndex=%u SubseqEndIndex=%u LoopCount=0x%X InitMode=%u\n",
            SubseqStartIndex, SubseqEndIndex, LoopCount, ReutCpgcInitMode);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  EnableGlobalControl=%u EnableConstantWriteStrobe=%u EnableDummyReads=%u\n",
            EnableGlobalControl, FALSE, FALSE);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "  AddrUpdateRateMode=%u)\n",
            CachelineAddrUpdateRate);
#endif
          SsaServicesHandle->SsaMemoryServerConfig->SetSeqConfig(
            SsaServicesHandle, Socket, Controller, Channel,
            pCpgcPointTestCfg->StartDelays[Controller][Channel],
            SubseqStartIndex, SubseqEndIndex, LoopCount, ReutCpgcInitMode,
            EnableGlobalControl, FALSE, FALSE, CachelineAddrUpdateRate);
        } // end for each channel
      } // end for each controller
    } // end for each socket
  } // end if at least one channel is configured for write-to-read ...
#endif // SUPPORT_FOR_TURNAROUNDS

  // initialize the lane failure status array
  ZeroMem (pPointTestData->LaneFailStats,  sizeof (pPointTestData->LaneFailStats));

  // run a point test with the current rank combination
  if (RunPointTest(SsaServicesHandle, pSystemInfo, pMemCfg,
    pMemPointTestCfg, pCpgcPointTestCfg, pPointTestData, pTestStat)) {
    return 1;
  }

  if (pLaneFailStats != NULL) {
    CopyMem (pLaneFailStats, &pPointTestData->LaneFailStats, sizeof (*pLaneFailStats));
  }

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not enabled for testing THEN skip it
        if (!IsChannelTestingEnabled(SsaServicesHandle, pSystemInfo, pMemCfg,
          pMemPointTestCfg, Socket, Controller, Channel)) {
          continue;
        }

        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  Socket=%u Controller=%u Channel=%u AnyLaneFailed=%u AllLanesFailed=%u\n",
          Socket, Controller, Channel,
          pPointTestData->LaneFailStats[Socket][Controller][Channel].AnyLaneFailed,
          pPointTestData->LaneFailStats[Socket][Controller][Channel].AllLanesFailed);
      } // end for each channel
    } // end for each controller
  } // end for each socket
#endif

  return 0;
} // end function RunCpgcPointTest

/**
@brief
  This function is used to clean up the CPGC point test.

  This function conforms to the CLEANUP_MEM_POINT_TEST function typedef in
  MemPointTest.h.

  If the IsLastCall input parameter is set to TRUE then this function will free
  the memory for the CpgcPointTest's private test status data structure and set
  the pointer to NULL, otherwise it will not.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IsLastCall         Flag indicating whether the call is the last.
  @param[in, out]  ppPointTestData    Pointer to pointer to specific memory point test data structure.
**/
VOID CleanupCpgcPointTest(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN BOOLEAN IsLastCall,
  IN OUT VOID **ppVoidPointTestData)
{
  CPGC_POINT_TEST_DATA *pPointTestData = *((CPGC_POINT_TEST_DATA**) ppVoidPointTestData);
  //UINT8 Socket, Controller;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "CleanupCpgcPointTest(IsLastCall=%u)\n", IsLastCall);
#endif

  // IF this is the last call to this function
  if (IsLastCall) {
    if (pPointTestData != NULL) {
      if (pPointTestData->pChannelTestData != NULL) {
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
        RcDumpHeapUsage ();
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "  Free(pBfr=%p)\n", pPointTestData->pChannelTestData);
#endif
        SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle,
          pPointTestData->pChannelTestData);
#if ENBL_MALLOC_DEBUG_LOG_MSGS
        RcDumpHeapUsage ();
#endif
      }

#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
      RcDumpHeapUsage ();
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  Free(pBfr=%p)\n", pPointTestData);
#endif
      SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle,
        pPointTestData);
      *ppVoidPointTestData = NULL;
#if ENBL_MEM_POOL_DEBUG_LOG_MSGS
      RcDumpHeapUsage ();
#endif
    }
  } // end if this is the last call to this function
} // end function CleanupCpgcPointTest

// end file CpgcPointTest.c
