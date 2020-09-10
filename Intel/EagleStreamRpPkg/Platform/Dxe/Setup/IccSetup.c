/**@file

@copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation.

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

#ifdef WS_ICC_SUPPORT

#include <Base.h>
#include <Protocol/HiiConfigAccess.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/DxeMeLib.h>
#include <Library/MeTypeLib.h>
#include <Protocol/Wdt.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/ConfigBlockLib.h>
#include <Library/MeUtilsLib.h>
#include "IccSetupData.h"
#include "SetupPlatform.h"

#define HZ_TO_10KHZ            10000
#define ICC_INFO_MAX_STR_SIZE  1024

#pragma pack(1)

typedef struct _ICC_PROFILE_DATA {
  UINT8 NumProfiles;
  UINT8 Profile;
  UINT8 ProfileSelectionAllowed;
  UINT8 RegLock;
} ICC_PROFILE_DATA;

#pragma pack()
static ICC_CLOCK_SETTINGS mSettingsBclk;
static BOOLEAN            mProfileChanged = FALSE;
static ICC_PROFILE_DATA   mIccProfileData = {0};

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HII_HANDLE             gHiiHandle;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                    gIccReset = FALSE;

/**
  Construct Request String (L"&OFFSET=%x&WIDTH=%x") base on the input Offset and Width.
  If the input RequestString is not NULL, new request will be cat at the end of it. The full
  request string will be constructed and return. Caller is responsible to free it.

  @param[in] RequestString   Current request string.
  @param[in] Offset          Offset of data in Storage.
  @param[in] Width           Width of data.

  @return Request string with input Offset and Width.
**/
EFI_STRING
EFIAPI
HiiConstructRequestString (
  IN EFI_STRING      RequestString, OPTIONAL
  IN UINTN           Offset,
  IN UINTN           Width
  )
{
  CHAR16             RequestElement[30];
  UINTN              StringLength;
  EFI_STRING         NewString;

  StringLength = UnicodeSPrint (
                   RequestElement,
                   sizeof (RequestElement),
                   L"&OFFSET=%x&WIDTH=%x",
                   Offset,
                   Width
                   );

  if (RequestString != NULL) {
    StringLength = StringLength + StrLen (RequestString);
  }

  NewString = AllocateZeroPool ((StringLength + 1) * sizeof (CHAR16));

  if (NewString == NULL) {
    return NULL;
  }

  if (RequestString != NULL) {
    StrCatS (NewString, StringLength + 1, RequestString);
    FreePool (RequestString);
  }

  StrCatS (NewString, StringLength + 1, RequestElement);
  return NewString;
}

////////////////////////////////////////////////////////////////////////////////////////////////

/**
  Update the Setup variable with values of frequency and spread.

  @param[in]   ClockSettings  Pointer to the ICC_CLOCK_SETTINGS buffer.
  @param[out]  SetupPtr       Pointer to the ICC_SETUP_DATA buffer.
**/
VOID
UpdateClockData (
  IN     ICC_CLOCK_SETTINGS             *ClockSettings,
     OUT ICC_SETUP_DATA                 *IccSetupPtr
  )
{
  IccSetupPtr->Frequency = (UINT16) ((ClockSettings->Freq + (HZ_TO_10KHZ/2)) / HZ_TO_10KHZ);
  IccSetupPtr->Spread = ClockSettings->SscPercent;
}

