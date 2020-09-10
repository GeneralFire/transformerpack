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

#include "EvContentInternal.h"
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/SsaUtilLib.h>
#include <Library/MemoryCoreLib.h>
#include "Platform.h"
#include "MarginParam.h"

/**
  @brief
  This function udpates the CPGC point test configuration header field with CPGC configuration version

  @param[in, out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

**/
VOID
UpdateCpgcPointTestCfgHeader (
  IN OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  )
{
  pCpgcPointTestCfg->Header.MajorVersion = CPGC2_MAJOR_VERSION;
  pCpgcPointTestCfg->Header.MinorVersion = 0;
}

/**
  @brief
  This function udpates the CPGC point test configuration based on the Margin1D test configuration

  @param[in ]  pMargin1DTestCfg   Pointer to Margin1D test configuration structure.
  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

  @retval status - SUCCESS/FAILURE
**/
UINT32
UpdateCpgcPointTestCfgByMargin1DCfg (
  IN CONST MARGIN_1D_CONFIG *pTestCfg,
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  )
{
  UpdateCpgcPointTestCfgHeader (pCpgcPointTestCfg);

  if (sizeof (pCpgcPointTestCfg->TrafficModes) != sizeof (pTestCfg->TrafficModes) ||
    sizeof (pCpgcPointTestCfg->AddrModes) != sizeof (pTestCfg->AddrModes) ||
    sizeof (pCpgcPointTestCfg->BurstLengths) != sizeof (pTestCfg->BurstLengths) ||
    sizeof (pCpgcPointTestCfg->LoopCounts) != sizeof (pTestCfg->LoopCounts) ||
    sizeof (pCpgcPointTestCfg->BinarySearchLoopCounts) != sizeof (pTestCfg->BinarySearchLoopCounts) ||
    sizeof (pCpgcPointTestCfg->StartDelays) != sizeof (pTestCfg->StartDelays) ||
    sizeof (pCpgcPointTestCfg->InterSubseqWaits) != sizeof (pTestCfg->InterSubseqWaits) ||
    sizeof (pCpgcPointTestCfg->EccDataSource) != sizeof (pTestCfg->EccDataSource) ||
    sizeof (pCpgcPointTestCfg->DcPolarity) != sizeof (pTestCfg->DcPolarity) ||
    sizeof (pCpgcPointTestCfg->DcBitmasks) != sizeof (pTestCfg->DcBitmasks) ||
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths) != sizeof (pTestCfg->UserDefinedDqdbPatternLengths) ||
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatterns) != sizeof (pTestCfg->UserDefinedDqdbPatterns) ||
    sizeof (pCpgcPointTestCfg->DqdbIncRateModes) != sizeof (pTestCfg->DqdbIncRateModes) ||
    sizeof (pCpgcPointTestCfg->DqdbIncRates) != sizeof (pTestCfg->DqdbIncRates) ||
    sizeof (pCpgcPointTestCfg->DqdbLaneRotationRates) != sizeof (pTestCfg->DqdbLaneRotationRates) ||
    sizeof (pCpgcPointTestCfg->DqdbUniseqModes) != sizeof (pTestCfg->DqdbUniseqModes) ||
    sizeof (pCpgcPointTestCfg->DqdbUniseqSeeds) != sizeof (pTestCfg->DqdbUniseqSeeds) ||
    sizeof (pCpgcPointTestCfg->DqdbLmnValues) != sizeof (pTestCfg->DqdbLmnValues) ||
    sizeof (pCpgcPointTestCfg->CadbModes) != sizeof (pTestCfg->CadbModes)) {
    return FAILURE;
  }

  pCpgcPointTestCfg->CpgcStopMode = pTestCfg->CpgcStopMode;
  CopyMem (&pCpgcPointTestCfg->TrafficModes,
    &pTestCfg->TrafficModes,
    sizeof (pCpgcPointTestCfg->TrafficModes));
  CopyMem (&pCpgcPointTestCfg->AddrModes,
    &pTestCfg->AddrModes,
    sizeof (pCpgcPointTestCfg->AddrModes));
  CopyMem (&pCpgcPointTestCfg->BurstLengths,
    &pTestCfg->BurstLengths,
    sizeof (pCpgcPointTestCfg->BurstLengths));
  CopyMem (&pCpgcPointTestCfg->LoopCounts,
    &pTestCfg->LoopCounts,
    sizeof (pCpgcPointTestCfg->LoopCounts));
  CopyMem (&pCpgcPointTestCfg->BinarySearchLoopCounts,
    &pTestCfg->BinarySearchLoopCounts,
    sizeof (pCpgcPointTestCfg->BinarySearchLoopCounts));
  CopyMem (&pCpgcPointTestCfg->StartDelays,
    &pTestCfg->StartDelays,
    sizeof (pCpgcPointTestCfg->StartDelays));
  CopyMem (&pCpgcPointTestCfg->InterSubseqWaits,
    &pTestCfg->InterSubseqWaits,
    sizeof (pCpgcPointTestCfg->InterSubseqWaits));
  CopyMem (&pCpgcPointTestCfg->EccDataSource,
    &pTestCfg->EccDataSource,
    sizeof (pCpgcPointTestCfg->EccDataSource));
  CopyMem (&pCpgcPointTestCfg->DcPolarity,
    &pTestCfg->DcPolarity,
    sizeof (pCpgcPointTestCfg->DcPolarity));
  CopyMem (&pCpgcPointTestCfg->DcBitmasks,
    &pTestCfg->DcBitmasks,
    sizeof (pCpgcPointTestCfg->DcBitmasks));
  pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns = \
    pTestCfg->EnableUserDefinedDqdbPatterns;
  CopyMem (&pCpgcPointTestCfg->UserDefinedDqdbPatternLengths,
    &pTestCfg->UserDefinedDqdbPatternLengths,
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths));
  CopyMem (&pCpgcPointTestCfg->UserDefinedDqdbPatterns,
    &pTestCfg->UserDefinedDqdbPatterns,
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatterns));
  CopyMem (&pCpgcPointTestCfg->DqdbIncRateModes,
    &pTestCfg->DqdbIncRateModes,
    sizeof (pCpgcPointTestCfg->DqdbIncRateModes));
  CopyMem (&pCpgcPointTestCfg->DqdbIncRates,
    &pTestCfg->DqdbIncRates,
    sizeof (pCpgcPointTestCfg->DqdbIncRates));
  CopyMem (&pCpgcPointTestCfg->DqdbLaneRotationRates,
    &pTestCfg->DqdbLaneRotationRates,
    sizeof (pCpgcPointTestCfg->DqdbLaneRotationRates));
  CopyMem (&pCpgcPointTestCfg->DqdbUniseqModes,
    &pTestCfg->DqdbUniseqModes,
    sizeof (pCpgcPointTestCfg->DqdbUniseqModes));
  CopyMem (&pCpgcPointTestCfg->DqdbUniseqSeeds,
    &pTestCfg->DqdbUniseqSeeds,
    sizeof (pCpgcPointTestCfg->DqdbUniseqSeeds));
  CopyMem (&pCpgcPointTestCfg->DqdbLmnValues,
    &pTestCfg->DqdbLmnValues,
    sizeof (pCpgcPointTestCfg->DqdbLmnValues));
  CopyMem (&pCpgcPointTestCfg->CadbModes,
    &pTestCfg->CadbModes,
    sizeof (pCpgcPointTestCfg->CadbModes));

  return SUCCESS;
}

