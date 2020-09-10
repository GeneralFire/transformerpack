/** @file
  The PEI Library Implements ME Init.

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
#include <Register/MeRegs.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/PeiMeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeShowBufferLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/PmcLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchInfoLib.h>
#include <Register/HeciRegs.h>
#include <Library/TimerLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <MeBiosPayloadHob.h>
#include "MeInitPostMem.h"
#include <Library/MeGetConfigBlock.h>
#include <MeDefines.h>
#include <Library/PsfLib.h>

EFI_STATUS
MctpConfigureAfterDid (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  );

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR mMctpConfigureNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gDramInitDoneSentPpiGuid,
    MctpConfigureAfterDid
  }
};

/**
  Check if HECI3 is supported and should be enabled.

  @param[in] MbpHob                   Pointer to MbpHob

  @retval    FALSE                    HECI3 isn't suported and should be disabled
**/
BOOLEAN
IsHeci3Supported (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob
  )
{
  return FALSE;
}

/**
  Disable ME Devices when needed

  @param[in] IsHeci3Supported    Determines if HECI3 is supported and should be enabled

  @retval EFI_STATUS             Operation status
  @retval EFI_INVALID_PARAMETER  Passed invalid input parameters
**/
EFI_STATUS
MeDeviceConfigure (
  IN BOOLEAN                      IsHeci3Supported
  )
{
  SPS_PEI_CONFIG       *SpsPeiConfig = NULL;
  ME_PEI_PREMEM_CONFIG *MePreMemConfig;
  ME_DEVICE_FUNC_CTRL  HeciCommunication1;
  ME_DEVICE_FUNC_CTRL  HeciCommunication3;
  ME_DEVICE_FUNC_CTRL  HeciCommunication4 = ME_DEVICE_DISABLED;
  ME_DEVICE_FUNC_CTRL  IderDeviceEnable = ME_DEVICE_DISABLED;

  SpsPeiConfig = GetSpsPreMemPeiConfigBlock (NULL);
  if (SpsPeiConfig == NULL) {
    if (MeTypeIsSps ()) {
      ASSERT (SpsPeiConfig != NULL);
      return EFI_INVALID_PARAMETER;
    }
  }

  MePreMemConfig = GetMePreMemConfigBlock (NULL);
  if (MePreMemConfig == NULL) {
    ASSERT (MePreMemConfig != NULL);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "[ME] Disabling ME functions:"));

  HeciCommunication1 = Enabled;
  HeciCommunication3 = Enabled;

  if (MePreMemConfig->HeciCommunication2 == ME_DEVICE_DISABLED) {
    DEBUG ((DEBUG_INFO, " %d (HECI-2)", HECI2));
    SetMeUefiFwHealthStatus (Heci2State, StatusOther);
  } else {
    SetMeUefiFwHealthStatus (Heci2State, StatusOk);
  }
  if (SpsPeiConfig != NULL) {
    HeciCommunication4 = SpsPeiConfig->HeciCommunication4;
    IderDeviceEnable = SpsPeiConfig->IderDeviceEnable;
  }
  if (HeciCommunication4 == ME_DEVICE_DISABLED) {
    DEBUG ((DEBUG_INFO, " %d (HECI-4)", HECI4));
    SetMeUefiFwHealthStatus (Heci4State, StatusOther);
  } else {
    SetMeUefiFwHealthStatus (Heci4State, StatusOk);
  }
  if (IderDeviceEnable == ME_DEVICE_DISABLED) {
    DEBUG ((DEBUG_INFO, " %d (IDE-R)", IDER));
    SetMeUefiFwHealthStatus (IdeRState, StatusOther);
  } else {
    SetMeUefiFwHealthStatus (IdeRState, StatusOk);
  }
  if (MePreMemConfig->KtDeviceEnable == ME_DEVICE_DISABLED) {
    DEBUG ((DEBUG_INFO, " %d (KT)", SOL));
    SetMeUefiFwHealthStatus (KTState, StatusOther);
  } else {
    SetMeUefiFwHealthStatus (KTState, StatusOk);
  }

  DEBUG ((DEBUG_INFO, "\n"));

  //
  // Before potential disabling HECI1
  // We have to store Firmware Statuses in to HOB
  //
  SaveFwStsToHob();

  MeDeviceControl (HECI1, HeciCommunication1);
  MeDeviceControl (HECI2, MePreMemConfig->HeciCommunication2);
  MeDeviceControl (HECI3, HeciCommunication3);
  MeDeviceControl (HECI4, HeciCommunication4);
  MeDeviceControl (IDER, IderDeviceEnable);
  MeDeviceControl (SOL, MePreMemConfig->KtDeviceEnable);

  return EFI_SUCCESS;
} // MeDeviceConfigure

