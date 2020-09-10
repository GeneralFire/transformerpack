/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2013 Intel Corporation. <BR>

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

#ifndef _PLATFORM_SETUP_UPDATE_LIB_H_
#define _PLATFORM_SETUP_UPDATE_LIB_H_


#include <Base.h>
#include <Uefi.h>

#include <Protocol/HiiConfigAccess.h>

#define PLATFORM_SETUP_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'S', 'E', 'T')
#define PLATFORM_SETUP_UPDATE_VERSION    01


// {CD1F9574-DD03-4196-96AD-4965146F9665}
STATIC EFI_GUID  gPlatformSetupUpdateConfigDataGuid =
{ 0xcd1f9574, 0xdd03, 0x4196, { 0x96, 0xad, 0x49, 0x65, 0x14, 0x6f, 0x96, 0x65 } };


typedef
EFI_STATUS
(*PLATFORM_SETUP_UPDATE_FORM_BEFORE_SETUP) (
  VOID
);

typedef
EFI_STATUS
(*PLATFORM_SETUP_FORM_UPDATE_CALLBACK) (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  );

typedef struct
{
  UINT32                                    Signature;
  UINT32                                    Version;

  PLATFORM_SETUP_UPDATE_FORM_BEFORE_SETUP   UpdateFormBeforeSetup;
  PLATFORM_SETUP_FORM_UPDATE_CALLBACK       FormUpdateCallback;

} PLATFORM_SETUP_UPDATE_DATA;

EFI_STATUS
PlatformUpdateFormBeforeSetup (
  VOID
);

EFI_STATUS
PlatformFormUpdateCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  );

EFI_STATUS
PlatformInitSetupUpdateData (
  VOID
);

EFI_STATUS
PlatformGetMainHiiHandle (
  IN  EFI_HII_HANDLE                          *HiiHandle
  );

EFI_STATUS
PlatformGetAdvanceHiiHandle (
  IN  EFI_HII_HANDLE                          *HiiHandle
  );

EFI_STATUS
PlatformGetErrorManagerHiiHandle (
  IN  EFI_HII_HANDLE                          *HiiHandle
  );

BOOLEAN
PlatformIsMainForm (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *ConfigAccess
  );

BOOLEAN
PlatformIsAdvanceForm (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *ConfigAccess
  );

BOOLEAN
PlatformIsErrorManagerForm (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *ConfigAccess
  );


#endif //_PLATFORM_SETUP_UPDATE_LIB_H_
