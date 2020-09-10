/** @file
  PEI RAS RC policy PPI update library class implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
**/

#include <PiPei.h>
#include <Library/RasRcPolicyPpiUpdateLib.h>  // *this* library class
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SetupLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Guid/SocketMemoryVariable.h>
#include <Guid/PartialMirrorGuid.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Memory/MemDefaults.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>

EFI_STATUS
GetAddressBasedMirrorData (
  IN OUT    RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  );

/**
  Update the RAS RC policy PPI.

  Modify the RAS RC policy PPI to set platform related and customized settings.
  The silicon's safe defaults are expected to have been set prior to calling
  this function. This function must be called before installing the PPI.

  @param[in, out] RasRcPolicyPpi  A pointer to the RAS RC policy PPI to set.
  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @retval EFI_SUCCESS             The policy has been updated as necessary.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
**/
EFI_STATUS
EFIAPI
UpdateRasRcPolicyPpi (
  IN OUT    RAS_RC_POLICY_PPI   *RasRcPolicyPpi,
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve redefinition error
  IN OUT    INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve redefinition error
  )
{
  EFI_STATUS                    Status;
  SOCKET_COMMONRC_CONFIGURATION CommonRcConfig;
  SOCKET_MEMORY_CONFIGURATION   MemoryConfig;
  BOOLEAN                       Is14nmCpu;

  ASSERT (RasRcPolicyPpi != NULL);
  if (RasRcPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetSpecificConfigGuid (
    &gEfiSocketCommonRcVariableGuid,
    (VOID *) &CommonRcConfig
    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetSpecificConfigGuid (
    &gEfiSocketMemoryVariableGuid,
    (VOID *) &MemoryConfig
    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  RasRcPolicyPpi->RasModes &= ~(CH_MLS);
  if (Is14nmCpu) {
    if (MemoryConfig.RankSparing == SPARING_ENABLED) {
      RasRcPolicyPpi->RasModes |= RK_SPARE;
    } else if (CommonRcConfig.MirrorMode == MIRROR_1LM_ENABLED) {
      RasRcPolicyPpi->RasModes |= FULL_MIRROR_1LM;
    } else if (CommonRcConfig.MirrorMode == MIRROR_2LM_ENABLED) {
      RasRcPolicyPpi->RasModes |= FULL_MIRROR_2LM;
    }
  } else {
    if (CommonRcConfig.MirrorMode == MIRROR_1LM_ENABLED) {
      RasRcPolicyPpi->RasModes |= FULL_MIRROR_1LM;
    } else if (CommonRcConfig.MirrorMode == MIRROR_2LM_ENABLED) {
      RasRcPolicyPpi->RasModes |= FULL_MIRROR_2LM;
    }
  }

  //
  // Demand Scrub Disable shall not be used according to b311409
  //
  RasRcPolicyPpi->RasModesEx |= DMNDSCRB_EN;

  if (MemoryConfig.PatrolScrub == PATROL_SCRUB_DIS){
    RasRcPolicyPpi->RasModesEx &= ~PTRLSCRB_EN;
  } else {
    RasRcPolicyPpi->RasModesEx |= PTRLSCRB_EN;
    if (MemoryConfig.PatrolScrub == PATROL_SCRUB_EOP) {
      RasRcPolicyPpi->RasModesEx |= PTRLSCRB_EOP_EN;
    }
  }

  if ((MemoryConfig.PlusOneEn) && !(IsCpuAndRevision (CPU_ICXSP, REV_ALL))) {
    RasRcPolicyPpi->RasModesEx |= SDDC_EN;
  } else {
    RasRcPolicyPpi->RasModesEx &= ~SDDC_EN;
  }

  if (MemoryConfig.ADDDCEn) {
    RasRcPolicyPpi->RasModesEx |= ADDDC_EN;
  } else {
    RasRcPolicyPpi->RasModesEx &= ~ADDDC_EN;
  }

  if (MemoryConfig.AdddcErrInjEn) {
    RasRcPolicyPpi->RasModesEx |= ADDDC_ERR_INJ_EN;
  } else {
    RasRcPolicyPpi->RasModesEx &= ~ADDDC_ERR_INJ_EN;
  }

  if (!Is14nmCpu) {
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) ||
      IsCpuAndRevision (CPU_SNR, REV_ALL) || IsHbmSku()) {
      if (MemoryConfig.PclsEn) {
        RasRcPolicyPpi->RasModesEx |= PCLS_EN;
      }
      else {
        RasRcPolicyPpi->RasModesEx &= ~PCLS_EN;
      }
    }

  }


  //
  // Initialize some fields from the address based mirror variable. If this
  // fails for any reason, use the setup data for partial mirror instead.
  //
  Status = GetAddressBasedMirrorData (RasRcPolicyPpi);
  if (EFI_ERROR (Status)) {
    if (CommonRcConfig.MirrorMode == PMIRROR_1LM_ENABLED) {
      RasRcPolicyPpi->RasModes |= PARTIAL_MIRROR_1LM;
    }

    if (CommonRcConfig.MirrorMode == PMIRROR_2LM_ENABLED) {
      RasRcPolicyPpi->RasModes |= PARTIAL_MIRROR_2LM;
    }

    if (MemoryConfig.partialmirrorsad0 == PMIRROR_ENABLED) {
      RasRcPolicyPpi->RasModes |= PARTIAL_MIRROR_1LM;
    }

    if (MemoryConfig.PartialMirrorUefi == PMIRROR_ENABLED) {
      RasRcPolicyPpi->RasModes |= PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM;
    }
  }

  RasRcPolicyPpi->McBankWarmBootClearError = (SetupData->SystemConfig.McBankWarmBootClearError) ? TRUE : FALSE;
  RasRcPolicyPpi->PoisonEn = SetupData->SystemConfig.PoisonEn;
  RasRcPolicyPpi->PfdEn = SetupData->SystemConfig.PfdEn;

  RasRcPolicyPpi->CrashLogFeature = SetupData->SystemConfig.CrashLogFeature;
  RasRcPolicyPpi->CrashLogOnAllReset = SetupData->SystemConfig.CrashLogOnAllReset;
  RasRcPolicyPpi->CrashLogClear = SetupData->SystemConfig.CrashLogClear;
  RasRcPolicyPpi->CrashLogReArm = SetupData->SystemConfig.CrashLogReArm;

  return EFI_SUCCESS;
} // UpdateRasRcPolicyPpi

/**
  Get address based mirror data.

  This function is NOT part of the library class API. The address based mirror
  feature uses an independent EFI varialble. Update the policy fields based on
  the contents of the variable, if it exists.

  @param[in, out] RasRcPolicyPpi  A pointer to the RAS RC policy PPI to set.

  @retval EFI_SUCCESS             The policy has been updated as necessary.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
  @retval EFI_NOT_FOUND           The variable does not exist.
**/
EFI_STATUS
GetAddressBasedMirrorData (
  IN OUT    RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  )
{
  EFI_STATUS                          Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI     *VariablePpi;
  UINTN                               VariableSize;
  ADDRESS_RANGE_MIRROR_VARIABLE_DATA  AddressBasedMirrorData;

  ASSERT (RasRcPolicyPpi != NULL);
  if (RasRcPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate the variable PPI.
  //
  Status = PeiServicesLocatePpi (
    &gEfiPeiReadOnlyVariable2PpiGuid,
    0,
    NULL,
    &VariablePpi
    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the variable. This is expected to fail often, since the variable does
  // not always exist.
  //
  VariableSize = sizeof (ADDRESS_RANGE_MIRROR_VARIABLE_DATA);
  Status = VariablePpi->GetVariable (
    VariablePpi,
    ADDRESS_RANGE_MIRROR_VARIABLE_REQUEST,
    &gAddressBasedMirrorGuid,
    NULL,
    &VariableSize,
    &AddressBasedMirrorData
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (AddressBasedMirrorData.MirrorVersion == 1) {
    RasRcPolicyPpi->RasModes |= PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM;
  }

  return EFI_SUCCESS;
} // GetAddressBasedMirrorData
