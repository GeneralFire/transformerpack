/**@file
  ME Setup Implementation

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

#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/AmtWrapperProtocol.h>
#include <Library/DebugLib.h>
#include <Library/SetupLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/DxeMeLib.h>
#include <Library/PttHeciLib.h>
#include <Library/SetupLib.h>
#include <IndustryStandard/Pci22.h>
#include <Cpu/Include/Library/BootGuardLib.h>
#include <Guid/MeRcVariable.h>
#include <MeRcConfiguration.h>
#include <MeAccess.h>
#include <CpuRegs.h>
#include "MeSetup.h"
#include "SetupPlatform.h"
#include "HeciRegs.h"

#define ARB_SVN_INFO_NUM         8 // The maximum number of ARB SVN information entries is 8.
#define ME_INFO_MAX_STR_SIZE  1024

///
/// HECI PCI Access Macro
///
#define HeciPciSegmentRead32(Register) HeciPciRead32(Register)

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN            gMeClientReset = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_HANDLE     mHiiHandle;
GLOBAL_REMOVE_IF_UNREFERENCED ME_SETUP           mMeSetup;

/**
  Initialize ME strings.

  @param[in] HiiHandle    HiiHandle Handle to HII database
  @param[in] Class        Indicates the setup class
**/
VOID
InitMeInfo (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Class
  )
{
  EFI_STATUS          Status;
  UINTN               VariableSize;
  UINT8               *MeFwSkuValue;
  CHAR16              StrBuf[ME_INFO_MAX_STR_SIZE];
  EFI_STRING          TmpString;
  static  EFI_STRING_ID MeMode[] = {
    STRING_TOKEN(STR_ME_FW_MODE_NORMAL_MODE),
    STRING_TOKEN(STR_ME_FW_MODE_FAILED),
    STRING_TOKEN(STR_ME_FW_MODE_ALT_DISABLED),
    STRING_TOKEN(STR_ME_FW_MODE_TEMP_DISABLED),
    STRING_TOKEN(STR_ME_FW_MODE_SECOVER),
    STRING_TOKEN(STR_ME_FW_MODE_FAILED),
    STRING_TOKEN(STR_ME_FW_MODE_FAILED),
  };
  static  UINT8 *FwMeSku[] = {
     (UINT8*) ME_FW_IMAGE_TYPE_CONSUMER_STRING,
     (UINT8*) ME_FW_IMAGE_TYPE_CORPORATE_STRING,
     (UINT8*) ME_FW_IMAGE_TYPE_UNIDENTIFIED_STRING,
  };
#ifndef PCH_ICL
  BOOLEAN              PttCapability;
  BOOLEAN              PttCurrentState;
#endif
  ME_INFO_SETUP_DATA   MeInfoSetupData;

  mHiiHandle = HiiHandle;

  DEBUG ((DEBUG_INFO, "<InitMeInfo>\n"));

  VariableSize = sizeof(ME_INFO_SETUP_DATA);
  Status = gRT->GetVariable (
                  L"MeInfoSetup",
                  &gMeInfoSetupGuid,
                  NULL,
                  &VariableSize,
                  &MeInfoSetupData
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  UnicodeSPrint (
    StrBuf,
    ME_INFO_MAX_STR_SIZE,
    L"%d.%d.%d.%d",
    MeInfoSetupData.MeMajor,
    MeInfoSetupData.MeMinor,
    MeInfoSetupData.MeHotFix,
    MeInfoSetupData.MeBuildNo
    );
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_VERSION_VALUE), StrBuf, NULL);


  if ((MeInfoSetupData.MeFirmwareInfo & ME_MODE_MASK) < sizeof (MeMode) / sizeof (EFI_STRING_ID)) {
    TmpString = HiiGetString (mHiiHandle, MeMode[(MeInfoSetupData.MeFirmwareInfo & ME_MODE_MASK)], NULL);
    if (TmpString != NULL) {
      UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%s", TmpString);
      HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_MODE_VALUE), StrBuf, NULL);
      FreePool (TmpString);
    }
  }

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID *)&mMeSetup);

  if (mMeSetup.MeImageType == ME_IMAGE_CONSUMER_SKU_FW) {
    MeFwSkuValue = FwMeSku[0];
  } else if (mMeSetup.MeImageType == ME_IMAGE_CORPORATE_SKU_FW) {
    MeFwSkuValue = FwMeSku[1];
  } else {
    MeFwSkuValue = FwMeSku[2];
  }

  UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%a", MeFwSkuValue);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_SKU_VALUE), StrBuf, NULL);

  //
  // Check if Heci device is enabled
  //
  if (HeciPciSegmentRead32 (PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    return;
  }

  UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"0x%08x", HeciPciSegmentRead32 (R_ME_HFS));
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_STATUS_1_VALUE), StrBuf, NULL);

  UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"0x%08x", HeciPciSegmentRead32 (R_ME_HFS_2));
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_STATUS_2_VALUE), StrBuf, NULL);

