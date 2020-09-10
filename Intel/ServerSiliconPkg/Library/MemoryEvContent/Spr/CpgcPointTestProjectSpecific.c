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

#include "ssabios.h"
#include <Library/MemoryEvContentLib.h>
#include "CpgcPointTest.h" 
#include "ChkRetStat.h"
#include "MemPointTest.h"

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/BitMaskLib.h>

/**
  @brief
  This function is used to set default values in the given CPGC point test
  configuration.

  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
**/
VOID 
SetCpgcPointTestCfgDefaults (
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  )
{
  UINT8 Controller, Channel, Uniseq, LmnIdx;

  // start by setting everything to 0
  ZeroMem (pCpgcPointTestCfg, sizeof (*pCpgcPointTestCfg));

  // Update header
  pCpgcPointTestCfg->Header.MajorVersion = CPGC2_MAJOR_VERSION;
  pCpgcPointTestCfg->Header.MinorVersion = 0;

  pCpgcPointTestCfg->CpgcStopMode = DoNotStopCpgcStopMode;

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      pCpgcPointTestCfg->TrafficModes[Controller][Channel] = LoopbackTrafficMode;
      pCpgcPointTestCfg->AddrModes[Controller][Channel] = LinearAddrMode;
      pCpgcPointTestCfg->BurstLengths[Controller][Channel] = 32;
      pCpgcPointTestCfg->LoopCounts[Controller][Channel] = 15;
      pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel] = 10;
      pCpgcPointTestCfg->StartDelays[Controller][Channel] = 0;
      pCpgcPointTestCfg->InterSubseqWaits[Controller][Channel] = 0;
      pCpgcPointTestCfg->EccDataSource[Channel] = ByteGroup0;
      pCpgcPointTestCfg->DcPolarity[Channel] = DcLow;
    } // end for each channel
  } // end for each controller

  pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns = FALSE;

  // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    pCpgcPointTestCfg->DqdbIncRateModes[Channel] = ExponentialDqdbIncRateMode;
    pCpgcPointTestCfg->DqdbIncRates[Channel] = 0;
    pCpgcPointTestCfg->DqdbLaneRotationRates[Channel] = 2;
  } // end for each channel

    // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // FOR each DqDB unisequencer:
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        pCpgcPointTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq] = GaloisLfsrDqdbUniseqMode;
      } // end for each WDB unisequencer

      pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][0] = 0x123456;
      pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][1] = 0xABCDEF;
      pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][2] = 0x876543;

      // FOR each DqDB LMN unisequencer:
      for (Uniseq = 0; Uniseq < MAX_DQDB_LMN_UNISEQ_CNT; Uniseq++) {
        // FOR each DqDB LMN value:
        for (LmnIdx = 0; LmnIdx < MAX_DQDB_LMN_VALUES_CNT; LmnIdx++) {
          pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][LmnIdx] = 1;
        } // end for each DqDB LMN value
      } // end for each DqDB LMN unisequencer

      pCpgcPointTestCfg->CadbModes[Controller][Channel] = NormalCadbMode;
    } // end for each channel
  } // end for each controller

  pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns = FALSE;

} // end function SetCpgcPointTestCfgDefaults

  /**
  @brief
  This function is used to log the CPGC point test configuration.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcPointTestCfg  Pointer to test configuration structure.
  **/
