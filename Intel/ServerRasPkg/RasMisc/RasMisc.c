/** @file
  Implementation of poison, viral, and SMBus error handlers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/ProcessorRasLib.h>
#include <Library/ViralPoisonLib.h>
#include <Library/CpuAndRevisionLib.h>
#include "SmbusRecovery.h"
#include "ShutdownSuppression.h"

RAS_POLICY                *mPolicyData;

/**
  The callback function to handle subsequent SMIs. This callback will be called by SmmCoreDispatcher.

  @param[in] DispatchHandle             Not used
  @param[in] DispatchContext            Not used
  @param[in, out] CommBuffer            Not used
  @param[in, out] CommBufferSize        Not used

  @retval EFI_SUCCESS                   Function successfully completed
**/
EFI_STATUS
EFIAPI
RasMiscHandler (
  IN EFI_HANDLE             DispatchHandle,
  IN CONST VOID            *DispatchContext,
  IN OUT VOID              *CommBuffer,
  IN OUT UINTN             *CommBufferSize
  )
{
  if (mPolicyData->ViralEn && mPolicyData->PoisonEn) {
    ProcessKtiFatalAndViralError ();
  }

  if (mPolicyData->SmbusErrorRecovery) {
    SmbusRecoverySmiHandler ();
  }

  return EFI_SUCCESS;
}

/**
  This function will enable/disable system viral and poison based on setup policy.

  @param[in] Protocol        Protocol unique ID.
  @param[in] Interface       Interface instance.
  @param[in] Handle          The handle on which the interface is installed..

  @retval    Status.
**/
EFI_STATUS
EFIAPI
SystemViralAndPoisonEnableCallBack (
  IN      CONST EFI_GUID   *Protocol,
  IN      VOID             *Interface,
  IN      EFI_HANDLE        Handle
  )
{
  POISON_VIRAL_FLAG         Flag;

  Flag = DisablePoisonViral;
  if (mPolicyData->PoisonEn) {
    Flag = EnablePoisonOnly;
    if (mPolicyData->ViralEn) {
      Flag = EnablePoisonViral;
    }
  }

  SetViralPoison (Flag);

  return EFI_SUCCESS;
}

/**
  Entry point for the RAS Misc driver.

  This function initializes, and installs RAS Misc error handlers.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.
**/
EFI_STATUS
EFIAPI
InitializeRasMisc (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                ImageHandle2;
  VOID                     *Registration;

  mPolicyData = GetRasPolicy ();

  if (mPolicyData->ViralEn || mPolicyData->SmbusErrorRecovery) {
    ImageHandle2 = NULL;
    Status = gSmst->SmiHandlerRegister (
             RasMiscHandler,
             NULL,
             &ImageHandle2);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "Error Registering RasMiscHandler, Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  //
  // Poison/viral enable/disable is at this driver, but only viral handler is
  // at this driver.
  //
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[RasMisc] EnableSystemViralAndPoison!\n"));

  EnableSystemViralAndPoison (
    mPolicyData->ViralEn,
    mPolicyData->PoisonEn,
    mPolicyData->ClearViralStatus,
    mPolicyData->IioPolicy.IoMcaEn
    );

  //
  // Register one callback to set poison and viral feature for the whole system
  // For both SmmExitBootServices and SmmLegacyBoot notification
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEdkiiSmmExitBootServicesProtocolGuid,
                    SystemViralAndPoisonEnableCallBack,
                    &Registration
                    );

  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEdkiiSmmLegacyBootProtocolGuid,
                    SystemViralAndPoisonEnableCallBack,
                    &Registration
                    );

  if (mPolicyData->SmbusErrorRecovery) {
    EnableSmbusRecovery ();
  }

  if ((mPolicyData->ShutdownSuppression != 0) && (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD , REV_ALL) || IsCpuAndRevision (CPU_SPRSP , REV_ALL))) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[RasMisc] ConfigureShutdownSuppression!\n"));
    ConfigureShutdownSuppression (mPolicyData->ShutdownSuppression);
  }
  return  EFI_SUCCESS;
}