/**
  Update the ICC submenu strings.

  @param[in] RequestedClockSettings   Pointer to ICC_CLOCK_SETTINGS buffer.
**/
VOID
UpdateSubmenuStrings (
  IN ICC_CLOCK_SETTINGS *ClockSettings
  )
{
  CHAR16  *TempBuffer;
  UINT16  StrFreqRange;
  UINT16  StrMaxSpread;
  UINT16  StrSpreadAdjust;
  CHAR16  StrBuf[ICC_INFO_MAX_STR_SIZE];

  StrFreqRange    =  STR_ICC_CLOCK1_FREQ_RANGE_TEXT;
  StrMaxSpread    =  STR_ICC_CLOCK1_MAX_SPREAD_TEXT;
  StrSpreadAdjust =  STR_ICC_CLOCK1_SPREAD_ADJUSTMENTS_TEXT;

  //
  // Frequency Range
  //
  UnicodeSPrint (
    StrBuf,
    ICC_INFO_MAX_STR_SIZE,
    L"%d.%02d - %d.%02d MHz",
    ClockSettings->MinFreq / 1000000,
    ClockSettings->MinFreq % 1000000,
    ClockSettings->MaxFreq / 1000000,
    ClockSettings->MaxFreq % 1000000
    );
  HiiSetString (gHiiHandle, STRING_TOKEN (StrFreqRange), StrBuf, NULL);

  //
  // Maximum Spread %
  //
  UnicodeSPrint (
    StrBuf,
    ICC_INFO_MAX_STR_SIZE,
    L"%d.%02d%%",
    ClockSettings->MaxSscPercent / 100,
    ClockSettings->MaxSscPercent % 100
    );
  HiiSetString (gHiiHandle, STRING_TOKEN (StrMaxSpread), StrBuf, NULL);

  //
  // Spread Mode Adjustments
  //
  if (ClockSettings->SscMode != ICC_SSC_NONE) {
    TempBuffer = HiiGetString (mAdvancedHiiHandle, STRING_TOKEN (STR_ICC_SSC_CHANGE_ALLOWED), NULL);
  } else {
    TempBuffer = HiiGetString (mAdvancedHiiHandle, STRING_TOKEN (STR_ICC_SSC_NOTHING_ALLOWED), NULL);
  }

  UnicodeSPrint (StrBuf, ICC_INFO_MAX_STR_SIZE, L"%s", TempBuffer);
  HiiSetString (gHiiHandle, STRING_TOKEN (StrSpreadAdjust), StrBuf, NULL);
  FreePool (TempBuffer);
}

/**
  Setup callback executed when user selects a ICC Profile from the BIOS UI.
  Changes visibility of other options.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action                 Specifies the type of action taken by the browser.
  @param[in]  QuestionId             A unique value which is sent to the original exporting driver
                                     so that it can identify the type of data to expect.
  @param[in]  Type                   The type of value for the question.
  @param[in]  Value                  A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest          On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS                The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES       Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR           The variable could not be saved.
  @retval EFI_UNSUPPORTED            The specified Action is not supported by the callback or happened after END_OF_POST.
  @retval EFI_INVALID_PARAMETER      The parameter of Value or ActionRequest is invalid.
**/
EFI_STATUS
EFIAPI
IccProfileCallback (
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN     EFI_BROWSER_ACTION                   Action,
  IN     EFI_QUESTION_ID                      KeyValue,
  IN     UINT8                                Type,
  IN     EFI_IFR_TYPE_VALUE                   *Value,
     OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  UINTN          Size;
  ICC_SETUP_DATA IccSetup;
  EFI_STRING     RequestString;

  Size = sizeof (ICC_SETUP_DATA);
  RequestString = NULL;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action < EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    return EFI_UNSUPPORTED;
  }

  if (Action >= EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    if (mIccProfileData.ProfileSelectionAllowed) {
      //
      // Perform change only if profile is selectable by BIOS
      //
      Value->u8 = 0;
    }
  }

  //
  // sanity check: can't choose profile that doesn't exist
  //
  if (Value->u8 > mIccProfileData.NumProfiles - 1) {
    Value->u8 = mIccProfileData.NumProfiles - 1;
  }

  //
  // when profile is changed, most other icc options can't be changed before reboot. hide those options.
  //
  RequestString = HiiConstructRequestString (
                    RequestString,
                    OFFSET_OF (ICC_SETUP_DATA, AllowAdvancedOptions),
                    sizeof (IccSetup.AllowAdvancedOptions)
                    );

  if (mProfileChanged || (Value->u8 != mIccProfileData.Profile)) {
    HiiGetBrowserData (&gEfiMeRcVariableGuid, ICC_SETUP_DATA_C_NAME, Size, (UINT8*) &IccSetup);
    IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
    HiiSetBrowserData (&gEfiMeRcVariableGuid, ICC_SETUP_DATA_C_NAME, sizeof (ICC_SETUP_DATA), (UINT8*) &IccSetup, RequestString);
  } else if (!mProfileChanged && (Value->u8 == mIccProfileData.Profile)) {
    HiiGetBrowserData (&gEfiMeRcVariableGuid, ICC_SETUP_DATA_C_NAME, Size, (UINT8*) &IccSetup);
    IccSetup.AllowAdvancedOptions = DISPLAY;
    HiiSetBrowserData (&gEfiMeRcVariableGuid, ICC_SETUP_DATA_C_NAME, sizeof (ICC_SETUP_DATA), (UINT8*) &IccSetup, RequestString);
  }
  FreePool (RequestString);
  return EFI_SUCCESS;
}