VOID LogCpgcPointTestCfg (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  )
{
  UINT8 Controller, Channel, Cacheline, Chunk, Uniseq, Idx;

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "CPGC Point Test Configuration:\n");
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  CpgcStopMode=%u\n", pCpgcPointTestCfg->CpgcStopMode);

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  Controller=%u\n", Controller);
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "    Channel=%u\n", Channel);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      TrafficMode=%u\n",
        pCpgcPointTestCfg->TrafficModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      AddrMode=%u\n",
        pCpgcPointTestCfg->AddrModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      BurstLength=%u\n",
        pCpgcPointTestCfg->BurstLengths[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      LoopCount=%u\n",
        pCpgcPointTestCfg->LoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      BinarySearchLoopCount=%u\n",
        pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      StartDelay=%u\n",
        pCpgcPointTestCfg->StartDelays[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      InterSubseqWait=%u\n",
        pCpgcPointTestCfg->InterSubseqWaits[Controller][Channel]);
    } // end for each channel
  } // end for each controller

    // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  Channel=%u\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    EccDataSource=%u\n", pCpgcPointTestCfg->EccDataSource[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    DcPolarity=%u\n", pCpgcPointTestCfg->DcPolarity[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    DcBitmasks: ");
    LogByteArrayAsHex (
      pCpgcPointTestCfg->DcBitmasks[Channel],
      (MAX_BUS_WIDTH / 8));
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
  } // end for each channel


  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  EnableUserDefinedDqdbPatterns=%u\n", pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns);

  // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  Channel=%u\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    UserDefinedDqdbPatternLength=%u\n", pCpgcPointTestCfg->UserDefinedDqdbPatternLengths[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    UserDefinedDqdbPattern:\n");

    for (Cacheline = 0; Cacheline < NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2; Cacheline++) {
      for (Chunk = 0; Chunk < CHUNKS_PER_CACHELINE; Chunk++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "      Cacheline=%u Chunk=%d 0x", Cacheline, Chunk);
        LogByteArrayAsHex (
          pCpgcPointTestCfg->UserDefinedDqdbPatterns[Channel][Cacheline][Chunk],
          (DQDB_PATTERN_WIDTH / 8));
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
      } // end for each chunk
    } // end for each cacheline

    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "      DqdbIncRateMode=%u\n",
      pCpgcPointTestCfg->DqdbIncRateModes[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "      DqdbIncRate=%u\n",
      pCpgcPointTestCfg->DqdbIncRates[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "      DqdbLaneRotationRate=%u\n",
      pCpgcPointTestCfg->DqdbLaneRotationRates[Channel]);
  } // end for each channel

    // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  Controller=%u\n", Controller);
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "    Channel=%u\n", Channel);
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "      DqdbUniseqModes[%d]=%u\n",
          Uniseq, pCpgcPointTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq]);
      }
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "      DqdbUniseqSeeds[%d]=0x%X\n",
          Uniseq, pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);
      }
      for (Uniseq = 0; Uniseq < MAX_DQDB_LMN_UNISEQ_CNT; Uniseq++) {
        for (Idx = 0; Idx < MAX_DQDB_LMN_VALUES_CNT; Idx++) {
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "      DqdbLmnValues[%d][%d]=%u\n",
            Uniseq, Idx, pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][Idx]);
        }
      }

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      CadbMode=%u\n",
        pCpgcPointTestCfg->CadbModes[Controller][Channel]);

    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n");
} // end function LogCpgcPointTestCfg

/**
@brief
This function is used to check the CPGC point test configuration for
correctness/consistency. It also fills in the given memory configuration
structure.

@param[in, out]  SsaServicesHandle  Pointer to SSA services.
@param[in]       pSystemInfo        Pointer to system information structure.
@param[in]       pCpgcInfo          Pointer to CPGC information structure.
@param[in]       pMemCfg            Pointer to memory configuration structure.
@param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
@param[in]       pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.
@param[out]      pTestStat          Pointer to test status in results metadata structure.

@retval  0  success
@retval  1  error occurred
**/
UINT8
ChkCpgcPointTestCfg (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MRC_SERVER_CPGC_INFO *pCpgcInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  OUT TEST_STATUS *pTestStat
  )
{
  UINT8 Socket, Controller, Channel, Uniseq;
  UINT16 BurstLength;

#if ENBL_FUNCTION_TRACE_DEBUG_MSGS
NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
  "ChkCpgcPointTestCfg()\n");
#endif

