/** @file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2020 Intel Corporation.

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

#include "DxeMePolicyUpdate.h"
#include <MeState.h>
#include <Library/MeUtilsLib.h>
//
// Record version
//
#define RECORD_REVISION_1              0x01
#define MAX_FW_UPDATE_BIOS_SELECTIONS  2

//
// Function implementations executed during policy initialization phase
//
/**
  This is to display localized message in the console. This is
  used to display message strings in local language. To display
  the message, the routine will check the message ID and ConOut
  the message strings.

  @param[in] MsgId                Me error message ID for displaying on screen message

**/
VOID
EFIAPI
ShowMeReportError (
  IN ME_ERROR_MSG_ID              MsgId
  )
{
  UINTN            MsgDelay;

  MsgDelay = HECI_MSG_DELAY;
  gST->ConOut->ClearScreen (gST->ConOut);

  switch (MsgId) {
  case MSG_EOP_ERROR:
    gST->ConOut->OutputString (gST->ConOut, L"Error sending End Of Post message to ME: HECI disabled, proceeding with boot!\n");
    break;

  case MSG_ME_FW_UPDATE_FAILED:
    gST->ConOut->OutputString (gST->ConOut, L"ME FW Update Failed, please try again!\n");
    break;

  case MSG_HMRFPO_LOCK_FAILURE:
    gST->ConOut->OutputString (gST->ConOut, L"(A7) Me FW Downgrade - Request MeSpiLock Failed\n");
    break;

  case MSG_HMRFPO_UNLOCK_FAILURE:
    gST->ConOut->OutputString (gST->ConOut, L"(A7) Me FW Downgrade - Request MeSpiEnable Failed\n");
    break;

  case MSG_ME_FW_UPDATE_WAIT:
    gST->ConOut->OutputString (gST->ConOut, L"Intel(R) Firmware Update is in progress. It may take up to 90 seconds. Please wait.\n");
    break;

  case MSG_ILLEGAL_CPU_PLUGGED_IN:
    gST->ConOut->OutputString (gST->ConOut, L"\n\n\rAn unsupported CPU/PCH configuration has been identified.\n");
    gST->ConOut->OutputString (gST->ConOut, L"\rPlease refer to the Platform Validation Matrix\n\rfor supported CPU/PCH combinations.");
    break;

  case MSG_PLAT_DISABLE_WAIT:
    gST->ConOut->OutputString (gST->ConOut, L"WARNING! Firmware encountered errors and will reboot the platform in 30 minutes.");
    MsgDelay = 5 * HECI_MSG_DELAY;
    break;

  default:
    DEBUG ((DEBUG_ERROR, "This Message Id hasn't been defined yet, MsgId = %x\n", MsgId));
    break;
  }

  gBS->Stall (MsgDelay);
}

/**
  Update the ME Policy Library

  @param[in, out] DxeMePolicy           The pointer to get ME Policy protocol instance

  @retval EFI_SUCCESS                   Initialization complete.
  @retval EFI_UNSUPPORTED               The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES          Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR              Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UpdateDxeMePolicy (
  IN OUT  ME_POLICY_PROTOCOL            *DxeMePolicy
  )
{
  EFI_STATUS              Status;
  EFI_EVENT               EndOfDxeEvent;
  VOID                    *mRegistration;
  ME_DXE_CONFIG           *MeDxeConfig;

  DEBUG ((DEBUG_INFO, "UpdateDxeMePolicy\n"));
  UpdateMePolicyFromSetup (DxeMePolicy);
  UpdateMePolicyFromMeSetup (DxeMePolicy);

  Status = GetConfigBlock ((VOID *) DxeMePolicy, &gMeDxeConfigGuid, (VOID *) &MeDxeConfig);
  if (!EFI_ERROR (Status)) {
    MeDxeConfig->MeReportError = ShowMeReportError;
  }

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UpdateMeSetupCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_NOTIFY,
    UpdateMeSetupCallback,
    NULL,
    &mRegistration
    );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Update ME Policy while MePlatformProtocol is installed.

  @param[in] MePolicyInstance     Instance of ME Policy Protocol

**/
VOID
UpdateMePolicyFromMeSetup (
  IN ME_POLICY_PROTOCOL           *MePolicyInstance
  )
{
  EFI_STATUS              Status;
  ME_RC_CONFIGURATION       MeSetup;
  ME_RC_CONFIGURATION       *MeSetupPtr;
#if ME_WS_SUPPORT
  ME_SETUP_STORAGE        MeSetupStorage;
#endif // ME_WS_SUPPORT
  ME_SETUP_STORAGE        *MeSetupStoragePtr;
#if ME_WS_SUPPORT
  UINTN                   Size;
#endif // ME_WS_SUPPORT
  ME_DXE_CONFIG           *MeDxeConfig;

  DEBUG ((DEBUG_INFO, "UpdateMePolicyFromMeSetup\n"));

#if ME_WS_SUPPORT
  Size = sizeof (ME_SETUP_STORAGE);
  Status = gRT->GetVariable (L"MeSetupStorage", &gMeSetupVariableGuid, NULL, &Size, &MeSetupStorage);
  if (EFI_ERROR (Status)) {
    MeSetupStoragePtr = NULL;
  } else {
    MeSetupStoragePtr = &MeSetupStorage;
  }
#else // ME_WS_SUPPORT
  MeSetupStoragePtr = NULL;
#endif // ME_WS_SUPPORT

  //
  // Get Me configuration from Setup Data
  //
  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);
  if (EFI_ERROR (Status)) {
    MeSetupPtr = NULL;
  } else {
    MeSetupPtr = &MeSetup;
  }

  Status = GetConfigBlock ((VOID *) MePolicyInstance, &gMeDxeConfigGuid, (VOID *) &MeDxeConfig);
