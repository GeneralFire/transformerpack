/** @file
  This file is SampleCode of the library for Intel PCH PEI Policy Update Pre-Memory.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include <PiPei.h>
#include <Library/IoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <PchSetupVariableCdfEbg.h>
#include <Library/SiPolicyLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/PchPcieRpLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PcdLib.h>
#include <Library/PchInfoLib.h>
#include <Library/UbaPcieBifurcationUpdateLib.h>
#include <EnterprisePcieConfig.h>
#include <PeiFiaMuxPolicyUpdate.h>


/**
  Update PCH General premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdatePchGeneralPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_GENERAL_PREMEM_CONFIG       *PchGeneralPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PchGeneralPreMemConfig->Port80Route = PchSetup->IchPort80Route;
}

/**
  Update DCI premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateDciPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_DCI_PREMEM_CONFIG           *DciPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  DciPreMemConfig->DciEn              = PchSetup->DciEn;
  DciPreMemConfig->DciDbcMode         = PchSetup->DciDbcMode;
  DciPreMemConfig->DciUsb3TypecUfpDbg = PchSetup->DciUsb3TypecUfpDbg;
}

/**
  Update Pch Trace Hub premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdatePchTraceHubPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_TRACE_HUB_PREMEM_CONFIG     *PchTraceHubPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchTraceHubPreMemConfigGuid, (VOID *) &PchTraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PchTraceHubPreMemConfig->TraceHub.EnableMode     = PchSetup->PchTraceHubMode;
  PchTraceHubPreMemConfig->TraceHub.MemReg0Size    = (UINT8) PchSetup->PchTraceHubMemReg0Size;
  PchTraceHubPreMemConfig->TraceHub.MemReg1Size    = (UINT8) PchSetup->PchTraceHubMemReg1Size;
  PchTraceHubPreMemConfig->TraceHub.AetEnabled     = PchSetup->AetEnableMode;
}

/**
  Update SM Bus premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateSmbusPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_SMBUS_PREMEM_CONFIG         *SmbusPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  SmbusPreMemConfig->Enable = TRUE;
  SmbusPreMemConfig->DynamicPowerGating = FALSE;
}

/**
  Update Lpc premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateLpcPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_LPC_PREMEM_CONFIG           *LpcPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gLpcPreMemConfigGuid, (VOID *) &LpcPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  LpcPreMemConfig->EnhancePort8xhDecoding = PchSetup->EnhancePort8xhDecoding;
  LpcPreMemConfig->LpcPmHAE               = FALSE;
}

/**
  Update WatchDog premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateWatchDogPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_WDT_PREMEM_CONFIG           *WatchDogPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gWatchDogPreMemConfigGuid, (VOID *) &WatchDogPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  WatchDogPreMemConfig->DisableAndLock = !PchSetup->WdtEnable;
}

#define PH3_EQ_PREMEM_CONFIG_UPDATE(entryNum)                                                                                                            \
  ASSERT((entryNum > 0) && (entryNum <= ENTERPRISE_PCIE_LINK_EQ_MAX_LIST_ENTRIES));                                                                      \
  PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph3Settings.Ph3Entries[entryNum-1].EntryFormat = PchSetup->PchPcieEqPh3FomHintTableFormatEntry##entryNum[RpIndex];  \
  if (PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph3Settings.Ph3Entries[entryNum-1].EntryFormat == EnterprisePcieEqFormatPreset) {                               \
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph3Settings.Ph3Entries[entryNum-1].D1 = PchSetup->PchPcieEqPh3FomHintTablePresetEntry##entryNum[RpIndex];         \
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph3Settings.Ph3Entries[entryNum-1].D2 = 0;                                                                        \
  } else {                                                                                                                                               \
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph3Settings.Ph3Entries[entryNum-1].D1 = PchSetup->PchPcieEqPh3FomHintTableD1Entry##entryNum[RpIndex];             \
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph3Settings.Ph3Entries[entryNum-1].D2 = PchSetup->PchPcieEqPh3FomHintTableD2Entry##entryNum[RpIndex];             \
  }

/**
  Update PcieRp premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdatePcieRpPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  UINT32                              RpIndex;
  UINT32                              RpEnabledMask;
  PCH_PCIE_RP_PREMEM_CONFIG           *PcieRpPreMemConfig;
  ENTERPRISE_PCIE_LINK_PREMEM_CONFIG  *PcieRpLinkPreMemConfig;
  EFI_STATUS                          Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieLinkPrememConfigGuid, (VOID *) &PcieRpLinkPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  RpEnabledMask = PcieRpPreMemConfig->RpEnabledMask;

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex ++) {
    if (PchSetup->PcieRootPortEn[RpIndex]) {
      RpEnabledMask |=  (UINT32) (1 << RpIndex);
    } else {
      RpEnabledMask &= ~(UINT32) (1 << RpIndex);
    }
  }
  PcieRpPreMemConfig->RpEnabledMask = RpEnabledMask;

  for (RpIndex = 0; RpIndex < GetPchMaxPcieClockNum(); RpIndex++) {
    PcieRpPreMemConfig->PcieClock[RpIndex].Usage = (UINT8)PchClockUsageUnspecified;
    PcieRpPreMemConfig->PcieClock[RpIndex].ClkReq = (UINT8)PCH_PCIE_NO_SUCH_CLOCK;
  }

  // Update the Gen3 Eq config
  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex ++) {
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].OverrideDefaultEqSettings          = !!PchSetup->PchPcieEqOverrideDefaultSettings[RpIndex];
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph1Settings.DownstreamPortTxPreset = PchSetup->PchPcieEqPh1DownstreamPortTransmitterPreset[RpIndex];
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph1Settings.UpstreamPortTxPreset   = PchSetup->PchPcieEqPh1UpstreamPortTransmitterPreset[RpIndex];
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph2Settings.LocalTxOverrideEnable  = PchSetup->Ph2LocalTransmitterOverrideEnable[RpIndex];
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph2Settings.TxCoeffPreCursor       = PchSetup->LocalTransmitterOverridePrecursorCoeff[RpIndex];
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph2Settings.TxCoeffCursor          = PchSetup->LocalTransmitterOverrideCursorCoeff[RpIndex];
    PcieRpLinkPreMemConfig->EqConfig[RpIndex].Ph2Settings.TxCoeffPostCursor      = PchSetup->LocalTransmitterOverridePostCursorCoeff[RpIndex];
    PH3_EQ_PREMEM_CONFIG_UPDATE(1);
    PH3_EQ_PREMEM_CONFIG_UPDATE(2);
    PH3_EQ_PREMEM_CONFIG_UPDATE(3);
    PH3_EQ_PREMEM_CONFIG_UPDATE(4);
    PH3_EQ_PREMEM_CONFIG_UPDATE(5);
    PH3_EQ_PREMEM_CONFIG_UPDATE(6);
    PH3_EQ_PREMEM_CONFIG_UPDATE(7);
    PH3_EQ_PREMEM_CONFIG_UPDATE(8);
    PH3_EQ_PREMEM_CONFIG_UPDATE(9);
    PH3_EQ_PREMEM_CONFIG_UPDATE(10);
    PH3_EQ_PREMEM_CONFIG_UPDATE(11);
    PH3_EQ_PREMEM_CONFIG_UPDATE(12);
  }
}
#undef PH3_EQ_PREMEM_CONFIG_UPDATE

/**
  Update HD Audio premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdateHdaPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  HDAUDIO_PREMEM_CONFIG           *HdaPreMemConfig;
  EFI_STATUS                      Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gHdAudioPreMemConfigGuid, (VOID *) &HdaPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  HdaPreMemConfig->DspEnable                 = PchSetup->PchHdAudioDsp;
  HdaPreMemConfig->Enable                    = PchSetup->PchHdAudio;
  HdaPreMemConfig->DspUaaCompliance          = PchSetup->PchHdAudioDspUaaCompliance;
  HdaPreMemConfig->AudioLinkHda.Enable       = TRUE;
  HdaPreMemConfig->AudioLinkHda.SdiEnable[0] = TRUE;
  HdaPreMemConfig->PowerGatingSupported      = FALSE;
  HdaPreMemConfig->VcType                    = PchSetup->PchHdaAudioVcType;

  //
  //  Override default policy settings
  //
  HdaPreMemConfig->AudioLinkDmic[0].Enable = FALSE;
  HdaPreMemConfig->AudioLinkDmic[1].Enable = FALSE;
  HdaPreMemConfig->AudioLinkSndw[0].Enable = FALSE;

}

/**
  Update PCIe Bifurcation premem policy.

  @param[in] SiPreMemPolicy Pointer to SI_PREMEM_POLICY_PPI
  @param[in] PchSetup       Pointer to PCH_SETUP
**/
STATIC
VOID
UpdatePcieBifurcationPreMemPolicy (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN  PCH_SETUP                *PchSetup
  )
{
  PCH_PCIE_RP_PREMEM_CONFIG *PcieRpPreMemConfig;
  UINT32                          CtrlIndex;
  PCIE_BIFURCATION_CONFIG         *PcieUbaBifurcation;
  PCH_SLOT_CONFIG_DATA_ENTRY_EX   *SlotUbaConfig;
  EFI_STATUS Status;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = PlatformGetPchPcieBifurcationConfig (&PcieUbaBifurcation,&SlotUbaConfig);
  ASSERT_EFI_ERROR (Status);
  for (CtrlIndex = 0; CtrlIndex < GetPchMaxPcieControllerNum(); CtrlIndex++) {
    PcieRpPreMemConfig->Bifurcation[CtrlIndex]     = PcieUbaBifurcation[CtrlIndex];
  }
}

/**
  This function performs PCH PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyPreMem (
  SI_PREMEM_POLICY_PPI             *SiPreMemPolicy
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  PCH_SETUP                         PchSetup;

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

  UpdatePchGeneralPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateDciPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdatePchTraceHubPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateSmbusPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateLpcPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateWatchDogPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdatePcieRpPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateHdaPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdatePcieBifurcationPreMemPolicy (SiPreMemPolicy, &PchSetup);
  UpdateFiaMuxPreMemPolicy (SiPreMemPolicy, &PchSetup);

  return Status;
}