#ifndef PCH_ICL
  PttHeciGetCapability (&PttCapability);
  PttHeciGetState (&PttCurrentState);

  UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%d / %d", PttCapability, PttCurrentState);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_PTT_CAP_STATE_VALUE), StrBuf, NULL);
#endif
}

VOID
MeRouteConfig (
  VOID
  )
{

  ME_SETUP_STORAGE        MeSetupStorage = {0};
  ME_SETUP_STORAGE        MeBackupStorage = {0};
  UINTN                   Size;
  EFI_STATUS              Status;
  UINT32                  EnableBitmap;
  UINT32                  DisableBitmap;
  HECI_CONTROL            *Heci;

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &Heci);
  if (MeIsAfterEndOfPost () || EFI_ERROR (Status)) {
    return;
  }

  Size = sizeof (ME_SETUP_STORAGE);
  gRT->GetVariable (L"MeSetupStorage", &gEfiMeRcVariableGuid, NULL, &Size, &MeSetupStorage);
  gRT->GetVariable (L"MeBackupStorage", &gEfiMeRcVariableGuid, NULL, &Size, &MeBackupStorage);

  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*) &mMeSetup);

  if (MeSetupStorage.InvokeArbSvnCommit != MeBackupStorage.InvokeArbSvnCommit) {
    if ((mMeSetup.AutoArbSvnCommit == 0) && (MeSetupStorage.InvokeArbSvnCommit == 1)) {
      Status = HeciArbSvnCommitMsg ();
    }
  }

  if (MeSetupStorage.MeStateControl != MeBackupStorage.MeStateControl) {
    gMeClientReset = TRUE;

    if (MeSetupStorage.MeStateControl == 0) {
      Status = HeciSetMeDisableMsg ();
    } else {
      Status = HeciSetMeEnableMsg ();
    }
  }

  if (MeSetupStorage.FwUpdEnabled != MeBackupStorage.FwUpdEnabled) {
    Status = HeciSetFwUpdateState (MeSetupStorage.FwUpdEnabled);
  }

  if (MeSetupStorage.MngState != MeBackupStorage.MngState) {
    gMeClientReset = TRUE;
    if (MeSetupStorage.MngState == 1) {
      //
      // User wants to enable Manageability Features
      //
      EnableBitmap  = AMT_BITMASK;
      DisableBitmap = CLEAR_FEATURES_BITMASK;
    } else {
      //
      // User wants to disable Manageability Features
      //
      EnableBitmap  = CLEAR_FEATURES_BITMASK;
      DisableBitmap = AMT_BITMASK;
    }
    Status = HeciFwFeatureStateOverride (EnableBitmap, DisableBitmap);
  }

  if (MeSetupStorage.PttState != MeBackupStorage.PttState) {
    gMeClientReset = TRUE;
  }


  if (MeSetupStorage.DelayedAuthenticationMode != MeBackupStorage.DelayedAuthenticationMode) {
    gMeClientReset = TRUE;
    HeciSetDamState (MeSetupStorage.DelayedAuthenticationMode);
  }

  if (MeSetupStorage.FipsModeSelect != MeBackupStorage.FipsModeSelect) {
    HeciSetFipsMode (MeSetupStorage.FipsModeSelect);
  }

  //
  // Remember current FW's state to avoid sending unnecessary messages next time
  //
  gRT->SetVariable (
         L"MeBackupStorage",
         &gEfiMeRcVariableGuid,
         EFI_VARIABLE_BOOTSERVICE_ACCESS,
         sizeof (ME_SETUP_STORAGE),
         &MeSetupStorage
         );

}