#if ME_SUPPORT_FLAG
  ASSERT_EFI_ERROR (Status);
#endif // ME_SUPPORT_FLAG

  if (MeSetupPtr != NULL) {
#if ME_WS_SUPPORT
    MeDxeConfig->MeFwDownGrade        = MeSetupPtr->MeFwDowngrade;
#endif // ME_WS_SUPPORT
    MeDxeConfig->CoreBiosDoneEnabled  = MeSetupPtr->CoreBiosDoneEnabled;
  }

  DEBUG ((DEBUG_INFO, "UpdateMePolicyFromMeSetup done, Status: %r\n", Status));
}

/**
  Update ME Policy if Setup variable exists.

  @param[in, out] MePolicyInstance     Instance of ME Policy Protocol

**/
VOID
UpdateMePolicyFromSetup (
  IN OUT ME_POLICY_PROTOCOL     *MePolicyInstance
  )
{
  EFI_STATUS                    Status;
#ifdef ME_TESTMENU_FLAG
  ME_RC_CONFIGURATION           SetupData;
  ME_DXE_CONFIG                 *MeDxeConfig;
#endif

  DEBUG ((DEBUG_INFO, "UpdateMePolicyFromSetup\n"));
  Status = EFI_SUCCESS;
#ifdef ME_TESTMENU_FLAG
  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&SetupData);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) MePolicyInstance, &gMeDxeConfigGuid, (VOID *) &MeDxeConfig);
#if ME_SUPPORT_FLAG
    ASSERT_EFI_ERROR (Status);
#endif // ME_SUPPORT_FLAG

  }
#endif

  DEBUG ((DEBUG_INFO, "UpdateMePolicyFromSetup done, Status: %r\n", Status));
}

