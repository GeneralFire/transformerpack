/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include "PeiPolicyInit.h"

#include <Ppi/MemoryPolicyPpi.h>
#include <Library/RcMemAllocLib.h>
#include <Library/PeiIioSiPolicyLib.h>
#include <Library/PeiMemoryPolicyLib.h>
#include <Library/PeiPsmiPolicyUpdateLib.h>
#include <Library/PeiIioSiPolicyUpdateLib.h>
#include <Library/PeiFusaSafPolicyUpdateLib.h>
#include <Library/PeiSecurityPolicyUpdateLib.h>
#include <Library/PeiCommonRcPolicyUpdateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SetupLib.h>

/**
  Initialize the UPI policy PPI.

  This is a local helper function to group together the allocation, default
  settings, platform update, and installation of the PPI.

  @param[in, out] SetupData     Pointer to SETUP_DATA structure.

  @retval EFI_STATUS            The PPI was initialized and installed.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate memory for the PPI.
**/
EFI_STATUS
InitializeUpiPolicyPpi (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  )
{
  EFI_STATUS      Status;
  UPI_POLICY_PPI  *UpiPolicyPpi;

  UpiPolicyPpi = (UPI_POLICY_PPI *) AllocateZeroPool (sizeof (UPI_POLICY_PPI));
  ASSERT (UpiPolicyPpi != NULL);
  if (UpiPolicyPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Set all Intel default policy settings.
  //
  Status = UpiCreatePolicyDefaults (UpiPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Update platform related and customized settings.
  //
  Status = UpdatePeiUpiPolicy (UpiPolicyPpi, SetupData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Install the PPI. Once installed, RC assumes the policy is finalized.
  //
  Status = UpiInstallPolicyPpi (UpiPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  return Status;
} // InitializeUpiPolicyPpi

/**
  Initialize the RAS RC policy PPI.

  This is a local helper function to group together the allocation, default
  settings, platform update, and installation of the PPI.

  @param[in, out] SetupData     Pointer to SETUP_DATA structure.

  @retval EFI_STATUS            The PPI was initialized and installed.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate memory for the PPI.
**/
EFI_STATUS
InitializeRasRcPolicyPpi (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  )
{
  EFI_STATUS        Status;
  RAS_RC_POLICY_PPI *RasRcPolicyPpi;

  RasRcPolicyPpi = (RAS_RC_POLICY_PPI *) AllocatePool (
    sizeof (RAS_RC_POLICY_PPI)
    );
  ASSERT (RasRcPolicyPpi != NULL);
  if (RasRcPolicyPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Set all Intel default policy settings.
  //
  Status = SetRasRcPolicyPpiDefaults (RasRcPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    FreePool (RasRcPolicyPpi);
    return Status;
  }

  //
  // Update platform related and customized settings.
  //
  Status = UpdateRasRcPolicyPpi (RasRcPolicyPpi, SetupData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    FreePool (RasRcPolicyPpi);
    return Status;
  }

  //
  // Install the PPI. Once installed, RC assumes the policy is finalized.
  //
  Status = InstallRasRcPolicyPpi (RasRcPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  return Status;
} // InitializeRasRcPolicyPpi

/**
  Initialize the Memory policy PPI.

  This is a local helper function to group together the allocation, default
  settings, platform update, and installation of the PPI.

  @param[in, out] SetupData     Pointer to SETUP_DATA structure.

  @retval EFI_STATUS            The PPI was initialized and installed.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate memory for the PPI.

**/
EFI_STATUS
InitializeMemoryPolicyPpi (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_START
  )
{
  EFI_STATUS      Status;
  MEMORY_POLICY_PPI  *PolicyPpi;
  SYS_SETUP          *SysSetup;

  PolicyPpi = (MEMORY_POLICY_PPI *) AllocateZeroPool (sizeof (MEMORY_POLICY_PPI));
  ASSERT (PolicyPpi != NULL);
  if (PolicyPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SysSetup = (SYS_SETUP *) RcAllocatePool (sizeof (SYS_SETUP));
  ASSERT (SysSetup != NULL);
  if (SysSetup == NULL) {
    FreePool (PolicyPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  PolicyPpi->SysSetup = SysSetup;

  //
  // Set all Intel default policy settings.
  //

  Status = SetMemoryPolicyDefaults (PolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    FreePool (PolicyPpi);
    return Status;
  }

  //
  // Update platform related and customized settings.
  //

  Status = UpdatePeiMemoryPolicy (PolicyPpi, SetupData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    FreePool (PolicyPpi);
    return Status;
  }

  //
  // Install the PPI. Once installed, RC assumes the policy is finalized.
  //

  Status = InstallMemoryPolicyPpi (PolicyPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;

} // InitializeMemoryPolicyPpi

/**
  Initialize IIO policies in config block table and installs PPI

  @retval EFI_SUCCESS          If IIO policy PPI installed successfully
  @retval EFI_ERROR            Failed to create and install IIO policy PPI
**/
EFI_STATUS
EFIAPI
InitializeIioPolicyPpi (
  VOID
)
{
  EFI_STATUS                   Status;
  IIO_POLICY_STRUCT           *IioPolicyPpi;

  //
  // Create IIO Config block table and add config blocks
  //
  Status = IioCreateConfigBlocks (&IioPolicyPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Update IIO Config block policies from setup
  //
  Status = IioUpdateConfigBlockPolicy (IioPolicyPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Install IioPolicyPpi.
  //
  Status = IioInstallPolicyPpi (IioPolicyPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  return Status;
}

/**
  Initialize Intel PEI Platform Policy

  @param[in] PeiServices            General purpose services available to every PEIM.

**/
VOID
EFIAPI
PeiSiPolicyInit (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS  Status;
  // APTIOV_SERVER_OVERRIDE_RC_START
  INTEL_SETUP_DATA  SetupData;

  ZeroMem (&SetupData, sizeof (INTEL_SETUP_DATA));
  // APTIOV_SERVER_OVERRIDE_RC_END
  Status = GetEntireConfig (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Get Entire Config! Status: %r\n", Status));
    return;
  }

  //
  // Initialize and install each policy PPI. If errors occur, try to continue.
  //

  Status = InitializeUpiPolicyPpi (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: UPI policy NOT installed! Status: %r\n", Status));
  }

  Status = InitializeRasRcPolicyPpi (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: RAS RC policy NOT installed! Status: %r\n", Status));
  }

  Status = InitializeMemoryPolicyPpi (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Memory policy NOT installed! Status: %r\n", Status));
  }

  Status = InitializeIioPolicyPpi ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: IIO policy NOT installed! Status: %r\n", Status));
  }

  //
  // The below policies do not have defaults in libraries, they have
  // their defaults in setup, so just call the update functions.
  //

  Status = UpdatePeiCpuPolicy (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Update PEI CPU policy! Status: %r\n", Status));
  }

  Status = UpdatePeiPsmiPolicy (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Update PEI PSMI policy! Status: %r\n", Status));
  }

  Status = UpdatePeiCommonRcPolicy (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Update PEI Common RC policy! Status: %r\n", Status));
  }

  Status = UpdatePeiSecurityPolicy (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Update PEI Security policy! Status: %r\n", Status));
  }

  Status = UpdatePeiFusaSafPolicy (&SetupData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Update PEI FuSa (SAF) policy! Status: %r\n", Status));
  }

} // PeiSiPolicyInit