/**
  This function validates the Flex Ratio setup value

  @param[in] EFI_FORM_CALLBACK_PROTOCOL   *This
  @param[in] UINT16                       KeyValue
  @param[in] EFI_IFR_DATA_ARRAY           *Data,
  @param[in] EFI_HII_CALLBACK_PACKET      **Packet

  @retval EFI_SUCCESS        The callback successfully handled the action
  @retval EFI_UNSUPPORTED    The specified action is not supported by the callback
**/
EFI_STATUS
EFIAPI
MeFormCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{

  EFI_STATUS   Status;
  BOOLEAN      PttCapability;
  BOOLEAN      PttCurrentState;
  CHAR16       StrBuf[ME_INFO_MAX_STR_SIZE];

  if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    switch (KeyValue) {
      case MNG_STATE_KEY:
        return EFI_SUCCESS;

     default:
       return EFI_UNSUPPORTED;
    }
  }

  if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
    switch (KeyValue) {

      case PTT_TRIGGER_FORM_OPEN_ACTION_KEY:
        Status = PttHeciGetCapability (&PttCapability);
        if (EFI_ERROR (Status)) {
          PttCapability = FALSE;
        }
        Status = PttHeciGetState (&PttCurrentState);
        if (EFI_ERROR (Status)) {
          PttCurrentState = FALSE;
        }

        UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%d / %d", PttCapability, PttCurrentState);
        HiiSetString (mHiiHandle, STRING_TOKEN (STR_PTT_CAP_STATE_VALUE), StrBuf, NULL);

        return EFI_SUCCESS;

      default:
         return EFI_UNSUPPORTED;
     }
  }

  return EFI_UNSUPPORTED;
}

