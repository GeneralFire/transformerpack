/** @file
  MarginParam.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#ifndef __MARGIN_PARAM_H__
#define __MARGIN_PARAM_H__

#include "ssabios.h"

#include "MemCfg.h"
#include "MemPointTest.h"

/**
  This function is used to determine if the given margin parameter requires
  incremental stepping.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   Incremental stepping is required.
  @retval  FALSE  Incremental stepping is not required.
**/
BOOLEAN IsIncrementalSteppingRequired(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup);

/**
  This function is used to determine if the given margin parameter requires
  an I/O reset after an error is detected.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   I/O reset is required.
  @retval  FALSE  I/O reset is not required.
**/
BOOLEAN IsIoResetAfterErrRequired(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup);

/**
  This function is used to determine if the given margin parameter requires
  a JEDEC initialization after an error is detected.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pSystemInfo        Pointer to system information structure.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       pMemPointTestCfg   Pointer to generic memory point test configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   Incremental stepping is required.
  @retval  FALSE  Incremental stepping is not required.
**/
BOOLEAN IsJedecInitAfterErrRequired(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MRC_SYSTEM_INFO *pSystemInfo,
  IN CONST MEM_CFG *pMemCfg,
  IN CONST MEM_POINT_TEST_CONFIG *pMemPointTestCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup);

/**
  This function is used to get strobe width for the given margin parameter and
  DIMM.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       pMemCfg            Pointer to memory configuration structure.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       Socket             Zero-based Socket.
  @param[in]       Controller         Zero-based Controller.
  @param[in]       Channel            Zero-based Channel.
  @param[in]       Dimm               Zero-based DIMM.

  @retval  Strobe width.
**/
UINT8 GetMarginParamStrobeWidth(
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN CONST MEM_CFG *pMemCfg,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN UINT8 Socket,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN UINT8 Dimm);

/**
  This function is used to determine whether the given margin group is for
  command/address signals.

  @param[in]  MarginGroup  Margin parameter group.

  @retval  TRUE   The margin group is Cmd group.
  @retval  FALSE  The margin group is not Cmd group.
**/
BOOLEAN
IsCmdMarginGroup (
  IN MRC_GT MarginGroup
  );

/**
  This function is used to determine whether the given margin group is for
  control signals.

  @param[in]  MarginGroup  Margin parameter group.

  @retval  TRUE   The margin group is Ctl group.
  @retval  FALSE  The margin group is not Ctl group.
**/
BOOLEAN
IsCtlMarginGroup (
  IN MRC_GT MarginGroup
  );

/**
  This function is used to determine if the given margin group is specific to
  DDR-T.

  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   The margin group is specific to DDR-T.
  @retval  FALSE  The margin group is not specific to DDR-T.
**/
BOOLEAN 
IsMarginGroupDdrTSpecific (
  IN MRC_GT MarginGroup
  );


/**
This function is used to determine if the given margin group is for
Early-Read-ID.

@param[in]       MarginGroup        Margin group.

@retval  TRUE   The margin group is for Early-Read-ID.
@retval  FALSE  The margin group is not for Early-Read-ID.
**/
BOOLEAN
IsMarginGroupForErid (
  IN MRC_GT MarginGroup
  );

/**
  @brief
  This function gets the given margin parameter's specificity.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       IoLevel            I/O level.
  @param[in]       MarginGroup        Margin group.
  @param[in]       LineNumber         Line number for error reporting.
  @param[out]      pSpecificity       Pointer to where margin parameter specificity will be placed.
  @param[out]      pTestStat          Pointer to test status in results metadata structure.

  @retval  0  success
  @retval  1  error occurred
**/
UINT8
GetMarginParamSpecificity (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN MRC_LT IoLevel,
  IN MRC_GT MarginGroup,
  IN UINT32 LineNumber,
  OUT MARGIN_PARAM_SPECIFICITY *pSpecificity,
  OUT TEST_STATUS *pTestStat
);

#endif // __MARGIN_PARAM_H__

// end file MarginParam.h
