/** @file
  Margin1DSweep.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __MARGIN_1D_SWEEP_H__
#define __MARGIN_1D_SWEEP_H__

#include <ReferenceCodeDataTypes.h>
#include "ssabios.h"
#include <SysHost.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemSweepLib.h>

#include "EvContentInternal.h"
#include "Margin1DCore.h"
#include <Library/MemoryEvContentLib.h>

typedef struct {
  COMMON_1D_PARAMETERS   CommonParameters;  // It must be the first variable!
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle;
  CORE_TEST_DATA        *CoreTestData;
} MARGIN_1D_PARAMETER;

/*

  Callback function to be executed before any sweep is done for most advanced training algorithms

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DPre1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to be executed after any sweep is done for most advanced training algorithms

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DPost1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to set up CPGC test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DSetupTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to start CPGC test

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DExecuteTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );


/*

  Callback function to interpret the CPGC test result and update sweep state.

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin1DResultHandler (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );


/**
  This function is used to save the Margin1D test result with 1D sweep in CA training mode.

  @param[in]       SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       ShmooDir             Margin direction.
  @param[in]       Socket               Processor socket within the system (0-based)
  @param[in]       ChannelMask          One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]       DimmRank             Structure to specify current dimm and rank under test (0-based)
  @param[in]       SweepResults         Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH][Signal]

  @retval  N/A
**/
VOID
SaveMargin1DResultInCaTrainingMode (
  IN     SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     SHMOO_DIRECTION        ShmooDir,
  IN     UINT8                  Socket,
  IN     UINT32                 ChannelBitMask,
  IN     MRC_RT                 DimmRank[MAX_CH],
  IN     struct baseMargin      (*SweepResults)[MAX_CH][SUB_CH][RCD_CA_SIGNALS_TO_SWEEP]
  );

/**
  This function is used to save the Margin1D test result with 1D sweep in CS training mode.

  @param[in]       SsaServicesHandle    Pointer to SSA services.
  @param[in, out]  pCoreTestData        Pointer to RMT core test data.
  @param[in]       ShmooDir             Margin direction.
  @param[in]       Socket               Processor socket within the system (0-based)
  @param[in]       ChannelMask          One-hot encoding to mask a channel (0: executes if enabled/populated, 1: skip/masked)
  @param[in]       DimmRank             Structure to specify current dimm and rank under test (0-based)
  @param[in]       SweepResults         Pointer to margin results data structure which is indexed as [MAX_CH][SUB_CH]

  @retval  N/A
**/
VOID
SaveMargin1DResultInCsTrainingMode (
  IN     SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN OUT CORE_TEST_DATA        *pCoreTestData,
  IN     SHMOO_DIRECTION        ShmooDir,
  IN     UINT8                  Socket,
  IN     UINT32                 ChannelBitMask,
  IN     MRC_RT                 DimmRank[MAX_CH],
  IN     struct baseMargin      (*SweepResults)[MAX_CH][SUB_CH]
  );

#endif //__MARGIN_1D_SWEEP_H__

