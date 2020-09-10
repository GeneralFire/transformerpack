/** @file
  This code provides PCH Trace Hub functions for EBG.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Library/DebugLib.h>
#include <Pch/IncludePrivate/PchConfigHob.h>
#include <Private/Library/DxeTraceHubLib.h>
#include <Library/HobLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>


/**
  Get PCH Trace Hub configuration. This version is for EBG. Trace Hub config is in PCH_CONFIG_HOB
  and this HOB for EBG has different definition than coresponding one for CDF and LBG.

  @param[in,out]  PchTraceHubConfig           Pointer to PCH Trace Hub configuration

  @return  Standard EFI_STATUS is returned
**/
EFI_STATUS
PchTraceHubGetConfig (
  IN OUT  TRACE_HUB_INIT_CONFIGS        *PchTraceHubConfig
  )
{
  EFI_HOB_GUID_TYPE *GuidHob;
  PCH_CONFIG_HOB    *PchConfigHob;
  UINT32             TraceHubBufferSizeTab[] = {0, SIZE_1MB, SIZE_8MB, SIZE_64MB, SIZE_128MB, SIZE_256MB, SIZE_512MB, SIZE_1GB};

  GuidHob = GetFirstGuidHob (&gPchConfigHobGuid);
  if (GuidHob == NULL) {
    ASSERT (GuidHob != NULL);
    return EFI_NOT_FOUND;
  }
  PchConfigHob = GET_GUID_HOB_DATA (GuidHob);
  PchTraceHubConfig->EnableMode = PchConfigHob->PchTraceHub.PchTraceHubMode;
  PchTraceHubConfig->MemReg0Size = TraceHubBufferSizeTab[PchConfigHob->PchTraceHub.MemReg0Size];
  PchTraceHubConfig->MemReg1Size = TraceHubBufferSizeTab[PchConfigHob->PchTraceHub.MemReg1Size];
  PchTraceHubConfig->AetEnableMode = (UINT8) PchConfigHob->PchTraceHub.AetEnableMode;

  //
  // Platform Debug Consent should be placed in PCH_TRACEHUB_HOB
  //
  PchTraceHubConfig->PlatformDebugConsent = 0;

  return EFI_SUCCESS;
}