// FOR each socket:
for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      // IF this channel is not enabled for testing:
      if (!IsChannelTestingEnabled (SsaServicesHandle, pSystemInfo, pMemCfg,
        pMemPointTestCfg, Socket, Controller, Channel)) {
        continue;
      }

      if (pCpgcPointTestCfg->TrafficModes[Controller][Channel] >= TrafficModeMax) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: TrafficMode value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->TrafficModes[Controller][Channel], TrafficModeMax);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      if (pCpgcPointTestCfg->AddrModes[Controller][Channel] > AddrModeMax) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: AddrMode value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->AddrModes[Controller][Channel], AddrModeMax);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      BurstLength = pCpgcPointTestCfg->BurstLengths[Controller][Channel];

      // IF the burst length exceeds the maximum linear value
      if (BurstLength > pCpgcInfo->MaxBurstLengthLinearVal) {
        UINT8 BurstLengthExponent;

        BurstLengthExponent = GetHighestBitSetInMask (BurstLength, 32);

        // IF the burst length is not a power of 2
        if (((UINT32)1 << BurstLengthExponent) != BurstLength) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: BurstLength value (%u) exceeds maximum linear value (%u) and is not a power of 2.\n",
            BurstLength, pCpgcInfo->MaxBurstLengthLinearVal);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }

        if (BurstLengthExponent > pCpgcInfo->MaxBurstLengthExponentVal) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: BurstLength value (%u) exceeds maximum exponential value (%u).\n",
            BurstLength, (1 << pCpgcInfo->MaxBurstLengthExponentVal));
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      } // end if the burst length exceeds the maximum linear value

      if (pCpgcPointTestCfg->LoopCounts[Controller][Channel] > pCpgcInfo->MaxLoopCountVal) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: LoopCount value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->LoopCounts[Controller][Channel], pCpgcInfo->MaxLoopCountVal);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      if (pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel] > pCpgcInfo->MaxLoopCountVal) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: BinarySearchLoopCount value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->BinarySearchLoopCounts[Controller][Channel], pCpgcInfo->MaxLoopCountVal);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      if (pCpgcPointTestCfg->StartDelays[Controller][Channel] > pCpgcInfo->MaxStartDelayVal) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: StartDelay value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->StartDelays[Controller][Channel], pCpgcInfo->MaxStartDelayVal);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      if (pCpgcPointTestCfg->InterSubseqWaits[Controller][Channel] > pCpgcInfo->MaxInterSubseqWaitVal) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: InterSubseqWait value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->InterSubseqWaits[Controller][Channel], pCpgcInfo->MaxInterSubseqWaitVal);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      // IF user defined DqDB patterns are enabled
      if (pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns) {
        if ((pCpgcPointTestCfg->UserDefinedDqdbPatternLengths[Channel] == 0) ||
          (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths[Channel] > NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2)) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: UserDefinedDqdbPatternLength value (%u) is out of range (0 < val < %u).\n",
            pCpgcPointTestCfg->UserDefinedDqdbPatternLengths[Channel], NUMBER_USER_DEFINED_DQDB_CACHELINES_CPCG2);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      } // end if user defined DqDB patterns are enabled

      if (pCpgcPointTestCfg->DqdbIncRateModes[Channel] > DqdbIncRateModeMax) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: DqdbIncRateMode value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->DqdbIncRateModes[Channel], DqdbIncRateModeMax);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

      // IF the DqDB increment rate mode is linear
      if (pCpgcPointTestCfg->DqdbIncRateModes[Channel] == LinearDqdbIncRateMode) {
        if (pCpgcPointTestCfg->DqdbIncRates[Channel] > pCpgcInfo->MaxDqdbIncRateLinearVal) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: DqdbIncRate value (%u) is out of range (0 <= val <= %u).\n",
            pCpgcPointTestCfg->DqdbIncRates[Channel], pCpgcInfo->MaxDqdbIncRateLinearVal);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      }
      // ELSE the DqDB increment rate mode is exponential
      else {
        if (pCpgcPointTestCfg->DqdbIncRates[Channel] > pCpgcInfo->MaxDqdbIncRateExponentVal) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: DqdbIncRate value (%u) is out of range (0 <= val <= %u).\n",
            pCpgcPointTestCfg->DqdbIncRates[Channel], pCpgcInfo->MaxDqdbIncRateExponentVal);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      } // end else the DqDB increment rate mode is exponential

        // FOR each DqDB unisequencer:
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        if (pCpgcPointTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq] > DqdbUniseqModeMax) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: DqdbUniseqMode value (%u) is out of range (0 <= val < %u).\n",
            pCpgcPointTestCfg->DqdbUniseqModes[Channel], DqdbUniseqModeMax);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }

