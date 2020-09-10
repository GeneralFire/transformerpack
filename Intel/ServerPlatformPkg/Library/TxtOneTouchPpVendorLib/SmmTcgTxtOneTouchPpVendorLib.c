/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015-2018 Intel Corporation. <BR>

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
//APTIOV_SERVER_OVERRIDE_START
//#include <Library/TcgPpVendorLib.h>
//APTIOV_SERVER_OVERRIDE_END

#include "SmmTcgTxtOneTouchPpVendorLibCommon.h"

/**
  Check and execute the requested physical presence command.

  This API should be invoked in BIOS boot phase to process pending request.

  Caution: This function may receive untrusted input.

  If OperationRequest < 128, then ASSERT().

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in, out] ManagementFlags  BIOS TPM Management Flags.
  @param[out]     ResetRequired    If reset is required to vendor settings in effect.
                                   True, it indicates the reset is required.
                                   False, it indicates the reset is not required.

  @return TPM Operation Response to OS Environment.
**/
UINT32
EFIAPI
TcgPpVendorLibExecutePendingRequest (
  IN UINT32                 OperationRequest,
  IN OUT UINT32             *ManagementFlags,
  OUT BOOLEAN               *ResetRequired
  )
{
  ASSERT (FALSE);
  return TCG_PP_OPERATION_RESPONSE_BIOS_FAILURE;
}

/**
  Check if there is a valid physical presence command request.

  This API should be invoked in BIOS boot phase to process pending request.

  Caution: This function may receive untrusted input.

  If OperationRequest < 128, then ASSERT().

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      ManagementFlags  BIOS TPM Management Flags.
  @param[out]     RequestConfirmed If the physical presence operation command required user confirm from UI.
                                   True, it indicates the command doesn't require user confirm.
                                   False, it indicates the command need user confirm from UI.

  @retval  TRUE        Physical Presence operation command is valid.
  @retval  FALSE       Physical Presence operation command is invalid.
**/
BOOLEAN
EFIAPI
TcgPpVendorLibHasValidRequest (
  IN UINT32                 OperationRequest,
  IN UINT32                 ManagementFlags,
  OUT BOOLEAN               *RequestConfirmed
  )
{
  ASSERT (FALSE);
  return FALSE;
}

/**
  The callback for TPM vendor specific physical presence which is called for
  Submit TPM Operation Request to Pre-OS Environment and
  Submit TPM Operation Request to Pre-OS Environment 2.

  This API should be invoked in OS runtime phase to interface with ACPI method.

  Caution: This function may receive untrusted input.

  If OperationRequest < 128, then ASSERT().

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      ManagementFlags  BIOS TPM Management Flags.

  @return Return Code for Submit TPM Operation Request to Pre-OS Environment and
          Submit TPM Operation Request to Pre-OS Environment 2.
**/
UINT32
EFIAPI
TcgPpVendorLibSubmitRequestToPreOSFunction (
  IN UINT32                 OperationRequest,
  IN UINT32                 ManagementFlags
  )
{
  ASSERT (OperationRequest >= TCG_PHYSICAL_PRESENCE_VENDOR_SPECIFIC_OPERATION);
  return TxtOneTouchLibSubmitRequestToPreOSFunction (OperationRequest, ManagementFlags);
}

/**
  The callback for TPM vendor specific physical presence which is called for
  Get User Confirmation Status for Operation.

  This API should be invoked in OS runtime phase to interface with ACPI method.

  Caution: This function may receive untrusted input.

  If OperationRequest < 128, then ASSERT().

  @param[in]      OperationRequest TPM physical presence operation request.
  @param[in]      ManagementFlags  BIOS TPM Management Flags.

  @return Return Code for Get User Confirmation Status for Operation.
**/
UINT32
EFIAPI
TcgPpVendorLibGetUserConfirmationStatusFunction (
  IN UINT32                 OperationRequest,
  IN UINT32                 ManagementFlags
  )
{
  ASSERT (OperationRequest >= TCG_PHYSICAL_PRESENCE_VENDOR_SPECIFIC_OPERATION);
  return TxtOneTouchLibGetUserConfirmationStatusFunction (OperationRequest, ManagementFlags);
}
