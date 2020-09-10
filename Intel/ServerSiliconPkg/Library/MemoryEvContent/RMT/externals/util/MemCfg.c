/** @file
  MemCfg.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2018 Intel Corporation. <BR>

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

#include "MemCfg.h"
#include "ChkRetStat.h"
#include <Library/BaseMemoryLib.h>

// flag to enable BIOS-SSA debug messages
#define ENBL_BIOS_SSA_API_DEBUG_MSGS (0)

/**
  This function fills in the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[out]      pMemCfg            Pointer to memory configuration structure.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8 GetMemCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  OUT MEM_CFG *pMemCfg,
  OUT TEST_STATUS *pTestStat)
{
  UINT8 Socket, Controller, Channel, Dimm;
  MRC_DIMM_INFO DimmInfo;

  ZeroMem (pMemCfg, sizeof (*pMemCfg));

  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not populated THEN skip it
    if ((pSystemInfo->SocketBitMask & (1 << Socket)) == 0) {
      continue;
    }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "GetControllerBitMask(Socket=%u)\n", Socket);
#endif
    SsaServicesHandle->SsaMemoryConfig->GetControllerBitMask(SsaServicesHandle,
      Socket, &pMemCfg->ControllerBitmasks[Socket]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  ControllerBitmask=0x%X\n", pMemCfg->ControllerBitmasks[Socket]);
#endif
    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not populated THEN skip it
      if ((pMemCfg->ControllerBitmasks[Socket] & (1 << Controller)) == 0) {
        continue;
      }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "  GetChannelBitMask(Socket=%u Controller=%u)\n",
        Socket, Controller);
#endif
      SsaServicesHandle->SsaMemoryConfig->GetChannelBitMask(SsaServicesHandle,
        Socket, Controller, &pMemCfg->ChannelBitmasks[Socket][Controller]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "    ChannelBitmask=0x%X\n",
        pMemCfg->ChannelBitmasks[Socket][Controller]);
#endif
      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not populated THEN skip it
        if ((pMemCfg->ChannelBitmasks[Socket][Controller] & (1 << Channel)) == 0) {
          continue;
        }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "    GetDimmBitMask(Socket=%u Controller=%u Channel=%u)\n",
          Socket, Controller, Channel);
#endif
        SsaServicesHandle->SsaMemoryConfig->GetDimmBitMask(
          SsaServicesHandle, Socket, Controller, Channel,
          &pMemCfg->DimmBitmasks[Socket][Controller][Channel]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "      DimmBitmask=0x%X\n",
          pMemCfg->DimmBitmasks[Socket][Controller][Channel]);
#endif
        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not populated THEN skip it
          if ((pMemCfg->DimmBitmasks[Socket][Controller][Channel] &
            (1 << Dimm)) == 0) {
            continue;
          }

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "      GetRankInDimm(Socket=%u Controller=%u Channel=%u Dimm=%u)\n",
            Socket, Controller, Channel, Dimm);
#endif
          SsaServicesHandle->SsaMemoryConfig->GetRankInDimm(
            SsaServicesHandle, Socket, Controller, Channel, Dimm,
            &pMemCfg->RankCounts[Socket][Controller][Channel][Dimm]);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "        RankCount=%u\n",
            pMemCfg->RankCounts[Socket][Controller][Channel][Dimm]);
#endif

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "      GetDimmInfo(Socket=%u Controller=%u Channel=%u Dimm=%u)\n",
            Socket, Controller, Channel, Dimm);
#endif
          SsaServicesHandle->SsaMemoryConfig->GetDimmInfo(
            SsaServicesHandle, Socket, Controller, Channel, Dimm,
            &DimmInfo);

#if ENBL_BIOS_SSA_API_DEBUG_MSGS
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "        EccSupport=%u DimmCapacity=%u BankCount=%u RowSize=%u ColumnSize=%u\n",
            DimmInfo.EccSupport, DimmInfo.DimmCapacity, DimmInfo.BankCount,
            DimmInfo.RowSize, DimmInfo.ColumnSize);
          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "        DeviceWidth=%u MemoryTech=%u MemoryPackage=%u\n",
            DimmInfo.DeviceWidth, DimmInfo.MemoryTech, DimmInfo.MemoryPackage);
#endif
          pMemCfg->MemoryTech[Socket][Controller][Channel][Dimm] = DimmInfo.MemoryTech;

          if ((DimmInfo.MemoryPackage == LrDimmMemoryPackage) && \
            (DimmInfo.MemoryTech != SsaMemoryDdrT)) {
            pMemCfg->IsLrDimms = TRUE;
          }
        } // end for each DIMM
      } // end for each channel
    } // end for each controller
  } // end for each socket

  return 0;
} // end function GetMemCfg

/**
  This function logs the contents of the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
**/
VOID LogMemCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN MEM_CFG *pMemCfg)
{
  UINT8 Socket, Controller, Channel, Dimm;

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n"
    "Memory Configuration:\n");
  // FOR each socket:
  for (Socket = 0; Socket < MAX_SOCKET_CNT; Socket++) {
    // IF this socket is not populated THEN skip it
    if ((pSystemInfo->SocketBitMask & (1 << Socket)) == 0) {
      continue;
    }

    NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
      "  Socket=%u\n", Socket);

    // FOR each controller:
    for (Controller = 0; Controller < MAX_CONTROLLER_CNT; Controller++) {
      // IF this controller is not populated THEN skip it
      if ((pMemCfg->ControllerBitmasks[Socket] & (1 << Controller)) == 0) {
        continue;
      }

      NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
        "    Controller=%u\n", Controller);

      // FOR each channel:
      for (Channel = 0; Channel < MAX_CHANNEL_CNT; Channel++) {
        // IF this channel is not populated THEN skip it
        if ((pMemCfg->ChannelBitmasks[Socket][Controller] &
          (1 << Channel)) == 0) {
          continue;
        }

        NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
          "      Channel=%u\n", Channel);

        // FOR each DIMM:
        for (Dimm = 0; Dimm < MAX_DIMM_CNT; Dimm++) {
          // IF this DIMM is not populated THEN skip it
          if ((pMemCfg->DimmBitmasks[Socket][Controller][Channel] &
            (1 << Dimm)) == 0) {
            continue;
          }

          NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
            "        Dimm=%u RankCount=%u MemoryTech=0x%X\n", Dimm,
            pMemCfg->RankCounts[Socket][Controller][Channel][Dimm],
            pMemCfg->MemoryTech[Socket][Controller][Channel][Dimm]);
        } // end for each DIMM
      } // end for each channel
    } // end for each controller
  } // end for each socket

  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "  IsLrDimms=%u\n", pMemCfg->IsLrDimms);
  NORMAL_LOG(SsaServicesHandle, SSA_D_ERROR,
    "\n");
} // end function LogMemCfg

// end file MemCfg.c