VOID
MeExtractConfig (
  VOID
  )
{
  EFI_STATUS              Status;
  UINT32                  MeMode;
  MEFWCAPS_SKU            CurrentFeatures;
  ME_SETUP_STORAGE        MeSetupStorage;
  UINTN                   Size;
  UINT32                  VariableAttr;
  AMT_WRAPPER_PROTOCOL    *AmtWrapper;
  HECI_FW_STS4_REGISTER   MeFwSts4;
  UINT32                  MsrValue;
  CHAR16                  StrBuf[ME_INFO_MAX_STR_SIZE];
  UINT8                   FwUpdateState;
  EFI_STRING_ID           FipsMode[] = {
    STRING_TOKEN(STR_DISABLED),
    STRING_TOKEN(STR_ENABLED)
  };
  GET_FIPS_MODE_DATA      FipsModeData;
  UINT32                  NumOfEntries;
  ARB_SVN_INFO_ENTRY      *ArbSvnInfo;
  EFI_STRING              TmpString;

  AmtWrapper   = NULL;
  VariableAttr = 0;
  NumOfEntries = ARB_SVN_INFO_NUM;
  ArbSvnInfo   = NULL;

  DEBUG ((DEBUG_INFO, "MeExtractConfig\n"));

  ArbSvnInfo = AllocateZeroPool (NumOfEntries * sizeof (ARB_SVN_INFO_ENTRY));
  if (ArbSvnInfo == NULL) {
    return;
  }
  Status = HeciArbSvnGetInfoMsg (&NumOfEntries, ArbSvnInfo);
  if (!EFI_ERROR (Status)) {
    UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%d", ArbSvnInfo->MinAllowedSvn);
    HiiSetString (mHiiHandle, STRING_TOKEN (STR_ARB_SVN_MIN_VALUE), StrBuf, NULL);

    UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%d", ArbSvnInfo->ExecutingSvn);
    HiiSetString (mHiiHandle, STRING_TOKEN (STR_ARB_SVN_CURR_VALUE), StrBuf, NULL);
  }
  if (ArbSvnInfo != NULL) {
    FreePool (ArbSvnInfo);
  }

  Size = sizeof (ME_SETUP_STORAGE);
  Status = gRT->GetVariable (L"MeSetupStorage", &gEfiMeRcVariableGuid, &VariableAttr, &Size, &MeSetupStorage);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MeSetupStorage does not exist. Assuming default values.\n"));
    VariableAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    MeSetupStorage.MngState = 1;
    MeSetupStorage.FwUpdEnabled = 1;
    MeSetupStorage.MeStateControl = 0;
    MeSetupStorage.AfterEoP = 0;
    MeSetupStorage.RemoteSessionActive = 0;
    MeSetupStorage.PttState = 0;
    MeSetupStorage.BootGuardSupport = 0;
    MeSetupStorage.MeasureBoot = 0;
    MeSetupStorage.TpmDeactivate = 0;
    MeSetupStorage.DelayedAuthenticationMode = 0;
    MeSetupStorage.FipsModeSelect = 0;
  }

  HeciGetDamState (&MeSetupStorage.DelayedAuthenticationMode);

  if (IsBootGuardSupported ()) {
    MeSetupStorage.BootGuardSupport = 1;
  }

  MsrValue = (UINT32) AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);
  if (MsrValue & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) {
    MeSetupStorage.MeasureBoot = 1;
  }

  if (MeIsAfterEndOfPost ()) {
    MeSetupStorage.AfterEoP = 1;
  } else {
    MeSetupStorage.AfterEoP = 0;
  }

  MeMode = ME_MODE_FAILED;
  MeBiosGetMeMode (&MeMode);

  if (MeMode == ME_MODE_NORMAL) {
    MeSetupStorage.MeStateControl = 1;
  }

  if (!MeSetupStorage.MeStateControl || MeSetupStorage.AfterEoP) {
    goto Done;
  }

  Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID**) &AmtWrapper);
  if (!EFI_ERROR (Status)) {
    MeSetupStorage.RemoteSessionActive = AmtWrapper->IsStorageRedirectionEnabled () ||
                                         AmtWrapper->IsSolEnabled () ||
                                         AmtWrapper->IsKvmEnabled ();
  }

  Status = HeciGetFwFeatureStateMsg (&CurrentFeatures);
  if (!EFI_ERROR (Status)) {
    MeSetupStorage.MngState   = !!CurrentFeatures.Fields.Amt;
    MeSetupStorage.PttState   = !!CurrentFeatures.Fields.PTT;
  }

  MeFwSts4.ul = HeciPciSegmentRead32 (R_ME_HFS_4);
  if (MeFwSts4.ul != 0xFFFFFFFF) {
    MeSetupStorage.TpmDeactivate = (UINT8) MeFwSts4.r.dTpm12Deactivated;
  }

  //
  // Skip Local Firmware update if it is not Consumer sku or not in normal mode
  //
  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*) &mMeSetup);

  ASSERT_EFI_ERROR (Status);

  if (mMeSetup.MeImageType != IntelMeConsumerFw) {
    goto Done;
  }

  Status = HeciGetFwUpdateState (&FwUpdateState);
  if (!EFI_ERROR (Status)) {
    MeSetupStorage.FwUpdEnabled = FwUpdateState;
  }

  Status = HeciGetFipsMode (&FipsModeData);
  if (!EFI_ERROR (Status)) {
    TmpString = HiiGetString (mHiiHandle, FipsMode[!!FipsModeData.FipsMode], NULL);
    if (TmpString != NULL) {
      UnicodeSPrint (StrBuf, ME_INFO_MAX_STR_SIZE, L"%s", TmpString);
      HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_FIPS_CURRENT_MODE_VALUE), StrBuf, NULL);
      FreePool (TmpString);
    }

    UnicodeSPrint (
      StrBuf,
      ME_INFO_MAX_STR_SIZE,
      L"%d.%d.%d.%d",
      FipsModeData.CryptoVersion.Major,
      FipsModeData.CryptoVersion.Minor,
      FipsModeData.CryptoVersion.Build,
      FipsModeData.CryptoVersion.Hotfix
      );
    HiiSetString (mHiiHandle, STRING_TOKEN (STR_ME_FW_FIPS_CRYPTO_VER_VALUE), StrBuf, NULL);
  }

