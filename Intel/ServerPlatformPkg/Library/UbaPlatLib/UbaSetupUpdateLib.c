/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 Intel Corporation. <BR>

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

#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>

#include <Library/UbaSetupUpdateLib.h>

#include <Protocol/UbaCfgDb.h>



#define PLATFORM_SETUP_INTERNAL_VAR_NAME    L"PlatformSetupInternal"

// {F8F46407-7114-4dc0-8374-72DAD4602BD0}
STATIC EFI_GUID gPlatformSetupInternalDataGuid =
{ 0xf8f46407, 0x7114, 0x4dc0, { 0x83, 0x74, 0x72, 0xda, 0xd4, 0x60, 0x2b, 0xd0 } };


typedef struct
{
  UINT32                                    Signature;
  UINT32                                    Version;

  EFI_HII_HANDLE                            AdvancedHiiHandle;
  EFI_HII_HANDLE                            ErrorManagerHiiHandle;
  EFI_HII_HANDLE                            MainHiiHandle;

  EFI_HII_CONFIG_ACCESS_PROTOCOL            *MainConfigAccess;
  EFI_HII_CONFIG_ACCESS_PROTOCOL            *AdvancedConfigAccess;
  EFI_HII_CONFIG_ACCESS_PROTOCOL            *EventLogConfigAccess;

} PLATFORM_SETUP_INTERNAL_DATA;

extern EFI_HII_HANDLE                  mAdvancedHiiHandle;
extern EFI_HII_HANDLE                  mErrorManagerHiiHandle;
extern EFI_HII_HANDLE                  mMainHiiHandle;

extern EFI_HII_CONFIG_ACCESS_PROTOCOL  mMainConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL  mAdvancedConfigAccess;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL  mEventLogConfigAccess;

EFI_STATUS
PlatformInitSetupUpdateData (
  VOID
)
{
  EFI_STATUS                            Status;
  PLATFORM_SETUP_INTERNAL_DATA          UpdateInternal;

  UpdateInternal.AdvancedConfigAccess = &mAdvancedConfigAccess;
  UpdateInternal.AdvancedHiiHandle    = mAdvancedHiiHandle;

  UpdateInternal.ErrorManagerHiiHandle  = mErrorManagerHiiHandle;
  UpdateInternal.EventLogConfigAccess   = &mEventLogConfigAccess;

  UpdateInternal.MainConfigAccess       = &mMainConfigAccess;
  UpdateInternal.MainHiiHandle          = mMainHiiHandle;

  Status = gRT->SetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof(PLATFORM_SETUP_INTERNAL_DATA),
                  &UpdateInternal
                  );

  return EFI_SUCCESS;
}

