/** @file
  PEI RAS RC policy PPI library class implementation.

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
#include <Library/RasRcPolicyPpiLib.h>    // *this* library class
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/RasRcPolicy.h>
#include <Library/PerformanceLib.h>

VOID
DebugPrintRasRcPolicyPpi (
  IN CONST  RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  );

/**
  Set RAS RC policy PPI defaults.

  Set all fields in the RAS RC policy PPI to safe defaults. Since all fields
  are set by this function, the PPI does not need to be zeroed before calling.

  @param[out] RasRcPolicyPpi    A pointer to the RAS RC policy PPI to set.

  @retval EFI_SUCCESS             The policy defaults are set.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
**/
EFI_STATUS
EFIAPI
SetRasRcPolicyPpiDefaults (
  OUT       RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  )
{
  ASSERT (RasRcPolicyPpi != NULL);
  if (RasRcPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RasRcPolicyPpi->RasModes = CH_INDEPENDENT;

  RasRcPolicyPpi->RasModesEx = 0;
  RasRcPolicyPpi->RasModesEx |= PTRLSCRB_EN;
  RasRcPolicyPpi->RasModesEx |= DMNDSCRB_EN;


  RasRcPolicyPpi->McBankWarmBootClearError = TRUE;
  RasRcPolicyPpi->PoisonEn = FALSE;
  RasRcPolicyPpi->PfdEn = FALSE;
  RasRcPolicyPpi->CrashLogFeature = TRUE;
  RasRcPolicyPpi->CrashLogOnAllReset = FALSE;
  RasRcPolicyPpi->CrashLogClear = TRUE;
  RasRcPolicyPpi->CrashLogReArm = TRUE;
  return EFI_SUCCESS;
} // SetRasRcPolicyPpiDefaults

/**
  Install the RAS RC policy PPI.

  Once the PPI is installed, RC assumes its values are final. Any platform
  customizations must be done before calling this function.

  @param[in]  RasRcPolicyPpi    A pointer to the RAS RC policy PPI to install.

  @retval EFI_SUCCESS             The policy is installed.
  @retval EFI_INVALID_PARAMETER   The PPI pointer parameter is null.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate a PPI descriptor.
  @retval EFI_OUT_OF_RESOURCES    There is no more space in the PPI database.
**/
EFI_STATUS
EFIAPI
InstallRasRcPolicyPpi (
  IN        RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PPI_DESCRIPTOR  *PpiDescriptor;
  PERF_START (NULL, "InstallRasRcPolicyPpi", "PEI", 0);
  ASSERT (RasRcPolicyPpi != NULL);

  if (RasRcPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocatePool (
    sizeof (EFI_PEI_PPI_DESCRIPTOR)
    );
  ASSERT (PpiDescriptor != NULL);
  if (PpiDescriptor == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PpiDescriptor->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI |
                          EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiDescriptor->Guid = &gRasRcPolicyPpiGuid;
  PpiDescriptor->Ppi = RasRcPolicyPpi;

  DEBUG_CODE_BEGIN ();
  DebugPrintRasRcPolicyPpi (RasRcPolicyPpi);
  DEBUG_CODE_END ();

  DEBUG ((DEBUG_INFO, "Installing RAS_RC_POLICY_PPI\n"));
  Status = PeiServicesInstallPpi (PpiDescriptor);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install RAS_RC_POLICY_PPI: %r\n", Status));
  }
  PERF_END (NULL, "InstallRasRcPolicyPpi", "PEI", 0);

  return Status;
} // InstallRasRcPolicyPpi

/**
  Debug-print the RAS RC policy PPI.

  Note: This function is NOT part of the library class API.
  Print all fields of the RAS RC policy using the DEBUG macro.

  @param[in]  RasRcPolicyPpi    A pointer to the RAS RC policy PPI to print.
**/
VOID
DebugPrintRasRcPolicyPpi (
  IN CONST  RAS_RC_POLICY_PPI   *RasRcPolicyPpi
  )
{
  ASSERT (RasRcPolicyPpi != NULL);
  if (RasRcPolicyPpi == NULL) {
    return;
  }

  DEBUG ((DEBUG_INFO, "RAS_RC_POLICY_PPI @ %p:\n", RasRcPolicyPpi));
  DEBUG ((DEBUG_INFO, "  RasModes:            0x%02x\n", RasRcPolicyPpi->RasModes));
  DEBUG ((DEBUG_INFO, "    FULL_MIRROR_1LM    %d\n", ((RasRcPolicyPpi->RasModes & FULL_MIRROR_1LM) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    FULL_MIRROR_2LM    %d\n", ((RasRcPolicyPpi->RasModes & FULL_MIRROR_2LM) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    CH_LOCKSTEP        %d\n", ((RasRcPolicyPpi->RasModes & CH_LOCKSTEP) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    RK_SPARE           %d\n", ((RasRcPolicyPpi->RasModes & RK_SPARE) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    PARTIAL_MIRROR_1LM %d\n", ((RasRcPolicyPpi->RasModes & PARTIAL_MIRROR_1LM) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    PARTIAL_MIRROR_2LM %d\n", ((RasRcPolicyPpi->RasModes & PARTIAL_MIRROR_2LM) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    STAT_VIRT_LOCKSTEP %d\n", ((RasRcPolicyPpi->RasModes & STAT_VIRT_LOCKSTEP) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "  RasModesEx:          0x%02x\n", RasRcPolicyPpi->RasModesEx));
  DEBUG ((DEBUG_INFO, "    SDDC_EN            %d\n", ((RasRcPolicyPpi->RasModesEx & SDDC_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    DMNDSCRB_EN        %d\n", ((RasRcPolicyPpi->RasModesEx & DMNDSCRB_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    PTRLSCRB_EN        %d\n", ((RasRcPolicyPpi->RasModesEx & PTRLSCRB_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    ADDDC_ERR_INJ_EN   %d\n", ((RasRcPolicyPpi->RasModesEx & ADDDC_ERR_INJ_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    PCLS_EN            %d\n", ((RasRcPolicyPpi->RasModesEx & PCLS_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    MIRROR_SCRUB_EN    %d\n", ((RasRcPolicyPpi->RasModesEx & MIRROR_SCRUB_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    ADDDC_EN           %d\n", ((RasRcPolicyPpi->RasModesEx & ADDDC_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "    PTRLSCRB_EOP_EN    %d\n", ((RasRcPolicyPpi->RasModesEx & PTRLSCRB_EOP_EN) == 0 ? 0 : 1)));
  DEBUG ((DEBUG_INFO, "CrashLogFeature        %d\n", RasRcPolicyPpi->CrashLogFeature));
  DEBUG ((DEBUG_INFO, "CrashLogOnAllReset     %d\n", RasRcPolicyPpi->CrashLogOnAllReset));
  DEBUG ((DEBUG_INFO, "CrashLogClear        %d\n", RasRcPolicyPpi->CrashLogClear));
  DEBUG ((DEBUG_INFO, "CrashLogReArm     %d\n", RasRcPolicyPpi->CrashLogReArm));

} // DebugPrintRasRcPolicyPpi