#if SUPPORT_FOR_TURNAROUNDS
        // IF the Wr2Rd turnaround traffic mode is specified
        if (pCpgcPointTestCfg->TrafficModes[Controller][Channel] == Wr2RdTrafficMode) {
          // only the LFSR unisequencer mode is supported
          if (pCpgcPointTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq] != LfsrDqdbUniseqMode) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: DqdbUniseqMode value (%u) must be %a when TrafficMode=%u.\n",
              pCpgcPointTestCfg->DqdbUniseqModes[Channel], LfsrDqdbUniseqMode, Wr2RdTrafficMode);
#endif
            ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
            return 1;
          }
        } //  end if the Wr2Rd turnaround traffic mode is specified
#endif // SUPPORT_FOR_TURNAROUNDS

          // IF the unisequencer is in LMN mode
        if (pCpgcPointTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq] == LmnDqdbUniseqMode) {
          if (Uniseq != 0) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: Only DqdbUniseq 0 supports LmnDqdbUniseqMode.\n");
#endif
            ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
            return 1;
          }

          if (pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][0] > pCpgcInfo->MaxDqdbUniseqLVal) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: DqdbLmnValue L value (%u) is out of range (0 <= val <= %u).\n",
              pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][0], pCpgcInfo->MaxDqdbUniseqLVal);
#endif
            ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
            return 1;
          }

          if (pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][1] > pCpgcInfo->MaxDqdbUniseqMVal) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: DqdbLmnValue M value (%u) is out of range (0 <= val <= %u).\n",
              pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][1], pCpgcInfo->MaxDqdbUniseqMVal);
#endif
            ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
            return 1;
          }

          if (pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][2] > pCpgcInfo->MaxDqdbUniseqNVal) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: DqdbLmnValue N value (%u) is out of range (0 <= val <= %u).\n",
              pCpgcPointTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][2], pCpgcInfo->MaxDqdbUniseqNVal);
#endif
            ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
            return 1;
          }
        }
        // ELSE the the unisequencer is not in LMN mode
        else {
          if (pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq] > pCpgcInfo->MaxDqdbUniseqSeedVal) {
#if ENBL_VERBOSE_ERROR_MSGS
            NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
              "ERROR: DqdbUniseqSeed value (%u) is out of range (0 <= val <= %u).\n",
              pCpgcPointTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq], pCpgcInfo->MaxDqdbUniseqSeedVal);
#endif
            ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
            return 1;
          }
        } // end else the the unisequencer is not in LMN mode
      } // end for each DqDB unisequencer:

      if (pCpgcPointTestCfg->CadbModes[Controller][Channel] > CadbModeMax) {
#if ENBL_VERBOSE_ERROR_MSGS
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "ERROR: CadbMode value (%u) is out of range (0 <= val < %u).\n",
          pCpgcPointTestCfg->CadbModes[Controller][Channel], CadbModeMax);
#endif
        ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
        return 1;
      }

#if SUPPORT_FOR_TURNAROUNDS
        // IF rank-to-rank turnaround testing is specified
      if (pMemPointTestCfg->RankTestModes[Socket][Controller][Channel] == Turnarounds) {
        // IF the traffic mode is not valid for rank-to-rank turnaround testing
        if ((pCpgcPointTestCfg->TrafficModes[Controller][Channel] != Wr2WrAndRd2RdTrafficMode) &&
          (pCpgcPointTestCfg->TrafficModes[Controller][Channel] != Rd2WrTrafficMode) &&
          (pCpgcPointTestCfg->TrafficModes[Controller][Channel] != Wr2RdTrafficMode) &&
          (pCpgcPointTestCfg->TrafficModes[Controller][Channel] != IdleTrafficMode)) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: TrafficMode value (%u) is out of range for RankCombination=Turnarounds.\n",
            pCpgcPointTestCfg->TrafficModes[Controller][Channel]);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      }
      // ELSE singles testing is specified
      else
