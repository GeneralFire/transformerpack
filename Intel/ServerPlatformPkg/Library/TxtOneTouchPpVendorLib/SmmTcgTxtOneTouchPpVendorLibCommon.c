/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015-2019 Intel Corporation. <BR>

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

#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
//APTIOV_SERVER_OVERRIDE_START
//#include <Library/TcgPpVendorLib.h>
//APTIOV_SERVER_OVERRIDE_END
#include <Protocol/SmmVariable.h>

#include "TxtOneTouchData.h"

#define PP_SUBMIT_REQUEST_TO_PREOS_SUCCESS                                  0
#define PP_SUBMIT_REQUEST_TO_PREOS_NOT_IMPLEMENTED                          1
#define PP_SUBMIT_REQUEST_TO_PREOS_GENERAL_FAILURE                          2
#define PP_SUBMIT_REQUEST_TO_PREOS_BLOCKED_BY_BIOS_SETTINGS                 3

#define PP_GET_USER_CONFIRMATION_NOT_IMPLEMENTED                                 0
#define PP_GET_USER_CONFIRMATION_BIOS_ONLY                                       1
#define PP_GET_USER_CONFIRMATION_BLOCKED_BY_BIOS_CONFIGURATION                   2
#define PP_GET_USER_CONFIRMATION_ALLOWED_AND_PPUSER_REQUIRED                     3
#define PP_GET_USER_CONFIRMATION_ALLOWED_AND_PPUSER_NOT_REQUIRED                 4

EFI_GUID  gTxtOneTouchFlagsGuid = TXT_ONE_TOUCH_FLAGS_GUID;

EFI_SMM_VARIABLE_PROTOCOL  *mTxtOneTouchSmmVariable;

/**
  The callback for TPM vendor specific physical presence which is called for
  Submit TPM Operation Request to Pre-OS Environment and
  Submit TPM Operation Request to Pre-OS Environment 2.

  This API should be invoked in OS runtime phase to interface with ACPI method.

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      ManagementFlags  BIOS TPM Management Flags.

  @return Return Code for Submit TPM Operation Request to Pre-OS Environment and
          Submit TPM Operation Request to Pre-OS Environment 2.
**/
UINT32
EFIAPI
TxtOneTouchLibSubmitRequestToPreOSFunction (
  IN UINT32                 OperationRequest,
  IN UINT32                 ManagementFlags
  )
{
  switch (OperationRequest) {
  case SET_NO_TXT_MAINTENANCE_FALSE:
  case SET_NO_TXT_REFURBISHING_FALSE:
  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0:
  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_1:
  case SET_NO_TXT_MAINTENANCE_TRUE:
  case SET_NO_TXT_REFURBISHING_TRUE:
  case EXEC_BOISAC_RESET_ESTABLISHMENT_BIT:
  case EXEC_BOISAC_RESET_AUX_INDEX:
    break;

  default:
    return PP_SUBMIT_REQUEST_TO_PREOS_NOT_IMPLEMENTED;
  }
  return PP_SUBMIT_REQUEST_TO_PREOS_SUCCESS;
}

/**
  The callback for TPM vendor specific physical presence which is called for
  Get User Confirmation Status for Operation.

  This API should be invoked in OS runtime phase to interface with ACPI method.

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      ManagementFlags  BIOS TPM Management Flags.

  @return Return Code for Get User Confirmation Status for Operation.
**/
UINT32
EFIAPI
TxtOneTouchLibGetUserConfirmationStatusFunction (
  IN UINT32                 OperationRequest,
  IN UINT32                 ManagementFlags
  )
{
  EFI_STATUS                     Status;
  UINTN                          DataSize;
  UINT32                         ReturnCode;
  BOOLEAN                        RequestConfirmed;
  TXT_ONE_TOUCH_FLAGS            TxtOneTouchFlags;

  if (mTxtOneTouchSmmVariable == NULL) {
    Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**)&mTxtOneTouchSmmVariable);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Get the Physical Presence flags
  //
  DataSize = sizeof (TXT_ONE_TOUCH_FLAGS);
  Status = mTxtOneTouchSmmVariable->SmmGetVariable (
                           TXT_ONE_TOUCH_FLAGS_VARIABLE,
                           &gTxtOneTouchFlagsGuid,
                           NULL,
                           &DataSize,
                           &TxtOneTouchFlags
                           );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] Get TxtOneTouch PP flags failure! Status = %r\n", Status));
    return PP_GET_USER_CONFIRMATION_BLOCKED_BY_BIOS_CONFIGURATION;
  }

  RequestConfirmed = FALSE;
  switch (OperationRequest) {
  case SET_NO_TXT_MAINTENANCE_FALSE:
  case SET_NO_TXT_REFURBISHING_FALSE:
    RequestConfirmed = TRUE;
    break;
  case SET_NO_TXT_MAINTENANCE_TRUE:
  case SET_NO_TXT_REFURBISHING_TRUE:
    RequestConfirmed = FALSE;
    break;

  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0:
  case TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X:
  case TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0:
  case TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0:
  case TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_0__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_0__VMXINSMX_1__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_0:
  case SENTER_1__VMXINSMX_0__VMXOUTSMX_1:
  case SENTER_1__VMXINSMX_1__VMXOUTSMX_1:
    if ((TxtOneTouchFlags.Flags & TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_MAINTENANCE) != 0) {
      RequestConfirmed = TRUE;
    } else {
      RequestConfirmed = FALSE;
    }
    break;

  case EXEC_BOISAC_RESET_ESTABLISHMENT_BIT:
  case EXEC_BOISAC_RESET_AUX_INDEX:
    if ((TxtOneTouchFlags.Flags & TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_REFURBISHING) != 0) {
      RequestConfirmed = TRUE;
    } else {
      RequestConfirmed = FALSE;
    }
    break;

  default:
    return PP_GET_USER_CONFIRMATION_NOT_IMPLEMENTED;
  }

  if (RequestConfirmed) {
    ReturnCode = PP_GET_USER_CONFIRMATION_ALLOWED_AND_PPUSER_NOT_REQUIRED;
  } else {
    ReturnCode = PP_GET_USER_CONFIRMATION_ALLOWED_AND_PPUSER_REQUIRED;
  }
  return ReturnCode;
}