/**
  @brief
  This function udpates the CPGC point test configuration based on the Margin2D test configuration

  @param[in ]  pMargin2DTestCfg   Pointer to Margin2D test configuration structure.
  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

  @retval status - SUCCESS/FAILURE
**/
UINT32
UpdateCpgcPointTestCfgByMargin2DCfg (
  IN CONST MARGIN_2D_CONFIG *pTestCfg,
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
  )
{
  UpdateCpgcPointTestCfgHeader (pCpgcPointTestCfg);

  if (sizeof (pCpgcPointTestCfg->TrafficModes) != sizeof (pTestCfg->TrafficModes) ||
    sizeof (pCpgcPointTestCfg->AddrModes) != sizeof (pTestCfg->AddrModes) ||
    sizeof (pCpgcPointTestCfg->BurstLengths) != sizeof (pTestCfg->BurstLengths) ||
    sizeof (pCpgcPointTestCfg->LoopCounts) != sizeof (pTestCfg->LoopCounts) ||
    sizeof (pCpgcPointTestCfg->BinarySearchLoopCounts) != sizeof (pTestCfg->BinarySearchLoopCounts) ||
    sizeof (pCpgcPointTestCfg->InterSubseqWaits) != sizeof (pTestCfg->InterSubseqWaits) ||
    sizeof (pCpgcPointTestCfg->EccDataSource) != sizeof (pTestCfg->EccDataSource) ||
    sizeof (pCpgcPointTestCfg->DcPolarity) != sizeof (pTestCfg->DcPolarity) ||
    sizeof (pCpgcPointTestCfg->DcBitmasks) != sizeof (pTestCfg->DcBitmasks) ||
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths) != sizeof (pTestCfg->UserDefinedDqdbPatternLengths) ||
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatterns) != sizeof (pTestCfg->UserDefinedDqdbPatterns) ||
    sizeof (pCpgcPointTestCfg->DqdbIncRateModes) != sizeof (pTestCfg->DqdbIncRateModes) ||
    sizeof (pCpgcPointTestCfg->DqdbIncRates) != sizeof (pTestCfg->DqdbIncRates) ||
    sizeof (pCpgcPointTestCfg->DqdbLaneRotationRates) != sizeof (pTestCfg->DqdbLaneRotationRates) ||
    sizeof (pCpgcPointTestCfg->DqdbUniseqModes) != sizeof (pTestCfg->DqdbUniseqModes) ||
    sizeof (pCpgcPointTestCfg->DqdbUniseqSeeds) != sizeof (pTestCfg->DqdbUniseqSeeds) ||
    sizeof (pCpgcPointTestCfg->DqdbLmnValues) != sizeof (pTestCfg->DqdbLmnValues) ||
    sizeof (pCpgcPointTestCfg->CadbModes) != sizeof (pTestCfg->CadbModes)) {
    return FAILURE;
  }

  pCpgcPointTestCfg->CpgcStopMode = pTestCfg->CpgcStopMode;
  CopyMem (&pCpgcPointTestCfg->TrafficModes, &pTestCfg->TrafficModes, sizeof (pCpgcPointTestCfg->TrafficModes));
  CopyMem (&pCpgcPointTestCfg->AddrModes, &pTestCfg->AddrModes, sizeof (pCpgcPointTestCfg->AddrModes));
  CopyMem (&pCpgcPointTestCfg->BurstLengths, &pTestCfg->BurstLengths, sizeof (pCpgcPointTestCfg->BurstLengths));
  CopyMem (&pCpgcPointTestCfg->LoopCounts, &pTestCfg->LoopCounts, sizeof (pCpgcPointTestCfg->LoopCounts));
  CopyMem (&pCpgcPointTestCfg->BinarySearchLoopCounts, &pTestCfg->BinarySearchLoopCounts, sizeof (pCpgcPointTestCfg->BinarySearchLoopCounts));
  CopyMem (&pCpgcPointTestCfg->InterSubseqWaits, &pTestCfg->InterSubseqWaits, sizeof (pCpgcPointTestCfg->InterSubseqWaits));
  CopyMem (&pCpgcPointTestCfg->EccDataSource, &pTestCfg->EccDataSource, sizeof (pCpgcPointTestCfg->EccDataSource));
  CopyMem (&pCpgcPointTestCfg->DcPolarity, &pTestCfg->DcPolarity, sizeof (pCpgcPointTestCfg->DcPolarity));
  CopyMem (&pCpgcPointTestCfg->DcBitmasks, &pTestCfg->DcBitmasks, sizeof (pCpgcPointTestCfg->DcBitmasks));
  pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns = pTestCfg->EnableUserDefinedDqdbPatterns;
  CopyMem (&pCpgcPointTestCfg->UserDefinedDqdbPatternLengths, &pTestCfg->UserDefinedDqdbPatternLengths, sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths));
  CopyMem (&pCpgcPointTestCfg->UserDefinedDqdbPatterns, &pTestCfg->UserDefinedDqdbPatterns, sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatterns));
  CopyMem (&pCpgcPointTestCfg->DqdbIncRateModes, &pTestCfg->DqdbIncRateModes, sizeof (pCpgcPointTestCfg->DqdbIncRateModes));
  CopyMem (&pCpgcPointTestCfg->DqdbIncRates, &pTestCfg->DqdbIncRates, sizeof (pCpgcPointTestCfg->DqdbIncRates));
  CopyMem (&pCpgcPointTestCfg->DqdbLaneRotationRates, &pTestCfg->DqdbLaneRotationRates, sizeof (pCpgcPointTestCfg->DqdbLaneRotationRates));
  CopyMem (&pCpgcPointTestCfg->DqdbUniseqModes, &pTestCfg->DqdbUniseqModes, sizeof (pCpgcPointTestCfg->DqdbUniseqModes));
  CopyMem (&pCpgcPointTestCfg->DqdbUniseqSeeds, &pTestCfg->DqdbUniseqSeeds, sizeof (pCpgcPointTestCfg->DqdbUniseqSeeds));
  CopyMem (&pCpgcPointTestCfg->DqdbLmnValues, &pTestCfg->DqdbLmnValues, sizeof (pCpgcPointTestCfg->DqdbLmnValues));
  CopyMem (&pCpgcPointTestCfg->CadbModes, &pTestCfg->CadbModes, sizeof (pCpgcPointTestCfg->CadbModes));

  // make sure that the target channel's traffic mode is loopback
  pCpgcPointTestCfg->TrafficModes[pTestCfg->TargetController][pTestCfg->TargetChannel] = LoopbackTrafficMode;

  return SUCCESS;
}

