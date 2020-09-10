/** @file
  This file is SampleCode of the library for Intel PCH PEI Debug Policy initialization.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#include <Ppi/SiPolicy.h>
#include <PiPei.h>
#include <PchSetupVariableCdfEbg.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PchInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>


/**
  Update Smbus Debug Pre Mem Policy.

  @param[in] SiPreMemPolicy       Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateSmbusDebugPreMemPolicy (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  SmbusPreMemConfig->SpdWriteDisable = PchSetup->SmbusSpdWriteDisable;
}

/**
  Update Hsio Debug Pre Mem Policy.

  @param[in] SiPreMemPolicy       Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateHsioDebugPreMemPolicy (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_HSIO_PREMEM_CONFIG          *HsioPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHsioPreMemConfigGuid, (VOID *) &HsioPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  HsioPreMemConfig->ChipsetInitMessage = PchSetup->ChipsetInitMessage;
  HsioPreMemConfig->BypassPhySyncReset = PchSetup->BypassPhySyncReset;
}

/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in] SiPreMemPolicy  The SI PREMEM Policy PPI instance
  @param[in] PchSetup        Pointer to PCH_SETUP buffer

**/
STATIC
VOID
UpdatePcieDebugPreMemPolicy (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_PCIE_RP_PREMEM_CONFIG       *PcieRpPreMemConfig;
  EFI_STATUS                      Status;
  UINT32                          CtrlIndex;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  for (CtrlIndex = 0; CtrlIndex < GetPchMaxPcieControllerNum(); CtrlIndex++) {
    if (PchSetup->PcieBifurcation[CtrlIndex] != PLATFORM_POR) {
      PcieRpPreMemConfig->Bifurcation[CtrlIndex] = PchSetup->PcieBifurcation[CtrlIndex];
    }
  }
}

/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in] SiPreMemPolicy  The SI PREMEM Policy PPI instance

  @retval EFI_SUCCESS        The PPI is installed and initialized.
  @retval EFI ERRORS         The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyDebugPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI  *SiPreMemPolicy
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  PCH_SETUP                       PchSetup;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyDebug Pre-Mem Start\n"));

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UpdateSmbusDebugPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateHsioDebugPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdatePcieDebugPreMemPolicy (SiPreMemPolicy, &PchSetup);

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyDebug Pre-Mem End\n"));

  return EFI_SUCCESS;
}