/**
  Functions performs HECI exchange with FW to update MePolicy settings.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateMeSetupCallback (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  EFI_STATUS                      Status;
  ME_RC_CONFIGURATION             MeSetup;
  ME_INFO_SETUP_DATA              MeInfoSetupData;
  ME_BIOS_PAYLOAD_HOB             *MbpHob;
  HECI_PROTOCOL                   *Heci;
#ifdef PERFORMANCE_FLAG
  FPDT_PERFORMANCE_PROTOCOL       *FpdtProtocol;
  PLATFORM_BOOT_PERFORMANCE_TABLE *Buffer;
#endif
  UINT32                          MeMode;
  VOID                            *VariableWriteArch;

  Status = gBS->LocateProtocol (
                  &gEfiVariableWriteArchProtocolGuid,
                  NULL,
                  &VariableWriteArch
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "UpdateMeSetup event start\n"));

  //
  // Default ME Mode
  //
  MeMode             = ME_MODE_FAILED;
#if ME_WS_SUPPORT
  MbpHob             = NULL;
#endif // ME_WS_SUPPORT

  ZeroMem (&MeInfoSetupData, sizeof(MeInfoSetupData));
  MeInfoSetupData.MeFirmwareInfo = ME_MODE_FAILED;
  //
  // Get Mbp Hob
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    MeInfoSetupData.MeMajor      = MbpHob->MeBiosPayload.FwVersionName.MajorVersion;
    MeInfoSetupData.MeMinor      = MbpHob->MeBiosPayload.FwVersionName.MinorVersion;
    MeInfoSetupData.MeHotFix     = MbpHob->MeBiosPayload.FwVersionName.HotfixVersion;
    MeInfoSetupData.MeBuildNo    = MbpHob->MeBiosPayload.FwVersionName.BuildVersion;
  }

  Status = gBS->LocateProtocol (&gHeciProtocolGuid, NULL, (VOID **) &Heci);
  if (!EFI_ERROR (Status)) {
    Status = Heci->GetMeMode (&MeMode);
  } else {
    Status = MeBiosGetMeMode (&MeMode);
  }
  if (!EFI_ERROR (Status)) {
    MeInfoSetupData.MeFirmwareInfo = (UINT8)MeMode;
  }

  Status = gRT->SetVariable (
                  L"MeInfoSetup",
                  &gMeInfoSetupGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (MeInfoSetupData),
                  &MeInfoSetupData
                  );
  ASSERT_EFI_ERROR (Status);
#ifdef PERFORMANCE_FLAG
  //
  // Locate Fpdt Protocol to allow inserting new record
  //
  Status = gBS->LocateProtocol (&gFpdtPerformanceProtocolGuid, NULL, (VOID **) &FpdtProtocol);

  if (!EFI_ERROR(Status) && (MbpHob != NULL) && MbpHob->MeBiosPayload.PlatBootPerfData.Available) {
    //
    // Protocol is available and Platform boot information is also available in MBP
    //

    //
    // Create Platform Boot Performance Table and populate fields from MBP
    // All timestamp values are required to be in nanosec unit for FPDT table
    //
    Status = (gBS->AllocatePool) (EfiReservedMemoryType, sizeof (PLATFORM_BOOT_PERFORMANCE_TABLE), (VOID **) &Buffer);
    ASSERT_EFI_ERROR (Status);
    (gBS->SetMem) (Buffer, sizeof (PLATFORM_BOOT_PERFORMANCE_TABLE), 0);

    Buffer->Header.Signature = PLATFORM_BOOT_TABLE_SIGNATURE;
    Buffer->Header.Length = sizeof(PLATFORM_BOOT_PERFORMANCE_TABLE);
    Buffer->PlatformBoot.Type = PLATFORM_BOOT_RECORD_TYPE;
    Buffer->PlatformBoot.Length = sizeof(PLATFORM_BOOT_TABLE_RECORD);
    Buffer->PlatformBoot.Revision = RECORD_REVISION_1;
    Buffer->PlatformBoot.TimestampDelta1 = (UINT64)((MbpHob->MeBiosPayload.PlatBootPerfData.MbpPerfData.PwrbtnMrst) * CONVERSION_MULTIPLIER);
    Buffer->PlatformBoot.TimestampDelta2 = (UINT64)((MbpHob->MeBiosPayload.PlatBootPerfData.MbpPerfData.MrstPltrst) * CONVERSION_MULTIPLIER);
    Buffer->PlatformBoot.TimestampDelta3 = (UINT64)((MbpHob->MeBiosPayload.PlatBootPerfData.MbpPerfData.PltrstCpurst) * CONVERSION_MULTIPLIER);

    Status = FpdtProtocol->UpdateRecord (FpdtProtocol, NULL, PLATFORM_BOOT_TABLE_PTR_TYPE, (UINT64)Buffer, 0);
    ASSERT_EFI_ERROR (Status);
  }
#endif // PERFORMANCE_FLAG

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);
#if ME_SUPPORT_FLAG
  ASSERT_EFI_ERROR (Status);
#endif // ME_SUPPORT_FLAG

  if (!EFI_ERROR(Status)) {
#if ME_WS_SUPPORT
    //
    // FW Downgrade BIOS Setup Option is only valid for one boot
    //
    MeSetup.MeFwDowngrade = 0;

    //
    // Reset Firmware Info
    //
    MeSetup.MeFirmwareInfo = MeInfoSetupData.MeFirmwareInfo;

    MeSetup.MeImageType            = 0;

    if (MbpHob != NULL) {
      //
      // Set Image Type for MEBx disposition
      //
      MeSetup.MeImageType = (UINT8) MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;

      //
      // Retrieve IntegratedTouch capability to report whether HECI3 should be enabled for S3 boot mode.
      //
      if ((MbpHob->MeBiosPayload.FwCapsSku.Available == 1) &&
          (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.IntegratedTouch == 1)) {
        MeSetup.HeciCommunication3  = 1;
      }
    }

    //
    // Update AMT settings
    //
#if AMT_SUPPORT
    MeSetup.UnConfigureMe  = 0;
    MeSetup.AmtCiraRequest = 0;
    if (MeSetup.MeImageType != ME_IMAGE_CORPORATE_SKU_FW) {
      MeSetup.Asf = 0;
      MeSetup.Amt = 0;
    }
#endif // AMT_SUPPORT

    Status = SetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&MeSetup);
#if ME_SUPPORT_FLAG
    ASSERT_EFI_ERROR (Status);
#endif // ME_SUPPORT_FLAG
#endif // ME_WS_SUPPORT
  }
  gBS->CloseEvent (Event);

  return;
}