Done:
  Status = gRT->SetVariable (
                  L"MeSetupStorage",
                  &gEfiMeRcVariableGuid,
                  VariableAttr,
                  sizeof (ME_SETUP_STORAGE),
                  &MeSetupStorage
                  );
  Status = gRT->SetVariable (
                  L"MeBackupStorage",
                  &gEfiMeRcVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (ME_SETUP_STORAGE),
                  &MeSetupStorage
                  );

  DEBUG ((DEBUG_INFO, "MeExtractConfig %d %d %d %d %d %d %d %d %d %d %d\n",
      MeSetupStorage.MngState,
      MeSetupStorage.FwUpdEnabled,
      MeSetupStorage.MeStateControl,
      MeSetupStorage.AfterEoP,
      MeSetupStorage.RemoteSessionActive,
      MeSetupStorage.PttState,
      MeSetupStorage.BootGuardSupport,
      MeSetupStorage.MeasureBoot,
      MeSetupStorage.TpmDeactivate,
      MeSetupStorage.DelayedAuthenticationMode,
      MeSetupStorage.FipsModeSelect));
}

/**
  Extract the ME storage configuration (only for WS).

  @param[in]  Request      Request packet
  @param[out] Progress     Progress string
  @param[out] Results      Results string

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
MeExtractConfigClientWrapper (
  IN     CONST EFI_STRING  Request,
     OUT       EFI_STRING  *Progress,
     OUT       EFI_STRING  *Results
  )
{
  EFI_STATUS              Status = EFI_INVALID_PARAMETER;
  ME_SETUP_STORAGE        MeSetupStorage;
  UINT32                  VariableAttr;
  UINTN                   Size;
  UINTN                   BufferSize;

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  ASSERT (Request  != NULL);
  ASSERT (Progress != NULL);
  ASSERT (Results  != NULL);

  if (Request == NULL || Progress == NULL || Results == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - Invalid Parameters\n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    goto MeExtractConfigWrapperExit;
  }

  MeExtractConfig ();

  Size = sizeof (ME_SETUP_STORAGE);
  Status = gRT->GetVariable (L"MeSetupStorage", &gEfiMeRcVariableGuid, &VariableAttr, &Size, &MeSetupStorage);

  *Progress = Request;
  BufferSize = sizeof (ME_SETUP_STORAGE);

  Status = mHiiConfigRouting->BlockToConfig (
                                mHiiConfigRouting,
                                Request,
                                (VOID *) &MeSetupStorage,
                                BufferSize,
                                Results,
                                Progress
                                );
  ASSERT_EFI_ERROR (Status);

MeExtractConfigWrapperExit:
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Route the results of processing forms to MeSetupStorage and call the MeRouteConfig function.

  @param[in]  Configuration  Configuration string
  @param[out] Progress       Progress string

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
MeRouteConfigClientWrapper (
  IN     CONST EFI_STRING  Configuration,
     OUT       EFI_STRING  *Progress
  )
{
  EFI_STATUS               Status = EFI_INVALID_PARAMETER;
  ME_SETUP_STORAGE         MeSetupStorage;
  UINTN                    BufferSize;
  UINTN                    Size;
  UINT32                   VariableAttr;

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  ASSERT (Configuration  != NULL);
  ASSERT (Progress != NULL);

  if (Configuration == NULL || Progress == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - Invalid Parameters\n", __FUNCTION__));
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    goto MeRouteConfigWrapperExit;
  }

  *Progress = Configuration;
  Status = gRT->GetVariable (
                  L"MeSetupStorage",
                  &gEfiMeRcVariableGuid,
                  &VariableAttr,
                  &Size,
                  &MeSetupStorage
                  );

  //
  // Convert <ConfigResp> to buffer data.
  //
  BufferSize = sizeof (MeSetupStorage);
  Status = mHiiConfigRouting->ConfigToBlock (
                               mHiiConfigRouting,
                               Configuration,
                               (VOID *) &MeSetupStorage,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - mHiiConfigRouting->ConfigToBlock() returns: %r\n", __FUNCTION__, Status));
    goto MeRouteConfigWrapperExit;
  }

  Status = gRT->SetVariable (
                  L"MeSetupStorage",
                  &gEfiMeRcVariableGuid,
                  VariableAttr,
                  sizeof (ME_SETUP_STORAGE),
                  &MeSetupStorage
                  );

  MeRouteConfig ();

MeRouteConfigWrapperExit:
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
  return Status;
}