/**
  Dump the values of ICC setup variable.

  @param[in]  IccSetup  The ICC setup variable.
**/
VOID
DebugDumpConfig (
  IN ICC_SETUP_DATA  IccSetup
  )
{
  DEBUG ((DEBUG_INFO, "Frequency           : %d\n", IccSetup.Frequency));
  DEBUG ((DEBUG_INFO, "Spread              : %d\n", IccSetup.Spread));
  DEBUG ((DEBUG_INFO, "ShowFrequency       : %d\n", IccSetup.ShowFrequency));
  DEBUG ((DEBUG_INFO, "ShowSpread          : %d\n", IccSetup.ShowSpread));
  DEBUG ((DEBUG_INFO, "ShowProfile         : %d\n", IccSetup.ShowProfile));
  DEBUG ((DEBUG_INFO, "Profile             : %d\n", IccSetup.Profile));
  DEBUG ((DEBUG_INFO, "AllowAdvancedOptions: %d\n", IccSetup.AllowAdvancedOptions));
}

/**
  Initialize ICC strings.

  @param[in] HiiHandle    HiiHandle Handle to HII database
  @param[in] Class        Indicates the setup class
**/
VOID
InitIccStrings (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Class
  )
{
  gHiiHandle = HiiHandle;
}


/**
  Get ICC clock settings from the CSME (via HECI interface) into mSettingsBclk variable.
**/
VOID
GetClockSettings (
  VOID
  )
{
  EFI_STATUS            Status;
  static BOOLEAN        StringInit = FALSE;

  if (MeIsAfterEndOfPost () || StringInit) {
    return;
  }

  //
  //  Get BCLK Settings
  //
  ZeroMem (&mSettingsBclk, sizeof (ICC_CLOCK_SETTINGS));

  mSettingsBclk.ReqClock = CLOCK1;
  Status = HeciGetIccClockSettings (&mSettingsBclk);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "(ICC) Get Clock Settings message failed, clock 1. Status = %r\n", Status));
    return;
  }

  StringInit = TRUE;
}

/**
  Update the visibility of some ICC setup options.

  @param[out]  SetupPtr   Pointer to ICC_SETUP_DATA.
**/
VOID
UpdateVisibility (
  OUT ICC_SETUP_DATA* SetupPtr
  )
{
  UINT32              MeMode;
  EFI_STATUS          Status;
  BOOLEAN             EopSent;

  EopSent = MeIsAfterEndOfPost ();
  Status = MeBiosGetMeMode (&MeMode);

  if (EFI_ERROR (Status)) {
    return;
  }

  if (EopSent || (MeMode != ME_MODE_NORMAL)) {
    SetupPtr->AllowAdvancedOptions = DONT_DISPLAY;
  } else {
    SetupPtr->AllowAdvancedOptions = DISPLAY;
  }
  if (EopSent || mIccProfileData.ProfileSelectionAllowed == DONT_DISPLAY || (MeMode != ME_MODE_NORMAL)) {
    SetupPtr->ShowProfile = DONT_DISPLAY;
  } else {
    SetupPtr->ShowProfile = DISPLAY;
  }

  // EBG PCH doesn't support dynamic changing of ICC settings
  SetupPtr->ShowFrequency = DONT_DISPLAY;
  SetupPtr->ShowSpread    = DONT_DISPLAY;
}

