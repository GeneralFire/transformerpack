/** @file
  This file is SampleCode of the library for Intel PCH PEI Debug Policy initialization.

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

#include <PiPei.h>
#include <PchSetupVariableCdfEbg.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PchInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/SataSocLib.h>
#include <Library/PchPolicyLib.h>
//#include <ConfigBlock/PcieRpConfig.h>


/**
  Update PCIE Root Port debug policies

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
  @param[in] PchSetup  Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePcieRpDebugPolicy (
  IN SI_POLICY_PPI  *SiPolicy,
  IN PCH_SETUP      *PchSetup
  )
{
  UINT8             Index;
  PCH_PCIE_CONFIG   *PcieRpConfig;
  EFI_STATUS        Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PcieRpConfig->EnablePort8xhDecode           = PchSetup->PcieRootPort8xhDecode;
  PcieRpConfig->PchPciePort8xhDecodePortIndex = PchSetup->Pcie8xhDecodePortIndex;

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {

      // EBG_TODO review instances for PcieRpConfig
    //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.LtrMaxSnoopLatency,                0x0846;
    //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.LtrMaxNoSnoopLatency,              0x0846;
    if (PchSetup->PchPcieLtrEnable[Index]) {
      // EBG_TODO review instances for PcieRpConfig
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.LtrMaxSnoopLatency                = 0x1003;
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.LtrMaxNoSnoopLatency              = 0x1003;
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.SnoopLatencyOverrideMode          = PchSetup->PchPcieSnoopLatencyOverrideMode[Index];
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.SnoopLatencyOverrideMultiplier    = PchSetup->PchPcieSnoopLatencyOverrideMultiplier[Index];
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.NonSnoopLatencyOverrideMode       = PchSetup->PchPcieNonSnoopLatencyOverrideMode[Index];
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier = PchSetup->PchPcieNonSnoopLatencyOverrideMultiplier[Index];
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.SnoopLatencyOverrideValue         = PchSetup->PchPcieSnoopLatencyOverrideValue[Index];
      //PcieRpConfig->RootPort[Index].PcieRpLtrConfig.NonSnoopLatencyOverrideValue      = PchSetup->PchPcieNonSnoopLatencyOverrideValue[Index];
    }
  }
}

/**
  Update PCH DMI debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePchDmiDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  UINT8                        Index;
  PCH_DMI_CONFIG               *PchDmiConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchDmiConfigGuid, (VOID *) &PchDmiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    if (PchSetup->PchPcieLtrEnable[Index] == TRUE) {
      PchDmiConfig->PwrOptEnable = TRUE;
      return;
    }
  }
}

/**
  Update SATA debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateSataDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  SATA_CONFIG         *SataConfig;
  UINT32              SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum (); SataCtrlIndex++) {
    SataConfig = GetPchSataConfig (SiPolicy, SataCtrlIndex);

    SataConfig->TestMode = PchSetup->SataTestMode[SataCtrlIndex];
  }
}

/**
  Update USB debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateUsbDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  USB_CONFIG                   *UsbConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  UsbConfig->XhciOcLock = PchSetup->PchXhciOcLock;
}

/**
  Update Power Management debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePmDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_PM_CONFIG                *PmConfig;
  EFI_STATUS                   Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PmConfig->DisableEnergyReport = (PchSetup->PchEnergyReport == 1)? 0 : 1;
}

/**
  Update P2sb debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateP2sbDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
  PCH_P2SB_CONFIG                 *P2sbConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  P2sbConfig->SbAccessUnlock = PchSetup->PchSidebandLock ? 0 : 1;
}


/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in, out] SiPolicy   The SI Policy PPI instance

  @retval EFI_SUCCESS        The PPI is installed and initialized.
  @retval EFI ERRORS         The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyDebug (
  IN OUT  SI_POLICY_PPI  *SiPolicy
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  PCH_SETUP                       PchSetup;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyDebug Post-Mem Start\n"));

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

  UpdatePcieRpDebugPolicy (SiPolicy, &PchSetup);
  UpdatePchDmiDebugPolicy (SiPolicy, &PchSetup);
  UpdateSataDebugPolicy (SiPolicy, &PchSetup);
  UpdateUsbDebugPolicy (SiPolicy, &PchSetup);
  UpdatePmDebugPolicy (SiPolicy, &PchSetup);
  UpdateP2sbDebugPolicy (SiPolicy, &PchSetup);

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyDebug Post-Mem End\n"));

  return Status;
}
