/** @file
  PEI PSMI policy update library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>

#include <Library/BaseMemoryLib.h>
#include <Library/PsmiPolicyLib.h>
#include <Library/SetupLib.h>
#include <Library/DebugLib.h>

/**
  Update the PSMI policy from Setup.

  @param[in]  SetupData         A pointer to the setup data.

  @retval None

**/

VOID
GetPlatformSetupValueforPsmi (
// APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.
  IN INTEL_SETUP_DATA  *SetupData
// APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.
  )
{
  struct psmiSetup  *PsmiPolicy;
  UINT8             Socket;

  PsmiPolicy = GetPsmiPolicy ();
  ASSERT (PsmiPolicy != NULL);
  if (PsmiPolicy == NULL) {
    return;
  }

  PsmiPolicy->GlobalPsmiEnable = SetupData->SocketConfig.SocketProcessorCoreConfiguration.GlobalPsmiEnable;

  for (Socket = 0; Socket<MAX_SOCKET; Socket++) {
    PsmiPolicy->PsmiTrace[Socket] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTrace[Socket];
    PsmiPolicy->PsmiHandlerSize[Socket] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiHandlerSize[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceRegion[0] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceRegion0[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[0] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceBufferSizeRegion0[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[0] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceMemTypeRegion0[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceRegion[1] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceRegion1[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[1] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceBufferSizeRegion1[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[1] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceMemTypeRegion1[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceRegion[2] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceRegion2[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[2] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceBufferSizeRegion2[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[2] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceMemTypeRegion2[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceRegion[3] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceRegion3[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[3] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceBufferSizeRegion3[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[3] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceMemTypeRegion3[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceRegion[4] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceRegion4[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceBufferSizeRegion[4] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceBufferSizeRegion4[Socket];
    PsmiPolicy->trace[Socket].PsmiTraceMemTypeRegion[4] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.PsmiTraceMemTypeRegion4[Socket];
  }

} // GetPlatformSetupValueforPsmi

/**
  Update PEI PSMI policy.

  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @return Update status.

**/

EFI_STATUS
EFIAPI
UpdatePeiPsmiPolicy (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT    INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  )
{
  GetPlatformSetupValueforPsmi (SetupData);

  return EFI_SUCCESS;

} // UpdatePeiPsmiPolicy