EFI_STATUS
PlatformUpdateFormBeforeSetup (
  VOID
)
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_SETUP_UPDATE_DATA              SetupUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (SetupUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformSetupUpdateConfigDataGuid,
                                    &SetupUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //ASSERT (SetupUpdateTable.Signature == PLATFORM_SETUP_UPDATE_SIGNATURE);
  //ASSERT (SetupUpdateTable.Version == PLATFORM_SETUP_UPDATE_VERSION);

  if (SetupUpdateTable.UpdateFormBeforeSetup != NULL) {
    return SetupUpdateTable.UpdateFormBeforeSetup ();
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
PlatformFormUpdateCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_STATUS                              Status;

  UBA_CONFIG_DATABASE_PROTOCOL      *UbaConfigProtocol = NULL;
  UINTN                                   DataLength = 0;
  PLATFORM_SETUP_UPDATE_DATA              SetupUpdateTable;

  Status = gBS->LocateProtocol (
                  &gUbaConfigDatabaseProtocolGuid,
                  NULL,
                  &UbaConfigProtocol
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DataLength  = sizeof (SetupUpdateTable);
  Status = UbaConfigProtocol->GetData (
                                    UbaConfigProtocol,
                                    &gPlatformSetupUpdateConfigDataGuid,
                                    &SetupUpdateTable,
                                    &DataLength
                                    );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //ASSERT (SetupUpdateTable.Signature == PLATFORM_SETUP_UPDATE_SIGNATURE);
  //ASSERT (SetupUpdateTable.Version == PLATFORM_SETUP_UPDATE_VERSION);

  if (SetupUpdateTable.FormUpdateCallback != NULL) {
    return SetupUpdateTable.FormUpdateCallback (This, Action, QuestionId, Type, Value, ActionRequest);
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
PlatformGetMainHiiHandle (
  IN  EFI_HII_HANDLE                          *HiiHandle
  )
{
  EFI_STATUS                                Status;
  UINTN                                     VariableSize;

  PLATFORM_SETUP_INTERNAL_DATA              UpdateInternal;

  VariableSize = sizeof (PLATFORM_SETUP_INTERNAL_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  NULL,
                  &VariableSize,
                  &UpdateInternal
                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *HiiHandle = UpdateInternal.MainHiiHandle;

  return EFI_SUCCESS;
}

EFI_STATUS
PlatformGetAdvanceHiiHandle (
  IN  EFI_HII_HANDLE                          *HiiHandle
  )
{
  EFI_STATUS                                Status;
  UINTN                                     VariableSize;

  PLATFORM_SETUP_INTERNAL_DATA              UpdateInternal;

  VariableSize = sizeof (PLATFORM_SETUP_INTERNAL_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  NULL,
                  &VariableSize,
                  &UpdateInternal
                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *HiiHandle = UpdateInternal.AdvancedHiiHandle;

  return EFI_SUCCESS;
}


EFI_STATUS
PlatformGetErrorManagerHiiHandle (
  IN  EFI_HII_HANDLE                          *HiiHandle
  )
{
  EFI_STATUS                                Status;
  UINTN                                     VariableSize;

  PLATFORM_SETUP_INTERNAL_DATA              UpdateInternal;

  VariableSize = sizeof (PLATFORM_SETUP_INTERNAL_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  NULL,
                  &VariableSize,
                  &UpdateInternal
                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *HiiHandle = UpdateInternal.ErrorManagerHiiHandle;

  return EFI_SUCCESS;
}

BOOLEAN
PlatformIsMainForm (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *ConfigAccess
  )
{
  EFI_STATUS                                Status;
  UINTN                                     VariableSize;

  PLATFORM_SETUP_INTERNAL_DATA              UpdateInternal;

  VariableSize = sizeof (PLATFORM_SETUP_INTERNAL_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  NULL,
                  &VariableSize,
                  &UpdateInternal
                  );

  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  if (ConfigAccess == UpdateInternal.MainConfigAccess) {
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
PlatformIsAdvanceForm (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *ConfigAccess
  )
{
  EFI_STATUS                                Status;
  UINTN                                     VariableSize;

  PLATFORM_SETUP_INTERNAL_DATA              UpdateInternal;

  VariableSize = sizeof (PLATFORM_SETUP_INTERNAL_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  NULL,
                  &VariableSize,
                  &UpdateInternal
                  );

  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  if (ConfigAccess == UpdateInternal.AdvancedConfigAccess) {
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
PlatformIsErrorManagerForm (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *ConfigAccess
  )
{
  EFI_STATUS                                Status;
  UINTN                                     VariableSize;

  PLATFORM_SETUP_INTERNAL_DATA              UpdateInternal;

  VariableSize = sizeof (PLATFORM_SETUP_INTERNAL_DATA);
  Status = gRT->GetVariable (
                  PLATFORM_SETUP_INTERNAL_VAR_NAME,
                  &gPlatformSetupInternalDataGuid,
                  NULL,
                  &VariableSize,
                  &UpdateInternal
                  );

  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  if (ConfigAccess == UpdateInternal.EventLogConfigAccess) {
    return TRUE;
  }

  return FALSE;
}