/**
  This function extracts ICC setup data basing on data retrieved from CSME.
**/
VOID
IccExtractConfig (
  VOID
  )
{
  ICC_SETUP_DATA   IccSetup;
  EFI_STATUS       Status;
  UINTN            Size;
  UINT8            IccProfile;
  UINT8            NumIccProfiles;
  UINT8            IccSelectAllowed;
  ICC_PROFILE_DESC *ProfileDescriptions;
  HECI_CONTROL     *Heci;

  ZeroMem ((VOID*) &IccSetup, sizeof (ICC_SETUP_DATA));

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &Heci);
  if (EFI_ERROR (Status)) {
    IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
    IccSetup.ShowProfile = DONT_DISPLAY;
    gRT->SetVariable (
            ICC_SETUP_DATA_C_NAME,
            &gEfiMeRcVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS,
            sizeof (IccSetup),
            &IccSetup
            );
    return;
  }

  Size = sizeof (IccSetup);
  Status = gRT->GetVariable (ICC_SETUP_DATA_C_NAME, &gEfiMeRcVariableGuid, NULL, &Size, &IccSetup);
  if (!EFI_ERROR (Status)) {
    //
    // if the variable exists, it means init was completed. Check for EoP only and hide everything if it's after EoP.
    //
    if (MeIsAfterEndOfPost ()) {
      IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
      IccSetup.ShowProfile = DONT_DISPLAY;
      gRT->SetVariable (
             ICC_SETUP_DATA_C_NAME,
             &gEfiMeRcVariableGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS,
             sizeof(IccSetup),
             &IccSetup
             );
    }
    return;
  }

  GetClockSettings ();

  //
  // Get IccProfile and update IccSetup
  //
  DEBUG ((DEBUG_INFO, "(ICC) Get IccProfile Start...\n"));
  ProfileDescriptions = (ICC_PROFILE_DESC*) AllocateZeroPool (sizeof (ICC_PROFILE_DESC) * MAX_NUM_ICC_PROFILES);
  if (ProfileDescriptions == NULL) {
    DEBUG ((DEBUG_ERROR, "(ICC) Failed to Allocate Memory Resource\n"));
    IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
    IccSetup.ShowProfile = DONT_DISPLAY;
    gRT->SetVariable (
            ICC_SETUP_DATA_C_NAME,
            &gEfiMeRcVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS,
            sizeof (IccSetup),
            &IccSetup
            );
    return;
  }
  Status = HeciGetIccProfile (&IccProfile, &NumIccProfiles, &IccSelectAllowed, ProfileDescriptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(ICC) Failed to Get ICC Profiles Status = %r\n", Status));
    IccSetup.AllowAdvancedOptions = DONT_DISPLAY;
    IccSetup.ShowProfile = DONT_DISPLAY;
    gRT->SetVariable (
            ICC_SETUP_DATA_C_NAME,
            &gEfiMeRcVariableGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS,
            sizeof (IccSetup),
            &IccSetup
            );
    return;
  }

  mIccProfileData.Profile = IccProfile;

  if (IccSelectAllowed) {
    mIccProfileData.ProfileSelectionAllowed = DISPLAY;
  } else {
    mIccProfileData.ProfileSelectionAllowed = DONT_DISPLAY;
  }

  if (NumIccProfiles <= 1) {
    // Firmware is unconfgured, and there are no profiles to select
    mIccProfileData.ProfileSelectionAllowed = DONT_DISPLAY;
  }

  mIccProfileData.NumProfiles = NumIccProfiles;
  mIccProfileData.RegLock = 0;

  IccSetup.Profile = IccProfile;

  UpdateClockData (&mSettingsBclk, &IccSetup);
  UpdateSubmenuStrings (&mSettingsBclk);
  UpdateVisibility (&IccSetup);

  gRT->SetVariable (
         ICC_SETUP_DATA_C_NAME,
         &gEfiMeRcVariableGuid,
         EFI_VARIABLE_BOOTSERVICE_ACCESS,
         sizeof (IccSetup),
         &IccSetup
         );

  DebugDumpConfig (IccSetup);
}

/**
  The function decides whether there is a need to update ICC profile. If so, then it will send request
  to the CSME to change the profile.

  @param[in]  Profile   Request value of Profile.
**/
VOID
MaybeChangeProfile (
  UINT8 Profile
  )
{
  static UINT8 LastSavedProfile;

  if (!mProfileChanged) {
    LastSavedProfile = mIccProfileData.Profile;
  }
  if (Profile != LastSavedProfile) {
    DEBUG ((DEBUG_INFO, "(ICC) Changing profile: old %d, new %d\n", LastSavedProfile, Profile));
    HeciSetIccProfile (Profile);
    LastSavedProfile = Profile;
    mProfileChanged = TRUE;
    gIccReset = TRUE;
  }
}

