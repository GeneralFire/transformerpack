/** @file
  This driver will clear platform password.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Guid/DebugMask.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

#include <Guid/SetupVariable.h>
#include <Library/SetupLib.h>
#include <Pins/GpioPinsVer3.h>
#include <Library/GpioLib.h>

//
//Password clear: GPP_L_5_FM_PASSWORD_CLEAR_N
//
BOOLEAN
EFIAPI
ProcessPwdClearHwJmpReg (
  VOID
  )
{

  BOOLEAN PasswordClearMode = FALSE;

  EFI_STATUS Status;
  UINT32 GpiValue;

  // GpioGetInputValue()  0: InputLow, 1: InpuHigh
  Status = GpioGetInputValue (GPIO_VER3_GPP_L5, &GpiValue);

  if (!GpiValue) {
    PasswordClearMode = TRUE;
  }

  return PasswordClearMode;
}

/*++

Routine Description:

  Process platform action about passwords

Arguments:

  ImageHandle  -  Handle for the image of this driver.
  SystemTable  -  Pointer to the EFI System Table.

Returns:

  Protocol registration return status

--*/
EFI_STATUS
PlatformPasswordEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                 Status;
  SYSTEM_CONFIGURATION       mSystemConfiguration;
  UINTN                      VariableSize;
  BOOLEAN                    IsClearPasswordJumper;
  BOOLEAN                    ClearPassword;
  EFI_GUID                   mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;

  IsClearPasswordJumper = ProcessPwdClearHwJmpReg ();
  if(IsClearPasswordJumper){
    DEBUG((EFI_D_ERROR, "Detect ClearPassowrdJumper inserted!\n"));
  }

  ClearPassword = FALSE;
  if(IsClearPasswordJumper) {
    ClearPassword = TRUE;
  }

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable (
                  L"Setup",
                  &mSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  &mSystemConfiguration
                  );
  if(EFI_ERROR(Status)){
    DEBUG((EFI_D_ERROR,"Fail to get variable SystemConfiguration %r\n", Status));
    return Status;
  }

  //
  //Password variable bad or clear jumper is pressed or in mfg mode, clear password
  //
  if(ClearPassword){
    ZeroMem (&mSystemConfiguration.AdminPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    ZeroMem (&mSystemConfiguration.UserPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    Status = SetSpecificConfigGuid (&mSystemConfigurationGuid, &mSystemConfiguration);
    ASSERT_EFI_ERROR(Status);
  }

  return EFI_SUCCESS;
}