/**
  @brief
  This function updates the CPGC point test configuration based on the RunCpgcPointTest test configuration

  @param[in ]  pRunCpgcPointTestCfg   Pointer to RunCpgcPointTest test configuration structure.
  @param[out]  pCpgcPointTestCfg  Pointer to CPGC point test configuration structure.

  @retval status - SUCCESS/FAILURE
**/
UINT32
UpdateCpgcPointTestCfgByRunCpgcPoinTestCfg (
  IN CONST RUN_CPGC_POINT_TEST_CONFIG *pTestCfg,
  OUT CPGC_POINT_TEST_CONFIG *pCpgcPointTestCfg
)
{
  UpdateCpgcPointTestCfgHeader (pCpgcPointTestCfg);

  if (sizeof (pCpgcPointTestCfg->TrafficModes) != sizeof (pTestCfg->TrafficModes) ||
    sizeof (pCpgcPointTestCfg->AddrModes) != sizeof (pTestCfg->AddrModes) ||
    sizeof (pCpgcPointTestCfg->BurstLengths) != sizeof (pTestCfg->BurstLengths) ||
    sizeof (pCpgcPointTestCfg->LoopCounts) != sizeof (pTestCfg->LoopCounts) ||
    sizeof (pCpgcPointTestCfg->BinarySearchLoopCounts) != sizeof (pTestCfg->LoopCounts) ||
    sizeof (pCpgcPointTestCfg->StartDelays) != sizeof (pTestCfg->StartDelays) ||
    sizeof (pCpgcPointTestCfg->InterSubseqWaits) != sizeof (pTestCfg->InterSubseqWaits) ||
    sizeof (pCpgcPointTestCfg->EccDataSource) != sizeof (pTestCfg->EccDataSource) ||
    sizeof (pCpgcPointTestCfg->DcPolarity) != sizeof (pTestCfg->DcPolarity) ||
    sizeof (pCpgcPointTestCfg->DcBitmasks) != sizeof (pTestCfg->DcBitmasks) ||
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths) != sizeof (pTestCfg->UserDefinedDqdbPatternLengths) ||
    sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatterns) != sizeof (pTestCfg->UserDefinedDqdbPatterns) ||
    sizeof (pCpgcPointTestCfg->DqdbIncRateModes) != sizeof (pTestCfg->DqdbIncRateModes) ||
    sizeof (pCpgcPointTestCfg->DqdbIncRates) != sizeof (pTestCfg->DqdbIncRates) ||
    sizeof (pCpgcPointTestCfg->DqdbLaneRotationRates) != sizeof (pTestCfg->DqdbLaneRotationRates) ||
    sizeof (pCpgcPointTestCfg->DqdbUniseqModes) != sizeof (pTestCfg->DqdbUniseqModes) ||
    sizeof (pCpgcPointTestCfg->DqdbUniseqSeeds) != sizeof (pTestCfg->DqdbUniseqSeeds) ||
    sizeof (pCpgcPointTestCfg->DqdbLmnValues) != sizeof (pTestCfg->DqdbLmnValues) ||
    sizeof (pCpgcPointTestCfg->CadbModes) != sizeof (pTestCfg->CadbModes)) {
    return FAILURE;
  }

  pCpgcPointTestCfg->CpgcStopMode = pTestCfg->CpgcStopMode;
  CopyMem (&pCpgcPointTestCfg->TrafficModes, &pTestCfg->TrafficModes, sizeof (pCpgcPointTestCfg->TrafficModes));
  CopyMem (&pCpgcPointTestCfg->AddrModes, &pTestCfg->AddrModes, sizeof (pCpgcPointTestCfg->AddrModes));
  CopyMem (&pCpgcPointTestCfg->BurstLengths, &pTestCfg->BurstLengths, sizeof (pCpgcPointTestCfg->BurstLengths));
  CopyMem (&pCpgcPointTestCfg->LoopCounts, &pTestCfg->LoopCounts, sizeof (pCpgcPointTestCfg->LoopCounts));
  CopyMem (&pCpgcPointTestCfg->BinarySearchLoopCounts, &pTestCfg->LoopCounts, sizeof (pCpgcPointTestCfg->LoopCounts));
  CopyMem (&pCpgcPointTestCfg->StartDelays, &pTestCfg->StartDelays, sizeof (pCpgcPointTestCfg->StartDelays));
  CopyMem (&pCpgcPointTestCfg->InterSubseqWaits, &pTestCfg->InterSubseqWaits, sizeof (pCpgcPointTestCfg->InterSubseqWaits));
  CopyMem (&pCpgcPointTestCfg->EccDataSource, &pTestCfg->EccDataSource, sizeof (pCpgcPointTestCfg->EccDataSource));
  CopyMem (&pCpgcPointTestCfg->DcPolarity, &pTestCfg->DcPolarity, sizeof (pCpgcPointTestCfg->DcPolarity));
  CopyMem (&pCpgcPointTestCfg->DcBitmasks, &pTestCfg->DcBitmasks, sizeof (pCpgcPointTestCfg->DcBitmasks));
  pCpgcPointTestCfg->EnableUserDefinedDqdbPatterns = pTestCfg->EnableUserDefinedDqdbPatterns;
  CopyMem (&pCpgcPointTestCfg->UserDefinedDqdbPatternLengths, &pTestCfg->UserDefinedDqdbPatternLengths, sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatternLengths));
  CopyMem (&pCpgcPointTestCfg->UserDefinedDqdbPatterns, &pTestCfg->UserDefinedDqdbPatterns, sizeof (pCpgcPointTestCfg->UserDefinedDqdbPatterns));
  CopyMem (&pCpgcPointTestCfg->DqdbIncRateModes, &pTestCfg->DqdbIncRateModes, sizeof (pCpgcPointTestCfg->DqdbIncRateModes));
  CopyMem (&pCpgcPointTestCfg->DqdbIncRates, &pTestCfg->DqdbIncRates, sizeof (pCpgcPointTestCfg->DqdbIncRates));
  CopyMem (&pCpgcPointTestCfg->DqdbLaneRotationRates, &pTestCfg->DqdbLaneRotationRates, sizeof (pCpgcPointTestCfg->DqdbLaneRotationRates));
  CopyMem (&pCpgcPointTestCfg->DqdbUniseqModes, &pTestCfg->DqdbUniseqModes, sizeof (pCpgcPointTestCfg->DqdbUniseqModes));
  CopyMem (&pCpgcPointTestCfg->DqdbUniseqSeeds, &pTestCfg->DqdbUniseqSeeds, sizeof (pCpgcPointTestCfg->DqdbUniseqSeeds));
  CopyMem (&pCpgcPointTestCfg->DqdbLmnValues, &pTestCfg->DqdbLmnValues, sizeof (pCpgcPointTestCfg->DqdbLmnValues));
  CopyMem (&pCpgcPointTestCfg->CadbModes, &pTestCfg->CadbModes, sizeof (pCpgcPointTestCfg->CadbModes));

  return SUCCESS;
}


