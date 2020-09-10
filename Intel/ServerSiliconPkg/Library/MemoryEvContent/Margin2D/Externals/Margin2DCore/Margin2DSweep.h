/** @file
  Margin2DSweep.h

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

#ifndef __MARGIN_2D_SWEEP_H__
#define __MARGIN_2D_SWEEP_H__

#include <ReferenceCodeDataTypes.h>
#include "ssabios.h"
#include <SysHost.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemSweepLib.h>

#include "EvContentInternal.h"
#include "Margin2DCore.h"
#include <Library/MemoryEvContentLib.h>

typedef struct {
  COMMON_1D_PARAMETERS   InnerCommonParameters;  // It must be the first variable!
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle;
  MARGIN2D_CORE_DATA    *CoreTestData;
  INT16                  OuterOffset;
  INT16                  InnerLeft;
  INT16                  InnerRight;
} MARGIN_2D_INNER_PARAMETER;

typedef struct {
  COMMON_1D_PARAMETERS   OuterCommonParameters;  // It must be the first variable!
  COMMON_1D_PARAMETERS   InnerCommonParameters;
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle;
  MARGIN2D_CORE_DATA    *CoreTestData;
  INT16                  OuterOffset;
  INT16                  InnerLeft;
  INT16                  InnerRight;
} MARGIN_2D_PARAMETER;

/*

  Callback function to be executed before any sweep starts

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DPre1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to be executed after any sweep is done

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DPost1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to process measurement or test result

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Margin2DResultHandler (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to set up a measurement or test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Inner1DSetupTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to execute/Start a measurement or test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
Inner1DExecuteTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

#endif //__MARGIN_2D_SWEEP_H__

