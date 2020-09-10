/** @file
  MemCfg.h

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

#ifndef __MEM_CFG_H__
#define __MEM_CFG_H__

#include "ssabios.h"
#include "ChkRetStat.h"
#include "Platform.h"

// memory configuration
typedef struct {
  // Note that the bitmask of the sockets populated is available in the MRC_SYSTEM_INFO structure
  UINT8 ControllerBitmasks[MAX_SOCKET_CNT]; ///< bitmasks for controllers populated
  UINT8 ChannelBitmasks[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT]; ///< bitmasks for channels populated
  UINT8 DimmBitmasks[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT]; ///< bitmasks of DIMMs populated
  UINT8 RankCounts[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT][MAX_DIMM_CNT]; ///< number or ranks
  // Note that the following field uses UINT8 instead of MEMORY_TECHNOLOGY_TYPES to conserve space.
  UINT8 MemoryTech[MAX_SOCKET_CNT][MAX_CONTROLLER_CNT][MAX_CHANNEL_CNT][MAX_DIMM_CNT]; ///< memory technology type
  BOOLEAN IsLrDimms; ///< flag indicating whether the system contains LRDIMMs (excludes DDR-T devices)
} MEM_CFG;

/**
  This function gets the memory configuration and stores it in the given structure.

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
  OUT TEST_STATUS *pTestStat);

/**
  This function logs the contents of the given memory configuration structure.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
**/
VOID LogMemCfg(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN MEM_CFG *pMemCfg);

#endif  // __MEM_CFG_H__

// end file MemCfg.h