/**
  @brief
  This function logs the test configuration.

  @param[in]  SsaServicesHandle  Pointer to SSA services.
  @param[in]  pTestCfg      Pointer to test configureation structure.
**/
VOID 
LogMargin1DTestCfg (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_1D_CONFIG *pTestCfg
  )
{
  UINT8 Controller, Channel, Dimm, Uniseq;
  UINT8 Cacheline, MaxCacheline;
  UINT8 Chunk, MaxChunk;
  UINT8 PatternSize;

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "Margin1D Test Configuration:\n");
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  IoLevel=%u\n",
    pTestCfg->IoLevel);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  MarginGroup=%u\n",
    pTestCfg->MarginGroup);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ShmooDirection=%u\n",
    pTestCfg->ShmooDirection);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  StepSize=%u\n",
    pTestCfg->StepSize);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  GuardBandSize=%u\n",
    pTestCfg->GuardBandSize);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  LaneShmooStopMode=%u\n",
    pTestCfg->LaneShmooStopMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  RankShmooStopMode=%u\n",
    pTestCfg->RankShmooStopMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ChannelShmooStopMode=%u\n",
    pTestCfg->ChannelShmooStopMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ControllerShmooStopMode=%u\n",
    pTestCfg->ControllerShmooStopMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  SocketShmooStopMode=%u\n",
    pTestCfg->SocketShmooStopMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  EnableOptimizedShmoo=%u\n",
    pTestCfg->EnableOptimizedShmoo);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  SocketBitmask=0x%X\n",
    pTestCfg->SocketBitmask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ControllerBitmask=0x%X\n",
    pTestCfg->ControllerBitmask);

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Controller=%u:\n", Controller);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      ChannelBitmask=0x%X\n",
      pTestCfg->ChannelBitmasks[Controller]);

    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      Channel=%u:\n", Channel);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        RankTestMode=%u\n",
        pTestCfg->RankTestModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        DimmABitmask=0x%X\n",
        pTestCfg->DimmABitmasks[Controller][Channel]);

      // FOR each DIMM:
      for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        Dimm=%u:\n", Dimm);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          RankABitmask=0x%X\n",
          pTestCfg->RankABitmasks[Controller][Channel][Dimm]);
      } // end for each DIMM

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        DimmBBitmask=0x%X\n",
        pTestCfg->DimmBBitmasks[Controller][Channel]);

      // FOR each DIMM:
      for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        Dimm=%u:\n", Dimm);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          RankBBitmask=0x%X\n",
          pTestCfg->RankBBitmasks[Controller][Channel][Dimm]);
      } // end for each DIMM

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        LaneValBitmask=0x");
      LogByteArrayAsHex (
        pTestCfg->LaneValBitmasks[Controller][Channel],
        MAX_BUS_WIDTH / 8);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        ChunkValBitmask=0x%X\n",
        pTestCfg->ChunkValBitmasks[Controller][Channel]);
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  CpgcStopMode=%u\n",
    pTestCfg->CpgcStopMode);

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Controller=%u:\n", Controller);
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      Channel=%u:\n", Channel);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        TrafficMode=%u\n",
        pTestCfg->TrafficModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        AddrMode=%u\n",
        pTestCfg->AddrModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        BurstLength=%u\n",
        pTestCfg->BurstLengths[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        LoopCount=%u\n"
        , pTestCfg->LoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        BinarySearchLoopCount=%u\n",
        pTestCfg->BinarySearchLoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        StartDelay=%u\n",
        pTestCfg->StartDelays[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        InterSubseqWait=%u\n",
        pTestCfg->InterSubseqWaits[Controller][Channel]);
    } // end for each channel
  } // end for each controller

    // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Channel=%u:\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      EccDataSource=%u\n",
      pTestCfg->EccDataSource[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DcPolarity=%u\n",
      pTestCfg->DcPolarity[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DcBitmasks: ");
    LogByteArrayAsHex (
      pTestCfg->DcBitmasks[Channel], MAX_BUS_WIDTH / 8);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
  } // end for each channel

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  EnableUserDefinedDqdbPatterns=%u\n",
    pTestCfg->EnableUserDefinedDqdbPatterns);

  PatternSize = sizeof (pTestCfg->UserDefinedDqdbPatterns[0][0][0]);
  MaxChunk = sizeof (pTestCfg->UserDefinedDqdbPatterns[0][0]) / \
    PatternSize;
  MaxCacheline = sizeof (pTestCfg->UserDefinedDqdbPatterns[0]) / \
    sizeof (pTestCfg->UserDefinedDqdbPatterns[0][0]);

  // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Channel=%u:\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      UserDefinedDqdbPatternLength=%u\n",
      pTestCfg->UserDefinedDqdbPatternLengths[Channel]);

    for (Cacheline = 0; Cacheline < MaxCacheline; Cacheline++) {
      // FOR each chunk:
      for (Chunk = 0; Chunk < MaxChunk; Chunk++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      UserDefinedDqdbPattern[%u][%u]=0x",
          Cacheline, Chunk);
        LogByteArrayAsHex (
          pTestCfg->UserDefinedDqdbPatterns[Channel][Cacheline][Chunk],
          PatternSize);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
      } // end for each chunk
    } // end for each cacheline

    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DqdbIncRateMode=%u\n",
      pTestCfg->DqdbIncRateModes[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DqdbIncRate=%u\n",
      pTestCfg->DqdbIncRates[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DqdbLaneRotationRate=%u\n",
      pTestCfg->DqdbLaneRotationRates[Channel]);
  } // end for each channel

    // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Controller=%u:\n", Controller);
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      Channel=%u:\n", Channel);

      // FOR each DqDB unisequencer:
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        Uniseq=%u:\n", Uniseq);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          DqdbUniseqMode=%u\n",
          pTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq]);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          DqdbUniseqSeed=0x%X\n",
          pTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);

        if (Uniseq == 0) {
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          DqdbLmnValue=%u %u %u\n",
            pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][0],
            pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][1],
            pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][2]);
        }
      } // end for each DqDB unisequencer

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        CadbMode=%u\n",
        pTestCfg->CadbModes[Controller][Channel]);
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ScramblerOverrideMode=%u\n",
    pTestCfg->ScramblerOverrideMode);