/**
  This function routes ICC setup data. In case of any change, the function will update ICC settings.
**/
VOID
IccRouteConfig (
  VOID
  )
{
  EFI_STATUS                        Status;
  ICC_SETUP_DATA                    IccSetup;
  UINTN                             Size = sizeof (ICC_SETUP_DATA);
  HECI_CONTROL                      *Heci;

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &Heci);

  ZeroMem ((VOID*) &IccSetup, sizeof (ICC_SETUP_DATA));
  if (MeIsAfterEndOfPost () || EFI_ERROR (Status)) {
    return;
  }

  gRT->GetVariable (ICC_SETUP_DATA_C_NAME, &gEfiMeRcVariableGuid, NULL, &Size, &IccSetup);
  MaybeChangeProfile (IccSetup.Profile);
}

/**
  Route the results of processing forms to IccSetup data and call the IccRouteConfig function.

  @param[in]  Configuration  Configuration string
  @param[out] Progress       Progress string

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
IccRouteConfigWrapper (
  IN     CONST EFI_STRING  Configuration,
     OUT       EFI_STRING  *Progress
  )
{
  EFI_STATUS               Status;
  ICC_SETUP_DATA           IccSetup;
  UINTN                    BufferSize;
  UINTN                    Size;
  UINT32                   VariableAttr;

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  ASSERT (Configuration  != NULL);
  ASSERT (Progress != NULL);

  if (Configuration == NULL || Progress == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - Invalid Parameters\n", __FUNCTION__));
    Status = EFI_INVALID_PARAMETER;
    ASSERT_EFI_ERROR (Status);
    goto IccSetupWrapperExit;
  }

  *Progress = Configuration;
  Status = gRT->GetVariable (
                  ICC_SETUP_DATA_C_NAME,
                  &gEfiMeRcVariableGuid,
                  &VariableAttr,
                  &Size,
                  &IccSetup
                  );

  //
  // Convert <ConfigResp> to buffer data.
  //
  BufferSize = sizeof (IccSetup);
  Status = mHiiConfigRouting->ConfigToBlock (
                                mHiiConfigRouting,
                                Configuration,
                                (VOID*) &IccSetup,
                                 &BufferSize,
                                Progress
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - mHiiConfigRouting->ConfigToBlock() returns: %r\n", __FUNCTION__, Status));
    goto IccSetupWrapperExit;
  }

  Status = gRT->SetVariable (
                  ICC_SETUP_DATA_C_NAME,
                  &gEfiMeRcVariableGuid,
                  VariableAttr,
                  sizeof (ICC_SETUP_DATA),
                  &IccSetup
                  );

  IccRouteConfig ();

IccSetupWrapperExit:
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
  return Status;
}



/**
  Extract the ICC setup configuration.

  @param[in]  Request      Request packet
  @param[out] Progress     Progress string
  @param[out] Results      Results string

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
IccExtractConfigWrapper (
  IN     CONST EFI_STRING  Request,
     OUT       EFI_STRING  *Progress,
     OUT       EFI_STRING  *Results
  )
{
  EFI_STATUS              Status;
  ICC_SETUP_DATA          IccSetup;
  UINT32                  VariableAttr;
  UINTN                   Size;
  UINTN                   BufferSize;

  DEBUG ((DEBUG_INFO, "%a () - start\n", __FUNCTION__));

  ASSERT (Request  != NULL);
  ASSERT (Progress != NULL);
  ASSERT (Results  != NULL);

  if (Request == NULL || Progress == NULL || Results == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - Invalid Parameters\n", __FUNCTION__));
    Status = EFI_INVALID_PARAMETER;
    ASSERT_EFI_ERROR (Status);
    goto IccExtractConfigWrapperExit;
  }

  IccExtractConfig ();

  Size = sizeof (ICC_SETUP_DATA);
  Status = gRT->GetVariable (ICC_SETUP_DATA_C_NAME, &gEfiMeRcVariableGuid, &VariableAttr, &Size, &IccSetup);

  *Progress = Request;
  BufferSize = sizeof (ICC_SETUP_DATA);

  Status = mHiiConfigRouting->BlockToConfig (
                                mHiiConfigRouting,
                                Request,
                                (VOID*) &IccSetup,
                                BufferSize,
                                Results,
                                Progress
                                );
  ASSERT_EFI_ERROR (Status);

IccExtractConfigWrapperExit:
  DEBUG ((DEBUG_INFO, "%a () - end, status: %r\n", __FUNCTION__, Status));
  return Status;
}
#endif // WS_ICC_SUPPORT