/**
  Configure HECI devices on End Of Pei

  @param[in]  PeiServices        Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor   Pointer to the descriptor for the Notification event that caused this function to execute.
  @param[in]  Ppi                Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS        The function completes successfully

**/
EFI_STATUS
EFIAPI
MeOnEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{

  DEBUG ((DEBUG_INFO, "%a() enter\n", __FUNCTION__));
    if (MeTypeIsClient ()) {
      //
      // Send optional HECI command
      //
      SendOptionalHeciCommand ();

      CreateMkhiVersionHob ();
    }

    //
    // Save ME/AMT policies in PEI phase to HOB for using in DXE.
    //
    SavePolicyToHob ();

  DEBUG ((DEBUG_INFO, "%a() done.\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Build Sps Guid Data Hob

  @param[in] SiPreMemPolicyPpi     Pointer to PreMem policy PPI
**/
VOID
BuildSpsGuidDataHob (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
  SPS_PEI_CONFIG            *SpsPeiConfig;
  UINTN                     HobSize;
  VOID                      *HobPtr;

  ASSERT (SiPreMemPolicyPpi != 0);
  if (SiPreMemPolicyPpi != NULL) {
    SpsPeiConfig = GetSpsPreMemPeiConfigBlock (SiPreMemPolicyPpi);
    if (MeTypeIsSps()) {
      ASSERT(SpsPeiConfig != 0);
    }
    if (SpsPeiConfig != NULL) {
      HobSize = SpsPeiConfig->Header.GuidHob.Header.HobLength;
      DEBUG ((DEBUG_INFO, "HobSize for SpsPeiConfig: %x\n", HobSize));
      HobPtr = BuildGuidDataHob (&gSpsPolicyHobGuid, SpsPeiConfig, HobSize);
      ASSERT (HobPtr != 0);
    }
  }
}

/**
  Configure MCTP when needed

  The function checks whether MCTP is supported, if so, then the BIOS
  configures MCTP.

  @param[in] MbpHob                   Pointer to MbpHob
  @param[in] MePeiConfig              Pointer to ME PEI Config
**/
VOID
MctpConfigureInternal (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob,
  IN ME_PEI_CONFIG       *MePeiConfig
  )
{
  if (IsMctpConfigurationSupported (MbpHob, MePeiConfig)) {
    PsfConfigureMctpCycle ();
  }
}

/**
  Configure MCTP after sending DramInitDone message.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The notify callback PPI.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
MctpConfigureAfterDid (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS           Status;
  ME_BIOS_PAYLOAD_HOB  *MbpHob;
  SI_POLICY_PPI        *SiPolicyPpi;
  ME_PEI_CONFIG        *MePeiConfig;
  VOID                 *TestPpi;

  Status = PeiServicesLocatePpi (
             &gDramInitDoneSentPpiGuid,
             0,
             NULL,
             &TestPpi
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate gSiPolicyPpiGuid fail. Status = %r\n", Status));
    return Status;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  MctpConfigureInternal (MbpHob, MePeiConfig);

  return EFI_SUCCESS;
}

/**
  Configure MCTP when needed

  In case of SPS FW the BIOS immediately calls function MctpConfigureInternal which
  is responsible for configuring of MCTP. In case of WS FW this configuration can
  be done after DramInitDone message, so the BIOS installs notification on this signal.

  @param[in] MbpHob                   Pointer to MbpHob
  @param[in] MePeiConfig              Pointer to ME PEI Config
**/
VOID
MctpConfigure (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob,
  IN ME_PEI_CONFIG       *MePeiConfig
  )
{
  EFI_STATUS Status;

  if (MeTypeIsSps ()) {
    MctpConfigureInternal (MbpHob, MePeiConfig);
  }

  if (MeTypeIsClient ()) {
    Status = PeiServicesNotifyPpi (mMctpConfigureNotifyList);
    ASSERT_EFI_ERROR (Status);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot install SyncMbpAfterDid notification, status = %r\n", Status));
    }
  }
}