#if SUPPORT_FOR_EXTENDED_RANGE
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  EnableExtendedRange=%u\n",
    pTestCfg->EnableExtendedRange);
#endif // SUPPORT_FOR_EXTENDED_RANGE
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  EnableMarginPlot=%u\n",
    pTestCfg->EnableMarginPlot);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TestStatusLogLevel=%u\n",
    pTestCfg->TestStatusLogLevel);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
} // end function LogMargin1DTestCfg


/**
  @brief
  This function logs the test configuration.

  @param[in]  SsaServicesHandle  Pointer to SSA services.
  @param[in]  pTestCfg           Pointer to Margin2D test configuration structure.
**/
VOID
LogMargin2DTestCfg (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MARGIN_2D_CONFIG *pTestCfg
  )
{
  UINT8 Controller, Channel, Dimm, Uniseq;
  UINT8 Cacheline, MaxCacheline;
  UINT8 Chunk, MaxChunk;
  UINT8 PatternSize;

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "Margin2D Test Configuration:\n");
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  IoLevel=%u\n",
    pTestCfg->IoLevel);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  SlowMarginGroup=%u\n",
    pTestCfg->SlowMarginGroup);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  FastMarginGroup=%u\n",
    pTestCfg->FastMarginGroup);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  SlowStepSize=%u\n",
    pTestCfg->SlowStepSize);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  FastStepSize=%u\n",
    pTestCfg->FastStepSize);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  GuardBandSize=%u\n",
    pTestCfg->GuardBandSize);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetSocket=%u\n",
    pTestCfg->TargetSocket);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetController=%u\n",
    pTestCfg->TargetController);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetChannel=%u\n",
    pTestCfg->TargetChannel);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetDimmA=%u\n",
    pTestCfg->TargetDimmA);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetRankA=%u\n",
    pTestCfg->TargetRankA);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetDimmB=%u\n",
    pTestCfg->TargetDimmB);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TargetRankB=%u\n",
    pTestCfg->TargetRankB);

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  LaneValBitmask=0x");
  LogByteArrayAsHex (
    pTestCfg->LaneValBitmasks,
    MAX_BUS_WIDTH / 8);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ChunkValBitmask=0x%X\n",
    pTestCfg->ChunkValBitmask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  SocketBitmask=0x%X\n",
    pTestCfg->SocketBitmask);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ControllerBitmask=0x%X\n",
    pTestCfg->ControllerBitmask);

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Controller=%u:\n", Controller);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      ChannelBitmask=0x%X\n",
      pTestCfg->ChannelBitmasks[Controller]);

    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      Channel=%u:\n", Channel);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        RankTestMode=%u\n",
        pTestCfg->RankTestModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        DimmABitmask=0x%X\n",
        pTestCfg->DimmABitmasks[Controller][Channel]);

      // FOR each DIMM:
      for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        Dimm=%u:\n", Dimm);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          RankABitmask=0x%X\n",
          pTestCfg->RankABitmasks[Controller][Channel][Dimm]);
      } // end for each DIMM

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        DimmBBitmask=0x%X\n",
        pTestCfg->DimmBBitmasks[Controller][Channel]);

      // FOR each DIMM:
      for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        Dimm=%u:\n", Dimm);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          RankBBitmask=0x%X\n",
          pTestCfg->RankBBitmasks[Controller][Channel][Dimm]);
      } // end for each DIMM
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  CpgcStopMode=%u\n",
    pTestCfg->CpgcStopMode);

  // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Controller=%u:\n", Controller);
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      Channel=%u:\n", Channel);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        TrafficMode=%u\n",
        pTestCfg->TrafficModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        AddrMode=%u\n",
        pTestCfg->AddrModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        BurstLength=%u\n",
        pTestCfg->BurstLengths[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        LoopCount=%u\n"
        , pTestCfg->LoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        BinarySearchLoopCount=%u\n",
        pTestCfg->BinarySearchLoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        InterSubseqWait=%u\n",
        pTestCfg->InterSubseqWaits[Controller][Channel]);
    } // end for each channel
  } // end for each controller

    // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Channel=%u:\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      EccDataSource=%u\n",
      pTestCfg->EccDataSource[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DcPolarity=%u\n",
      pTestCfg->DcPolarity[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DcBitmasks: ");
    LogByteArrayAsHex (
      pTestCfg->DcBitmasks[Channel], MAX_BUS_WIDTH / 8);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
  } // end for each channel

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  EnableUserDefinedDqdbPatterns=%u\n",
    pTestCfg->EnableUserDefinedDqdbPatterns);

  PatternSize = sizeof (pTestCfg->UserDefinedDqdbPatterns[0][0][0]);
  MaxChunk = sizeof (pTestCfg->UserDefinedDqdbPatterns[0][0]) / \
    PatternSize;
  MaxCacheline = sizeof (pTestCfg->UserDefinedDqdbPatterns[0]) / \
    sizeof (pTestCfg->UserDefinedDqdbPatterns[0][0]);

  // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Channel=%u:\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      UserDefinedDqdbPatternLength=%u\n",
      pTestCfg->UserDefinedDqdbPatternLengths[Channel]);

    for (Cacheline = 0; Cacheline < MaxCacheline; Cacheline++) {
      // FOR each chunk:
      for (Chunk = 0; Chunk < MaxChunk; Chunk++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      UserDefinedDqdbPattern[%u][%u]=0x",
          Cacheline, Chunk);
        LogByteArrayAsHex (
          pTestCfg->UserDefinedDqdbPatterns[Channel][Cacheline][Chunk],
          PatternSize);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
      } // end for each chunk
    } // end for each cacheline

    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DqdbIncRateMode=%u\n",
      pTestCfg->DqdbIncRateModes[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DqdbIncRate=%u\n",
      pTestCfg->DqdbIncRates[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      DqdbLaneRotationRate=%u\n",
      pTestCfg->DqdbLaneRotationRates[Channel]);
  } // end for each channel

    // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "    Controller=%u:\n", Controller);
    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "      Channel=%u:\n", Channel);

      // FOR each DqDB unisequencer:
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        Uniseq=%u:\n", Uniseq);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          DqdbUniseqMode=%u\n",
          pTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq]);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          DqdbUniseqSeed=0x%X\n",
          pTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);

        if (Uniseq == 0) {
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "          DqdbLmnValue=%u %u %u\n",
            pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][0],
            pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][1],
            pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][2]);
        }
      } // end for each DqDB unisequencer

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "        CadbMode=%u\n",
        pTestCfg->CadbModes[Controller][Channel]);
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  ScramblerOverrideMode=%u\n",
    pTestCfg->ScramblerOverrideMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  EnableMarginPlot=%u\n",
    pTestCfg->EnableMarginPlot);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "  TestStatusLogLevel=%u\n",
    pTestCfg->TestStatusLogLevel);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
} // end function LogMargin2DTestCfg