#endif // SUPPORT_FOR_TURNAROUNDS
      {
        // IF the traffic mode is not valid for singles testing
        if ((pCpgcPointTestCfg->TrafficModes[Controller][Channel] == Wr2WrAndRd2RdTrafficMode) ||
          (pCpgcPointTestCfg->TrafficModes[Controller][Channel] == Rd2WrTrafficMode) ||
          (pCpgcPointTestCfg->TrafficModes[Controller][Channel] == Wr2RdTrafficMode)) {
#if ENBL_VERBOSE_ERROR_MSGS
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "ERROR: TrafficMode value (%u) is out of range for RankCombination=Singles.\n",
            pCpgcPointTestCfg->TrafficModes[Controller][Channel]);
#endif
          ProcError (SsaServicesHandle, UnsupportedValue, __FILE__, __LINE__, pTestStat);
          return 1;
        }
      } // end else singles testing is specified
    } // end for each channel
  } // end for each controller
} // end for each socket

return 0;
} // end function ChkCpgcPointTestCfg

  /**
  @brief
  This function logs the CPGC information.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
  **/
VOID LogCpgcInfo (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_SERVER_CPGC_INFO *pCpgcInfo
  )
{
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "CPGC Information:\n");
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberDqdbCachelines=%u\n", pCpgcInfo->MaxNumberDqdbCachelines);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbIncRateExponentVal=%u\n", pCpgcInfo->MaxDqdbIncRateExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbIncRateLinearVal=%u\n", pCpgcInfo->MaxDqdbIncRateLinearVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberDqdbUniseqs=%u\n", pCpgcInfo->MaxNumberDqdbUniseqs);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbUniseqSeedVal=0x%X\n", pCpgcInfo->MaxDqdbUniseqSeedVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbUniseqLVal=0x%X\n", pCpgcInfo->MaxDqdbUniseqLVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbUniseqMVal=0x%X\n", pCpgcInfo->MaxDqdbUniseqMVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbUniseqNVal=0x%X\n", pCpgcInfo->MaxDqdbUniseqNVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbUniseqSeedSaveRateVal=0x%X\n", pCpgcInfo->MaxDqdbUniseqSeedSaveRateVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDqdbInvertDcShiftRateExponentVal=0x%X\n", pCpgcInfo->MaxDqdbInvertDcShiftRateExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberCadbRows=%u\n", pCpgcInfo->MaxNumberCadbRows);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberCadbUniseqs=%u\n", pCpgcInfo->MaxNumberCadbUniseqs);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxCadbUniseqSeedVal=0x%X\n", pCpgcInfo->MaxCadbUniseqSeedVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxCadbUniseqLVal=0x%X\n", pCpgcInfo->MaxCadbUniseqLVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxCadbUniseqMVal=0x%X\n", pCpgcInfo->MaxCadbUniseqMVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxCadbUniseqNVal=0x%X\n", pCpgcInfo->MaxCadbUniseqNVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDummyReadLVal=0x%X\n", pCpgcInfo->MaxDummyReadLVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDummyReadMVal=0x%X\n", pCpgcInfo->MaxDummyReadMVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxDummyReadNVal=0x%X\n", pCpgcInfo->MaxDummyReadNVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxStartDelayVal=0x%X\n", pCpgcInfo->MaxStartDelayVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  IsLoopCountExponential=%u\n", pCpgcInfo->IsLoopCountExponential);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxLoopCountVal=0x%X\n", pCpgcInfo->MaxLoopCountVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberSubseqs=%u\n", pCpgcInfo->MaxNumberSubseqs);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxBurstLengthExponentVal=0x%X\n", pCpgcInfo->MaxBurstLengthExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxBurstLengthLinearVal=0x%X\n", pCpgcInfo->MaxBurstLengthLinearVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxInterSubseqWaitVal=0x%X\n", pCpgcInfo->MaxInterSubseqWaitVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxOffsetAddrUpdateRateVal=0x%X\n", pCpgcInfo->MaxOffsetAddrUpdateRateVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxAddrInvertRateVal=0x%X\n", pCpgcInfo->MaxAddrInvertRateVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRankAddrIncRateExponentVal=0x%X\n", pCpgcInfo->MaxRankAddrIncRateExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRankAddrIncRateLinearVal=0x%X\n", pCpgcInfo->MaxRankAddrIncRateLinearVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MinRankAddrIncVal=%d\n", pCpgcInfo->MinRankAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRankAddrIncVal=%d\n", pCpgcInfo->MaxRankAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxBankAddrIncRateExponentVal=0x%X\n", pCpgcInfo->MaxBankAddrIncRateExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxBankAddrIncRateLinearVal=0x%X\n", pCpgcInfo->MaxBankAddrIncRateLinearVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MinBankAddrIncVal=%d\n", pCpgcInfo->MinBankAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxBankAddrIncVal=%d\n", pCpgcInfo->MaxBankAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRowAddrIncRateExponentVal=0x%X\n", pCpgcInfo->MaxRowAddrIncRateExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRowAddrIncRateLinearVal=0x%X\n", pCpgcInfo->MaxRowAddrIncRateLinearVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MinRowAddrIncVal=%d\n", pCpgcInfo->MinRowAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRowAddrIncVal=%d\n", pCpgcInfo->MaxRowAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxColAddrIncRateExponentVal=0x%X\n", pCpgcInfo->MaxColAddrIncRateExponentVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxColAddrIncRateLinearVal=0x%X\n", pCpgcInfo->MaxColAddrIncRateLinearVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MinColAddrIncVal=%d\n", pCpgcInfo->MinColAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxColAddrIncVal=%d\n", pCpgcInfo->MaxColAddrIncVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberRankMapEntries=%u\n", pCpgcInfo->MaxNumberRankMapEntries);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberBankMapEntries=%u\n", pCpgcInfo->MaxNumberBankMapEntries);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberRowAddrSwizzleEntries=%u\n", pCpgcInfo->MaxNumberRowAddrSwizzleEntries);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxRowAddrSwizzleVal=%u\n", pCpgcInfo->MaxRowAddrSwizzleVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxStopOnNthErrorCountVal=0x%X\n", pCpgcInfo->MaxStopOnNthErrorCountVal);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  MaxNumberErrorCounters=%u\n", pCpgcInfo->MaxNumberErrorCounters);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n");
} // end function LogCpgcInfo


  /**
  @brief
  This function checks the given CPGC information values against the
  corresponding preprocessor definition values.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pCpgcInfo          Pointer to CPGC information structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
  **/
