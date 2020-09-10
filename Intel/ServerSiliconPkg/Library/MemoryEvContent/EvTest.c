/** @file
  EvTest.c

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


#include <Library/MemoryEvContentLib.h>
#include <EvItpDownloadAgent.h>

/**
  This function calls the EV test content.

  @param[in, out]  SsaServicesHandle  Pointer to SSA services.
  @param[in]       TestType           EV test type: 0 - RMT, 1 - Margin1D, 2 - Margin2, 3 - PointTest, 4 - SetMarginParam
  @param[in]       Config             Pointer to test configuration structure.

  @retval EFI_SUCCESS
          EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
RunEvTest (
  IN OUT SSA_BIOS_SERVICES_PPI *SsaServicesHandle,
  IN UINT8 TestType,
  IN VOID *Config
  )
{
  EFI_STATUS RetVal;

  switch (TestType)
  {
  case BSSA_RMT:
    RetVal = RmtTestEntryPoint (SsaServicesHandle, (RMT_CONFIG *)Config);
    break;
  case BSSA_MARGIN_1D:
    RetVal = Margin1DTestEntryPoint (SsaServicesHandle, (MARGIN_1D_CONFIG *)Config);
    break;
  case BSSA_MARGIN_2D:
    RetVal = Margin2DTestEntryPoint (SsaServicesHandle, (MARGIN_2D_CONFIG *)Config);
    break;
  case BSSA_POINT_TEST:
    RetVal = RunCpgcPointTestEntryPoint (SsaServicesHandle, (RUN_CPGC_POINT_TEST_CONFIG *)Config);
    break;
  case BSSA_SET_MARGIN_PARAM:
    RetVal = SetMarginParamTestEntryPoint (SsaServicesHandle, (SET_MARGIN_PARAM_CONFIG *)Config);
    break;
  default:
    RetVal = EFI_UNSUPPORTED;
    break;
  }

  return RetVal;
}
