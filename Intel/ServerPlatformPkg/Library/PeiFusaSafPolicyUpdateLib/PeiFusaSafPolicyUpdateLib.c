/**@file
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

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <Library/SetupLib.h>
#include <Library/DebugLib.h>
#include <Library/Stl2BiosLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/FusaSafPolicyLib.h>
#include <Library/SecurityPolicyDefinitions.h>

/**

  Update policy from Stl2Bios variable or setup in case when variable is not accessible.

  @param[in]  SetupData         A pointer to the setup data.

  @retval EFI_SUCCESS          The FuSa (SAF) policy set correct.
  @retval EFI_NOT_FOUND        The FuSa (SAF) HOB is not found.
  @retval EFI_NOT_FOUND        The PPI descriptor is not found in the database.
  @retval EFI_UNSUPPORTED      The Saf is not supported by BIOS

**/
EFI_STATUS
GetPlatformSetupValueForFusaSaf (
  //APTIOV_SERVER_OVERRIDE_RC_START : Match Intel Setup Data definition 
  IN OUT INTEL_SETUP_DATA  *SetupData
  //APTIOV_SERVER_OVERRIDE_RC_END : Match Intel Setup Data definition 
  )
{
  FUSA_SAF_POLICY                   *FusaSafPolicy               = NULL;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *PpiVariableServices         = NULL;
  EFI_STATUS                        Status                       = EFI_SUCCESS;
  STL_2_BIOS_FUSA_SAF_DATA_VARIABLE Stl2BiosFusaSafVariableData;
  UINTN                             Stl2BiosFusaSafVariableSize  = sizeof (Stl2BiosFusaSafVariableData);

  //
  // Restore FuSa (SAF) HOB.
  // In case when it's not exist function create HOB with default values and return pointer
  //
  FusaSafPolicy = GetFusaSafPolicy ();
  if (FusaSafPolicy == NULL) {
    Status = EFI_NOT_FOUND;
    goto Error_GetPlatformSetupValueForFusaSaf;
  }

  if (SetupData->SocketConfig.SocketProcessorCoreConfiguration.SafSupport != SECURITY_POLICY_ENABLE) {
    // Continue to update policy only when SAF Support requested by the user.
    Status = EFI_UNSUPPORTED;
    goto Error_GetPlatformSetupValueForFusaSaf;
  }

  // Get the Variable Services PPI
  Status = PeiServicesLocatePpi (
    &gEfiPeiReadOnlyVariable2PpiGuid,
    0,
    NULL,
    (VOID **)&PpiVariableServices
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a Error: Unable to locate Variable Services PPI.\n", __FUNCTION__));
    goto Error_GetPlatformSetupValueForFusaSaf;
  }
  // try read Stl2Bios variable
  Status = PpiVariableServices->GetVariable (
    PpiVariableServices,
    STL_2_BIOS_FUSA_SAF_DATA_VARIABLE_NAME,
    &gStl2BiosFusaSafUefiVariableGuid,
    NULL,
    &Stl2BiosFusaSafVariableSize,
    &Stl2BiosFusaSafVariableData
    );

  if (Status == EFI_SUCCESS) {
    // update policy from STL created UEFI variable
    FusaSafPolicy->SafSupport = SetupData->SocketConfig.SocketProcessorCoreConfiguration.SafSupport;
    FusaSafPolicy->EnableSaf = (UINT8)(Stl2BiosFusaSafVariableData.Bits.Enable);
    FusaSafPolicy->SafImgLoadAddr = Stl2BiosFusaSafVariableData.SafImagePathAddr;
  }
Error_GetPlatformSetupValueForFusaSaf:
  return Status;
}

/**

  Update PEI FuSa (SAF) policy.

  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @retval EFI_SUCCESS           The  FuSa (SAF) policy set correct.
  @retval EFI_NOT_FOUND         Guid is not found.
  @retval EFI_NOT_FOUND         The PPI descriptor is not found in the database.
  @retval EFI_DEVICE_ERROR      Data can't be committed.
  @retval EFI_INVALID_PARAMETER Guid is NULL.

**/
EFI_STATUS
EFIAPI
UpdatePeiFusaSafPolicy (
//APTIOV_SERVER_OVERRIDE_RC_START : Match Intel Setup Data definition 
  IN OUT    INTEL_SETUP_DATA          *SetupData
//APTIOV_SERVER_OVERRIDE_RC_END : Match Intel Setup Data definition 
  )
{
  EFI_STATUS       Status = EFI_SUCCESS;

  Status = GetPlatformSetupValueForFusaSaf (SetupData);

  return Status;
}