UINT8 ChkCpgcInfo (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_SERVER_CPGC_INFO *pCpgcInfo,
  OUT TEST_STATUS *pTestStat)
{
  if ((pCpgcInfo->MaxNumberDqdbUniseqs > MAX_DQDB_UNISEQ_CNT) ||
    (pCpgcInfo->MaxNumberCadbRows > CADB_PATTERN_ROW_CNT) ||
    (pCpgcInfo->MaxNumberCadbUniseqs > MAX_CADB_UNISEQ_CNT)) {
#if ENBL_VERBOSE_ERROR_MSGS
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "ERROR: CpgcInfo value(s) exceed preprocessor value(s).\n");
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "MaxNumberDqdbUniseqs=%u MAX_DQDB_UNISEQ_CNT=%u\n"
      "MaxNumberCadbRows=%u CADB_PATTERN_ROW_CNT=%u\n",
      pCpgcInfo->MaxNumberDqdbUniseqs, MAX_DQDB_UNISEQ_CNT,
      pCpgcInfo->MaxNumberCadbRows, CADB_PATTERN_ROW_CNT);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "MaxNumberCadbUniseqs=%u MAX_CADB_UNISEQ_CNT=%u\n",
      pCpgcInfo->MaxNumberCadbUniseqs, MAX_CADB_UNISEQ_CNT);
#endif
    ProcError (SsaServicesHandle, Failure, __FILE__, __LINE__, pTestStat);
    return 1;
  }

  return 0;
} // end function ChkCpgcInfo

/**
  @brief
  This function updates the CPGC point test ERID setting

  @param[out]  pRunCpgcPointTestCfg   Pointer to RunCpgcPointTest test configuration structure.
  @param[in]   pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

**/
VOID
SetCpgcPointTestCfgErid (
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg,
  IN BOOLEAN EnableErid
  )
{
}

/**
  @brief
  This function get the CPGC point test ERID setting

  @param[in]  pRunCpgcPointTestCfg   Pointer to RunCpgcPointTest test configuration structure.

  @retval TRUE/FALSE - ERID enable or disable
**/
BOOLEAN
GetCpgcPointTestCfgErid (
  IN CONST CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  )
{
  return FALSE;
}
