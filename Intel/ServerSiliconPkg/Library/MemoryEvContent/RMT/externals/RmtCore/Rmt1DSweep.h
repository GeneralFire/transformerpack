/** @file
  Rmt1DSweep.h

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

#ifndef __RMT_1D_SWEEP_H__
#define __RMT_1D_SWEEP_H__

#include <ReferenceCodeDataTypes.h>
#include "ssabios.h"
#include <SysHost.h>
#include "EvContentInternal.h"
#include "RmtCore.h"
#include <Library/MemoryEvContentLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemSweepLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Memory/JedecDefinitions.h>


#ifndef BUFFERS_PER_SUBCH
#define BUFFERS_PER_SUBCH         (MAX_STROBE / 4)
#endif

//
// Define the list of registers need to be saved/modified/restored in the pre/post sweep
//
typedef struct {
  UINT8 DbRwA0;
  UINT8 DbRwA1;
} DB_REGISTERS;

typedef struct {
  COMMON_1D_PARAMETERS   CommonParameters;  // It must be the first variable!
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle;
  RMT_CORE_TEST_DATA    *CoreTestData;
  DB_REGISTERS           DbRegs[MAX_CH][SUB_CH][BUFFERS_PER_SUBCH];
} RMT_MARGIN_1D_PARAMETER;


/*

  Callback function to set up a measurement or test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtSetupTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to execute/Start a measurement or test

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtExecuteTest (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to process measurement or test result

  @param[in,out]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtResultHandler (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to be executed before any sweep starts

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtPre1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );

/*

  Callback function to be executed after any sweep ends

  @param[in]  CommonParamaters  Pointer to the context information

  @retval   MRC_STATUS_SUCCESS      Callback function executed properly

*/
MRC_STATUS
EFIAPI
RmtPost1DSweep (
  IN        COMMON_1D_PARAMETERS    *CommonParameters
  );
#endif //__RMT_1D_SWEEP_H__

