/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmVarCheck.h>
#include <Library/SecuredSetVariableLib.h>
#include <Protocol/SmmExitBootServices.h>

BOOLEAN                       mEndOfDxe = FALSE;
BOOLEAN                       mExitBootServices = FALSE;

/**
  SetVariable check handler platform.

  @param[in] VariableName       Name of Variable to set.
  @param[in] VendorGuid         Variable vendor GUID.
  @param[in] Attributes         Attribute value of the variable.
  @param[in] DataSize           Size of Data to set.
  @param[in] Data               Data pointer.

  @retval EFI_SUCCESS           The SetVariable check result was success.
  @retval EFI_WRITE_PROTECTED   Write protected to runtime.

**/
EFI_STATUS
EFIAPI
PlatformSetVariableCheckHandlerOnEndOfDxe (
  IN CHAR16     *VariableName,
  IN EFI_GUID   *VendorGuid,
  IN UINT32     Attributes,
  IN UINTN      DataSize,
  IN VOID       *Data
  )
{
  if (!mEndOfDxe) {
    // Start to do variable check after EndOfDxe event signaled
    return EFI_SUCCESS;
  }

  //
  // If variable is secured variable, we should return write protected.
  //
  if (IsSecuredVariable (VariableName, VendorGuid, CheckPoint_EndOfDxe)) {
    return EFI_WRITE_PROTECTED;
  }

  // Not found, so pass.
  return EFI_SUCCESS;
}

/**
  Platform secure variable check handler.

  @param[in] VariableName       Name of Variable to set.
  @param[in] VendorGuid         Variable vendor GUID.
  @param[in] Attributes         Attribute value of the variable.
  @param[in] DataSize           Size of Data to set.
  @param[in] Data               Data pointer.

  @retval EFI_SUCCESS           The SetVariable check result was success.
  @retval EFI_WRITE_PROTECTED   Write protected to runtime.

**/
EFI_STATUS
EFIAPI
PlatformSetVariableCheckHandlerOnExitBootServices (
  IN CHAR16     *VariableName,
  IN EFI_GUID   *VendorGuid,
  IN UINT32     Attributes,
  IN UINTN      DataSize,
  IN VOID       *Data
  )
{
  if (!mExitBootServices) {
    // Start to do variable check after exit boot services event signaled
    return EFI_SUCCESS;
  }

  //
  // If variable is secured variable, we should return write protected.
  //
  if (IsSecuredVariable (VariableName, VendorGuid, CheckPoint_ExitBootService)) {
    return EFI_WRITE_PROTECTED;
  }

  // Not found, so pass.
  return EFI_SUCCESS;
}

/**
  SMM END_OF_DXE protocol notification event handler.

  @param  Protocol   Points to the protocol's unique identifier
  @param  Interface  Points to the interface instance
  @param  Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   This callback function returns successfully

**/
EFI_STATUS
EFIAPI
PlatformSmmEndOfDxeCallback (
  IN CONST EFI_GUID                       *Protocol,
  IN VOID                                 *Interface,
  IN EFI_HANDLE                           Handle
  )
{
  DEBUG ((EFI_D_INFO, "%a: EndOfDxe event callback. Set mEndOfDxe to TRUE.\n", __FUNCTION__));
  mEndOfDxe = TRUE;

  return EFI_SUCCESS;
}

/**
  SMM exit boot services protocol notification event handler.

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
PlatformSmmExitBootServicesCallback (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  DEBUG ((EFI_D_INFO, "%a: ExitBootServices event callback. Set mExitBootServices to TRUE.\n", __FUNCTION__));
  mExitBootServices = TRUE;

  return EFI_SUCCESS;
}

/**
  Platform Secure Variable Driver main entry point.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       Variable service successfully initialized.

**/
EFI_STATUS
EFIAPI
PlatformSecureVariableSmmEntry (
  IN EFI_HANDLE                           ImageHandle,
  IN EFI_SYSTEM_TABLE                     *SystemTable
  )
{
  EFI_STATUS                    Status;
  EDKII_SMM_VAR_CHECK_PROTOCOL  *EdkiiSmmVarCheckProtocol;
  VOID                          *SmmEndOfDxeRegistration;
  VOID                          *SmmExitBootServicesRegistration;

  EdkiiSmmVarCheckProtocol = NULL;

  Status = gSmst->SmmRegisterProtocolNotify (
                  &gEfiSmmEndOfDxeProtocolGuid,
                  PlatformSmmEndOfDxeCallback,
                  &SmmEndOfDxeRegistration
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "%a: Failed to register EndOfDxe callback!!\n", __FUNCTION__));
    return Status;
  }

  Status = gSmst->SmmRegisterProtocolNotify (
                  &gEdkiiSmmExitBootServicesProtocolGuid,
                  PlatformSmmExitBootServicesCallback,
                  &SmmExitBootServicesRegistration
                  );

  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "%a: Failed to register exit boot services callback!!\n", __FUNCTION__));
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gEdkiiSmmVarCheckProtocolGuid,
                    NULL,
                    (VOID **) &EdkiiSmmVarCheckProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "%a: Failed to locate gEdkiiSmmVarCheckProtocolGuid protocol!!\n", __FUNCTION__));
    return Status;
  } else {
    Status = EdkiiSmmVarCheckProtocol->SmmRegisterSetVariableCheckHandler (PlatformSetVariableCheckHandlerOnEndOfDxe);
    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR, "%a: Failed to register platform handler for SetVariableCheck at end of DXE!!\n", __FUNCTION__));
      return Status;
    }

    Status = EdkiiSmmVarCheckProtocol->SmmRegisterSetVariableCheckHandler (PlatformSetVariableCheckHandlerOnExitBootServices);
    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR, "%a: Failed to register platform handler for SetVariableCheck at exit bootservices!!\n", __FUNCTION__));
      return Status;
    }
  }

  return EFI_SUCCESS;
}