/**
  @brief
  This function is used to log the CPGC point test configuration.

  @param[in]       SsaServicesHandle  Pointer to SSA services.
  @param[in]       pTestCfg           Pointer to test configuration structure.
**/
VOID LogRunCpgcTestCfg (
  IN SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST RUN_CPGC_POINT_TEST_CONFIG *pTestCfg
  )
{
  UINT8 Socket, Controller, Channel, Dimm, Cacheline, Chunk, Uniseq, Idx;

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "Test Configuration:\n");
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  RankIterationIndex=0x%X\n", pTestCfg->RankIterationIndex);

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  SocketBitmask=0x%X\n", pTestCfg->SocketBitmask);

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    Socket=%u\n", Socket);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "      ControllerBitmask=0x%X\n", pTestCfg->ControllerBitmasks[Socket]);

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "        Controller=%u\n", Controller);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "          ChannelBitmask=0x%X\n", pTestCfg->ChannelBitmasks[Socket][Controller]);

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "            Channel=%u\n", Channel);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "              RankTestMode=%u\n",
          pTestCfg->RankTestModes[Controller][Channel]);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "              DimmABitmask=0x%X\n",
          pTestCfg->DimmABitmasks[Controller][Channel]);

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "                Dimm=%u\n", Dimm);
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "                  RankABitmask=0x%X\n",
            pTestCfg->RankABitmasks[Socket][Controller][Channel][Dimm]);
        } // end for each DIMM

        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "              DimmBBitmask=0x%X\n",
          pTestCfg->DimmBBitmasks[Controller][Channel]);

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "                Dimm=%u\n", Dimm);
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "                  RankBBitmask=0x%X\n",
            pTestCfg->RankBBitmasks[Socket][Controller][Channel][Dimm]);
        } // end for each DIMM
      } // end for each channel
    } // end for each controller
  } // end for each socket

    // FOR each controller:
  for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    Controller=%u\n", Controller);

    // FOR each channel:
    for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "        Channel=%u\n", Channel);

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "          LaneValBitmask=0x");
      LogByteArrayAsHex (
        pTestCfg->LaneValBitmasks[Controller][Channel],
        MAX_BUS_WIDTH / 8);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");

        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "          ChunkValBitmask=0x%02X\n",
          pTestCfg->ChunkValBitmasks[Controller][Channel]);
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  CpgcStopMode=%u\n", pTestCfg->CpgcStopMode);

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
        pTestCfg->TrafficModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      AddrMode=%u\n",
        pTestCfg->AddrModes[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      BurstLength=%u\n",
        pTestCfg->BurstLengths[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      LoopCount=%u\n",
        pTestCfg->LoopCounts[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      StartDelay=%u\n",
        pTestCfg->StartDelays[Controller][Channel]);
      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      InterSubseqWait=%u\n",
        pTestCfg->InterSubseqWaits[Controller][Channel]);
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  EnableUserDefinedDqdbPatterns=%u\n", pTestCfg->EnableUserDefinedDqdbPatterns);

  // FOR each channel:
  for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "  Channel=%u\n", Channel);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    UserDefinedDqdbPatternLength=%u\n", pTestCfg->UserDefinedDqdbPatternLengths[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "    UserDefinedDqdbPattern:\n");

    for (Cacheline = 0; Cacheline < NUMBER_USER_DEFINED_DQDB_CACHELINES_CPGC2; Cacheline++) {
      for (Chunk = 0; Chunk < CHUNKS_PER_CACHELINE; Chunk++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "      Cacheline=%u Chunk=%d 0x", Cacheline, Chunk);
        LogByteArrayAsHex (
          pTestCfg->UserDefinedDqdbPatterns[Channel][Cacheline][Chunk],
          NUMBER_BYTE_PER_DQDB_CHUNK_CPGC2);
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, "\n");
      } // end for each chunk
    } // end for each cacheline

    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "      DqdbIncRateMode=%u\n",
      pTestCfg->DqdbIncRateModes[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
      "      DqdbIncRate=%u\n",
      pTestCfg->DqdbIncRates[Channel]);
    NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR, 
      "      DqdbLaneRotationRate=%u\n",
      pTestCfg->DqdbLaneRotationRates[Channel]);
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
          Uniseq, pTestCfg->DqdbUniseqModes[Controller][Channel][Uniseq]);
      }
      for (Uniseq = 0; Uniseq < MAX_DQDB_UNISEQ_CNT; Uniseq++) {
        NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
          "      DqdbUniseqSeeds[%d]=0x%X\n",
          Uniseq, pTestCfg->DqdbUniseqSeeds[Controller][Channel][Uniseq]);
      }
      for (Uniseq = 0; Uniseq < MAX_DQDB_LMN_UNISEQ_CNT; Uniseq++) {
        for (Idx = 0; Idx < MAX_DQDB_LMN_VALUES_CNT; Idx++) {
          NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
            "      DqdbLmnValues[%d][%d]=%u\n",
            Uniseq, Idx, pTestCfg->DqdbLmnValues[Controller][Channel][Uniseq][Idx]);
        }
      }

      NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
        "      CadbMode=%u\n",
        pTestCfg->CadbModes[Controller][Channel]);
    } // end for each channel
  } // end for each controller

  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  ScramblerOverrideMode=%u\n", pTestCfg->ScramblerOverrideMode);
  NORMAL_LOG (SsaServicesHandle, SSA_D_ERROR,
    "  TestStatusLogLevel=%u\n", pTestCfg->TestStatusLogLevel);
} // end function LogRunCpgcTestCfg
